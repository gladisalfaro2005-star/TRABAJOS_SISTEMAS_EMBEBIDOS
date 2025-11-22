/* 
 * File:   newmain.c
 * Author: gladi
 *
 * Created on 24 de octubre de 2025, 11:58
 */
#include <xc.h>
#define _XTAL_FREQ 20000000
#include "PIC16F877A.h"

/*
 * 
 */


void main(void)  {
    
     TRISB = 0b11111111;
     TRISC = 0b00000000;


    
    while (1){
         if (PORTBbits.RB1 == 1) {
             PORTC = 0b11111111;
            __delay_ms(1000);
        } 
         
          if (PORTBbits.RB1 == 0) {
             PORTC = 0b00000000;
            __delay_ms(1000);
        } 
  
            
            
    }
    return;
}

