/*
 * File:   adc.c
 * Author: gladi
 *
 * Created on 5 de noviembre de 2025, 7:33
 */


#include <xc.h>
#define _XTAL_FREQ 20000000
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "PIC16F877A.h"


#define RS PORTBbits.RB6
#define EN PORTBbits.RB7


void lcdCommand(unsigned char cmd){ //enviar comandos

    RS = 0;
    PORTD = cmd;
    EN = 1;
    __delay_ms(1);
    EN = 0;
    __delay_ms(1);

}

void lcd_init(){
    lcdCommand (0x38); //configura display contres matrices de 5por7
 lcdCommand(0x06); //mover el cursor
  lcdCommand(0x0f); //display on blinking
   lcdCommand(0x01); //borra pantalla 
    lcdCommand(0x080); //mueve el cursor a la primera linea
    __delay_ms(1);
}

void lcdSendDataByte(unsigned char data){ //enviar datos

    RS = 1;
    PORTD = data;
    EN = 1;
    __delay_ms(1);
    EN = 0;
    __delay_ms(1);

}

void adc_init() {
    TRISA = 0b00000001; //pin an0 del puerto a como entrada
    ADCON1 = 0b11001110;   //bit adfm=1 justificado a kla derecha adcs2 combinar con acon para relojpcfg- seleccionar canal
    ADCON0 = 0b10000001;  //primer bit(0) enciecde el adc bit2 godone=0 3 aq 5 canal 0
    __delay_us(2);

}

void lcdSendString(char *addr){
    while(*addr){
        lcdSendDataByte(*addr);
        addr++;
    }

}
//leer adc
unsigned int adc_read_an0(void){
    __delay_us(2); //adquisition time
    GO_nDONE = 1;
    while(GO_nDONE);
    return ((unsigned int)ADRESH <<8|ADRESL);
    
}
void main(void) {
    
    TRISD = 0b00000000; //salida del micro y entrada de la lcd
    TRISB = 0b00000000;
    
    lcd_init();
    adc_init();
    
    char buffer[16];
    unsigned int prev_mv = 0xff; //valor del voltaje de adc
    
    while(1){
        uint16_t raw = adc_read_an0();
        uint16_t mV = (uint16_t)raw*5000L/1023U;
        
        if(mV!=prev_mv){
            lcdCommand(0x80);
            sprintf(buffer,"raw:%4u", raw);//copiar al buffer
            lcdSendString(buffer);
            
            lcdCommand(0xC0);
            sprintf(buffer,"mV:%4u", mV);//copiar al buffer
            lcdSendString(buffer);
            
            prev_mv = mV;
        
        
        }
        
        __delay_ms(100);
    }
    return;
}
