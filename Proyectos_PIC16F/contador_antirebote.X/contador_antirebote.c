/*
 * File:   contador_antirebote.c
 * Author: gladi
 *
 * Created on 3 de noviembre de 2025, 8:33
 */
#include <xc.h>
#define _XTAL_FREQ 20000000
#define DEBOUNCE_MS 20
#include "PIC16877AAAA.h"


const unsigned char DISPLAY[10] = {
    0b11111111, 
    0b00001100,
    0b00110111, 
    0b00011111, 
    0b00001100,
    0b00011011, 
    0b00111011, 
    0b00001110, 
    0b11111111, 
    0b00011110  
};

const unsigned char DISPLAYE[10] = 
{
    0b00000001,
    0b00000000, 
    0b00000010, 
    0b00000010, 
    0b00000011, 
    0b00000011, 
    0b00000011, 
    0b00000000, 
    0b11111111, 
    0b00000011 
};

void main(void) {
    
    TRISA = 0b00000000;  
    TRISB = 0b11111111; 
    TRISE = 0b00000000;
  
    unsigned char counter = 0;

    while (1){
        
        if(PORTBbits.RB7 == 1){
            counter++;
            if(counter == 10) counter = 0;

            PORTA = DISPLAY[counter];
            PORTE = DISPLAYE[counter];
            
            while(PORTBbits.RB7 == 1); 
            __delay_ms(1000);
        }
    }
}