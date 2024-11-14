#include <TimerFive.h>         //libreria que usaremos para leer siempre en el mismo instante de tiempo los datos bluethoot
#include <SoftwareSerial.h>    //libreria que usaremos para transmitir y recibir los datos que llegan desde el modulo bluetooth

int BT_Rx = 12;                //pines conectados al modulo bluetooth {cambiar la conexion al 10  y al 11 que son los que funcionaron en el practivo de comunicaicon de datos}
int BT_Tx = 11;

SoftwareSerial BT(BT_Rx, BT_Tx); //definimos con la libreria SoftwareSerial la comunicacion arduino, siempre que se mencione BT.comandoserial
//se ejecutara el comando serialsolo que utilizando los pines definidos anterormente
//esto lo hacemos con la finalidad de no estar conectando y desconectando el modulo bluetooth cada vez que se carga un codigo
//nuevo y tambien no queda disponible el puerto serie para ver datos de control en la PC.

//variables temporales del loop, cada cuanto enviar datos por bluethoot y puerto serie
unsigned long tiempoEnvio = 0;
unsigned long tiempoMonitoreo = 0;
unsigned long TInicioMezclado = 0;
unsigned long previousMillis = 0;
unsigned long TiempoMotorOn = 5000;
unsigned long TiempoMotorOff = 2000;
unsigned long currentMillis = 0; // Tiempo actual

//variables control de transmicion bluetooth
int g = 0;
char valor = 'F';
String estado;
byte flagTransmicion = 1;
byte continuar = 0;

//variables de recepcion bluetooth
int convinacion = 0; //numero que indicara la canatidad a reponer + bombo
int bomboSeleccionado = 0;
int valorMaxReposicion = 0;
byte activarMezcla = 0;
byte vaciar = 0;

//variables estados de motores
byte flagParadaR = 0; //controla el parado de emergencia de la reposicion
byte flagR = 0;       //detecta si los niveles que se envian son para Reposicion
byte flagM = 0;       //detecta si los niveles que se envian son para Mezcla
byte EMezclador = 0;
byte EBomba1 = 0;
byte EBomba2 = 0;
byte EBombaM = 0;
byte EBombaR = 0;
byte EValvula1 = 0;
byte EValvula2 = 0;
byte EProceso = 0;
byte EBomboM = 0;
int horaRest = 0;
int minRest = 0;
int error = 0;
byte desechar = 0;
byte arranque2 = 0;
byte detener = 0;

//variables control de nivel
int i = 0;
int x = 25;
int bandera1 = 0;
float alpha = 0.5;  // Factor de suavizado, entre 0 y 1 para reducir oscilacion sensores
//trig y eco son pines de control de los sensores
int trig = 16;
int eco = 17;
//duracion es el tiempo que tarda en enviarce y recibir el rebote de las ondas de ultrasonido
float duracion;
//distancia es donde se almacena un valor derivado de una opercaion matematica
float distancia;
//son variables donde se gurada el nivel de cada bombo
float distancia1;
float distancia2;
float distancia3;
byte constrainedPorcentaje1 = 0;
byte constrainedPorcentaje2 = 0;
byte constrainedPorcentaje3 = 0;

//mediciones de distancias filtradas
byte Fporcentaje1 = 0;
byte Fporcentaje2 = 0;
byte Fporcentaje3 = 0;

//estadistica
#define NUM_READINGS 15  // Número de lecturas a promediar

float readings1[NUM_READINGS];  // Arreglo para almacenar las lecturas
float readings2[NUM_READINGS];  // Arreglo para almacenar las lecturas
float readings3[NUM_READINGS];  // Arreglo para almacenar las lecturas
int readIndex = 0;  // Índice para la lectura actual
float total1 = 0;  // Suma de las lecturas
float total2 = 0;  // Suma de las lecturas
float total3 = 0;  // Suma de las lecturas
float average1 = 0;  // Promedio de las lecturas
float average2 = 0;  // Promedio de las lecturas
float average3 = 0;  // Promedio de las lecturas

