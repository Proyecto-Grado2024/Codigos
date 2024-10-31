#importacion de librerias
import os
import xml.etree.ElementTree as ET
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from PIL import Image
from sklearn.model_selection import train_test_split
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv2D, MaxPooling2D, Flatten, Dense
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.layers import Dropout
from tensorflow.data import Dataset
from tensorflow.keras.layers import BatchNormalization
from tensorflow.keras.regularizers import l2
from tensorflow.keras.initializers import glorot_uniform

#ruta de datos
dir_oidium='C:/Users/Alex13/Desktop/lechugascnn/oidium'
#print(os.listdir(dir_oidium))

#Lectura de archivos XML y extraccion de datos
oidium_datos = []
for archivos_xml in os.listdir(dir_acelga):
    if archivos_xml.endswith('.xml'):
        proceso = ET.parse(os.path.join(dir_acelga, archivos_xml))
        root = proceso.getroot()

        direccion_imagen = root.find('path').text
        buscar_etiqueta = root.find('.//object[name="oidium"]')
        recuadro = buscar_etiqueta.find("bndbox")
        x1 = int(recuadro.find('xmin').text)
        y1 = int(recuadro.find('ymin').text)
        x2 = int(recuadro.find('xmax').text)
        y2 = int(recuadro.find('ymax').text)

        oidium_datos.append({'direccion_imagen': direccion_imagen, 'x1': x1, 'y1': y1, 'x2': x2, 'y2': y2})

#procesamiento de imagenes
oicium_pro = []
oicium_et = []
contador_ac = 0
for data in oicium_datos:
    # print(data['direccion_imagen'])
    imagen = Image.open(data['direccion_imagen'])
    recorte = imagen.crop((data['x1'], data['y1'], data['x2'], data['y2']))
    redimencion = recorte.resize((224, 224))
    normalizacion = np.array(redimencion) / 255.0
    acelga_pro.append(normalizacion)
    acelga_et.append(0)

    contador_ac += 1
print(contador_ac)

#preparacion de datos para el entrenamiento
imagenes=oidium_pro
etiquetas=oidium_et

imagenes_array=np.array(imagenes)
etiquetas_array=np.array(etiquetas)

# Dividir los datos de manera estratificada
X_train, X_test, y_train, y_test = train_test_split(
    imagenes_array, etiquetas_array, test_size=0.2, random_state=52, stratify=etiquetas_array
)

# Verificar la proporción de clases en los conjuntos de entrenamiento y validación
print("Proporción de clases en conjunto de entrenamiento:")
unique_train, counts_train = np.unique(y_train, return_counts=True)
print(dict(zip(unique_train, counts_train)))

print("Proporción de clases en conjunto de validación:")
unique_test, counts_test = np.unique(y_test, return_counts=True)
print(dict(zip(unique_test, counts_test)))

#construccion del modelo
model = Sequential([
    Conv2D(32, (3, 3),activation='relu', input_shape=(224, 224, 3) ),
    #BatchNormalization(),
    MaxPooling2D(2, 2),
    Conv2D(64, (3, 3), activation='relu'),
    BatchNormalization(),
    MaxPooling2D(2, 2),
    Conv2D(128, (3, 3), activation='relu'),
    BatchNormalization(),
    MaxPooling2D(2, 2),
    Flatten(),
    Dense(64, activation='relu', kernel_initializer=glorot_uniform()),  # Agregar regularización L2 aquí
    Dense(64, activation='relu'),
    Dropout(rate=0.1),
    Dense(2, activation='softmax')
])

print("x_train:", len(X_train))
print("y_train:", len(y_train))

# Conversión de etiquetas a one-hot encoding
from tensorflow.keras.utils import to_categorical

# Convertir las etiquetas a one-hot encoding
y_train_one_hot = to_categorical(y_train, num_classes=2)
y_test_one_hot = to_categorical(y_test, num_classes=2)
print(y_train_one_hot[:194])
#print(y_test_one_hot[:5])

#compilacion del modelo
model.compile(loss='categorical_crossentropy', optimizer=Adam(learning_rate=0.002), metrics=['accuracy'])

# Graficar la arquitectura del modelo
from tensorflow.keras.utils import plot_model
plot_model(model, to_file='modelo.png', show_shapes=True)

#entrenamioento del modelo
history=model.fit(X_train, y_train_one_hot, epochs=20, batch_size=32, validation_data=(X_test, y_test_one_hot))
test_loss=model.evaluate(X_test,y_test_one_hot)
print('Test accuracy:',test_loss)
