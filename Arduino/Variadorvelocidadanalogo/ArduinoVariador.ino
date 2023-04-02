/*  
Cambio de velocidad, setpoint analógo
Variador: Simatics G110
*/

/*                                                                                                                                 
 * Para este ejemplo conectar el módulo del potenciometro X9C103S así:
 * 1 - INC - Arduino pin 2
 * 2 - U/D - Arduino pin 3
 * 3 - VH  - 5V
 * 4 - VSS - GND
 * 5 - VW  - Pin de entrada para etapa de amplificación
 * 6 - VL  - GND
 * 7 - CS  - Arduino pin 4
 * 8 - VCC - 5V
 */


#include <DigiPotX9Cxxx.h>

DigiPot pot(2,3,4); //Configuración de pines
String Velocidad;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando");  
  Serial.println("Ingrese un valor entre 0 y 100 para cambiar la velocidad del motor entre 0 y 100%"); 
}

void loop() {
  if(Serial.available()>0){
    Velocidad=Serial.readString();//Leer datos de String como serial
    if(isDigit(Velocidad.charAt(0))){//Comprobar si el dato ingresado es un digito 0...9
      int VelEntero=Velocidad.toInt();
      Serial.println("Cambiando velocidad");
      DecrementarResistencia();//Decrementar resistencia hasta el minimo, siempre que haya un nuevo dato de velocidad, colocar el potenciometro a 0
      for (int i=0; i<VelEntero; i++) {
        pot.increase(1);
        delay(20);
    }
    }else{
      Serial.println("La velocidad introducida no es número");
    }
      
  }

  }



void DecrementarResistencia(void){
    for (int i=0; i<100; i++) {//Decrementar resistencia hasta el minimo
    pot.decrease(1);
    delay(20);
  }
}
