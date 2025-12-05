#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Pines para LCD puerto D
#define LCD_DATA_PORT PORTD
#define LCD_DATA_DDR  DDRD
#define LCD_CTRL_PORT PORTB
#define LCD_CTRL_DDR  DDRB
#define RS PB0  
#define EN PB1  // capturar datos

void lcdCommand(unsigned char cmd);
void lcd_init(void);
void lcdSendDataByte(unsigned char data);
void lcdSendString(char *addr);
void adc_init(void);
uint16_t adc_read_channel(uint8_t channel);

void lcdCommand(unsigned char cmd) {
    // Enviar comandos
    LCD_CTRL_PORT &= ~(1 << RS);  // RS = 0 (comando)
    LCD_DATA_PORT = cmd;          // Enviar comando al puerto de datos
    LCD_CTRL_PORT |= (1 << EN);   // EN = 1
    _delay_ms(1);
    LCD_CTRL_PORT &= ~(1 << EN);  // EN = 0
    _delay_ms(1);
}

//inicilaizar LCD
void lcd_init(void) {
    // Configurar puertos como salida
    LCD_DATA_DDR = 0xFF;   // Puerto D salida
    LCD_CTRL_DDR |= (1 << RS) | (1 << EN);  
    
    _delay_ms(20);  // delay para LCD
    
    lcdCommand(0x38);  // configurar 2 líneas y matriz 5x8
    lcdCommand(0x06);  // incrementar 
    lcdCommand(0x0F);  // display ON
    lcdCommand(0x01);  // clear
    _delay_ms(2);
    lcdCommand(0x80);  // cursor linea 1
}

void lcdSendDataByte(unsigned char data) {
    // Enviar datos
    LCD_CTRL_PORT |= (1 << RS);   // RS = 1 
    LCD_DATA_PORT = data;         // Enviar dato al puerto 
    LCD_CTRL_PORT |= (1 << EN);   // EN = 1
    _delay_ms(1);
    LCD_CTRL_PORT &= ~(1 << EN);  // EN = 0
    _delay_ms(1);
}

void lcdSendString(char *addr) {
    while(*addr) {
        lcdSendDataByte(*addr);
        addr++;
    }
}

void adc_init(void) {
    
    ADMUX = (1 << REFS0);

    // adc y preescaler
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}


uint16_t adc_read_channel(uint8_t channel) {
    // Mantener referencia y limpiar bits
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    
    ADCSRA |= (1 << ADSC); //conversión

    
    while (ADCSRA & (1 << ADSC));

    return ADC;
}


int main(void) {
    char buffer[16];
    uint16_t prev_mv = 0xFFFF;
    uint16_t raw, mV;
    
    
    lcd_init();
    adc_init();
    
    while(1) {
        raw = adc_read_channel(0);  // Leer A0
        mV = (uint16_t)((uint32_t)raw * 5000UL / 1023UL);
        
        if(mV != prev_mv) {//mostrar RAW
           
            lcdCommand(0x80);  // Parte de arriba
            sprintf(buffer, "RAW:%4u", raw);
            lcdSendString(buffer);
            
            // Mostrar valor en mV
            lcdCommand(0xC0);  // abajo
            sprintf(buffer, "mV:%4u", mV);
            lcdSendString(buffer);
            
            prev_mv = mV;
        }
        
        _delay_ms(100);
    }
    
    return 0;
}
