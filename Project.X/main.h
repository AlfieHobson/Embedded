#include<pic.h>              //include MCU head file
__CONFIG(0x1832);
#include <xc.h>  
#include <stdio.h>
#include <stdbool.h>

struct Temp {
    char tenth;
    char first;
    char dp;
    int temperature;
};

struct Time {
    char sec1;
    char sec2;
    char min1;
    char min2;
    char hour1;
    char hour2;
    char day1;
    char day2;
    char month1;
    char month2;
    char week1;
    char week2;
    char year1;
    char year2;
    int valid;
};

struct Limits {
    int lowLimit;
    int low;
    int high;
    int highLimit;
};

