/*
    ========= VirtuinoBluetooth Class methods
   vPinMode(int pin, int state)                                  set pin as digital OUTPUT or digital INPUT.  (Insteed default pinMode method

  ========= Virtuino General methods=====hola mundo
   void vDigitalMemoryWrite(int digitalMemoryIndex, int value)   write a value to a Virtuino digital memory   (digitalMemoryIndex=0..31, value range = 0 or 1)
   int  vDigitalMemoryRead(int digitalMemoryIndex)               read  the value of a Virtuino digital memory (digitalMemoryIndex=0..31, returned value range = 0 or 1)
   void vMemoryWrite(int analogMemoryIndex, float value)         write a value to Virtuino float memory       (memoryIndex=0..31, value range as float value)
   float vMemoryRead(int analogMemoryIndex)                      read the value of  Virtuino analog memory    (analogMemoryIndex=0..31, returned value range = 0..1023)
   run()                                                         neccesary command to communicate with Virtuino android app  (on start of void loop)
   void vDelay(long milliseconds);                               Pauses the program (without block communication) for the amount of time (in miliseconds) specified as parameter
   int getPinValue(int pin)                                      read the value of a Pin. Usefull for PWM pins

  ========= Virtuino General methods======escritura en la memoria virtuino
   void vDigitalMemoryWrite(int digitalMemoryIndex, int value)   write a value to a Virtuino digital memory   (digitalMemoryIndex=0..31, value range = 0 or 1)
   int  vDigitalMemoryRead(int digitalMemoryIndex)               read  the value of a Virtuino digital memory (digitalMemoryIndex=0..31, returned value range = 0 or 1)
   void vMemoryWrite(int analogMemoryIndex, float value)         write a value to Virtuino float memory       (memoryIndex=0..31, value range as float value)
   float vMemoryRead(int analogMemoryIndex)                      read the value of  Virtuino analog memory    (analogMemoryIndex=0..31, returned value range = 0..1023)
   run()                                                         neccesary command to communicate with Virtuino android app  (on start of void loop)
   int getPinValue(int pin)                                      read the value of a Pin. Usefull for PWM pins
   void vDelay(long milliseconds);                               Pauses the program (without block communication) for the amount of time (in miliseconds) specified as parameter
   void clearTextBuffer();                                       Clear the text received text buffer
   int textAvailable();                                          Check if there is text in the received buffer
   String getText(byte ID);                                      Read the text from Virtuino app
   void sendText(byte ID, String text);                          Send text to Virtuino app
*/
int i = 0;
int x = 25;

int trig = 2;
int eco = 3;
float duracion;
float distancia;
float distancia1;
float distancia2;
float distancia3;

void setup() {

  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(3, INPUT);
  pinMode(5, INPUT);
  pinMode(7, INPUT);
  Serial.begin(9600);

}

void loop() {

  while (i <= 3) {
    digitalWrite(trig, HIGH);
    delay(1);
    digitalWrite(trig, LOW);
    duracion = pulseIn(eco, HIGH);
    distancia = duracion / 58.2;
    if (i==0) {
      distancia1 = distancia;
      Serial.print("distancia1:");
      Serial.println(distancia1);
    }
    if (i==1) {
      distancia2 = distancia;
      Serial.print("distancia2:");
      Serial.println(distancia2);
    }
    if (i==2) {
      distancia3 = distancia;
      Serial.print("distancia3:");
      Serial.println(distancia3);
    }
    trig = trig + 2;
    eco = eco + 2;
    i = i + 1; 
  }

  i = 0;
  trig = 2;
  eco = 3;

  if (distancia1 <= 3) {}
  if (distancia1 >= x) {}

  if (distancia2 <= 3) {}
  if (distancia2 >= x) {}

  if (distancia3 <= 3) {}
  if (distancia3 >= x) {}

  //bluethot
  //acordarce de que se toco la libreria virtuinobluethoot.h, ejemplo en video https://www.youtube.com/watch?v=gVpm7020-0A
  //comunicacion para controlar el llenado y la mezcla
  //control de bombas
  //control de caudal
  //sistema de mezclado

}
