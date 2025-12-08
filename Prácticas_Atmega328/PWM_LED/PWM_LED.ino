#include <avr/io.h>
#include <util/delay.h>

// ADC0 en PC0 pin A0
static inline void initADC0(void) {
    ADMUX = (1 << REFS0); // Referencia AVCC 
    ADCSRA = (1 << ADPS1) | (1 << ADPS0); // Prescaler 8
    ADCSRA |= (1 << ADEN); // Habilitar ADC
}

// PWM en OC0A pin D6 en arduino
void initPWM(void) {
    DDRD |= (1 << PD6); // PD6 salida

    TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01); 
    TCCR0B = (1 << CS01); // Prescaler 8
}

int main(void) {
    uint16_t pot;

    initADC0();
    initPWM();

    while (1) {
        ADCSRA |= (1 << ADSC); // conversión
        loop_until_bit_is_clear(ADCSRA, ADSC); // Esperar
        pot = ADC; // Leer ADC 0 a 1023

        OCR0A = pot >> 2; // Escalar a 0–255 para PWM

        _delay_ms(10);
    }

    return 0;
}
