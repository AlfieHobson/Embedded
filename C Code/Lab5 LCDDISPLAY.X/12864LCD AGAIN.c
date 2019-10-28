#include <xc.h>  
//begin config 
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator) 
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled) 
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled) 
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled) 
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming) 
//end config 
 
#define rs RA5 
#define rw RA4 
#define e  RA3 
#define psb RA2

 
void init();                   //I/O init 
void lcd_init();               //LCD init 
void writecmd(char x);         //display one byte 
void writechar(char x);        //LCD display setting. 
void delay();                  //delay 
 
//I/O initialisation 
void init()  
{     
   ADCON1=0X07;               //a port as ordinary i/o.    
   TRISA=0X00;                //a port as output.     
   TRISD=0X00;                //d port as output.  
} 
 
//--------------------------------------- //LCD init 
void lcd_init()  
{
    writecmd(0x0e);               //display on,cursor on,blink on.  0000 1110    
    writecmd(0x1);                //clr screen                      0000 0001
    writecmd(0x38);               //8 bits 2 lines 5*8 mode.        0011 10xx     
    
    
}     
//-------------------------------------- 
//write a byte to LCD. 
void writechar(char x)  
{   
    psb = 1;
    rs=1;                      //is data not command.   
    rw=0;                      //is write not read.   
    PORTD=x;                   //data send to PORTD   
    e=0;                       //pull low enable signal.   
    delay();                   //for a while.  
    e=1;                       //pull high to build the rising edge.  
} 
 
//-------------------------------------- 
//LCD display setting  
void writecmd(char x)  
{   
     psb = 1;
     rs=0;                     //is command not data    
     rw=0;                     //is write not read.     
     PORTD=x;     
     e=0;                      //pull low enable signal.             
     delay();                  //for a while.                          
     e=1;                      //pull high to build the rising edge  
} 
 
//-------------------------------------- 
//delay 
void delay()  
{    
    int i;    
    for(i=0;i<5000;i++);  
} 
 
//--------------------------------------- 
//main 
void main()  
{     
      
        init();                //I/O init        
        lcd_init();            //LCD init           
        writechar('h');
        writechar('e');
        writechar('l');
        writechar('l');
        writechar('o');
        
        /* First attempt at looping to next address.
        /for (int x = 0; x<35; ++x) writecmd(0x14);         //Shift the cursor right to the end of the first line. Loops 35 as first line has 40 characters - hello`   0001 01xx
        */
        
        // ADDRESS OF THE SECOND LINE.
        writecmd(0xC0);             //command is 1100 0000. Address is 100 000 = 0x40.
        writechar('w');
        writechar('o');
        writechar('r');
        writechar('l');
        writechar('d');
        
        
        while(1){}
}