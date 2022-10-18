#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif
#include "ModbusIP_ESP8266.h"
#include "max6675.h"
//Pines para modulo MAX6675
int thermoDO = 19;
int thermoCS = 5;
int thermoCLK = 18;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

//Modbus Registers Offsets
const int TERMO = 1;//Dirección de holding register para temperatura
const int REG=2; //Dirección de Holding Register para activar o desactivar salida desde KTP400
const int LED=22;//Salida que activa módulo Relé para activar motor trifásico
//ModbusIP object
ModbusIP mb;
//ModbusIP mb2;
IPAddress remote(192, 168, 0, 2);//Direccion IP de pantalla KTP400

long ts;

void setup() {
    Serial.begin(115200);
    pinMode(LED,OUTPUT);//LED como salida
    WiFi.begin("linksys","remotelab");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    mb.server();    //Start Modbus IP
    mb.addHreg(TERMO);
    mb.addHreg(REG);
    ts = millis();
}
int i=0;
uint16_t res = 0;
void loop() {
   //Call once inside loop() - all magic here
   mb.task();

   //Read each two seconds
   if (millis() > ts + 1000) {
       ts = millis();
       //Leer temperatura cada segundo y almacenarla en el respectivo Holding Register
       double Temperatura=thermocouple.readCelsius();
       mb.Hreg(TERMO,int(Temperatura));
       Serial.println(mb.Hreg(REG));
       if(mb.Hreg(REG)==1){//Encender el Motor deacuerdo al valor de la variable (TAG) "Encender" en TIA Portal
        digitalWrite(LED,LOW);
       }else{
        digitalWrite(LED,HIGH);
       }
   }
   delay(100);
}
