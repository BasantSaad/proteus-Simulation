#ifndef KEYPAD_H
#define KEYPAD_H

#include <p18f6722.h> // Example header for PIC18F6722
#include <stdint.h>   // Includes standard integer types [cite: 128]

// ENUMS for Pull Mode [cite: 143, 144, 146, 147]
typedef enum {
    PULL_UP,
    PULL_DOWN
} PullType;

// STRUCTURES for Keypad Configuration [cite: 148, 160]
typedef struct {
    volatile unsigned char *PORT; // PORT register address [cite: 149]
    volatile unsigned char *LAT;  // LAT register address [cite: 151]
    volatile unsigned char *TRIS; // TRIS register address [cite: 153]

    // Pin configurations (up to 4 rows and 4 columns) [cite: 155, 156, 157, 158]
    uint8_t rowPins[4];
    uint8_t colPins[4];
    uint8_t numRows;
    uint8_t numCols;

    PullType pullMode; // PULL_UP or PULL_DOWN mode [cite: 159, 170]
} Keypad_Config;

// FUNCTION PROTOTYPES [cite: 162]
void Keypad_Delay(unsigned long x); // Simple delay function [cite: 171]
void Keypad_Init(Keypad_Config *config); // Initializes keypad pins [cite: 172]
char Keypad_GetKey(Keypad_Config *config); // Scans and returns the pressed key [cite: 173]

#endif // KEYPAD_H