#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif
#include "ModbusIP_ESP8266.h"
#include "max6675.h"

int thermoDO = 19;
int thermoCS = 5;
int thermoCLK = 18;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

//Modbus Registers Offsets
//const int SENSOR_IREG = 100;
const int TEMREG = 100;
const int Coil = 101;
const int LED=22;
//ModbusIP object
ModbusIP mb; //Se crea el objeto modbus para ESP32

long ts;

void setup() {
    Serial.begin(115200);
 
    WiFi.begin("linksys","remotelab");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());//192.168.0.100
    pinMode(LED,OUTPUT);//LED como salida
    mb.server();    //Comenzar el ESP32 en modo servidor
    mb.addIreg(TEMREG); //Crear registro (input register) en la direccion seleccionada (100)-para Funcion (04) modbus
    mb.addCoil(Coil);//Agregar coil en dirección 101-para función (05) modbus
    ts = millis();
}

void loop() {
   //Call once inside loop() - all magic here
   mb.task();

   //Cada segundo actualizar el valor del registro con el valor del conversor análogo digital
   if (millis() > ts + 1000) {
       ts = millis();
       double Temperatura=thermocouple.readCelsius();//Leer temperatura de sensor MAX6675
       mb.Ireg(TEMREG,int(Temperatura));//Almacenar Temperatura en HoldingREG (TEMREG)
   }
   digitalWrite(LED, mb.Coil(Coil));
   delay(10);
}
