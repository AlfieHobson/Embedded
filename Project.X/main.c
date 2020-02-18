#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "main.h"
#include "Thermometer.h"
#include "LCD.h"
#include "buzzer.h"
#include "buttonMatrix.h"
#include "RTC.h"
#include "userInput.h"
#include "EEPROM.h"

//begin config 
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator) 
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled) 
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT disabled) 
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR enabled) 
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming) 
//end config 

void displayTemp();
void checkTemp();
void alarmMode();
void systemReset();
void displayTime();
void delayLong();

struct Temp currentTemp;
struct Time currentTime;

struct Temp savedTemp;
struct Time savedTime;

struct Limits dayLimits = {170 , 210 , 215 , 250};;
struct Limits nightLimits = {170 , 205 , 210 , 245};;

const char *weekdays[] = {"MON","TUE","WED","THU","FRI","SAT","SUN"};

// Throughout the program the LCD is completely cleared.
// On the main screen the time and date only refresh if their values change
// If the screen is cleared (and a day hasn't passed), the date will not appear as it has not changed.
// This bool is to counter that.
bool screenCleared = 1;

int main(int argc, char** argv) {

    /* Initialises LCD display
       PORT A & D as Output */
    LCD_init();
    LCD_clear();   
    
    // If the time needs to be reset on the clock
    RTC_setTimeBackup();
    
    
    // Is EEPROM EMPTY?
    bool emptyEEPROM = EEPROM_isEmpty();
    if (emptyEEPROM) // If EEPROM is empty, this is first boot. Default limits are written to EEPROM
    {   
        // Write default limits to EEPROM
        EEPROM_limitsWrite(dayLimits,1);
        EEPROM_limitsWrite(nightLimits,0);
    }
        
    // Read EEPROM LIMITS
    dayLimits = EEPROM_limitsRead(1);
    nightLimits = EEPROM_limitsRead(0);
    
    // Main program Loop
    while (1) {
        // Get temperature
        currentTemp = therm_getTemp();
        
        // Get time
        currentTime = RTC_getTime();

        LCD_init();
        // DISPLAY TIME On Line 1 & 2
        displayTime();

        // DISPLAY TEMP On Line 3
        displayTemp();
        
        // Check Temperature
        checkTemp();

        // Check for a button press. If pressed the user wants to change a variable of the program.
        int pushButton = pushButtons_checkButtons();
        
        switch (pushButton) {
            case 0: // RB0 indicates the user wants to change the time.
                currentTime = userInput_getTime(currentTime);
                if (currentTime.valid == 1)
                {
                    RTC_setTime(currentTime);
                    // SAVE TIME
                }

                screenCleared = 1; 
                break;
            case 1: // RB1 indicates the user wants to change the date.
                currentTime = userInput_getDate(currentTime);
                
                // If the user wants to save their new time.
                if (currentTime.valid == 1)
                {
                    RTC_setTime(currentTime);
                    // SAVE TIME
                }

                screenCleared = 1;
                break;
            case 2: // RB2 indicates the user wants to change the day temp limits
                dayLimits = userInput_getTempLimits(dayLimits,1);
                EEPROM_limitsWrite(dayLimits,1);
                screenCleared = 1;
                break;
            case 3: // RB3 indicates the user wants to change the night temp limits
                nightLimits = userInput_getTempLimits(nightLimits,0);
                EEPROM_limitsWrite(nightLimits,0);
                screenCleared = 1;
                break;
        }
    }
    return (0);
}


void displayTime() {

    LCD_init();
    // Check if second has changed. If it has update time section of the screen
    if (currentTime.sec2 != savedTime.sec2 || screenCleared) {
        LCD_firstLine();
        LCD_write(weekdays[currentTime.week2 - 1],3);
        //LCD_write("FRI", 3);
        LCD_write("     ", 5);
        LCD_writeChar(currentTime.hour1 + 48);
        LCD_writeChar(currentTime.hour2 + 48);
        LCD_write(":", 1);
        LCD_writeChar(currentTime.min1 + '0');
        LCD_writeChar(currentTime.min2 + '0');
        LCD_write(":", 1);
        LCD_writeChar(currentTime.sec1 + '0');
        LCD_writeChar(currentTime.sec2 + '0');

        // If the date has changed, or if the LCD screen has recently been cleared.
        if (currentTime.day2 != savedTime.day2 || screenCleared == 1)
        {
            LCD_secondLine();
            LCD_write("        ", 8);
            LCD_writeChar(currentTime.day1 + '0');
            LCD_writeChar(currentTime.day2 + '0');
            LCD_write("/", 1);
            LCD_writeChar(currentTime.month1 + '0');
            LCD_writeChar(currentTime.month2 + '0');
            LCD_write("/", 1);
            LCD_writeChar(currentTime.year1 + '0');
            LCD_writeChar(currentTime.year2 + '0');
        }
        savedTime = currentTime;
    }

}

