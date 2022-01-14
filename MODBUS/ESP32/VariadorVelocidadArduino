#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif
#include "ModbusIP_ESP8266.h"
#include "ModbusRTU.h"
#define ID_ESCLAVO 1
ModbusRTU mb1;
ModbusIP mb2;



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
  
  //Serial1.begin(9600, SERIAL_8N1); // Init Serial on default pins
  Serial2.begin(19200, SERIAL_8N1, 16, 17); // Override default pins for ESP32
  //Serial2.begin(19200,SERIAL_8N1,RXD2,TXD2); //SERIAL_8N1 8-bit No parity 1 Stop bit
  //mb1.begin(&Serial1);
  mb1.begin(&Serial2, 5);  // Specify RE_DE control pin
  //mb1.server(1);
  mb1.master();
  //mb1.addHreg(20);//Registro ne direccion 20
  mb2.server();
  mb2.addHreg(0); //Registro servidor TCP/IP (0 dirección)
  mb2.addHreg(1); //Registro servidor TCP/IP (Valor a apuntar al registro)
  mb2.addHreg(2); //Registro servidor TCP/IP (Comando:0 escritura, 1: lectura)
  mb2.addHreg(3); //Registro servidor TCP/IP (Valor de lectura obtenido del holding register)
  mb2.addHreg(4); //Registro servidor TCP/IP (Valor de lectura obtenido del holding register)
}



uint16_t resp=0; //Variable para almacenamiento de lectura de valor
int direccion = 0;
int valor = 0;
int comando=0; //0 escritura, 1 lectura
int lectura=0;
void loop() {
  //mb2.readHreg(remote, TO_REG, &res);  // Initiate Read Coil from Modbus Slave);
    //mb1.writeHreg(remote, 1, &res);
  //mb1.task();
  direccion= mb2.Hreg(0); //Registro 0 es la dirección o offset del registro a escribir
  valor=mb2.Hreg(1);//1 es el valor a apuntar a ese registro
  comando=mb2.Hreg(2);//Lectura de comando para variador
  //mb1.Hreg(20,res);
  if (!mb1.slave()) {
    if(comando==0&&direccion<4){//Si dirección esta en el rango de 1 a 3 la operación de escritura y lectura es valida
      mb1.writeHreg(ID_ESCLAVO,direccion,valor);
    }else{
      mb1.readHreg(ID_ESCLAVO,direccion,&resp,1); //Lectura de 1 holding register en la dirección o offset indicados
      mb2.Hreg(3,resp);
      lectura=mb2.Hreg(3);
    }
  }
  mb1.task();
  mb2.task();
  delay(50);
  Serial.print("valor: ");
  Serial.print(valor);
  Serial.print(" comando: ");
  Serial.print(comando);
  Serial.print(" direccion: ");
  Serial.print(direccion);
  Serial.print(" lectura: ");
  Serial.println(lectura);
}
