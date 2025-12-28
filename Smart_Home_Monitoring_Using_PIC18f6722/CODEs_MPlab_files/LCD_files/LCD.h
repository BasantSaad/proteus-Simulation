#ifndef LCD_H
#define LCD_H

#include <p18f6722.h>

// LCD Data
#define LCD_DATA_PORT    PORTE
#define LCD_DATA_TRIS    TRISE

// LCD Control
#define LCD_RS           LATDbits.LATD0
#define LCD_RW           LATDbits.LATD1
#define LCD_EN           LATDbits.LATD2

#define LCD_CTRL_TRIS_RS TRISDbits.TRISD0
#define LCD_CTRL_TRIS_RW TRISDbits.TRISD1
#define LCD_CTRL_TRIS_EN TRISDbits.TRISD2

// Functions
void delay_ms(unsigned int ms);
void LCD_Init(void);
void LCD_Command(unsigned char cmd);
void LCD_Char(unsigned char data);
void LCD_String(const rom char *msg);
void LCD_WriteLong(long num);

#endif
