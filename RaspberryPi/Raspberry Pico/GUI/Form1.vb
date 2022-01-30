Public Class Form1

    Dim i As New Integer 'Variable para muestra leida
    Dim LecturaSerial As String 'Variable para almacener muestra de puerto serial
    Private Sub Chart1_Click(sender As Object, e As EventArgs) Handles Chart1.Click

    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        SerialPort1.PortName = ComboBox1.SelectedItem.ToString 'Obtener nombre de puerto seleccionado del usuario para abrirlo
        'Label2.Text = ComboBox1.SelectedItem.ToString
        SerialPort1.BaudRate = 9600 '9600 Baudios de velocidad
        If SerialPort1.IsOpen Then
            MessageBox.Show("Puerto Abierto, cierre el puerto y vuelva a intentarlo")
        Else
            Try
                SerialPort1.Open() 'Abrir puerto serial
                MessageBox.Show("Conectado...") 'Mostrar mensaje
                Timer1.Interval = 1000 'Ajustar Timer a intervalo de 1 segundo, el timer leera los datos del buffer del puerto serial
                AddHandler Timer1.Tick, AddressOf ActualizarGrafica 'Generar Handler, cada vez que pase 1 segundo leer el buffer serial
                Timer1.Start() 'Iniciar Timer
                'AddHandler SerialPort1.DataReceived, AddressOf ActualizarGrafica 'Si hay datos de recepción, generar interrupcion (IRQ)
            Catch
                MessageBox.Show("Puerto inexistente, chequee el puerto de comunicación...")
            End Try

        End If

    End Sub

    Public Sub ActualizarGrafica(sender As Object, e As EventArgs)

        If SerialPort1.IsOpen Then
            If SerialPort1.BytesToRead > 0 Then 'Si hay Bytes disponibles en el Buffer
                LecturaSerial = SerialPort1.ReadLine 'Leer hasta que hay un terminador de la cadena
                Label2.Text = "Valor Leído: " + LecturaSerial 'Asignar a etiqueta 2 el valor leído
                i = i + 1 'Muestra actual
                Chart1.Series("Lectura").Points.AddXY(i.ToString, LecturaSerial) 'Actualizar el chart con los datos leídos
            End If
        End If

    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        If SerialPort1.IsOpen Then
            SerialPort1.Close() 'Cerrar puerto serie
            Timer1.Stop() 'Detener Timer
            MessageBox.Show("Desconectado...")
        Else
            MessageBox.Show("El puerto seleccionado no esta abierto...")
        End If
    End Sub


    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        ObtenerPuertos() 'Actualizar puertos disponibles
    End Sub

    Private Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click
        ObtenerPuertos()
    End Sub

    Public Sub ObtenerPuertos()
        ComboBox1.Items.Clear() 'Borrar Todos los items del combobox cuando se actualizan los puertos

        For Each Puerto As String In My.Computer.Ports.SerialPortNames 'Leer puertos disponibles en PC
            ComboBox1.Items.Add(Puerto)
        Next
    End Sub
End Class
