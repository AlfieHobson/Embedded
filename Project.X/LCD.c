#include "main.h"

#define rs RA5 
#define rw RA4 
#define e  RA3 
#define psb RA2

void LCD_writeCmd(char x);  

//delay 
void LCD_delay(void)  {for(int i=0;i<50;i++);} 

//I/O init 
void LCD_init(void)  {     
   ADCON1=0X07;               //a port as ordinary i/o.    
   TRISA=0X00;                //a port as output.     
   TRISD=0X00;                //d port as output.  
   PORTD=0x00;
   PORTA=0x00;
   psb = 1;
   LCD_writeCmd(0x0e);               //display on,cursor on,blink on.  0000 1110    
   LCD_writeCmd(0x38);               //8 bits 2 lines 5*8 mode.        0011 x0xx   
} 


//LCD display setting  
void LCD_writeCmd(char x)  {
    rs = 0; //COMMAND   
    rw = 0;
    e = 0;      // pull low enable signal.  
    LCD_delay();
    e = 1;      // pull high
    LCD_delay();
    PORTD = x;
    LCD_delay();
    e = 0;      //pull low enable signal.                               
} 

//write a byte to LCD. 
void LCD_writeChar(char x) {
    rs = 1;     //DATA
    rw = 0;
    e = 0;
    LCD_delay();
    e = 1;
    LCD_delay();
    PORTD = x;
    LCD_delay();
    e = 0; //pull high to build the rising edge.  
} 

void LCD_write(char string[], char len) {
    // For each char, write onto display.
    for(int i=0;i<len;i++)       //total 16 bytes to display.
    {
      LCD_writeChar(string[i]);       //search table to display
    }
}

void LCD_clear(void){
    LCD_writeCmd(0x1);  // Clear contents of display
}

/* Sets pointer address to first line*/
void LCD_firstLine(void){ LCD_writeCmd(0x80); } 
/* Sets pointer address to second line*/
void LCD_secondLine(void){ LCD_writeCmd(0x90); } // 1001 0000
/* Sets pointer address to third line*/
void LCD_thirdLine(void) {LCD_writeCmd(0xC8); } // 1100 1000 
/* Sets pointer address to fourth line*/
void LCD_fourthLine(void) {LCD_writeCmd(0xD8); } // 1101 1000