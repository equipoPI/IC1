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

  TiempoHor = HoraTotal / 100;
  TiempoMin = HoraTotal % 100;
  TiempoHorUso = TiempoHor * 3600000;
  TiempoMinUso = TiempoMin * 60000;

  liquido1 = Ingrediente1 - 10000;
  liquido2 = Ingrediente2 - 20000;

  //inicio de transferencia de contenido de bombos 1 y 2 al bombo de mezcla
  if (liquido1 >= cantidad1 and liquido1 > 0) {
    //bomba dc correspondiente
  }

  if (liquido2 >= cantidad2 and liquido2 > 0) {
    //bomba dc correspondiente
  }

  //finalizacion de transferencia de contenido de bombos 1 y 2 al bombo de mezcla
  if (liquido1 <= cantidad1) {
    liquido1 = 0;
    cantidad1 = 0;
    //bomba dc correspondiente
  }

  if (liquido2 <= cantidad2) {
    liquido2 = 0;
    cantidad2 = 0;
    //bomba dc correspondiente
  }

  if (activarMezcla == 1) {
    //motor de mezclador
  }

  if (activarMezcla == 0) {
    //motor de mezclador
  }

  if (vaciar == 1) {
    //bomba del bombo de mezcla
  }
}
