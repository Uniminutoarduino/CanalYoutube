from machine import UART, Pin
from machine import Timer
from machine import ADC
import time



def EnviarLectura(t): #El método debe tener un argumento en este caso t
    print ('Enviando...')
    ValorADC=adc.read_u16() #Leer valor de
    Voltaje=ValorADC*(3.3/65535) #Conversion a voltaje 10 bits
    uart1.write(str(round(Voltaje,2))) #Enviar valor convertido del ADC
    uart1.write('\n') #Escribir el carácter terminador de la cadena a enviar


adc=ADC(Pin(28)) #Canal Analogo en el pin 28. 
uart1= UART(1,baudrate=9600, tx=Pin(4), rx=Pin(5)) #Configurar UART
tim = Timer(period=1500, mode=Timer.PERIODIC, callback=EnviarLectura) #Crear Timer con callback cada 1.5s
