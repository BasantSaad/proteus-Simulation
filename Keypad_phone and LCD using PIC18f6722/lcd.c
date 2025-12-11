#include "lcd.h"

void delay(unsigned long x) {
    while(x--);
}

// Generate enable pulse for LCD
void LCD_PulseEnable(LCD_Config *lcd) {
    *(lcd->LAT_E) |= (1 << lcd->bit_E);   // Set E high
    delay(5);                             // Small delay (270ns)
    *(lcd->LAT_E) &= ~(1 << lcd->bit_E);  // Set E low
}
// Write a byte to LCD (data or command)
static void LCD_WriteByte(LCD_Config *lcd, unsigned char d, char isData) {
    char i;

    if (isData)
        *(lcd->LAT_RS) |= (1 << lcd->bit_RS);  // RS = 1 for data
    else
        *(lcd->LAT_RS) &= ~(1 << lcd->bit_RS); // RS = 0 for command

    *(lcd->LAT_RW) &= ~(1 << lcd->bit_RW); // RW = 0 for write

    // Set data pins
    for (i = 0; i < 8; i++) {
        if (d & (1 << i))
            *(lcd->LAT_DATA[i]) |= (1 << lcd->bit_DATA[i]);
        else
            *(lcd->LAT_DATA[i]) &= ~(1 << lcd->bit_DATA[i]);
    }

    LCD_PulseEnable(lcd); // Send pulse
}

#include <delays.h>

void delay_ms(unsigned int ms)
{
    while(ms--)
        Delay1KTCYx(2);   // ~1ms at 8MHz
}

// Send command to LCD
void LCD_Command(LCD_Config *lcd, unsigned char cmd) {
    LCD_WriteByte(lcd, cmd, 0);
    delay(2); // Short delay (~2ms)
}

// Send data (character) to LCD
void LCD_Char(LCD_Config *lcd, unsigned char data) {
    LCD_WriteByte(lcd, data, 1);
    delay(1); // Short delay (~1ms)
}

// Send string to LCD
void LCD_String(LCD_Config *lcd, const char *str) {
    while (*str) 
        LCD_Char(lcd, *str++);
}
// Initialize LCD
void LCD_Init(LCD_Config *lcd) {
    char i;
    TRISD = 0x00;  // Force data pins to output (important for Proteus)

    // Set control pins as output
    *(lcd->TRIS_RS) &= ~(1 << lcd->bit_RS);
    *(lcd->TRIS_RW) &= ~(1 << lcd->bit_RW);
    *(lcd->TRIS_E) &= ~(1 << lcd->bit_E);

    // Set data pins as output
    for (i = 0; i < 8; i++)
        *(lcd->TRIS_DATA[i]) &= ~(1 << lcd->bit_DATA[i]);

    delay(312500); // Wait 20ms for LCD power-up

    LCD_Command(lcd, 0x38); // Function set: 8-bit, 2 lines, 5x7 font
    LCD_Command(lcd, 0x0C); // Display ON, Cursor OFF
    LCD_Command(lcd, 0x01); // Clear display
    delay_ms(20);
    LCD_Command(lcd, 0x06); // Entry mode: increment cursor
}