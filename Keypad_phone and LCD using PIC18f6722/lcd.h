#ifndef LCD_H
#define LCD_H

#include <p18f6722.h>
#include "delays.h"
#pragma config LVP = OFF
ADCON1 = 0x0F;
CMCON = 0x07;  
TRISD = 0x00;
TRISC = 0x00;

typedef struct {
    volatile unsigned char *LAT_RS;
    volatile unsigned char *LAT_RW;
    volatile unsigned char *LAT_E;

    volatile unsigned char *TRIS_RS;
    volatile unsigned char *TRIS_RW;
    volatile unsigned char *TRIS_E;

    volatile unsigned char *LAT_DATA[8];
    volatile unsigned char *TRIS_DATA[8];

    unsigned char bit_RS;
    unsigned char bit_RW;
    unsigned char bit_E;
    unsigned char bit_DATA[8];
} LCD_Config;

void delay(unsigned long x);
TRISD = 0x00;   // Force all D0-D7 to output

void LCD_Init(LCD_Config *lcd);
void LCD_Command(LCD_Config *lcd, unsigned char cmd);
void LCD_Char(LCD_Config *lcd, unsigned char data);
void LCD_String(LCD_Config *lcd, const char *str);

#endif