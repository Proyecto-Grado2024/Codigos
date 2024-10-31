#Importa la clase YOLO desde la librería ultralytics
from ultralytics import YOLO
#Carga del modelo preentrenado YOLOv8 con segmentación
model = YOLO('oidium.pt')
# Imprimir la arquitectura del modelo
print(model)