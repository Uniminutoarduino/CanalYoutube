import tkinter as tk
import tkinter.messagebox as msgbox
import easymodbus.modbusClient
import threading

modbus_client=None

class Ventana(tk.Tk):
    flag=0 #Variable para determinar si hubo conexión correcta al cliente modbus
    t=None

    def __init__(self):
        super().__init__() #Crear ventana
        self.title('GUI Modbus') #Cambio de titulo de la ventana
        self.label=tk.Label(self,text='') #Crear una etiqueta para la ventana
        self.label.pack(fill=tk.BOTH,expand=1,padx=60,pady=40) #Colocar la etiqueta en toda la ventana
        Boton_conectar=tk.Button(self,text="Conectar",command=self.conectar)
        Boton_conectar.pack(side=tk.LEFT,padx=(20,0), pady=(0,40))
        Boton_activarcoil=tk.Button(self,text="Activar coil",command=self.Activar)
        Boton_activarcoil.pack(side=tk.LEFT,padx=(10,0),pady=(0,40))
        Boton_desactivarcoil=tk.Button(self,text="Apagar coil",command=self.Desactivar)
        Boton_desactivarcoil.pack(side=tk.LEFT,padx=(10,0),pady=(0,40))
        Boton_desconectar=tk.Button(self,text='Desconectar',command=self.Desconectar)
        Boton_desconectar.pack(side=tk.RIGHT,padx=(10,0),pady=(0,40))
        self.modbus_client = easymodbus.modbusClient.ModbusClient('192.168.0.100', 502)  # Direccion IP de ESP32

    def conectar(self):
        try:
            self.flag=1
            self.modbus_client.connect()
            print ('Conectado...')
            self.InicioLectura()#Iniciar lectura de holding register en ESP32
        except:
            self.flag = 0 #Garantizar que si hay error el flag sea 0 para que no haya intentos de escritura y lectura
            msgbox.showerror('Error','Error de conexión \n revise IP de cliente')
            print ('Error de conexión',str(easymodbus.modbusClient.Exceptions.ConnectionException))

    def Activar(self):
        if(self.flag==1):
            self.modbus_client.write_single_coil(101,True) #Activar coil en dirección 100
        else:
            msgbox.showerror('Error', 'Primero realice conexión a cliente')
            print ('Primero realice conexión a cliente')

    def Desactivar(self):
        if(self.flag==1):
            self.modbus_client.write_single_coil(101, False)  # Apagar coil en dirección 100
        else:
            msgbox.showerror('Error', 'Primero realice conexión a cliente')
            print('Primero realice conexión a cliente')

    def InicioLectura(self):
        if(self.flag==1):
            input_registers = self.modbus_client.read_inputregisters(100, 100)  # Read input registers 1 to 10 from server
            print('Valor leído: ',input_registers[0])
            self.label.configure(text='Valor leído: '+str(input_registers[0]))
            t=threading.Timer(2.0, self.InicioLectura)
            t.start()
        if(self.flag==2):#Si flag es 2 se debe cancelar el timer y cerrar conexion modbus
            t.cancel()
            self.modbus_client.close()

    def Desconectar(self):
        if(self.flag==1):
            self.flag=2
        else:
            msgbox.showerror('Error','Primero realice conexión a cliente...')

if __name__=="__main__":
    ventana=Ventana()
    ventana.mainloop()
