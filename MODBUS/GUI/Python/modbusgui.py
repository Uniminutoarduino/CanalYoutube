import PySimpleGUI as sg
import easymodbus.modbusClient
import threading

sg.theme('BluePurple')

layout = [[sg.Text('Modbus GUI Python'), sg.Text(size=(15,1))],[sg.Button('Conectar'),  sg.Button('ON Coil'), sg.Button('OFF Coil'), sg.Button('Desconectar'), sg.Button('Salir')]]

window = sg.Window('Modbus GUI', layout)
modbus_client = easymodbus.modbusClient.ModbusClient('192.168.0.2', 502)  # Direccion IP de ESP32
FlagLectura =  0

def InicioLecturaSensor():
    input_registers = modbus_client.read_inputregisters(100, 100)  # Read input registers 1 to 10 from server
    print('Valor leído: ', input_registers[0])
    t = threading.Timer(2.0, InicioLecturaSensor)
    t.start()


while True:  # Event Loop
    event, values = window.read()
    print(event, values)
    if event == sg.WIN_CLOSED or event == 'Salir':
        break

    if event == 'Conectar':
        FlagLectura=1
        modbus_client.connect()
        InicioLecturaSensor()

    if event == 'ON Coil':
        if FlagLectura == 1 :
            modbus_client.write_single_coil(101, True)  # Activar coil en dirección 100
        else:
            sg.popup('Error','Primero realiza conexión con cliente')

    if event == 'OFF Coil':
        if FlagLectura == 1 :
            modbus_client.write_single_coil(101, False)  # Activar coil en dirección 100
        else:
            sg.popup('Error','Primero realiza conexión con cliente')

    if event == 'Desconectar':
        if FlagLectura == 1 :
            FlagLectura = 0
            modbus_client.close()
        else:
            sg.popup('Error','Primero realiza conexión con cliente')

window.close()
