#include<pic.h>                        //include MCU head file     
#include "main.h"
#include "LCD.h"
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//__CONFIG(0x1832);       
//__CONFIG _DEBUG_OFF&_CP_ALL&_WRT_HALF&_CPD_ON&_LVP_OFF&_BODEN_OFF&_PWRTE_ON&_WDT_OFF&_HS_OSC     

#define i_o   RB4                      //1302I_O           
#define sclk  RB0                      //1302 clock        
#define rst   RB5                      //1302 enable bit   

//unsigned char time_rx;
//unsigned char time_rx @ 0x30;        //define receive reg.
//static volatile bit time_rx7   @ (unsigned)&time_rx*8+7;   //receive reg highest.
//static volatile bit temp0     @ (unsigned)&temp*8+0;

char * RTC_seperateNibbles(char hex);
char RTC_combineNibbles(char first, char second);
struct Time RTC_splitTime(void);
void RTC_init(void);                      //port initilize subroutine.
void RTC_ds1302_init(void);                    //DS1302 initilize subroutine.
void RTC_writeClock(void);                       //set time subroutine.
void RTC_readClock(void);                       //get time subroutine.
void RTC_time_write_1(unsigned char time_tx);    //write one byte subroutine.
unsigned char RTC_time_read_1(void);          //read one byte subroutine.


//                     sec, min,hour,day,month,week,year,control word.
//                     0     1    2   3   4     5    6      7
// Time to be used incase the RTC has to be set manually.
const char RTCtable[]={0x30,0x59,0x12,0x06,0x11,0x06,0x06,0x00};

// Table the setTime saves to.
char setTable[7];
// Table the getTime saves to.
char getTable[7];

//----------------------------------------------

void RTC_setTime(struct Time time) {
    RTC_init();                     //port initilize.
    RTC_ds1302_init();              //DS1302 initilize.
    // Format struct into array the clock can read.

    setTable[0] = RTC_combineNibbles(time.sec1,time.sec2);
    setTable[1] = RTC_combineNibbles(time.min1,time.min2);
    setTable[2] = RTC_combineNibbles(time.hour1,time.hour2);
    setTable[3] = RTC_combineNibbles(time.day1,time.day2);
    setTable[4] = RTC_combineNibbles(time.month1,time.month2);
    setTable[5] = RTC_combineNibbles(time.week1,time.week2);
    setTable[6] = RTC_combineNibbles(time.year1,time.year2);
    setTable[7] = 0x00;
 
    RTC_writeClock();
}

struct Time RTC_getTime(){
    RTC_init();                     //port initilize.
    RTC_ds1302_init();                   //DS1302 initilize.
    RTC_readClock();                      //Gets time from clock and puts into table1
    struct Time t = RTC_splitTime();                //Creates splitTimeTable from timeTable. Splitting its values into usable bytes.
    return t;
}

struct Time RTC_splitTime(){ 
    char *sec = RTC_seperateNibbles(getTable[0]);
    char sec1 = *sec;
    char sec2 = *(sec + 1);
    
    char *min = RTC_seperateNibbles(getTable[1]);
    char min1 = *min;
    char min2 = *(min + 1);

    char *hour = RTC_seperateNibbles(getTable[2]);
    char hour1 = *hour;
    char hour2 = *(hour + 1);
    
    char *day = RTC_seperateNibbles(getTable[3]);
    char day1 = *day;
    char day2 = *(day + 1);
    
    char *month = RTC_seperateNibbles(getTable[4]);
    char month1 = *month;
    char month2 = *(month + 1);
    
    char *week = RTC_seperateNibbles(getTable[5]);
    char week1 = *week;
    char week2 = *(week + 1);
    
    char *year = RTC_seperateNibbles(getTable[6]);
    char year1 = *year;
    char year2 = *(year + 1);
    
    struct Time t = {sec1, sec2, min1, min2, hour1,hour2,day1,day2,month1,month2,week1,week2,year1,year2,1};
    return t;
}

// Clock byte to two nibbles.
char * RTC_seperateNibbles(char hex){
    char firstByte, secondByte;
    // And firstByte
    secondByte = hex & 0x0F;   
    firstByte = hex & 0xF0; 
    firstByte = firstByte >>4;   
    char dataSplit[] = {firstByte, secondByte};
    return dataSplit;
}

