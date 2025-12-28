#include "lcd.h"
#include <delays.h>

void delay_ms(unsigned int ms) {
    unsigned int i;
    for (i = 0; i < ms; i++) {
        Delay1KTCYx(2);
    }
}

void LCD_Command(unsigned char cmd) {
    LCD_DATA_PORT = cmd;
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 1;
    delay_ms(1);
    LCD_EN = 0;
    delay_ms(2);
}

void LCD_Char(unsigned char data) {
    LCD_DATA_PORT = data;
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 1;
    delay_ms(1);
    LCD_EN = 0;
    delay_ms(1);
}

void LCD_Init(void) {
    LCD_DATA_TRIS = 0x00;

    LCD_CTRL_TRIS_RS = 0;
    LCD_CTRL_TRIS_RW = 0;
    LCD_CTRL_TRIS_EN = 0;

    LCD_RS = 0; LCD_RW = 0; LCD_EN = 0;

    delay_ms(20);

    LCD_Command(0x38);
    LCD_Command(0x0C);
    LCD_Command(0x01);
    LCD_Command(0x06);
}

void LCD_String(const rom char *msg) {
    while (*msg) {
        LCD_Char(*msg++);
    }
}

void LCD_WriteLong(long num) {
    char str[12];
    int i = 0, j;
    char temp;

    if (num == 0) {
        LCD_Char('0');
        return;
    }

    if (num < 0) {
        LCD_Char('-');
        num = -num;
    }

    while (num > 0) {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }

    for (j = 0; j < i / 2; j++) {
        temp = str[j];
        str[j] = str[i - 1 - j];
        str[i - 1 - j] = temp;
    }

    for (j = 0; j < i; j++) {
        LCD_Char(str[j]);
    }
}
