#include "KEYPAD.h"
#include "lcd.h"

unsigned char Keypad_Read(void) {
    unsigned char row, col;

    KEYPAD_TRIS = 0xF0;
    INTCON2bits.RBPU = 0;

    for (row = 0; row < 4; row++) {
        LATB = 0xFF;

        if (row == 0) LATBbits.LATB0 = 0;
        if (row == 1) LATBbits.LATB1 = 0;
        if (row == 2) LATBbits.LATB2 = 0;
        if (row == 3) LATBbits.LATB3 = 0;

        if (PORTBbits.RB4 == 0) col = 0;
        else if (PORTBbits.RB5 == 0) col = 1;
        else if (PORTBbits.RB6 == 0) col = 2;
        else if (PORTBbits.RB7 == 0) col = 3;
        else continue;

        delay_ms(20);

        while (!PORTBbits.RB4 || !PORTBbits.RB5 ||
               !PORTBbits.RB6 || !PORTBbits.RB7);

        if (row == 0) return "789/"[col];
        if (row == 1) return "456*"[col];
        if (row == 2) return "123-"[col];
        if (row == 3) return "C0=+"[col];
    }
    return 0;
}
