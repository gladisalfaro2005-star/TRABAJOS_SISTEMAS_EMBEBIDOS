/*
 * File:   PWMREUSABLE_MOTORVELOCIDADYGIRO.c
 * Author: gladi
 *
 * Created on 25 de noviembre de 2025, 18:56
 */


#include <xc.h>
#include "PIC16F877AAAA.h"

#define DEBOUNCE_MS 20
#define _XTAL_FREQ 20000000



void pwm_init(void){
    TRISCbits.TRISC2 = 0; // salida pwm
    CCP1CON = 0b00001100; 

    PR2 = 61;     
    T2CON = 0b00000101; 
    
    CCPR1L = 0;
}

void set_percent(unsigned char percent){
    
    if(percent > 100) percent = 100;
    
    unsigned int max_counts = 4 * (PR2 + 1);
    unsigned long dc = ((unsigned long)percent * max_counts) / 100;

    CCPR1L = (unsigned char)(dc >> 2);
    CCP1CONbits.CCP1X = (dc >> 1) & 1;
    CCP1CONbits.CCP1Y = dc & 1;
}


void main(void){
    
    TRISB = 0xFF;
    TRISA = 0x00;

    pwm_init();
    unsigned char velocity = 0;

    while(1){
        if(PORTBbits.RB1){ //más velocidad
            if(velocity < 100) velocity++;
            set_percent(velocity);
            __delay_ms(20);
        }
        if(PORTBbits.RB2){ //menos
            if(velocity > 0) velocity--;
            set_percent(velocity);
            __delay_ms(20);
        }
        
        if(PORTBbits.RB3){//clockwise
            PORTA = 0x01;
        }
        
        if(PORTBbits.RB4){//no clockwise
            PORTA = 0x02;
        }
        
        if(PORTBbits.RB5){ //stooop
            PORTA = 0x00;
        }
    }
}
