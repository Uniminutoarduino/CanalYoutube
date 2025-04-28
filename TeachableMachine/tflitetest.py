from keras.models import load_model  # TensorFlow is required for Keras to work
import cv2  # Install opencv-python
import numpy as np
import h5py

#Correccion problema con groups en tensorflow
f = h5py.File("keras_model.h5", mode="r+")
model_config_string = f.attrs.get("model_config")

if model_config_string.find('"groups": 1,') != -1:
    model_config_string = model_config_string.replace('"groups": 1,', '')
f.attrs.modify('model_config', model_config_string)
f.flush()

model_config_string = f.attrs.get("model_config")

assert model_config_string.find('"groups": 1,') == -1
f.close()



# Disable scientific notation for clarity
np.set_printoptions(suppress=True)

# Cargar el modelo realizado en Teachable Machine
model = load_model("keras_model.h5", compile=False)

# Cargar las etiquetas
class_names = open("labels.txt", "r").readlines()

camera = cv2.VideoCapture(0) #activar el video mediante opencv

while True:
    # Iniciar la captura de frames de la camara
    ret, image = camera.read()

    # Reajustar las imagenes a tamaño 224px * 224 px
    image = cv2.resize(image, (224, 224), interpolation=cv2.INTER_AREA)

    # Mostrar la iamgen en una ventana
    cv2.imshow("Webcam Image", image)

    # Make the image a numpy array and reshape it to the models input shape.
    image = np.asarray(image, dtype=np.float32).reshape(1, 224, 224, 3)

    # Normalizar imagen
    image = (image / 127.5) - 1

    # Hacer las predicciones del modelo con un puntaje de confidencia (0-100%)
    prediction = model.predict(image)
    index = np.argmax(prediction)
    class_name = class_names[index]
    confidence_score = prediction[0][index]

    # Imprimir en ventana la clase y el intervalo de confianza en la detección
    print("Clase:", class_name[2:], end="")
    print("Puntaje de confianza:", str(np.round(confidence_score * 100))[:-2], "%")

    # Listen to the keyboard for presses.
    keyboard_input = cv2.waitKey(1)

    # Si se presiona la tecla escape, se sale del programa
    if keyboard_input == 27:
        break

camera.release()
cv2.destroyAllWindows()
