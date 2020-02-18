#include "main.h" // include main to get limit structure

/* EEPROM LAYOUT
 *  ADDRESS      DATA
 *      0       dayLowLimit
 *      1       dayLow     
 *      2       dayHigh
 *      3       dayLowLimit           
 * 
 *      4       nightLowLimit
 *      5       nightLow
 *      6       nightHigh   
 *      7       nightHighLimit
 */


struct Limits EEPROM_limitsRead (bool day){
    int addressStart;
    // If day, set address to 0 and read day limits.
    // If night, set address to 4, where night limits begin.
    if (day) addressStart = 0; 
    else     addressStart = 4;
    
    // Read limits from EEPROM
    int lowLimit, low, high, highLimit;
    lowLimit = (int)eeprom_read(addressStart);
    low = (int)eeprom_read(addressStart+1);
    high = (int)eeprom_read(addressStart+2);
    highLimit = (int)eeprom_read(addressStart+3);
    
    // Return Limits
    struct Limits dayLimit = {lowLimit, low, high, highLimit};
    return dayLimit;
}

void EEPROM_limitsWrite (struct Limits limits, bool day){
    int addressStart;
    // If day, set address to 0 and write day limits.
    // If night, set address to 4, where night limits begin.
    if (day) addressStart = 0; 
    else     addressStart = 4; 
    
    // Write limits to EEPROM
    eeprom_write(addressStart,limits.lowLimit);
    eeprom_write(addressStart + 1,limits.low);
    eeprom_write(addressStart + 2,limits.high);
    eeprom_write(addressStart + 3,limits.highLimit);
}

bool EEPROM_isEmpty(void) {
    bool empty = 0;
    
    // Check the first 4 bytes where the day limits remain.
    int test1 = (int)eeprom_read(0);   
    int test2 = (int)eeprom_read(1);
    int test3 = (int)eeprom_read(2);
    int test4 = (int)eeprom_read(3);
    
    // If all set to FF, it is assumed that EEPROM is empty.
    if (test1 == 255 && test2 == 255 && test3 == 255 && test4 == 255 ) empty = 1; 
    
    return empty;
}