//variables control de caudal
volatile double waterFlow1;
volatile double waterFlow2;
double Ingrediente1 = 0;
double Ingrediente2 = 0;
double cantidad1 = 0;
double cantidad2 = 0;
double liquido1 = 0;
double liquido2 = 0;
int bandera_c = 1;
byte terminoLlenadoLiquido1 = 0;  //banderas de estado que activan el motor de mezcla cuando termina el
byte terminoLlenadoLiquido2 = 0;  //proceso de llenado del bombo de mezcla

//variables de tiempo de duracion de la mezcla
int TiempoHor = 0;  //en estas variables se guradara el tiempo que se envia desde la app y es el que debera de durar el mezclado
int TiempoMin = 0;
long TiempoHorUso = 0;
long TiempoMinUso = TiempoMin * 60000;
byte MotorOn = 1;
byte MotorOff = 0;


void setup() {
  //configuracion puerto serie para mandar señales de control a la PC
  Serial.begin(9600);
  Serial.print("Listo");

  //pines de sensores ultrasonicos, nivel
  pinMode(17, INPUT);  //outputs son los pines de triger
  pinMode(19, INPUT);  //input son los pines de eco
  pinMode(21, INPUT);
  pinMode(16, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(20, OUTPUT);

  //pines de bombas
  //Bombas dc y motor de mezcla dc
  pinMode(4, OUTPUT); //bomba deposito m
  pinMode(5, OUTPUT); //bomba deposito 2
  pinMode(6, OUTPUT); //bomba deposito 1
  pinMode(7, OUTPUT);
  //Bomba ac y electrovalvulas ac
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);

  //Bombas dc y motor de mezcla dc
  digitalWrite(4, HIGH);  //in 4, bomba deposito m
  digitalWrite(5, HIGH);  //in 3, bomba deposito 1
  digitalWrite(6, HIGH);   //in 2, bomba deposito 2
  digitalWrite(7, HIGH);  //in 1 , mezclador
  //Bomba ac y electrovalvulas ac
  digitalWrite(8, HIGH); //in 4 electrovalvula a Bombo 2
  digitalWrite(9, HIGH); //in 3 es la bomba del camión
  digitalWrite(10, HIGH); //in 2 electrovalvula a Bombo 1
  digitalWrite(13, LOW); //no c utiliza

  //configuracion de caudal
  waterFlow1 = 0;
  waterFlow2 = 0;

  attachInterrupt(digitalPinToInterrupt(2), pulse1, RISING);  //DIGITAL Pin 2: interrupcion externa con la llegada de un pulso del caudalimetro
  attachInterrupt(digitalPinToInterrupt(3), pulse2, RISING);  //DIGITAL Pin 3: interrupcion externa con la llegada de un pulso del caudalimetro

  //configuracion bluethooot
  BT.begin(9600);

  for (int K = 0; K < NUM_READINGS; K++) { //esta funcion se encarga de poner todas las posiciones de las cadenas que se usaran para
    readings1[K] = 0;                      //hacer los calculos estadisticos
    readings2[K] = 0;
    readings3[K] = 0;
  }

  Timer5.initialize(250000);        //configuramos el timer para que cada 200 ms lea los datos que vienen del arduino mediante la funcion lectura,
  //la configuracion se debe de escibir en micro segundos
  Timer5.attachInterrupt(lectura);  //funcion encargada de recibir variables tipo texto desde el modulo, a su vez tambien
  //transforma esos datos en variables numericas aptas para su trabajo
}

void loop() {
  //Funcion de monitoreo a travez del PC
  monitoreo();
  if ((tiempoMonitoreo + 2000) <= millis()) {
    //monitoreo();                          //envia datos al PC para control interno del programa
    tiempoMonitoreo = millis();
  }

  //control de nivel de depositos
  nivel();                                  //esta funcion se encarga de obtener la distancia del sensor
  filtrado();                               //esta funcion utiliza calculos de estadisticas aplicados a los sensores
  //esto se hace con el fin de eliminar medidas incorrectas

  //funciones relacionadas con Bluetooth
  if ((tiempoEnvio + 1000) <= millis()) {
    enviarValores();             //envia datos al modulo BT para retroalimentacion a la aplicacion del celular
    tiempoEnvio = millis();
  }

  //comunicacion para controlar el llenado y la mezcla
  //control de bomba a reposicoin de bombos
  activacion();

  //control de bombas a bombo de mezcla

  //control de caudal
  caudal();

  //sistema de mezclado

}


