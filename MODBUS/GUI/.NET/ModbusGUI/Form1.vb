Imports EasyModbus 'Importar librería al proyecto
'Importante agregar la dll en dependencias del proyecto, click derecho y buscar la dll (EasyModBus.dll)
'Todos los métodos son de tipo public
'Desarrollado por Talos Electrónico

Public Class Form1
    Dim Cliente As New ModbusClient 'Crear objeto para acciones con librería ModBus
    Dim lecturavar(1) As Integer 'Variable para almacener registro leido, debe ser de tipo Array
    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        Conectar() 'Conectar con el cliente
    End Sub

    Public Sub Conectar()

        Try
            Cliente = New ModbusClient("192.168.0.100", 502) 'Crear conexión con cliente en la IP y puertos indicados
            Cliente.Connect() 'Crear Conexión
            MessageBox.Show("Conexión creada", "Info", MessageBoxButtons.OK, MessageBoxIcon.Information) 'Mostrar mensaje OK
            Lectura() 'Iniciar lectura de registro de entrada en dirección 100
        Catch
            MessageBox.Show("Verifique primero conexión", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)
        End Try
    End Sub

    Public Sub Lectura()
        Timer1.Interval = 1500 'Configurar Timer con intervalo de 1.5 segundos (1500ms)
        AddHandler Timer1.Tick, AddressOf EventoLectura 'Crear un Handler mapara manejar el evento cuando el tiempo pase
        Timer1.Start() 'Iniciar Timer 1
    End Sub

    Public Sub EventoLectura() 'Handler para lectura de registro de entrada
        If Cliente.Connected Then
            lecturavar = Cliente.ReadInputRegisters(100, 1) 'Lectura de un registro de entrada en dirección 100, función (04) modbus
            Label1.Text = "Valor Leído: " + lecturavar(0).ToString 'Colocar el valor leído en la etiqueta de la forma
        End If
    End Sub

    Public Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        If Cliente.Connected Then
            Cliente.Disconnect() 'Desconectar del cliente
            Timer1.Stop() 'Detener temporizador de lectura de datos
            MessageBox.Show("Desconectado...", "Desconexion", MessageBoxButtons.OK, MessageBoxIcon.Information)
        Else
            MessageBox.Show("Error, verifique primero conexión")
        End If
    End Sub

    Public Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click
        If Cliente.Connected Then
            Cliente.WriteSingleCoil(101, True) 'Activar coil en la dirección 101, funcion (05) modbus
        Else
            MessageBox.Show("Verifique primero conexión con el cliente")
        End If

    End Sub

    Public Sub Button4_Click(sender As Object, e As EventArgs) Handles Button4.Click
        If Cliente.Connected Then
            Cliente.WriteSingleCoil(101, False) 'Apagar coil en direccion 101, función (05) modbus
        Else
            MessageBox.Show("Verifique primero conexión con el cliente")
        End If

    End Sub

End Class
