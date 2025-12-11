#include <p18f6722.h>

// Configuration bits
#pragma config OSC = HS   // High-speed oscillator
#pragma config WDT = OFF  // Watchdog Timer off

#define _XTAL_FREQ 4000000  // 4 MHz crystal

//////////////////////////////////////////////////////////////////////////////////////////////////
// 7-Segment Display Setup
//////////////////////////////////////////////////////////////////////////////////////////////////

// Common Cathode 7-segment patterns (0-9)
const unsigned char segment_patterns[] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

// Ports
#define SEGMENT_PORT LATB     // Segments a-g
#define DIGIT_PORT LATC       // Digit select
#define DIGIT1_EN 0b00000001  // Bit0 = Digit1
#define DIGIT2_EN 0b00000010  // Bit1 = Digit2

//////////////////////////////////////////////////////////////////////////////////////////////////
// Timer0 Setup
//////////////////////////////////////////////////////////////////////////////////////////////////

void timer0_init() {
    T0CON = 0x80; // 16-bit mode, prescaler assigned to TMR0, timer ON
    TMR0H = 0x5E; // Preload for 1-second timing
    TMR0L = 0xA0;
    T0CONbits.TMR0ON = 1; // Start Timer0
}

unsigned int timer0_read() {
    return (TMR0H << 8) | TMR0L;
}

void timer0_reload() {
    // Reload to count remaining ticks for 1 second
    TMR0H = 0x5E;
    TMR0L = 0xA0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Display Function
//////////////////////////////////////////////////////////////////////////////////////////////////

void display_2digit(unsigned char value) {
    // Digit 1 (tens)
    SEGMENT_PORT = segment_patterns[value / 10];
    DIGIT_PORT = DIGIT1_EN;
    __delay_ms(5);  // Small delay for persistence of vision

    // Digit 2 (ones)
    SEGMENT_PORT = segment_patterns[value % 10];
    DIGIT_PORT = DIGIT2_EN;
    __delay_ms(5);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////////////////////////////////////////////////

void main() {
    unsigned char seconds = 0;
    unsigned int count;

    // Initialize ports
    TRISB = 0x00; // PORTB output for segments
    TRISC = 0x00; // PORTD output for digits
    LATB = 0x00;
    LATC = 0x00;

    timer0_init();

    while(1) {
        // Read Timer0
        count = timer0_read();

        if(count >= 65535) { // Timer overflow
            seconds++;
            if(seconds >= 100) seconds = 0; // reset after 99
            timer0_reload(); // reload for next second
        }

        // Display current seconds
        display_2digit(seconds);
    }
}