//###ventana bluetooth###
//esta funciones hay que actualizarlas porque daran error si se ejecutan de esta forma, se tomaro de ejemplo de un codigo
//el codigo es CodControl del TP3 de comunicacion de datos de 4 año de ing en comp

void lectura() {
  if (BT.available()) { //Si el puerto serie (Bluetooth) está disponible
    valor = BT.read();  //Lee el dato entrante via Bluetooth


    if (valor == 'F') { //Si el dato entrante es una F, se para la reposicion
      g = 1;
      frenadoReposicion();
      flagParadaR = 1;
    }

    if (valor == 'R') {  //Si el dato entrante es una R, indica cantidad/max de reposicion
      g = 2;
      obtencionEntero();     //Llama la función que controla el valor a guradar el valor
      flagR = 1;
      flagM = 0;
      flagParadaR = 0;
    }

    if (valor == 'G') {  //Si el dato entrante es una G, indica Bombo Reposicion
      g = 3;
      obtencionEntero();     //Llama la función que controla el valor a guradar el valor
    }

    if (valor == 'S') {  //Si el dato entrante es una S, indica que se realizo el seteo de los valores de reposicion en el celular
      g = 4;
      obtencionEntero();     //Llama la función que controla el valor a guradar el valor
    }

    if (valor == 'C') {  //Si el dato entrante es una C indica el valor de Ingrediente y comenzar
      g = 5;
      obtencionEntero();     //Llama la función que controla el valor a guradar el valor
      continuar = 1;
    }

    if (valor == 'c') {  //Si el dato entrante es una c indica el valor de Ingrediente2 y comenzar
      g = 6;
      obtencionEntero();     //Llama la función que controla el valor a guradar el valor
    }

    if (valor == 'V') {  //Si el dato entrante es una V, llega una señal de vaciar o de desechar. Activa la bomba del Bombo de Mezcla
      g = 7;
      vaciar = 1;
      obtencionEntero();     //Llama la función que controla el valor a guradar el valor
    }

    if (valor == 'D') {  //Si el dato entrante es una D se detiene el proceso de mezclado
      g = 8;
      activarMezcla = 0;
      continuar = 0;
      detener = 1;
    }

    if (valor == 'A') {  //Si el dato entrante es una A se continua el mezclado
      g = 10;
      // obtencionEntero();     //Llama la función que controla el valor a guradar el valor
      continuar = 1;
    }

    if (valor == 'T') {  //Si el dato entrante es una T
      flagTransmicion = 1;
      //Llama la función que controla el valor a guradar el valor
    }

    if (valor == 'H') {  //Si el dato entrante es una H indica que le esta llegando un valor a referencia de Horas
      g = 11;
      obtencionEntero();     //Llama la función que controla el valor a guradar
    }

    if (valor == 'h') {  //Si el dato entrante es una H indica que le esta llegando un valor a referencia de minutos
      g = 12;
      obtencionEntero();     //Llama la función que controla el valor a guradar
    }

    if (valor == 'X') {  //Si el dato entrante es una H indica que le esta llegando un valor a referencia de minutos
      desechar = 1;   //Llama la función que controla el valor a guradar
    }
  }
}

void obtencionEntero() {
  delay(30);
  while (BT.available()) {
    char c = BT.read();   //Lee el dato entrante y lo almacena en una variable tipo char
    estado += c;     //Crea una cadena tipo String con los datos entrates
  }

  if (estado.length() > 0) { //Se verifica que la cadena tipo String tenga un largo mayor a cero

    if (g == 1) {                  //dependiendo del valor de g
      //registroR = estado.toInt();  //Guarda en un registro el dato en forma de entero (int)
    }

    if (g == 2) {
      //registroG = estado.toInt();  //Guarda en un registro el dato en forma de entero (int)
      convinacion = estado.toInt();
    }

    if (g == 3) {
      bomboSeleccionado = estado.toInt();
    }

    if (g == 5) {
      Ingrediente1 = estado.toInt();
    }

    if (g == 6) {
      Ingrediente2 = estado.toInt();
    }

    if (g == 11) {
      TiempoHor = estado.toInt();
    }

    if (g == 12) {
      TiempoMin = estado.toInt();
    }

    g = 0;            //vuelve la variable g a 0
    estado = "";      //Limpia la variable para poder leer posteriormente nuevos datos

    Serial.print("valor recivido:");
    Serial.println(TiempoHor);

  }
}

