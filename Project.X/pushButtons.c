#include "main.h"

// Will check the set of 4 push buttons on PORT B and return an integer accordingly if pressed.
// Returns 5 if no buttons has been pressed.

int pushButtons_checkButtons(void) {
    // Initialisation
    TRISB = 0x0F; // First 4 bits of PORT B as OUTPUT

    // Check each button bit.
    if (RB0 == 0)
        return 0;
    else if (RB1 == 0)
        return 1;
    else if (RB2 == 0)
        return 2;
    else if (RB3 == 0)
        return 3;
    return 4;
}