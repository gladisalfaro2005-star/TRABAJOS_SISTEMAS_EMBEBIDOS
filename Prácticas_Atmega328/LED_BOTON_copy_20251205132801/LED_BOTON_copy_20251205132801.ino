unsigned char estadoLed = 0;

void setup() {
  DDRB = B00000000;   
  PORTB = B00000100;  
  DDRD = B00000100;  
}

void loop() {
  if (PINB == B00000100) {   
    delay(20);               // Antirebote y botón presionado
    
    if (PINB == B00000100) {

      // Toggle:
      estadoLed = !estadoLed;

      if (estadoLed)
        PORTD = B00000100;   // encender
      else
        PORTD = B00000000;   // apagar
      while (PINB == B00000100);
    }
  }
}