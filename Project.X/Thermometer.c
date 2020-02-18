#include"main.h"
//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

//THE configure of MCU,watchdog OFF,electrify delay OPEN,power down check OFF,
//LOW power programme OFF,encrypt,4M crystal HS surge.
 
# define uch unsigned char                   //                                          
# define DQ RA0                              //define 18B20 data PORT                    
# define DQ_DIR TRISA0                       //define 18B20 D PORT direct register       
# define DQ_HIGH() DQ_DIR =1                 //set data PORT INPUT                       
# define DQ_LOW() DQ = 0; DQ_DIR = 0         //set data PORT OUTPUT                      
 unsigned char TLV=0 ;                       //temperature high byte                     
 unsigned char THV=0;                        //temperature low byte                      
 unsigned char TZ=0;                         //temperature integer after convert         
 unsigned char TX=0;                         //temperature decimal  after convert        
 unsigned int wd;                            //temperature BCD code  after convert       
                                                                                         
unsigned char tenth;                           //integer ten bit                          
unsigned char first;                           //integer Entries bit                      
unsigned char dp;                              //ten cent bit                             
                              
//-----------------------------------------------------------
//delay function              
void therm_delay(char x,char y) 
{
  char z;
  do{
      z=y;
      do{;}while(--z);
     }while(--x);
 }
//instruction time:7+(3*(Y-1)+7)*(X-1)if add call instruction,set page , seven instruction of  transfer parameter
                                                                                                                        
//is 14+(3*(Y-1)+7)*(X-1)
                                                                                                                        


//------------------------------------------------
//system initialize function           
void therm_init(void)
{
  ADCON1=0X07;                                //set A PORT general data PORT. Digital Pins   
  TRISA=0X00;                                 //set A PORT direct OUTPUT       
  TRISD=0X00;                                 //set D PORT direct OUTPUT       
}

//-----------------------------------------------
//reset DS18B20 function   
therm_reset(void)
{
  char presence=1;
  while(presence)
  { 
    DQ_LOW() ;                                //MAIN MCU PULL LOW                                                                       
    therm_delay(2,70);                              //delay 503us                                                                             
    DQ_HIGH();                                //release general line and wait for resistance pull high general line and keep 15~60us    
    therm_delay(2,8);                               //delay 70us                                                                              
    if(DQ==1) presence=1;                     // not receive responsion signal,continue reset                                           
    else presence=0;                          //receive responsion signal                                                               
    therm_delay(2,60);                              //delay 430us                                                                             
   }
  }

//-----------------------------------------------
//write 18b20 one byte function     
void therm_write_byte(uch val)
{
 uch i;
 uch temp;
 for(i=8;i>0;i--)
 {
   temp=val&0x01;                             //shift the lowest bit                   
   DQ_LOW();                                                                           
   NOP();                                                                              
   NOP();                                                                              
   NOP();                                                                              
   NOP();                                                                              
   NOP();                                     //pull high to low,produce write time    
   if(temp==1)  DQ_HIGH();                    //if write 1,pull high                   
   therm_delay(2,7);                                //delay 63us                             
   DQ_HIGH();                                                                          
   NOP();                                                                              
   NOP();                                                                              
   val=val>>1;                                //right shift a bit                      
  }
}

//------------------------------------------------
//18b20 read a byte function  
uch therm_read_byte(void)
{
 uch i;
 uch value=0;                                 //read temperature         
 static __bit j;
 for(i=8;i>0;i--)
 {
   value>>=1; 
   DQ_LOW();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();                                    //6us              
   DQ_HIGH();                                // pull high       
   NOP();                                                       
   NOP();                                                       
   NOP();                                                       
   NOP();                                                       
   NOP();                                   //4us               
   j=DQ;                                                        
   if(j) value|=0x80;                                           
   therm_delay(2,7);                              //63us              
  }
  return(value);
}

//-------------------------------------------------
//start temperature convert function   
void therm_get_temp(void)
{ 
int i;
DQ_HIGH();
therm_reset();                              //reset,wait for  18b20 responsion                                                                                                               
therm_write_byte(0XCC);                     //ignore ROM matching                                                                                                                            
therm_write_byte(0X44);                     //send  temperature convert command                                                                                                              
                                                                                                                                                                             
therm_reset();                              //reset again,wait for 18b20 responsion                                                                                                          
therm_write_byte(0XCC);                     //ignore ROM matching                                                                                                                            
therm_write_byte(0XBE);                     //send read temperature command                                                                                                                  
TLV=therm_read_byte();                      //read temperature low byte                                                                                                                      
THV=therm_read_byte();                      //read temperature high byte                                                                                                                     
DQ_HIGH();                            //release general line                                                                                                                           
TZ=(TLV>>4)|(THV<<4)&0X3f;            //temperature integer                                                                                                                            
TX=TLV<<4;                            //temperature decimal                                                                                                                            
if(TZ>100) TZ/100;                    //not display hundred bit                                                                                                                        
first=TZ%10;                     //integer Entries bit                                                                                                                            
tenth=TZ/10;                    //integer ten bit                                                                                                                                
wd=0;                                                                                                                                                                                  
if (TX & 0x80) wd=wd+5000;
if (TX & 0x40) wd=wd+2500;
if (TX & 0x20) wd=wd+1250;
if (TX & 0x10) wd=wd+625;                //hereinbefore four instructions are turn  decimal into BCD code                         
dp=wd/1000;                          //ten cent bit                                                                                                                                               
NOP();                                                                                                                            
}                                                                                                                                 

//--------------------------------------------------
//main function    
struct Temp therm_getTemp(void)
{
    therm_init();                 //call system initialise function                                                                                                                                                                                                                                                                                                                             
    therm_get_temp();             //call temperature convert function                                                                                                                                   
    //display();              //call display function                                                                                                                                              
    
    int mergeTemp = (tenth*100) + (first*10) + dp;
    
    struct Temp t = { tenth, first, dp , mergeTemp};
    return t;
           
}

   
