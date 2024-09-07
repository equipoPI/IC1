void pulse1()  //medir la cantidad de flujo de agua
{
  waterFlow1 += 1.0 / 450.0;        //450 es el valor para el caudalimetro de 1/2 pulgada, nosotros tenemos 1/4 pulgada
}                                  // y suu valor es 5880

void pulse2()   //medir la cantidad de flujo de agua
{
  waterFlow2 += 1.0 / 450.0;        //450 es el valor para el caudalimetro de 1/2 pulgada, nosotros tenemos 1/4 pulgada
}                                  // y suu valor es 5880


void caudal() {
  cantidad1 = waterFlow1;
  cantidad2 = waterFlow2;
}
