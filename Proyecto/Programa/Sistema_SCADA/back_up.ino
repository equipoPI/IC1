//caudal que funciona

/*  volatile double waterFlow;
    int bandera = 1;

    void setup() {
    pinMode(3, OUTPUT);
    Serial.begin(9600);  //baudrate
    waterFlow = 0;
    attachInterrupt(0, pulse, RISING);  //DIGITAL Pin 2: Interrupt 0
    }
    void loop() {
    if (bandera == 1) {
      digitalWrite(3, LOW);
      delay(2000);
      bandera = bandera + 1;
    }

    digitalWrite(3, HIGH);

    Serial.print("waterFlow:");
    Serial.print(waterFlow);
    Serial.println("   L");
    delay(500);

    if (waterFlow > 50) {
      digitalWrite(3, LOW);
      delay(2000);
      waterFlow = 0;
    }
    }

    void pulse()   //measure the quantity of square wave
    {
    waterFlow += 1.0 / 450.0;
    }
  */
