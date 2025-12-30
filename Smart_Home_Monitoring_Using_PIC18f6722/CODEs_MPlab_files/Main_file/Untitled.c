
#include <p18f6722.h>
#include "lcd.h"
#include "security.h"
#include "temp_control.h"
#include "ldr_control.h"
#include "Timer2.h"
#include "timer0.h"
// ===== Config =====

#pragma config OSC = HS
#pragma config WDT = OFF


void main(void) {
    // 1. ALL variables must be at the very top for MCC18
	unsigned int ldr_reading;
    unsigned int temp_reading;
	unsigned int temperature_celsius;

    ADCON1 = 0x0E;      // AN0 is Analog 
    LCD_Init();
    Security_Init();
    Timer0_Init();                  // Initialize Timer0 for button toggle

    Security_Run();  // This will only return after correct password is entered
    Three_PWM_Init();
	ADC_Init();


	while(1){
    // 4. Once we get here, access was granted
        if(reset_flag) {
            System_Reset();         // Perform system reset
            reset_flag = 0;         // Clear flag
	    	//delay_ms(1000);
        }

	    LCD_Command(0x01);
	    LCD_String("Temp Control Sys");
	    LCD_Command(0xC0);
	    LCD_String("Starting...");
	    //delay_ms(1000);

		temp_reading = ADC_Read(0);
		ldr_reading = ADC_Read(1);
		temperature_celsius = ADC_to_Temperature(temp_reading);
        Control_Temperature(temp_reading);
	    
		Set_Lamp_Brightness(ldr_reading);
        Display_Temperature_LCD(temperature_celsius);
        Display_LDR_LCD(ldr_reading);

	    delay_ms(2000);
	    
	}
}