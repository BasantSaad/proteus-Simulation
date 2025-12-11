#include "keypad.h"

// Key map for a 4x4 keypad [cite: 177, 178, 179, 180, 181, 182]
static const char keypadLayout4x4[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Key map for a 3x4 keypad (included for completeness, as per PDF structure) [cite: 183, 185, 186, 187, 188]
static const char keypadLayout3x4[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

// Simple software delay function [cite: 189, 190]
void Keypad_Delay(unsigned long x) {
    while (x--);
}

// Keypad Initialization function [cite: 198]
void Keypad_Init(Keypad_Config *config) {
    uint8_t i;

    // Configure row pins as input [cite: 200, 201, 202]
    for (i = 0; i < config->numRows; i++) {
        *(config->TRIS) |= (1 << config->rowPins[i]);
    }

    // Configure column pins as output [cite: 203, 204, 205]
    for (i = 0; i < config->numCols; i++) {
        *(config->TRIS) &= ~(1 << config->colPins[i]);
    }

    // Set all column pins high (or low, depending on logic) [cite: 206, 207, 208]
    // The provided code snippet has a potential error in setting all columns high (|= is usually used for setting a bit)
    // Assuming the intent is to set all column pins to an initial state (often high for scanning).
    // The original logic is: *(config->LAT) = (1 << config->colPins[i]); which only sets the LAT for the current pin in the loop, overwriting previous ones.
    // A better approach would be:
    for (i = 0; i < config->numCols; i++) {
        *(config->LAT) |= (1 << config->colPins[i]);
    }

    // Set pull mode for the row inputs (specific to PIC18F) [cite: 209]
    if (config->pullMode == PULL_UP) {
        INTCON2bits.RBPU = 0; // enable internal pull-up (for PORTB example) [cite: 210, 213, 214]
    } else {
        INTCON2bits.RBPU = 1; // disable (use external pull-down) [cite: 211, 215]
    }
}

// Keypad Scan function [cite: 218]
char Keypad_GetKey(Keypad_Config *config) {
    uint8_t row, col;

    // Loop through each column [cite: 220, 221]
    for (col = 0; col < config->numCols; col++) {
        uint8_t c;

        // Set all columns high (part of the scanning logic setup) [cite: 222]
        for (c = 0; c < config->numCols; c++) {
            *(config->LAT) |= (1 << config->colPins[c]); // Setting all to high
        }

        // Drive the current column low (active column) [cite: 226, 227, 228]
        *(config->LAT) &= ~(1 << config->colPins[col]);

        // Check each row [cite: 229, 230, 231]
        for (row = 0; row < config->numRows; row++) {
            uint8_t rowBit = (1 << config->rowPins[row]); [cite: 232]
            
            // Check for key press based on pull mode

            if (config->pullMode == PULL_UP) { // Key press pulls row low (0) [cite: 233, 234]
                if ((*(config->PORT) & rowBit) == 0) { // Key pressed
                    Keypad_Delay(10); // Debounce delay [cite: 235]
                    if ((*(config->PORT) & rowBit) == 0) { // Re-check after debounce
                        // Wait for key release (optional, for single press detection) [cite: 238]
                        while ((*(config->PORT) & rowBit) == 0); 
                        // Return key character [cite: 239, 245]
                        return (config->numCols == 4) ? 
                            keypadLayout4x4[row][col] : 
                            keypadLayout3x4[row][col];
                    }
                }
            } else { // PULL_DOWN: Key press pulls row high (1) [cite: 237]
                if ((*(config->PORT) & rowBit) != 0) { // Key pressed
                    Keypad_Delay(10); // Debounce delay [cite: 240]
                    if ((*(config->PORT) & rowBit) != 0) { // Re-check after debounce
                        // Wait for key release (optional, for single press detection) [cite: 242]
                        while ((*(config->PORT) & rowBit) != 0); 
                        // Return key character [cite: 243, 246]
                        return (config->numCols == 4) ? 
                            keypadLayout4x4[row][col] : 
                            keypadLayout3x4[row][col];
                    }
                }
            }
        }
    }

    // Restore all columns to high after scan (assuming high is the default inactive state)
    for (col = 0; col < config->numCols; col++) {
        *(config->LAT) |= (1 << config->colPins[col]);
    }

    return '\0'; // No key pressed [cite: 244]
}