void enviarValores() {
  if (flagTransmicion == 1) {
    // Enviar los valores leidos a la aplicación a través de Bluetooth
    // Se ejecuta cada vez que se recibe el caracter que indica una nueva conexion ('E')
    //La transmicion se realiza de esta forma ya que la aplicacion android lo recibira todo como texto y utilizara
    // | para delimitar cada valor y asignare a cada uno una posicion en una lista que podra ser interpretada como numero
    //eso no ayudara para mostrar o visualizar todos los datos que queramos
    BT.print(average1);                     //BT.print(porcentajeReal1);
    BT.print("|");
    BT.print(constrainedPorcentaje1);         //BT.print(porcentajeReal1);
    BT.print("|");
    BT.print(average2);                     //BT.print(distanciaReal2);
    BT.print("|");
    BT.print(constrainedPorcentaje2);         //BT.print(porcentajeReal2);
    BT.print("|");
    BT.print(average3);                     //BT.print(distanciaReal3);
    BT.print("|");
    BT.print(constrainedPorcentaje3);         //BT.print(porcentajeReal3);
    BT.print("|");
    BT.print(cantidad1);
    BT.print("|");
    BT.print(cantidad2);
    BT.print("|");
    BT.print(EBomba1);
    BT.print("|");
    BT.print(EBomba2);
    BT.print("|");
    BT.print(EBombaM);
    BT.print("|");
    BT.print(EMezclador);
    BT.print("|");
    BT.print(EBombaR);
    BT.print("|");
    BT.print(error);
    BT.print("|");
    BT.print(horaRest);
    BT.print("|");
    BT.print(minRest);
    BT.print("|");
    BT.print(EProceso);
    BT.print("|");
    BT.print(EBomboM);
    BT.print("\n"); // Fin de línea. Importante.
    // t = t + 1;
    //flagTransmicion = 0;
  }
}


//###ventana control_bombas###

void frenadoReposicion() {
  flagParadaR = 1;
  EBombaR = 0;
  //apagado bomba repo
  digitalWrite(9, HIGH);
  bomboSeleccionado = 0;
  //apagado electrovalvula
  digitalWrite(10, HIGH); //in 2 electrovalvula a Bombo 1
  //apagado electrovalvula
  digitalWrite(8, HIGH); //in 4 electrovalvula a Bombo 2
}

