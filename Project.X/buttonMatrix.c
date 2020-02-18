#include "main.h"

/*
 * SEE QL200_SCH.
 *     PAGE 2
 *     4X4 KEY MATRIX
       
   BUTTON SETUP
 * K10 = 1110 0111  =  0xE7        K18 = 1011 0111  =  0xB7
 * K11 = 1110 1011  =  0xEB        K19 = 1011 1011  =  0xBB
 * K12 = 1110 1101  =  0xED        K20 = 1011 1101  =  0xBD
 * K13 = 1110 1110  =  0xEE        K21 = 1011 1110  =  0xBE
 * K14 = 1101 0111  =  0xD7        K22 = 0111 0111  =  0x77
 * K15 = 1101 1011  =  0xDB        K23 = 0111 1011  =  0x7B
 * K16 = 1101 1101  =  0xDD        K24 = 0111 1101  =  0x7D
 * K17 = 1101 1110  =  0xDE        K25 = 0111 1110  =  0x7E
 */

int button;

void matrix_init(void) {
    TRISC = 0XF0; //First 4 bits as INPUT, Last 4 bits as OUTPUT                               
    PORTC = 0XFF; //Clear PORTC
}

//delay program

void matrix_buttonDelay(void) //delay program 
{
    int i; //define integer variable
    for (i = 0x1000; i--;); //delay
}

void matrix_setInputs(int counter) {
    switch (counter) {
        case 0: RC0 = 0;
        RC1 = 1;
        RC2 = 1;
        RC3 = 1;
            break;
        case 1: RC0 = 1; RC1 = 0; RC2 = 1; RC3 = 1;
            break;
        case 2: RC0 = 1; RC1 = 1; RC2 = 0; RC3 = 1;
            break;
        case 3: RC0 = 1; RC1 = 1; RC2 = 1; RC3 = 0;
            break;
    }
}

int matrix_checkButtons(void) {
    // Initalise Matrix
    matrix_init();

    int counter = 0;
    //unsigned char outputBits[]={0xFE, 0xFD, 0xFB, 0xF7}; 

    for (int i = 0; i < 4; i++) 
    {
        matrix_setInputs(i);
        matrix_buttonDelay();
        for (int j = 0; j < 4; j++) {
            //button = PORTC; // Get current PORT C
            //button = button & 0xf0; // Clear the OUTPUT bits as they are no longer needed. Only need to check input bits
            //If the first row is pressed
            if (RC4 == 0) {
                return counter += 1;
            }                //If the second row is pressed
            else if (RC5 == 0) {
                return counter += 2;
            }                //If the third row is pressed
            else if (RC6 == 0) {
                return counter += 3;
            }
                //If the fourth row is pressed
            else if (RC7 == 0) {
                return counter += 4;
            }
        }
        counter += 4;
    }
    return 0;
}