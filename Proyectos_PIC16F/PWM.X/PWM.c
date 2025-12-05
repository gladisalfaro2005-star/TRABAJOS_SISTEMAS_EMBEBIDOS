/*
 * File:   PWM.c
 * Author: gladi
 *
 * Created on 10 de noviembre de 2025, 8:28
 */


#include <xc.h>
#define _XTAL_FREQ 4000000

void main(void) {
    
    TRISC = 0b00000000; //0-pin de salida
    TRISB = 0b00001111; //pin 0,1,2 y 3 como entrada, oush buttons
    T2CON = 0b00000101; //prescaler de 4 y timer 2 on
    PR2 = 249; //frecuencia de 1khz del pwm
    
    while(1){
    if (PORTBbits.RB0 == 1){ //duty cycle 100 porciento
        CCPR1L = 0b11111010;//1000
        CCP1CON = 0b00001111;
     
    }
    
    if (PORTBbits.RB1 == 1){ //dutycycle 75
        CCPR1L = 0b10111011;
        CCP1CON = 0b00001111;
     
    }
    
    if (PORTBbits.RB2 == 1){ //duty cycle 50
        CCPR1L = 0b01111101;
        CCP1CON = 0b00001111;
     
    }
    
    if (PORTBbits.RB3 == 1){ // dutycycle 25
        CCPR1L = 0b00111110;
        CCP1CON = 0b00001111;
     
    }
    
    if (PORTBbits.RB4 == 1){ // dutycycle 25
        CCPR1L = 0b00000000;
        CCP1CON = 0b00001111;
     
    }
    }
    return;
}
