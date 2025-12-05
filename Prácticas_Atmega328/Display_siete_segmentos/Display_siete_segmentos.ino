unsigned char display[10] = {
  B00111111,  // 0
  B00000110,  // 1
  B01011011,  // 2
  B01001111,  // 3
  B01100110,  // 4
  B01101101,  // 5
  B01111101,  // 6
  B00000111,  // 7
  B01111111,  // 8
  B01101111   // 9
};

unsigned char counter = 0;

void setup() {
  DDRB = B00000000;     // Puerto B entrada
  PORTB = B00000100;    // PB2
  DDRD = B11111111;     // PORTD salida
  PORTD = display[0];   // 0 al inicio
}

void loop() {

  //puerto B2
  if (PINB & B00000100) { 

    delay(80);

    if (PINB & B00000100) {
      counter++;
      PORTD = display[counter];
    }

    if (counter == 10) {
      counter = 0;
    }
  }
}