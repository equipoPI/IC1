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
      obtencionEntero();     //Llama la función que controla el valor a guradar el valor numerico de cada color
      flagR = 1;
      flagM = 0;
      flagParadaR = 0;
    }

    if (valor == 'G') {  //Si el dato entrante es una G, indica Bombo Reposicion
      g = 3;
      obtencionEntero();     //Llama la función que controla el valor a guradar el valor numerico de cada color
    }

    if (valor == 'S') {  //Si el dato entrante es una S, indica que se realizo el sete de los valores de reposicion en el celular
      g = 4;
      obtencionEntero();     //Llama la función que controla el valor a guradar el valor numerico de cada color
    }

    if (valor == 'C') {  //Si el dato entrante es una C
      g = 5;
      obtencionEntero();     //Llama la función que controla el valor a guradar el valor numerico de cada color
    }

    if (valor == 'c') {  //Si el dato entrante es una C
      g = 6;
      obtencionEntero();     //Llama la función que controla el valor a guradar el valor numerico de cada color
    }

    if (valor == 'V') {  //Si el dato entrante es una B
      g = 7;
      obtencionEntero();     //Llama la función que controla el valor a guradar el valor numerico de cada color
    }

    if (valor == 'D') {  //Si el dato entrante es una B
      g = 8;
      obtencionEntero();     //Llama la función que controla el valor a guradar el valor numerico de cada color
    }

    if (valor == 'B') {  //Si el dato entrante es una B
      g = 9;
      obtencionEntero();     //Llama la función que controla el valor a guradar el valor numerico de cada color
    }

    if (valor == 'A') {  //Si el dato entrante es una B
      g = 10;
      obtencionEntero();     //Llama la función que controla el valor a guradar el valor numerico de cada color
    }

    if (valor == 'T') {  //Si el dato entrante es una B
      flagTransmicion = 1;
      //Llama la función que controla el valor a guradar el valor numerico de cada color
    }

    if (valor == 'H') {  //Si el dato entrante es una B
      g = 11;
      obtencionEntero();     //Llama la función que controla el valor a guradar el valor numerico de cada color
      //Llama la función que controla el valor a guradar el valor numerico de cada color
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
      HoraTotal = estado.toInt();
    }

    g = 0;            //vuelve la variable g a 0
    estado = "";      //Limpia la variable para poder leer posteriormente nuevos datos
    Serial.print("valor recivido:");
    Serial.println(valor);
    Serial.print("valor:");
    Serial.println(HoraTotal);

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
