import asyncio

async def main():
    from ultralytics import YOLO
    import cv2
    import torch
    from telegram import Bot

    # Configuración del bot de Telegram
    telegram_token = '6668450352:AAEUMLtr0kGOX4jlmiZvy-axIlGDEwE8LZI'
    chat_id = '-4163970982'#'6489254534'  # Reemplaza con tu chat ID

    # Función para enviar un mensaje de Telegram
    async def send_telegram_message(message):
        bot = Bot(token=telegram_token)
        await bot.send_message(chat_id=chat_id, text=message)

    try:
        # Cargar el modelo entrenado
        model = YOLO("oidiumV.pt")

        # Ruta de la imagen que quieres analizar
        image_path = "captura6.jpg"

        # Leer la imagen
        frame = cv2.imread(image_path)

        # Verificar si la imagen se ha cargado correctamente
        if frame is None:
            print("Error: No se pudo cargar la imagen.")
        else:
            # Realizar la predicción
            resultados = model.predict(frame, imgsz=640, conf=0.84)

            # Verificar si se detectó 'oidium'
            deteccion_oidium = False

            # Obtener la clave correspondiente a 'oidium' en model.names
            class_id = None
            for key, value in model.names.items():
                if value == 'oidium':
                    class_id = key
                    break

            if class_id is not None:
                for result in resultados:
                    for box in result.boxes:
                        if box.cls == class_id:  # Verifica que la clase sea 'oidium'
                            deteccion_oidium = True

                            # Obtener las coordenadas de la caja
                            x1, y1, x2, y2 = map(int, box.xyxy[0])

                            # Convertir la clase y la confianza a tipos apropiados
                            class_name = model.names[int(box.cls.item())]
                            confidence = box.conf.item()

                            # Dibujar el recuadro en la imagen
                            cv2.rectangle(frame, (x1, y1), (x2, y2), (221, 48, 241), 12)
                            label = f"{class_name} {confidence:.2f}"
                            cv2.putText(frame, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (122, 110, 23), 3)

                            break  # Si solo quieres un recuadro por imagen, quita este break para dibujar todos los recuadros

            # Guardar la imagen anotada
            output_image_path = "resultado.jpg"
            cv2.imwrite(output_image_path, frame)

            if deteccion_oidium:
                mensaje = f"Oidium detectado en la imagen ."
                print(mensaje)
                await send_telegram_message(mensaje)
            else:
                mensaje = "No se detectó Oidium en la imagen."
                print(mensaje)
                await send_telegram_message(mensaje)
    finally:
        await asyncio.sleep(1)  # Esperar un segundo antes de salir para permitir que las tareas en segundo plano finalicen

# Ejecutar el bucle de eventos de asyncio
asyncio.run(main())
