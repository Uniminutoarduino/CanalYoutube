import socket
import sys
socketc=socket.socket(socket.AF_INET, socket.SOCK_STREAM) #Crear Socket
server_address = ('192.168.0.241', 2020) #Asignar IP y puerto para servidor
socketc.bind(server_address) #TCP 2020
print('starting up on {} port {}'.format(*server_address))
socketc.listen(1)
comando='1'
while True:
    # Wait for a connection
    print("Esperando conexion")
    connection, client_address = socketc.accept()
    try:
        print("Conexion desde IP...", client_address)

        # Recibir datos en chuncks (segmentos)
        while True:
            data = connection.recv(4)
            print("recibido {!r}".format(data))
            if data:
                print("Retornando datos al cliente")
                connection.sendall(comando)
            else:
                print("No hay datos desde cliente...", client_address)
                break

    finally:
        # Cerrar la conexion
        connection.close()
