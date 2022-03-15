#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else
 #include <WiFi.h>
#endif
#include "ModbusIP_ESP8266.h"

const int REG = 0;               // Modbus dirección o Offset del holding register
IPAddress remote(192, 168, 0, 20);  // Esta es la dirección de un dispositivo servidor en este caso el PLCS7-1200
const int LOOP_COUNT = 2;

ModbusIP mb;  //Objeto MODBUS

void setup() {
  Serial.begin(115200);
 
  WiFi.begin("linksys"); //Este es el nombre del SSID, se puede añadir el password en el segundo argumento
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  mb.client(); //Inicio en modo cliente de ESP32
}

uint16_t res = 0;
uint8_t show = LOOP_COUNT;

void loop() {
  if (mb.isConnected(remote)) {   //Chequear si hay conexión con el servidor modbus
    mb.readHreg(remote, REG, &res);  //Leer un holding register en la dirección asignada en la IP remota.
  } else {
    mb.connect(remote);           //REconectar si hay problema
  }
  mb.task();                      // Ejecutar funciones de cliente modbus
  delay(100);                     // Pulling interval
  if (!show--) {                   // Se muestran los resultados de esta manera cada segundo, también se puede usar milis
    Serial.println(res);
    show = LOOP_COUNT;
  }
}
