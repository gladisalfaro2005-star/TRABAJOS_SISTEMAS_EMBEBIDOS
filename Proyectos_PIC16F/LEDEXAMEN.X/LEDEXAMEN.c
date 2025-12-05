/*
 * File:   LEDEXAMEN.c
 * Author: gladi
 *
 * Created on 1 de diciembre de 2025, 7:21
 */


#include <xc.h>
#define _XTAL_FREQ 20000000

void main(void)  {
    
     TRISB = 0b11111111; 
     TRISB = 0b00000000;


    
    while (1){
         if (PORTBbits.RB1 == 1) {
             PORTB = 0b00000001;
            __delay_ms(100);
        } 
         
          if (PORTBbits.RB1 == 0) {
             PORTB = 0b00000000;
            __delay_ms(100);
        } 
  
            
            
    }
    return;
}

