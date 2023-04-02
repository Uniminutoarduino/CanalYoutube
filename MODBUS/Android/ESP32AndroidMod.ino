#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif
#include "ModbusIP_ESP8266.h"

//Modbus Registers Offsets
const int LED=18; //Selección de GP18 para coil en tarjeta ESP32
const int LED_COIL=100;//Registro para LED o activación de Relé en dirección 100
//Objeto Modbus
ModbusIP mb;


void setup() {
    Serial.begin(115200);//Configuración de puerto serial a 11200 baudios
    pinMode(LED,OUTPUT);//LED como salida (coil)
    WiFi.begin("Tenda_B60BB8","ZDq9tEGe"); //SSID y contraseña
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());//Imprime IP local para saber la conexión en Android Studio
    mb.server();    //Iniciar Modbus en modo servidor
    mb.addCoil(LED_COIL);//Agregar coil y apuntarlo a registro
}

void loop() {
   //Llamar  función modbus para realizar la conexión y enlace con el coil seleccionado
   mb.task();
   digitalWrite(LED, mb.Coil(LED_COIL));//Escribir el coil o GP18 de la tarjeta deacuerdo a la información recibida por el protocolo Modbus desde la app
   delay(100);
}
