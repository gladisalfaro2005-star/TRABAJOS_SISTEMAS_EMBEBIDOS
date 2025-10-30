/*
 * File:   Counter.c
 * Author: gladi
 *
 * Created on 30 de octubre de 2025, 7:25
 */

#define XTAL_FREQ 20000000
#include <xc.h>
unsigned char counter = 0;

void main(void) {
    
    TRISA = 0x01;
    TRISB = 0x00;
    PORTB = 0x3f;
    
    while(1){
    
        unsigned char button = PORTAbits.RA0;
        
        if (button == 1){
            counter+1
        }
        
        if (counter == 10)[
                counter = 0];
        
    
    }
    
    switch (counter){
            
            case 0:
                PORTB = 0x3f;
                break;
                
            case 1:
                PORTB = 0x06;
                break;
                
            case 2:
                PORTB = 0x5b;
                break;
                
            case 3:
                PORTB = 0x4f;
                break;
                
            case 4:
                PORTB = 0x3b;
                break;
                
            case 5:
                PORTB = 0x3b;
                break;
                
            case 6:
                PORTB = 0x3b;
                break;
                
            case 7:
                PORTB = 0x3b;
                break;
               
            case 8:
                PORTB = 0x7f;
                break;
                
            case 9:
                PORTB = 0x6f;
                break;
                
    }
}
    return;
}