/// Takes temp and displays. Adds 48 to accommodate for ASCII shift.

void displayTemp() {
    LCD_thirdLine();
    LCD_writeChar(currentTemp.tenth + 48);
    LCD_writeChar(currentTemp.first + 48);
    LCD_writeChar('.');
    LCD_writeChar(currentTemp.dp + 48);
    LCD_writeChar('C');
    LCD_write("     ",5);
    //LCD_delay();
}

void checkTemp() {
    int lowLimit, low, high, highLimit;
    // What time is it? Set values below accordingly ***********************************************************************************************
    // 6:30 Am - 7:30 PM = DAY TIME
    // 7:30 Pm - 6:30 AM = NIGHT TIME
    bool day = RTC_isDay(currentTime);
    // If day, set limits to dayLimits
    if (day) {
        lowLimit = dayLimits.lowLimit;
        low = dayLimits.low;
        high = dayLimits.high;
        highLimit = dayLimits.highLimit;
    }
    else { //// If night, set limits to nightLimits
        lowLimit = nightLimits.lowLimit;
        low = nightLimits.low;
        high = nightLimits.high;
        highLimit = nightLimits.highLimit;
    }
    
    
    LCD_fourthLine();
    // DANGER. LOW LIMIT
    if (currentTemp.temperature < lowLimit) {
        // GO INTO ALARM MODE
        LCD_write("TOO LOW! ALARM! ", 16);
        delayLong();
        alarmMode();
    }// LOW
    else if (currentTemp.temperature < low) {
        LCD_write("Low - Heating ON", 16);
        delayLong();
    }// DANGER. HIGH LIMIT  (This is before the HIGH because it needs to be checked first.)
    else if (currentTemp.temperature > highLimit) {
        // GO INTO ALARM MODE
        LCD_write("TOO HIGH! ALARM!", 16);
        delayLong();
        alarmMode();
    }// HIGH
    else if (currentTemp.temperature > high) {
        LCD_write("High - AC ON    ", 16);
        delayLong();
    }// If optimal temperature zone
    else {
        LCD_write("Optimal         ", 16);
        delayLong();
    }

}

void alarmMode(void) {
    // CHECK 0 BREAKS WHILE LOOP
    int alarm = 1;
    while (alarm == 1) {
        buzzer_alarmOn();
        int button = matrix_checkButtons();
        // If any corner button is pressed
        if (button == 19 || button == 16 || button == 1 || button == 4) {
            buzzer_alarmOff();
            systemReset();
            alarm = 0;
        }
    }
}

void systemReset(void) {
    LCD_init();
    LCD_clear();
    LCD_firstLine();
    LCD_write("Alarm Resetting", 16);
    LCD_secondLine();
    LCD_write("in:", 3);
    LCD_thirdLine();
    LCD_writeChar('5');

    // Going to count down in real time.
    currentTime = RTC_getTime(); // Get time to countdown 5 seconds.
    int counter = 4;
    char currentSecond = currentTime.sec2;
    while (counter != 0) // When 4 seconds has passed, exit loop and method.
    {
        currentTime = RTC_getTime(); // Get current time.
        if (currentTime.sec2 != currentSecond) { // If current second is not the saved second, a second has passed. Update board
            LCD_thirdLine();
            LCD_writeChar(counter + 48);
            currentSecond = currentTime.sec2; // Set saved second to current second
            counter--;
        }
    }
    LCD_clear();
}

void delayLong() {
    int i;
    for (i = 0; i < 5000; i++);
}

