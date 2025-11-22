/*
 * File:   PWMREASUABLE.c
 * Author: gladi
 *
 * Created on 12 de noviembre de 2025, 7:28
 */


#include <xc.h>
#define _XTAL_FREQ 4000000//4MHZ

void pwm_init(void){
    TRISC = 0b00000000; //donde esta el pwm, puerto c como salida
    CCP1CON = 0b00001100; //Configuración modo PWM y limpiamos 2 bits del utycycle
    CCPR1L = 0b00000000; //limpir 8 bits del duty cycle
    PR2 = 249;
    T2CON = 0b00000101; // endendido tm2 y prescaler
}

void set_percent(unsigned char percent){//bits q van al registro del duty cydle o int
    if(percent>100){
        percent = 100;
    }
    
    unsigned int max_counts = (unsigned int)(4u*(PR2+1));//unsigned int-16 bits
    unsigned int dc = (unsigned int)((unsigned long)percent*max_counts/1000UL);
    CCPR1L = (unsigned char)(dc>>2); 
    CCP1CONbits.CCP1X = (dc>>1)&1;
    CCP1CONbits.CCP1Y = dc&1;
}
void main(void) {
    TRISB = 0b11111111; //entrada
    TRISA = 0b00000000; //salida
    unsigned char counter = 0;
    pwm_init();
    


    while(1){
        
         if (counter>=100){
            counter = 0;
        }
         
        if (counter<=100){
            counter = 0;
        }
        
        if(PORTBbits.RB0==1){ //clockwise
            PORTA = 0b00000001;
        }
        
        if(PORTBbits.RB1==1){
            PORTA = 0b00000010;
     
        }
 
    if(PORTBbits.RB2 == 1){     // botón para sentido A
        counter++;
        __delay_ms(20);
        set_percent(counter);
    }

    if(PORTBbits.RB3 == 1){     // botón para sentido B
        counter--;
        set_percent(counter);
        __delay_ms(20);
    }
        
    if(PORTBbits.RB4 == 1){     // botón para sentido 
        __delay_ms(20);
        set_percent(0);
        
    }


}

   
}




