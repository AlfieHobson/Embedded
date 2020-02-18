//#include<pic.h>                                                                                                 
// __CONFIG(0x1832);   
#include "main.h"
                                                        
#define  buz  RE0                              //define buzzer control I/O                                       
                                                                                                                
//------------------------------------------------------                                                        
//Buzzer delay function                                                                                                
void buzzer_delay(void)  {    
    int i;    
    for(i=0;i<5000;i++);  
}
                                                                                                                
//------------------------------------------------------                                                        
//Alarm function                                                                                                
void buzzer_alarmOn(void)                                                                                               
{       
  TRISE=0;      //Set port C to output
  buz = 1;      // Buzzer on
  buzzer_delay();  // Wait
  buz = 0;      // Buzzer off
  buzzer_delay();  // Wait
}    

void buzzer_alarmOff(void) 
{
    buz = 0;
}
                                                                                                        
                                                                                                                
                                                                                                                