int muestra=0; //Valor de la muestra
float promedio=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//Iniciar puerto serial a 9600 baudios

}

void loop() {
  // put your main code here, to run repeatedly:
  int ADCVar=analogRead(A0);//Lectura ADC en canal AN0
  float voltaje=(5.0/1023.0)*ADCVar; //Conversion valor de ADC a voltaje.
  float temperatura=32.87*voltaje-44.834; //Ecuacion caracteristica del sensor PT100.
  promedio=promedio+temperatura;
  if(muestra>=10){
    promedio=promedio/10;//Asigancion de promedio
    Serial.print("Temperatura: ");
    Serial.println(promedio);
    promedio=0;
    muestra=0;
  }
  muestra=muestra+1;
  delay(100);//Tiempo de muestreo de 0.5 segundos.

}
