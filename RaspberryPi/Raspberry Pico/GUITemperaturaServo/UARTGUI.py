from machine import UART, Pin
from machine import Timer
from machine import ADC
import time



def EnviarLectura(t): #El método debe tener un argumento en este caso t
    print ('Enviando...')
    ValorADC=adc.read_u16() #Leer valor de
    Voltaje=ValorADC*(3.3/65535) #Conversion a voltaje 16 bits
    Temperatura=(150.0)*(Voltaje/1.5) # Conversion a Temperatura
    uart1.write(str(round(Temperatura,2))) #Enviar valor convertido del ADC
    uart1.write('\n') #Escribir el carácter terminador de la cadena a enviar


adc=ADC(Pin(28)) #Canal Analogo en el pin 28. 
uart1= UART(1,baudrate=9600, tx=Pin(4), rx=Pin(5)) #Configurar UART
tim = Timer(period=1500, mode=Timer.PERIODIC, callback=EnviarLectura) #Crear Timer con callback cada 1.5seg
servo1=machine.PWM(Pin(17)) #Servo1 en GPIO17 (Pinza)
servo2=machine.PWM(Pin(16)) #Servo2 en GPIO16 (Base)
servo1.freq(50) #Servomotores a 50Hz de frecuencia
servo2.freq(50)

while True:
    if uart1.any()>0: #Si hay datos disponibles en puerto serial
        print ('detectado') #imprimir detectado
        valor=uart1.read() #Leer dato enviado desde consola
        valorsrt=str(valor.decode('UTF-8')) #Transformar a string mediante decodificador UTF-8
        print (valorsrt) #Imprimir valor leido
        #Mover los servomotores de acuerdo al comando
        if (valorsrt=='D'):
            print('movimiento derecha')
            servo2.duty_u16(6554) #Cambiar duty cycle de servo 2ms
        elif (valorsrt=='I'):
            print ('movimiento izquierda')
            servo2.duty_u16(1638) #Cambiar duty cycle de servo 0.5ms
        elif (valorsrt=='A'):
            print('Abrir pinza')
            servo1.duty_u16(1638)
        elif (valorsrt=='C'):
            print('Cerrar pinza')
            servo1.duty_u16(6554)
            
