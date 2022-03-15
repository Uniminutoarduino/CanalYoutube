#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif
#include "ModbusIP_ESP8266.h"
#include "max6675.h"
//Configuración de pines para modulo MAX6675
int thermoDO = 19;
int thermoCS = 5;
int thermoCLK = 18;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO); //Configurar lectura de termocupla con MAX6675

//Offsets o direcciones de registros (Holding registers) para modbus
const int SENSOR_HREG = 0;
const int TERMO = 1;
//ModbusIP object
ModbusIP mb;

long ts;

void setup() {
    Serial.begin(115200);
    //Conectar con SSID de red local
    WiFi.begin("linksys");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP()); //Mostrar IP asignada

    mb.server();    //ESP32 en modo servidor
    mb.addHreg(SENSOR_HREG);//Crear Holding registers en la dirección seleccionada
    mb.addHreg(TERMO);
    ts = millis();
}
int i=0;
void loop() {
   //Llamar a librerias y funciones para protocolo MODBUS
   mb.task();

   //Leer temperatura e incrementar variable cada 2 segundos
   if (millis() > ts + 2000) {
       ts = millis();
       i++;//Incremento de variable
       if(i>100){ 
        i=0;
       }
       int Temperatura=thermocouple.readCelsius(); //Leer Temperatura en grados celsius
       mb.Hreg(SENSOR_HREG, i); //Escribir valor de contador y temperatura en los holding registers
       mb.Hreg(TERMO,Temperatura);
       Serial.print(mb.Hreg(SENSOR_HREG));
       Serial.println(mb.Hreg(TERMO));
   }
   delay(10);
}
