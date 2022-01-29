#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif
#include "ModbusIP_ESP8266.h"

//Modbus Registers Offsets
const int SENSOR_IREG = 100;
const int Coil = 101;
const int LED=22;
const int LED2=23;
int variableEnviar=0;//Variable entera a enviar
//ModbusIP object
ModbusIP mb; //Se crea el objeto modbus para ESP32

long ts;

void setup() {
    Serial.begin(115200);
 
    WiFi.begin("linksys");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    pinMode(LED,OUTPUT);//LED como salida
    mb.server();    //Comenzar el ESP32 en modo servidor
    mb.addIreg(SENSOR_IREG); //Crear registro en la direccion seleccionada (100)
    mb.addCoil(Coil);
    ts = millis();
}

void loop() {
   //Call once inside loop() - all magic here
   mb.task();

   //Cada dos segundo actualizar el valor del registro con el valor del conversor análogo digital
   if (millis() > ts + 1000) {
       ts = millis();
       float sensor=analogRead(A0);
       mb.Ireg(SENSOR_IREG, round(sensor)); //Aquí guardo el valor de la variable en el registro SENSOR_IREG. Este registro luego es leido por el software Modbus poll 
   }
   digitalWrite(LED, mb.Coil(Coil));
   delay(10);
}
