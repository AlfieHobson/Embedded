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

void main () { 
    // -----------------------------               Moves led down PORT C using bit shift
    TRISC = 0x00;
    PORTC = 0x00;
    for (;;) {
        for (int i=0; i<8; i++) {
            PORTC = 1 << i;         //Bit shift left. Moving the 00000001 to 00000010     Starts at 1. Bit shifts that 1 time, 2 time etc...
            delay(); 
        }
    } 
    
    /* -----------------------------                Sets all Seven segs to 1
    TRISD = 0x00;
    TRISA = 0x00;
    ADCON1 = 0x07;
    
    PORTA = 0x00;
    PORTD = 0xF9;
    
    
    /*------------------------------                  Displays 1,2,3 on 7-seg    
    TRISD = 0x00;   //PORT D CONTROLS WHAT NUMBER IS DISPLAYED. Setting all as output
    TRISA = 0x00;   //PORT A CONTROLS WHICH 7 SEGMENT IS USED. Setting all as output
    ADCON1 = 0x07;  //Data sheet page 130. Makes all pins digital 
    
    PORTD = 0xf9;   // Number 1
    PORTA = 0xfe;   // On display 1
    PORTA = 0xff;   // Turn off displays
    
    PORTD = 0xa4;   // Number 2
    PORTA = 0xfd;   // On display 2
    PORTA = 0xff;   // Turn off displays
    
    PORTD = 0xb0;   // Number 3
    PORTA = 0xfb;   // On display 3
    PORTA = 0xff;   // Turn off displays */
} 

/* LCD NOTES
 * Small one    Two lines
 * Big one      FOur lines + animations
 * 
 * LOOK AT LECTURE SLIDES
 * RS, RW and E? Time diagram
 * Y - AXIS
 * RS           High or low    (write data is low) (Command or data)
 * RW           Read or write  (low is write)
 * E            Enabled or disabled
 * DB0 - DB7    Data
 * 
 * X - AXIS (When it will happen)
 * Horizontal arrows indicate put a delay 
 * Use data sheet to find commands, All stuff is saved in LCD RAM
 * */