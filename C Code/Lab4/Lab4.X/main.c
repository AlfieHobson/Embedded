#include <xc.h>
//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit SerialProgramming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config
void delay () {
    for (unsigned i=0; i<5000; i++);
} 

void checkButton (int index) {
    if (PORTB == 1 << index) {
        PORTC = 1 << index;
    }
    if (PORTA == 1 << index) {
        PORTC = 1 << index;
    }
}

void main () { 
    
    ADCON1 = 0x07;  //Data sheet page 130. Makes all pins digital 
    TRISB = 0xFF;   // Set B as inputs
    TRISA = 0xFF;   // Set A as inputs 
    TRISC = 0x00;   // Set C as outputs
    PORTB = 0x00;
    PORTA = 0x00;

    while(1){
        for (int i=0; i<8; i++) {
            checkButton(i);
        }
        
        /*
        // Bit shift to change 0x00 later...
        if (PORTB == 0x00) {
            PORTC = 0x01;  // 1 LED     0000 0001
        } else {
            PORTC = 0x07;  // 3 LEDS    0000 0111
        }*/
    }
    
}