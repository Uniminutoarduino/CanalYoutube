/*----------------------------------------------------------------------
Código modificado tomado de ReadDemo proporcionado por los desarrolladores de Settimino
Código creado: 23 de abril/2022
Autor: taloselectrónico
//Si se comentarea la línea DO_IT SMALL incluir este buffer para la transmisión de datos

  Size=1024;
  Target = &Buffer; // Uses a larger buffer

    
----------------------------------------------------------------------*/
#include "Platform.h"
#include "Settimino.h"

// Uncomment next line to perform small and fast data access
#define DO_IT_SMALL //Transferencia usando Buffer interno-PDU máximo hasta 10 bytes, para transferir un mayor número 
//MAC de modulo W5100
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0F, 0x08, 0xE1 };

IPAddress Local(192,168,0,90); // IP de Arduino+W5100
IPAddress PLC(192,168,0,1);   // Dirección IP PLC
IPAddress Gateway(192, 168, 0, 3); //Dirección IP Gateway (Router)
IPAddress Subnet(255, 255, 255, 0); //Máscara de Red

int DBNum = 1; //Número de Datablock debe estar en el PLC (Configurado con Acceso Global)
//byte Buffer[20];//Buffer de datos, debe ser mayor a 10, maximo 1024 pero depende de la mamoria de Arduino
byte BufferW[1];//Buffer para escritura de salidas

S7Client Client; //Instancia de cliente S7

unsigned long Elapsed; // To calc the execution time
//----------------------------------------------------------------------
// Setup : Init Ethernet and Serial port
//----------------------------------------------------------------------
void setup() {
    //Abrir puerto Serial
    Serial.begin(115200);
//--------------------------------Wired Ethernet Shield Initialization    
    // Start the Ethernet Library
    EthernetInit(mac, Local);
    delay(2000); //Retraso de 2 segundos, mientras inicia modulo W5100
    Serial.println("");
    Serial.println("Conectado por cable...");  
    Serial.print("IP local");
    Serial.println(Ethernet.localIP());  
}
//----------------------------------------------------------------------
// Conexión a PLC
//----------------------------------------------------------------------
bool Connect()
{
    int Result=Client.ConnectTo(PLC, 
                                  0,  //Para PLC S7-1200 se pide el Rack 0
                                  0); //Para PLC S7-1200 se pide el Slot 0
    Serial.print("Connecting to ");Serial.println(PLC);  
    if (Result==0) 
    {
      Serial.print("Connected ! PDU Length = ");Serial.println(Client.GetPDULength());//Tamaño del PDU entre 240 bytes a 960 bytes
    }
    else
      Serial.println("Connection error");
    return Result==0;
}
//----------------------------------------------------------------------
// Función para convertir BigEndian (S7) a littleEndian (Arduino)
//----------------------------------------------------------------------
void Dump(void *Buffer, int Length)//Funcion para convertir de BigEndian a LittleEndian
{
  int i, cnt=0;
  pbyte buf;
  
  if (Buffer!=NULL)
    buf = pbyte(Buffer);
  else  
    buf = pbyte(&PDU.DATA[0]);

  Serial.print("[ Arreglando BigEndian->LittleEndian ");Serial.print(Length);
  Serial.println(" bytes ]===========================");
  for (i=0; i<Length; i++)
  {
    cnt++;
    if (buf[i]<0x10)
      Serial.print("0");
    Serial.print(buf[i], HEX);
    Serial.print(" ");
    if (cnt==16)
    {
      cnt=0;
      Serial.println();
    }
  }  
  Serial.println("===============================================");
}
//----------------------------------------------------------------------
// Funcoón para imprimir error de protoclo y tipo
//----------------------------------------------------------------------
void CheckError(int ErrNo)
{
  Serial.print("Error No. 0x");
  Serial.println(ErrNo, HEX);
  
  // Checks if it's a Severe Error => we need to disconnect
  if (ErrNo & 0x00FF)
  {
    Serial.println("SEVERE ERROR, disconnecting.");
    Client.Disconnect(); 
  }
}

//----------------------------------------------------------------------
// Main Loop
//----------------------------------------------------------------------
void loop() 
{
  int Size, Result;
  void *TargetB;
  
  
  // Connection
  while (!Client.Connected)
  {
    if (!Connect())
      delay(500);
  }
  
  Serial.print("Leyendo ");Serial.print(Size);Serial.println(" bytes");
  //Areas probadas en el ejemplo: S7AreaPE: Entradas, S7AreaPA: Salidas, S7AreaMW: Merkers->Bytes de memoria MB
  BufferW[0]=0x0F; //Byte para escribir en la salida, 0b00000001
  TargetB=&BufferW;
  Result=Client.WriteArea(S7AreaPA,0,0,1,TargetB); //Escribir en Área de salidas (Q0.0), etc. 
  delay(500);
}
