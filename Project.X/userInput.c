#include <stdio.h>
#include "LCD.h"
#include "main.h"   // Include main to understand Time Struct
#include "buttonMatrix.h"
#include "RTC.h"

void userInput_displayLimits(struct Limits limits, int day);
void userInput_displayTime(struct Time time);
void userInput_displayDate (struct Time date);

/*
 *         K10     K11     K12     K13
 *         K14     K15     K16     K17
 *         K18     K19     K20     K21
 *         K22     K23     K24     K25
 * 
 *         13    9     5    1
 *         14    10    6    2
 *         15    11    7    3
 *         16    12    8    4
 * 
 *     If you're looking at it upside down (BIG LCD WAY)
 * 
 *         K25     K24     K23     K22
 *         K21     K20     K19     K18
 *         K17     K16     K15     K14
 *         K13     K12     K11     K10
 *      
 *         4    8    12    16
 *         3    7    11    15
 *         2    6    10    14
 *         1    5    9     13
 */
int daysInMonth[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

struct Time userInput_getDate(struct Time date)
{
    userInput_displayDate(date);
    
    //SET THE CURSOR BACK TO THE FIRST POSITION
    LCD_writeCmd(0x84);
    
    int dateAddressCounter = 0;
    bool buttonPress = 0;
    bool increment = 0;
    bool decrement = 0;
    // Left address : 0x82
    // Middle address : 0x83
    // Right address : 0x85
    // LOOP THROUGH BUTTON INPUT
    while (1)
    {
        int button = matrix_checkButtons();
        switch (button)
        {
            case 0:
                buttonPress = 0;
                break;
            case 2:
                //Move the cursor LEFT.
                //Do not allow left movement if the cursor is at the first address.
                if (!buttonPress && dateAddressCounter != 0) dateAddressCounter--;
                buttonPress = 1;
                break;
            case 5:
                if (!buttonPress) decrement = 1;
                break;
            case 7: 
                if (!buttonPress) increment = 1;
                break;    
            case 10:
                //Move RIGHT.
                //Do not allow right movement if the cursor is at the third address.
                if (!buttonPress && dateAddressCounter != 3) dateAddressCounter++;
                buttonPress = 1;
                break;
                
            case 13:
                date.valid = 0;
                return date;
                break;
            case 16:
                date.valid = 1;
                return date;
                break;    
        }
        // Get current month, and how many days there are. -1
        int month = ((date.month1)*10) + (date.month2) -1; //12
        int dayLimit = daysInMonth[month];              //31
        
        

        // Which value are we changing?
        switch (dateAddressCounter) 
        {
            int day;
            // Day
            case 0:
                LCD_writeCmd(0x82);
                
                // Convert to nibbles to an int we can use for comparison.
                day = ((date.day1)*10) + (date.day2);
                
                if (increment == 1)
                {
                    // If the current day is the max for the current month, set to 0.
                    if (day >= dayLimit)  day = 1;
                    else                  day ++;
                    
                    // Now we have to split our int Day back into two nibbles.
                    date.day1 = (day / 10) % 10;
                    date.day2 = (day % 10);
                    
                    userInput_displayDate(date);
                    increment = 0;
                }
                else if (decrement == 1) {
                    if (day == 0)   day = dayLimit;
                    else            day --;
                    
                    // Now we have to split our int Day back into two nibbles.
                    date.day1 = (day / 10) % 10;
                    date.day2 = (day % 10);
                    
                    userInput_displayDate(date);
                    decrement = 0;
                }
                break;

            // Month
            case 1:
                LCD_writeCmd(0x83);
                
                // Convert to nibbles to an int we can use for comparison.
                int month = ((date.month1)*10) + (date.month2);
                day = ((date.day1)*10) + (date.day2);
                
                if (increment == 1)
                {
                    // If current month == 12, next month is 1.
                    if (month == 12)  month = 1;
                    else              month ++;
                    
                    // Now we have to split our int month back into two nibbles.
                    date.month1 = (month / 10) % 10;
                    date.month2 = (month % 10);
                    
                    // Is day still alright
                    if (day > daysInMonth[month-1] ) 
                    {
                        day = daysInMonth[month-1];
                        date.day1 = (day / 10) % 10;
                        date.day2 = (day % 10);
                    }
                    userInput_displayDate(date);
                    increment = 0;
                }
                if (decrement == 1)
                {
                    // If current month == 12, next month is 1.
                    if (month == 1)  month = 12;
                    else              month --;
                    
                    // Now we have to split our int month back into two nibbles.
                    date.month1 = (month / 10) % 10;
                    date.month2 = (month % 10);
                    
                    // Is day still alright
                    if (day > daysInMonth[month-1] ) 
                    {
                        day = daysInMonth[month-1];
                        date.day1 = (day / 10) % 10;
                        date.day2 = (day % 10);
                    }
                    
                    userInput_displayDate(date); 
                    decrement = 0;
                }
                break;

            // Year
            case 2:
                LCD_writeCmd(0x85);
                
                // Convert to nibbles to an int we can use for comparison.
                int year = ((date.year1)*10) + (date.year2);           
                if (increment == 1)
                {
                    // If current month == 12, next month is 1.
                    if (year == 99)  year = 0;
                    else             year ++;
                    

                    // Have to split our int month back into two nibbles.
                    date.year1 = (year / 10) % 10;
                    date.year2 = (year % 10);
                    
                    userInput_displayDate(date); 
                    increment = 0;
                }
                if (decrement == 1)
                {
                    // If current year == 00, next year is 99.
                    if (year == 0)  year = 99;
                    else            year --;
                    
                    // Have to split our int month back into two nibbles.
                    date.year1 = (year / 10) % 10;
                    date.year2 = (year % 10);
                    
                    userInput_displayDate(date); 
                    decrement = 0;
                }
                break;
        }
        
    }
    
}


struct Time userInput_getTime(struct Time time) {
    // DISPLAY CURRENT TIME
    userInput_displayTime(time);

    // GO TO FIRST POSITION
    LCD_writeCmd(0x82);

    int timeAddressCounter = 0;
    bool buttonPress = 0;
    bool increment = 0;
    bool decrement = 0;
    int exit = 0;
    // Left address : 0x82
    // Middle address : 0x83
    // Right address : 0x85
    // LOOP THROUGH BUTTON INPUT
    // LOOP THROUGH BUTTON INPUT
    while (1) 
    {
        int button = matrix_checkButtons();
        switch (button) {
            case 0:     // No button is being pressed
                buttonPress = 0;
                break;
            case 2:     // Move LEFT
                //Do not allow left movement if the cursor is at the first address.
                if (!buttonPress && timeAddressCounter != 0) timeAddressCounter--;
                buttonPress = 1;
                break;
            case 5:     // DECREMENT
                if (!buttonPress) decrement = 1;
                break;
            case 7:     // INCREMENT
                if (!buttonPress) increment = 1;
                break;
            case 10:    // Move RIGHT.
                //Do not allow right movement if the cursor is at the third address.
                if (!buttonPress && timeAddressCounter != 3) timeAddressCounter++;
                buttonPress = 1;
                break;
            case 13:    // EXIT without saving
                time.valid = 0;
                return time;
                break;
            case 16:    // SAVE
                time.valid = 1;
                return time;
                break;
        }

        switch (timeAddressCounter) {
            //HOURS
            case 0:
                LCD_writeCmd(0x82);
                if (increment == 1)
                {
                    if (time.hour2 == 0x09)
                    {
                        time.hour1++;
                        time.hour2 = 0x00;
                    }
                    else if (time.hour2 == 0x03 && time.hour1 == 0x02)
                    {
                        time.hour1 = 0x00;
                        time.hour2 = 0x00;
                    }
                    else
                    {
                        time.hour2++;
                    }
                    userInput_displayTime(time);
                    increment = 0;
                }
                else if (decrement == 1)
                {
                    if (time.hour1 == 0x00 && time.hour2 == 0x00)
                    {
                        time.hour1 = 0x02;
                        time.hour2 = 0x03;
                    }
                    else if (time.hour2 == 0x00)
                    {
                        time.hour2 = 0x09;
                        if (time.hour1 == 0x00)
                        {
                            time.hour1 = 0x05;
                        }
                        else
                        {
                            time.hour1--;
                        }
                    } 
                    else
                    {
                        time.hour2--;
                    }
                    userInput_displayTime(time);
                    decrement = 0;
                }
                break;

            //MINUTES
            case 1:
                LCD_writeCmd(0x83);
                if (increment == 1)
                {
                    if (time.min2 == 0x09)
                    {
                        time.min2 = 0x00;
                        if (time.min1 == 0x05)
                        {
                            time.min1 = 0x00;
                        }
                        else
                        {
                            time.min1++;
                        }     
                    }
                    else
                    {
                        time.min2++;
                    }
                    userInput_displayTime(time);
                    increment = 0;
                }
                else if (decrement == 1)
                {
                   if (time.min1 == 0x00 && time.min2 == 0x00)
                    {
                        time.min1 = 0x05;
                        time.min2 = 0x09;
                    }
                   else if (time.min2 == 0x00)
                   {
                       time.min1--;
                       time.min2 = 0x09;
                   }
                   else
                   {
                       time.min2--;
                   }
                   userInput_displayTime(time);
                   decrement = 0;
                }
                break;

            //SECONDS
            case 2:
                LCD_writeCmd(0x85);
                if (increment == 1)
                {
                    if (time.sec2 == 0x09)
                    {
                        time.sec2 = 0x00;
                        if (time.sec1 == 0x05)
                        {
                            time.sec1 = 0x00;
                        }
                        else
                        {
                            time.sec1++;
                        }     
                    }
                    else
                    {
                        time.sec2++;
                    }
                    userInput_displayTime(time);
                    increment = 0;
                }
                else if (decrement == 1)
                {
                   if (time.sec1 == 0x00 && time.sec2 == 0x00)
                    {
                        time.sec1 = 0x05;
                        time.sec2 = 0x09;
                    }
                   else if (time.sec2 == 0x00)
                   {
                       time.sec1--;
                       time.sec2 = 0x09;
                   }
                   else
                   {
                       time.sec2--;
                   }
                   userInput_displayTime(time);
                   decrement = 0;
                }
                break;
        }
    }
}

struct Limits userInput_getTempLimits(struct Limits limit, int day) {

    // Make a new set of limits. Prevents us from erasing original limits incase the user wants to cancel the operation
    struct Limits newLimits = limit;
    // Display current limits.
    userInput_displayLimits(newLimits,day);
    // Set cursor to first variable
    LCD_writeCmd(0x91);

    int addressCounter = 0;
    bool buttonPress = 0;
    bool increment = 0;
    bool decrement = 0;
    // Top left address : 0x91
    // Top right address : 0x96
    // Bottom left address : 0x99
    // Bottom right address : 0x9E
    // LOOP THROUGH BUTTON INPUT
    int exit = 0;
    while (exit == 0) {
        int button = matrix_checkButtons();
        switch (button) {
                // ONE OF THESE HAS TO MAKE exit = 1 TO EXIT THE LOOP
            case 0:
                buttonPress = 0;
                break;
                break;
            case 2: // MOVE LEFT
                // If address counter is at 0, don't decrement.
                if (!buttonPress && addressCounter != 0) addressCounter--;
                buttonPress = 1;
                break;
            case 5: //Down
                decrement = 1;
                break;
            case 7: // Up
                increment = 1;
                break;
            case 10: // MOVE RIGHT
                // If address counter is at 4, we are at the end. No where else to go.
                if (!buttonPress && addressCounter != 4) addressCounter++;
                buttonPress = 1;
                break;
            case 13: // Exit without saving. Return the original limits.
                return limit;
                break;
            case 16: // Save and exit. Return the new limits
                return newLimits;
                break;
        }
        // Sets the cursor position.
        switch (addressCounter) {
            case 0:
                // Low Danger Limit, Top Left
                LCD_writeCmd(0x91);
                // If the increment button has been pressed, increment limit at this area of the screen, the lowLimit.
                // Also check if the limit is at 100, if so, don't let it increment
                if (increment == 1 && newLimits.lowLimit!= 100) {
                    newLimits.lowLimit++;
                    userInput_displayLimits(newLimits,day);
                    increment = 0;
                } 
                // If the decrement button has been pressed, decrement limit at this area of the screen, the lowLimit.
                // Also check if the limit is at 0, if so, don't let it decrement
                else if (decrement == 1) {
                    newLimits.lowLimit--;
                    userInput_displayLimits(newLimits,day);
                    decrement = 0;
                }
                break;
            case 1:
                // Low limit, Top Right
                LCD_writeCmd(0x96);
                if (increment == 1) {
                    newLimits.low++;
                    userInput_displayLimits(newLimits,day);
                    increment = 0;
                } else if (decrement == 1) {
                    newLimits.low--;
                    userInput_displayLimits(newLimits,day);
                    decrement = 0;
                }
                break;
            case 2:
                // High Limit, Bottom Left
                LCD_writeCmd(0x99);
                if (increment == 1) {
                    newLimits.high++;
                    userInput_displayLimits(newLimits,day);
                    increment = 0;
                } else if (decrement == 1) {
                    newLimits.high--;
                    userInput_displayLimits(newLimits,day);
                    decrement = 0;
                }
                break;
            case 3:
                // High Danger limit, Bottom right
                LCD_writeCmd(0x9E);
                if (increment == 1) {
                    newLimits.highLimit++;
                    userInput_displayLimits(newLimits,day);
                    increment = 0;
                } else if (decrement == 1) {
                    newLimits.highLimit--;
                    userInput_displayLimits(newLimits,day);
                    decrement = 0;
                }
                break;
        }
    }
}

void userInput_displayLimits(struct Limits limits, int day) {
    LCD_init();
    LCD_clear();
    LCD_writeCmd(0x81);
    if (day)  LCD_write("DAY LIMITS", 10);
    else LCD_write("NIGHT LIMITS", 12);
    LCD_secondLine();

    // TOP LEFT, Low limit
    char LL0 = (limits.lowLimit / 100) % 10;
    char LL1 = (limits.lowLimit / 10) % 10;
    char LL2 = (limits.lowLimit % 10);

    LCD_writeChar(' ');
    LCD_writeChar(LL0 + '0');
    LCD_writeChar(LL1 + '0');
    LCD_writeChar('.');
    LCD_writeChar(LL2 + '0');

    // TOP RIGHT, Low
    LCD_write("      ", 6);
    char L0 = (limits.low / 100) % 10;
    char L1 = (limits.low / 10) % 10;
    char L2 = (limits.low % 10);

    LCD_writeChar(L0 + '0');
    LCD_writeChar(L1 + '0');
    LCD_writeChar('.');
    LCD_writeChar(L2 + '0');

    // BOTTOM LEFT, High
    LCD_fourthLine();
    char H0 = (limits.high / 100) % 10;
    char H1 = (limits.high / 10) % 10;
    char H2 = (limits.high % 10);

    LCD_writeChar(' ');
    LCD_writeChar(H0 + '0');
    LCD_writeChar(H1 + '0');
    LCD_writeChar('.');
    LCD_writeChar(H2 + '0');

    // BOTTOM RIGHT, High Limit
    LCD_write("      ", 6);
    char HL0 = (limits.highLimit / 100) % 10;
    char HL1 = (limits.highLimit / 10) % 10;
    char HL2 = (limits.highLimit % 10);

    LCD_writeChar(HL0 + '0');
    LCD_writeChar(HL1 + '0');
    LCD_writeChar('.');
    LCD_writeChar(HL2 + '0');

    //_eereg_eeprom_read ADDRESS 10C
    
}

void userInput_displayTime (struct Time time)
{
    LCD_init();
    LCD_clear();
    //LCD_firstLine();
    LCD_writeCmd(0x82);
    LCD_writeChar(time.hour1 + 48);
    LCD_writeChar(time.hour2 + 48);
    LCD_writeChar(':');
    LCD_writeChar(' ');
    LCD_writeChar(time.min1 + '0');
    LCD_writeChar(time.min2 + '0');
    LCD_writeChar(':');
    LCD_writeChar(time.sec1 + '0');
    LCD_writeChar(time.sec2 + '0');
}
void userInput_displayDate (struct Time date) {
    LCD_init();
    LCD_clear();
    LCD_writeCmd(0x82);
    LCD_writeChar(date.day1 + 48);
    LCD_writeChar(date.day2 + 48);
    LCD_writeChar('/');
    LCD_writeChar(date.month1 + '0');
    LCD_writeChar(date.month2 + '0');
    LCD_writeChar('/');
    LCD_writeChar(date.year1 + '0');
    LCD_writeChar(date.year2 + '0');
}