// Combines two lower nibbles together to one byte.
char RTC_combineNibbles(char first, char second)
{  
    char returnChar= first << 4;
    returnChar = returnChar | second;
    return returnChar;
}

bool RTC_isDay(struct Time time) {
    // If before 10 o'clock
    if (time.hour1 == 0x00)
    {
        // If the hour == 6 , and min is greater or equal to 30, day.
        if(time.hour2 == 0x06 && time.min1 >= 0x03)
        {
            return 1;
        }
        // If the time is greater than 6, day
        else if(time.hour2 > 0x06)
        {
            return 1;
        }
        // else night
        return 0;
    }
    // Else when time is between 10 and 19 o'clock
    else if (time.hour1 == 0x01)
    {
        // If hour == 19, and min is less than 30,day
        if(time.hour2 == 0x09 && time.min1 <= 0x03)
        {
            return 1;
        }
        // If time is less than 19, day
        if(time.hour2 < 0x09)
        {
            return 1;
        }
        //else night
        return 0;
    }
    else // Else time is either 20, 21, 22, 23 o'clock
        return 0;
}

//---------------------------------------------
//DS1302 initilize.
void RTC_ds1302_init(void)
  {
   sclk=0;                            //pull low clock
   rst =0;                            //reset DS1302
   rst=1;                             //enable DS1302
   RTC_time_write_1(0x8e);                //send control command
   RTC_time_write_1(0);                   //enable write DS1302
   rst=0;                             //reset
  }

//---------------------------------------------
//set time.
void RTC_writeClock(void)
  {
    RTC_init();                     //port initilize.
    RTC_ds1302_init();  
    
   int i;                             //define the loop counter.
   rst=1;                             //enable DS1302
   RTC_time_write_1(0xbe);                //
   for(i=0;i<8;i++)                   //continue to write 8 bytes.
     {
       //time_write_1(RTCtable[i]);
       RTC_time_write_1(setTable[i]);        //write one byte
     }
   rst=0;                             //reset
   }

void RTC_setTimeBackup(void)
  {
    RTC_init();                     //port initilize.
    RTC_ds1302_init();  
    
   int i;                             //define the loop counter.
   rst=1;                             //enable DS1302
   RTC_time_write_1(0xbe);                //
   for(i=0;i<8;i++)                   //continue to write 8 bytes.
     {
       RTC_time_write_1(RTCtable[i]);
     }
   rst=0;                             //reset
   }
//---------------------------------------------
//get time.
void RTC_readClock(void)
 {
   int i;                             //set loop counter.
   rst=1;                             //enable DS1302
   RTC_time_write_1(0xbf);                //
   for(i=0;i<7;i++)                   //continue to read 7 bytes.
     {
        getTable[i]=RTC_time_read_1();      //
     }
    rst=0;                            //reset DS1302 */
 }

//--------------------------------------------
//write one byte
void RTC_time_write_1(unsigned char time_tx)
 {
    int j;                            //set the loop counter.
    for(j=0;j<8;j++)                  //continue to write 8bit
      {
        i_o=0;                        //
        sclk=0;                       //pull low clk
        if(time_tx&0x01)              //judge the send bit is 0 or 1.
          {
            i_o=1;                    //is 1
          }
        time_tx=time_tx>>1;           //rotate right 1 bit.
        sclk=1;                       //pull high clk
       }
      sclk=0;                         //finished 1 byte,pull low clk
  }

//---------------------------------------------
//read one byte.
unsigned char RTC_time_read_1(void)
 {
    unsigned char time_rx=0;
   int j;                            //set the loop counter.  
   TRISB4=1;                         //continue to write 8bit 
   for(j=0;j<8;j++)                  
      {
        sclk=0;                       //pull low clk                   
        time_rx=time_rx>>1;           //judge the send bit is 0 or 1.  
        if(i_o)time_rx=time_rx|0x80;
        sclk=1;                       //pull high clk                 
      }                                                              
    TRISB4=0;                        //finished 1 byte,pull low clk  
    sclk=0;                          
    return(time_rx);                 
  }

//--------------------------------------------
//pin define func
void RTC_init(void)
  {
    ADCON1=0X06;                    //a port all i/o
    TRISB=0X02;                     //rb1 input, others output
    OPTION_REG=0X00;                //open b port internal pull high.
   }