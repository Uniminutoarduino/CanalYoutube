Public Class Form1

    Dim i As New Integer 'Variable to count the current sample
    Dim SerialRead As String 'Variable to save data from the serial port


    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        SerialPort1.PortName = ComboBox1.SelectedItem.ToString 'Get serial port name of the TTL to USB converter
        'Label2.Text = ComboBox1.SelectedItem.ToString
        SerialPort1.BaudRate = 9600 '9600 baudrate
        If SerialPort1.IsOpen Then 'Check if port is opened
            MessageBox.Show("Port opened, first close it") 'If open, first close
        Else
            Try
                SerialPort1.Open() 'Open serial port
                MessageBox.Show("Connected...") 'Show connected message
                Timer1.Interval = 1000 'Set timer with a period of 1 sec
                AddHandler Timer1.Tick, AddressOf UpdatePlot 'Handler to update the plot with data of the serial port
                Timer1.Start() 'Start Timer
            Catch
                MessageBox.Show("The port doesn't exist, check the port")
            End Try

        End If

    End Sub

    Public Sub UpdatePlot(sender As Object, e As EventArgs)

        If SerialPort1.IsOpen Then
            If SerialPort1.BytesToRead > 0 Then 'If there are bytes to read from serial port
                SerialRead = SerialPort1.ReadLine 'Read string until EOF found
                Label2.Text = "Reading value: " + SerialRead 'Write label text with the variable
                i = i + 1 'Current sample
                Chart1.Series("Reading").Points.AddXY(i.ToString, SerialRead) 'Update chart with new data
            End If
        End If

    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        If SerialPort1.IsOpen Then
            SerialPort1.Close() 'Close serial port
            Timer1.Stop() 'Stop timer
            MessageBox.Show("Disconnected...")
        Else
            MessageBox.Show("The selected port is closed...")
        End If
    End Sub


    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        GetPorts() 'Updata available ports
    End Sub

    Private Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click
        GetPorts()
    End Sub

    Public Sub GetPorts() 'Function to get the port available in the PC
        ComboBox1.Items.Clear() 'Borrar Todos los items del combobox cuando se actualizan los puertos

        For Each Port As String In My.Computer.Ports.SerialPortNames 'Read ports
            ComboBox1.Items.Add(Port)
        Next
    End Sub

    Private Sub Button4_Click(sender As Object, e As EventArgs) Handles Button4.Click
        'Command to Open clipper
        SerialPort1.Write("A")
    End Sub

    Private Sub Button6_Click(sender As Object, e As EventArgs) Handles Button6.Click
        'Command to move right
        SerialPort1.Write("D")
    End Sub

    Private Sub Button7_Click(sender As Object, e As EventArgs) Handles Button7.Click
        'Command to move left
        SerialPort1.Write("I")
    End Sub

    Private Sub Button5_Click(sender As Object, e As EventArgs) Handles Button5.Click
        'Command to Close clipper
        SerialPort1.Write("C")
    End Sub
End Class