void activacion() {
  if (convinacion >= 1000 and convinacion <= 1100) {
    bomboSeleccionado = 1;
    valorMaxReposicion = convinacion - 1000;
  }

  if (convinacion >= 2000 and convinacion <= 2200) {
    bomboSeleccionado = 2;
    valorMaxReposicion = convinacion - 2000;
  }

  if (flagParadaR == 0) { //en el caso de que se presione parar por algun motivo se desactivan la siguientes
    //ejecuciones y se deberan cargar los valores nuevamente si se quiere reanudar
    if (bomboSeleccionado == 1) {
      if (valorMaxReposicion <= Fporcentaje1 and EBombaR == 0) {
        error = 722;
      }

      if (valorMaxReposicion > Fporcentaje1 and Fporcentaje1 < 100) {
        EBombaR = 1;
        //encendido electrovalvula
        digitalWrite(10, LOW); //in 2 electrovalvula a Bombo 1
        //encendido bomba repo
        digitalWrite(9, LOW);
      }

      if (valorMaxReposicion <= Fporcentaje1) {
        EBombaR = 0;
        flagParadaR = 1;
        valorMaxReposicion = 0;
        //apagado bomba repo
        digitalWrite(9, HIGH);
        bomboSeleccionado = 0;
        //apagado electrovalvula
        digitalWrite(10, HIGH); //in 2 electrovalvula a Bombo 1
      }
    }

    if (bomboSeleccionado == 2) {
      if (valorMaxReposicion <= Fporcentaje2 and EBombaR == 0) {
        error = 722;
      }

      if (valorMaxReposicion > Fporcentaje2 and Fporcentaje2 < 100) {
        EBombaR = 1;
        //encendido bomba repo
        digitalWrite(9, LOW);
        //encendido electrovalvula
        digitalWrite(8, LOW); //in 4 electrovalvula a Bombo 2
      }

      if (valorMaxReposicion <= Fporcentaje2 or Fporcentaje2 == 100) {
        EBombaR = 0;
        flagParadaR = 1;
        valorMaxReposicion = 0;
        //apagado bomba repo
        digitalWrite(9, HIGH);
        //apagado electrovalvula
        digitalWrite(8, HIGH); //in 4 electrovalvula a Bombo 2
        bomboSeleccionado = 0;
      }
    }
  }
  /* byte EMezclador = false;
    byte EBomba1 = false;
    byte EBomba2 = false;
    byte EValvula1 = false;
    byte EValvula2 = false;*/


  TiempoHorUso = TiempoHor * 3600000;
  TiempoMinUso = TiempoMin * 60000;

  liquido1 = (Ingrediente1 - 10000) / 1000;
  liquido2 = (Ingrediente2 - 20000) / 1000;


  if (continuar == 1) {
    //inicio de transferencia de contenido de bombos 1 y 2 al bombo de mezcla
    if (liquido1 > cantidad1 and liquido1 > 0) {
      //bomba dc correspondiente
      digitalWrite(5, LOW);  //in 3, bomba deposito 1
      EBomba1 = 1;
      EProceso = 1;
    }

    if (liquido2 > cantidad2 and liquido2 > 0 and arranque2 == 1) {
      //bomba dc correspondiente
      digitalWrite(6, LOW);   //in 2, bomba deposito 2
      EBomba2 = 1;
      EProceso = 1;
    }

    //finalizacion de transferencia de contenido de bombos 1 y 2 al bombo de mezcla
    if (liquido1 < cantidad1) {
      terminoLlenadoLiquido1 = 1;
      //bomba dc correspondiente
      digitalWrite(5, HIGH);  //in 3, bomba deposito 1
      EBomba1 = 0;
      arranque2 = 1;
    }

    if (liquido2 < cantidad2 and terminoLlenadoLiquido1 == 1) {
      terminoLlenadoLiquido2 = 1;
      //bomba dc correspondiente
      digitalWrite(6, HIGH);   //in 2, bomba deposito 2
      EBomba2 = 0;
      arranque2 = 0;
    }

    if (terminoLlenadoLiquido1 == 1 and terminoLlenadoLiquido2 == 1) {
      activarMezcla == 1;
      terminoLlenadoLiquido1 = 0;
      terminoLlenadoLiquido2 = 0;
      liquido1 = 0;
      liquido2 = 0;
      TInicioMezclado = millis();
      currentMillis = millis();
    }
  }

  if (activarMezcla == 1) {
    
    EProceso = 1;

    // Encender el motor por 5 segundos
    if ((currentMillis - previousMillis) >= TiempoMotorOn and MotorOn == 1) {
      //apagar el motor
      digitalWrite(7, HIGH);  //in 1 , mezclador
      previousMillis = currentMillis;
      MotorOn = 0;
      MotorOff = 1;
    }

    // Apagar el motor por 2 segundos
    if ((currentMillis - previousMillis) >= TiempoMotorOff and MotorOff == 1) {
      //encender motor
      digitalWrite(7, LOW);  //in 1 , mezclador
      previousMillis = currentMillis;
      MotorOn = 1;
      MotorOff = 0;
    }
    //motor de mezclador
  }

  if ((activarMezcla == 0 and continuar == 0) or detener == 1) {
    EProceso = 0;
    //apagar motor de mezclador
    digitalWrite(7, HIGH);  //in 1 , mezclador

    //apagado bomba Bombo1
    digitalWrite(5, HIGH);  //in 3, bomba deposito 1
    EBomba1 = 0;

    //apagado bomba Bombo2
    digitalWrite(6, HIGH);   //in 2, bomba deposito 2
    EBomba2 = 0;
    desechar = 0;
    detener = 0;
    digitalWrite(4, HIGH);  //in 4, bomba deposito m
  }

  if (desechar == 1) {
    //prender bomba del bombo de mezcla
    digitalWrite(4, LOW);  //in 4, bomba deposito m
    EBombaM = 1;
    liquido1 = 0;
    cantidad1 = 0;
    liquido2 = 0;
    cantidad2 = 0;
  }

  if (vaciar == 1) {
    if (EProceso == 2) {
      //prender bomba del bombo de mezcla
      digitalWrite(4, LOW);  //in 4, bomba deposito m
      EBombaM = 1;
      liquido1 = 0;
      cantidad1 = 0;
      liquido2 = 0;
      cantidad2 = 0;
    }

    if (EProceso == 0 and EProceso == 1) {
      digitalWrite(4, LOW);  //in 4, bomba deposito m
      EBombaM = 1;
      liquido1 = 0;
      cantidad1 = 0;
      liquido2 = 0;
      cantidad2 = 0;

      //apagado bomba Bombo1
      digitalWrite(5, HIGH);  //in 3, bomba deposito 1
      EBomba1 = 0;

      //apagado bomba Bombo2
      digitalWrite(6, HIGH);   //in 2, bomba deposito 2
      EBomba2 = 0;
    }
  }

  if (constrainedPorcentaje3 < 15) {
    digitalWrite(4, HIGH);  //in 4, bomba deposito m
    vaciar = 0;
    EBombaM = 0;
    desechar = 0;
  }

  if (activarMezcla == 1) {
    horaRest = ((TInicioMezclado + TiempoMin + TiempoHor) - millis()) / 3600000;
    minRest = ((TInicioMezclado + TiempoMin + TiempoHor) - millis()) / 60000;

    if ((TInicioMezclado + TiempoMin + TiempoHor) < millis()) {
      liquido1 = 0;
      cantidad1 = 0;
      liquido2 = 0;
      cantidad2 = 0;
      activarMezcla == 0;
      EProceso = 2;
      terminoLlenadoLiquido1=0;
      terminoLlenadoLiquido2=0;
      arranque2=0;
    }
  }
}

