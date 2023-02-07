import easymodbus.modbusClient
import time
#import easymodbus.modbusClient
modbus_client = easymodbus.modbusClient.ModbusClient('192.168.0.21',502) #Crar instancia para conexion en esta IP y puerto
modbus_client.connect() #Conectar a cliente modbus

c=0

while True:
    try:
        modbus_client.write_single_register(c, c)	#Escribir valores a registro acorde variable
        modbus_client.write_single_coil(2, 1)	#Activar coil 2
        listavalores=modbus_client.read_holdingregisters(15,4) #Leer 4 holding registers desde la posición 15
        print ('Valor 1: '+str(listavalores[0])) #Imprimir valores leidos
        print ('Valor 2: '+str(listavalores[1]))
        print ('Valor 3: '+str(listavalores[2]))
        print ('Valor 4: '+str(listavalores[3]))
        Entradas = modbus_client.read_discreteinputs(0,2)	#Leer dos entradas (I0.0, I0.1)
        print ("I0.0: "+str(Entradas[0]))
        print ("I0.1: "+str(Entradas[1]))
        c=c+1
        if c>8:
            c=0
        time.sleep(0.3)
    except KeyboardInterrupt:
        break
    
modbus_client.close() #Cerrar cliente modbus
