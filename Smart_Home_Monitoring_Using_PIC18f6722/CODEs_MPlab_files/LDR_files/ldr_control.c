#include "ldr_control.h"
#include "lcd.h"

void Set_Lamp_Brightness(unsigned int ldr_value) {
    unsigned int duty_cycle;
    
    // Clamp LDR value to expected range (11-989)
    if(ldr_value < 11) ldr_value = 11;
    if(ldr_value > 989) ldr_value = 989;
    
    // Inverse mapping: dark room = bright lamp
    duty_cycle = (unsigned long)(989 - ldr_value) * 499 / (989 - 11);
    
    // Set CCP3 (Lamp) duty cycle
    CCPR3L = duty_cycle >> 2;
    CCP3CONbits.DC3B0 = (duty_cycle & 0x01);
    CCP3CONbits.DC3B1 = ((duty_cycle >> 1) & 0x01);
}
void Display_LDR_LCD(unsigned int ldr_value)
{
    unsigned int ldr_percent = (unsigned int)(((unsigned long)ldr_value * 100) / 989);
    
    LCD_Command(0xC0);
    LCD_String("LDR: ");
    
    if(ldr_percent == 100)
    {
        LCD_String("100%");
    }
    else
    {
        LCD_Char((ldr_percent / 10) + '0'); // Tens
        LCD_Char((ldr_percent % 10) + '0'); // Ones
        LCD_Char('%');
		
		/*// Display duty value (0–624)
		LCD_String("Reading: ");
		LCD_Char((ldr_reading / 1000) + '0');           // Hundreds
		LCD_Char((ldr_reading / 100) + '0');           // Hundreds
		LCD_Char(((ldr_reading % 100) / 10) + '0');    // Tens
		LCD_Char((ldr_reading % 10) + '0');  */          // Ones
    }
}