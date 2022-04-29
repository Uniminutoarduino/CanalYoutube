#Conexión MODBUS con PLC usando librería easymodbus
#Creador: TalosElectrónico
#Fecha: 28/04/2022
import easymodbus.modbusClient #Importar libreria easymodbus como cliente
import time


if __name__=="__main__":
    modbusclient=easymodbus.modbusClient.ModbusClient('192.168.0.20',502) #Conectar con PLC en IP y puerto 205
    modbusclient.connect()

    while True:
        HoldingRegs = modbusclient.read_holdingregisters(0,2) #Leer dos registros desde la dirección 0
        print(HoldingRegs) #Imprime la información de los holding regs como una lista
        print('Valor posición 0 de registro: '+str(HoldingRegs[0])) #inprimir posicion 0 del holding reg
        discreteInputs = modbusclient.read_discreteinputs(0, 1) #Lectura de una entrada (I0.0)
        print('Valor lectura entrada I0.0: '+str(discreteInputs)) #Imprime verdadero o falso de acuerdo al valor lógico de la entrada leida
        modbusclient.write_single_register(2,1) #Escribe la posición 2 del array de registro de datos con el valor de 1
        modbusclient.write_single_coil(1,1) #Activa la salida Q0.1 del PLCS7-1200
        time.sleep(1)
