#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

const char* ssid = "Tenda_B60BB8"; //SSID
const char* password = "ZDq9tEGe"; //Password
Adafruit_SHT31 sht31 = Adafruit_SHT31(); //Iniciar instancia sensor SHT31
String TempS;
String HumS;
int block=1; //Variable para iniciar comunicación y enviar datos por plotly.js
String mensaje;

using namespace websockets;
WebsocketsClient client; //Crear un cliente para websocket-
WebsocketsServer server;
void setup() {
  Serial.begin(115200);
  //Conectar a Wifi
  WiFi.begin(ssid, password);

  //Espera a que se conecte
  for(int i = 0; i < 15 && WiFi.status() != WL_CONNECTED; i++) {
      Serial.print(".");
      delay(1000);
  }
  //Mostrar IP asignada
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP

  server.listen(8003); //Servidor en puerto 8003
  Serial.print("Servidor Activo?");
  Serial.println(server.available());
  if (! sht31.begin(0x44)) {   //Esperar a que haya conexión con SHT31 en la dirección 0x44
    Serial.println("No se encuentra SHT31");
    while (1) delay(1);
  }
}

void loop() {
  float t = sht31.readTemperature(); //Leer temperatura
  float h = sht31.readHumidity(); //Leer humedad
  //Imprimir en consola temperatura y humedad
  Serial.print("Humidity(%RH): ");
  Serial.println(h);
  Serial.print(" Temperature(C): ");
  Serial.println(t);
  TempS=String(t);//Convertir temperatura en String para enviarla por websocket
  HumS=String(h); //Convertir humedad en String para enviarla por websocket
  if(block==1){//Si dato leido es 1 entonces crear la conexión y generar recepción continua
    client = server.accept();
    client.poll();
    block=2;
  }
  
  if(client.available()) {
    auto msg = client.readBlocking();
    Serial.print("Leyendo...");
    Serial.println(msg.data());//Imprimiendo data leido desde página con plotly (Cliente)
    client.send(TempS+","+HumS);
    //Cerrar la conexión si dato leido es 2 (comando)
    if(msg.data()=="2"){
      client.close();
    }
  }
  delay(50);//Retraso de 50ms
}
