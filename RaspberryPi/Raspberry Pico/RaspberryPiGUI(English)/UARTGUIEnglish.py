from machine import UART, Pin
from machine import Timer
from machine import ADC
import time

adc=ADC(Pin(28)) #Analog channel on Pin GPIO28 
uart1= UART(1,baudrate=9600, tx=Pin(4), rx=Pin(5)) #UART configuration, GPIO4 transmission pin, GPIO5 reception pin


servo1=machine.PWM(Pin(17)) #Servo1 in GPIO17 (Clipper)
servo2=machine.PWM(Pin(16)) #Servo2 in GPIO16 (Base robot)
servo1.freq(50) #Sermotors with a standard frequency of 50Hz.
servo2.freq(50)


def SendReading(t): #The function must have an argument in this case t
    print ('sending...')
    ADCValue=adc.read_u16() #Read ADC value
    Voltage=ADCValue*(3.3/65535) #Transform 16-bit value from ADC to voltage scale
    Temperature=(150.0)*(Voltage/1.5) # Temperature conversion for LM35
    uart1.write(str(round(Temperature,2))) #Send temperature value rounded to 2 decimals
    uart1.write('\n') #Send EOF character (line breaker) to finish the transmission

tim = Timer(period=1500, mode=Timer.PERIODIC, callback=SendReading) #Timer to send temperature sensor values to GUI with a period of 1.5 secs.

while True:
    if uart1.any()>0: #If there is data available from the GUI
        print ('detected') #print in console 
        uartvalue=uart1.read() #Read data from UART
        valoruartsrt=str(uartvalue.decode('UTF-8')) #Transform serial data to string and decode it in format UTF-8
        print (valoruartsrt) #print data decoded
        #Move the servomotors according to the detected command, please check the times of your motors
        #Duty cycle goes from 0-100% (0-65535) 16-bit number
        if (valoruartsrt=='D'):
            print('Right movement')
            servo2.duty_u16(6554) #Duty cycle for right movement (2ms)
        elif (valoruartsrt=='I'):
            print ('Left movement')
            servo2.duty_u16(1638) #Duty cycle for left movement (0.5ms)
        elif (valoruartsrt=='A'):
            print('Open clipper')
            servo1.duty_u16(1638) #Duty cycle to open clipper (0.5ms)
        elif (valoruartsrt=='C'):
            print('Close clipper')
            servo1.duty_u16(6554) #Duty cycle to close clipper (2ms)
            
                

