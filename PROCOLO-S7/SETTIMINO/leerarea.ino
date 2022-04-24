/*----------------------------------------------------------------------
Código modificado tomado de ReadDemo proporcionado por los desarrolladores de Settimino
Código creado: 23 de abril/2022
Autor: taloselectrónico
//Si se comentarea la línea DO_IT SMALL iincluir este buffer ára la transmisión de datos

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

// Following constants are needed if you are connecting via WIFI
// The ssid is the name of my WIFI network (the password obviously is wrong)
//char ssid[] = "SKYNET-AIR";    // Your network SSID (name)
//char pass[] = "yourpassword";  // Your network password (if any)
IPAddress Gateway(192, 168, 0, 3); //Dirección IP Gateway (Router)
IPAddress Subnet(255, 255, 255, 0); //Máscara de Red

int DBNum = 1; //Número de Datablock debe estar en el PLC (Configurado con Acceso Global)
byte Buffer[20];//Buffer de datos, debe ser mayor a 10, maximo 1024 pero depende de la mamoria de Arduino
byte BufferW[1];//Buffer para escritura de salidas

S7Client Client; //Instancia de cliente S7

unsigned long Elapsed; // To calc the execution time
//----------------------------------------------------------------------
// Setup : Init Ethernet and Serial port
//----------------------------------------------------------------------
void setup() {
    //Abrir puerto Serial
    Serial.begin(115200);
    
/*#ifdef S7WIFI
//--------------------------------------------- ESP8266 Initialization    
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    WiFi.config(Local, Gateway, Subnet);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.print("Local IP address : ");
    Serial.println(WiFi.localIP());
#else*/
//--------------------------------Wired Ethernet Shield Initialization    
    // Start the Ethernet Library
    EthernetInit(mac, Local);
    delay(2000); //Retraso de 2 segundos, mientras inicia modulo W5100
    Serial.println("");
    Serial.println("Conectado por cable...");  
    Serial.print("IP local");
    Serial.println(Ethernet.localIP());
//#endif   
}
//----------------------------------------------------------------------
// Connects to the PLC
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
// Dumps a buffer (a very rough routine)
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
// Prints the Error number
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
// Profiling routines
//----------------------------------------------------------------------
/*void MarkTime()
{
  Elapsed=millis();
}
//----------------------------------------------------------------------
void ShowTime()
{
  // Calcs the time
  Elapsed=millis()-Elapsed;
  Serial.print("Job time (ms) : ");
  Serial.println(Elapsed);   
}*/
//----------------------------------------------------------------------
// Main Loop
//----------------------------------------------------------------------
void loop() 
{
  int Size, Result;
  void *Target;
  void *TargetB;

  Size=10;
  Target = NULL; // Uses the internal Buffer (PDU.DATA[])

  
  // Connection
  while (!Client.Connected)
  {
    if (!Connect())
      delay(500);
  }
  
  Serial.print("Reading ");Serial.print(Size);Serial.print(" bytes from DB");Serial.println(DBNum);
  // Get the current tick
  //MarkTime();
  //Areas probadas en el ejemplo: S7AreaPE: Entradas, S7AreaPA: Salidas, S7AreaMW: Merkers->Bytes de memoria MB
  Result=Client.ReadArea(S7AreaPE, //Area de lectura de entradas o DB, PE es para leer entradas, PA para leer salidas
                         DBNum,    // Número de DB
                         0,        //Comenzar en byte 0
                         Size,     //Leer 11 bytes
                         Target);  // Target es el buffer de almacenamiento de datos, si es NULL se almacenará internante, sino no se debe asignar la dirección del buffer da datos (ejm: Size=1024;Target = &Buffer;)
  if (Result==0)//Si resultado es 0 se recibierón los datos correctamente. Si el resultado mayor a 0 hubo un error
  {
    //ShowTime();
    Dump(Target, Size); //Arreglar el formato de Big Endian a Little Endian para Arduino
  }else{
    CheckError(Result);
  }
    
  delay(500);  
  BufferW[0]=0x01; //Byte para escribir en la salida, 
  TargetB=&BufferW;
  int Res=Client.WriteArea(S7AreaPA,0,0,1,TargetB); //Escribir en Área de salidas (Q0.0), etc. 
}
