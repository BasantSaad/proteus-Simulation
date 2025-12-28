#ifndef KEYPAD_H
#define KEYPAD_H

#include <p18f6722.h>

// Keypad
#define KEYPAD_PORT PORTB
#define KEYPAD_TRIS TRISB

unsigned char Keypad_Read(void);

#endif
