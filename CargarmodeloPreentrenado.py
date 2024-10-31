#Importa la clase YOLO desde la librería ultralytics
from ultralytics import YOLO
#Carga del modelo preentrenado YOLOv8 con segmentación
model = YOLO('yolov8m-seg.pt')
# Imprimir la arquitectura del modelo
print(model)