//###ventana estadistica_filtros###
void filtrado() { // Restar la lectura más antigua de la suma total

  total1 = total1 - readings1[readIndex];
  total2 = total2 - readings2[readIndex];
  total3 = total3 - readings3[readIndex];

  // guardar los valores de distancia
  readings1[readIndex] = distancia1;
  readings2[readIndex] = distancia2;
  readings3[readIndex] = distancia3;

  // Añadir la nueva lectura a la suma total
  total1 = total1 + readings1[readIndex];
  total2 = total2 + readings2[readIndex];
  total3 = total3 + readings3[readIndex];

  // Avanzar al próximo índice
  readIndex = readIndex + 1;

  // Si llegamos al final del arreglo, volver al inicio
  if (readIndex >= NUM_READINGS) {
    readIndex = 0;
  }

  // Calcular el promedio
  average1 = total1 / NUM_READINGS;
  average2 = total2 / NUM_READINGS;
  average3 = total3 / NUM_READINGS;

  Fporcentaje1 = map(average1, 27, 6, 0, 100);
  Fporcentaje2 = map(average2, 27, 6, 0, 100);
  Fporcentaje3 = map(average3, 27, 6, 0, 100);

  constrainedPorcentaje1 = constrain(Fporcentaje1, 0, 100); // Limitar el valor para que no se pase de 100
  constrainedPorcentaje2 = constrain(Fporcentaje2, 0, 100); // Limitar el valor para que no se pase de 100
  constrainedPorcentaje3 = constrain(Fporcentaje3, 0, 100); // Limitar el valor para que no se pase de 100

}

//###ventana funcion_de_caudal###
void pulse1()  //medir la cantidad de flujo de agua
{
  waterFlow1 += 1.0 / 450;        //450 es el valor para el caudalimetro de 1/2 pulgada, nosotros tenemos 1/4 pulgada
}                                 // y su valor es 5880

