import serial #libreria para trabajar comunicacion serial
import time


ser = serial.Serial( #crear variable para trabajar con puerto serial. Abrir COM3, 9600 baudios, 1 bit de parada, 8 bits de datos
    port='COM9',\
    baudrate=9600,\
    parity=serial.PARITY_EVEN,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
        timeout=3)

def leertemperaturahum():
    print ("lectura temperatura-humedad") #Imprimir en consola este mensaje
    x= b'\x01\x04\x00\x01\x00\x02\x20\x0B' #Trama para solictar temperatura y humedad del sensor
    for i in x:
        envio=chr(i) #TRansformacion de byte a caracter
        ser.write(envio.encode()) #Envio por puerto serial la trama modbus para solictar temp y hum

ser.isOpen() #Abrir puerto serial con los parametros anteriores
leertemperaturahum() #Solictar por primera vez datos al sensor

while True:
    line=ser.readline() #Leer datos de sensor por puerto serial
    if len(line)>0: #Si el tamaño de los datos retornados es mayor a 0, imprimir cadena leida
        print(line)
        try:
            temp=float((line[3]*256+line[4])/100) #Conversion de datos leidos a variable flotante para temperatura
            hum=float((line[5]*256+line[6])/100) #Conversion de datos leidos a variable flotante para humedad
            print (str(temp)+' '+str(hum))
        except Exception as e:
            print("error lectura"+str(e))
        ser.flush() #limpia buffer esperando nuevos datos de entrada y de salida
        leertemperaturahum() #Se envia la peticion de humedad y temperatura
    time.sleep(2) #Hacer proceso cada 2 segundos
