#include <avr/io.h>
#include <util/delay.h>

// ADC0 en PC0 pin A0
static inline void initADC0(void) {
    ADMUX = (1 << REFS0); // Referencia AVCC 
    ADCSRA = (1 << ADPS1) | (1 << ADPS0); // Prescaler 8
    ADCSRA |= (1 << ADEN); // Habilitar ADC
}

// PWM en OC0A pin 9 en arduino
void initPWM(void) {
    DDRB |= (1 << PB1); // pin 9 en Arduino 

    TCCR1A = (1 << COM1A1) | (1 << WGM10);
    TCCR1B = (1 << WGM12) | (1 << CS11); // Prescaler 8
}

int main(void) {
    uint16_t pot;

    initADC0();
    initPWM();

    while (1) {
        ADCSRA |= (1 << ADSC); // conversión
        loop_until_bit_is_clear(ADCSRA, ADSC); // Esperar
        pot = ADC; // Leer ADC 0 a 1023

       OCR1A = pot >> 2; // Escalar a 0–255 para PWM

        _delay_ms(10);
    }

    return 0;
}