void pulse2()   //medir la cantidad de flujo de agua
{
  if (terminoLlenadoLiquido1 == 1) {
    waterFlow2 += 1.0 / 450;        //450 es el valor para el caudalimetro de 1/2 pulgada, nosotros tenemos 1/4 pulgada
  }                                 // y su valor es 5880
}

void caudal() {
  cantidad1 = waterFlow1;
  cantidad2 = waterFlow2;
}

//###ventana funcion_de_nivel###
void nivel() {
  while (i <= 3) {                      //tiene que ser menor que 3 para que se realice una vez por cada sensor
    digitalWrite(trig, HIGH);
    delayMicroseconds(5);
    digitalWrite(trig, LOW);
    duracion = pulseIn(eco, HIGH);      //funcion que detecta cuanto tarda en llegar la señal al arduino por el pin de eco
    distancia = duracion / 58.2;        //calcula la distancia en funcion del valor encontrado anteriormente, el valor utilizado para el calculo es
    //sacado de la data sheet
    if (i == 0) {                       //en funcion de en que vez de ejecucion se encuentre el bucle el valor obtenido de distancia se guarda en una variable diferente
      distancia2 = distancia;
    }

    if (i == 1) {
      distancia3 = distancia;
    }

    if (i == 2) {
      distancia1 = distancia;
    }

    trig = trig + 2;                     //se incrementan las variables de los pines a leer y poner en halto en dos para ir cambiando de sensor
    eco = eco + 2;
    i = i + 1;                           //se incrementa el valor de i en 1
  }

  i = 0;                                 //se resetean los valores de los pines y de i una vez que se termino de ejecutar el bucle
  trig = 16;
  eco = 17;
}

//###ventana funciones_de_monitoreo###
void monitoreo() {
  /*Serial.print("distancia1:");
    Serial.print(distancia1);
    Serial.print("  distancia2:");
    Serial.print(distancia2);
    Serial.print("  distancia3:");
    Serial.println(distancia3);
    /*Serial.print("Fdistancia1:");
    Serial.print(filtroDistancia1);
    Serial.print("  Fdistancia2:");
    Serial.print(filtroDistancia2);
    Serial.print("  Fdistancia3:");
    Serial.println(filtroDistancia3);
    Serial.print("Porcentaje1:");
    Serial.print(porcentaje1);
    Serial.print("  Porcentaje2:");
    Serial.print(porcentaje2);
    Serial.print("  Porcentaje3:");
    Serial.println(porcentaje3);
    Serial.print("FPorcentaje1:");
    Serial.print(Fporcentaje1);
    Serial.print("  FPorcentaje2:");
    Serial.print(Fporcentaje2);
    Serial.print("  FPorcentaje3:");
    Serial.println(Fporcentaje3);
    /*Serial.print("Porcentaje1C:");
    Serial.print(constrainedPorcentaje1);
    Serial.print("  Porcentaje2C:");
    Serial.print(constrainedPorcentaje2);
    Serial.print("  Porcentaje3C:");
    Serial.println(constrainedPorcentaje3);
    Serial.print("Porcentaje1R:");
    Serial.print(porcentajeReal1);
    Serial.print("  Porcentaje2R:");
    Serial.print(porcentajeReal2);
    Serial.print("  Porcentaje3R:");
    Serial.println(porcentajeReal3);*/
  Serial.println(valor);
  Serial.print("TiempoHor:");
  Serial.print( TiempoHor);
  Serial.print("  TiempoMin:");
  Serial.println( TiempoMin);
  Serial.print("Cantidad1:");
  Serial.print( cantidad1);
  Serial.print("  Cantidad2:");
  Serial.println( cantidad2);
  Serial.print("activarMezcla:");
  Serial.print( activarMezcla);
  Serial.print("  terminoLlenadoLiquido1:");
  Serial.print( terminoLlenadoLiquido1);
  Serial.print("  terminoLlenadoLiquido2:");
  Serial.println( terminoLlenadoLiquido2);
  Serial.print(" arranque2:");
  Serial.println(arranque2);
  
}
