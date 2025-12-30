#include "temp_control.h"
#include <delays.h>
#include "lcd.h"

void Temp_Init(void) {
    // ADC Setup
    TRISAbits.TRISA0 = 1;   // AN0 as input
    ADCON1 = 0x0E;          // AN0 is analog, others digital (Vref+ = VDD, Vref- = VSS)
    ADCON2 = 0xBE;          // Right justified, 20 TAD, Fosc/64
    ADCON0 = 0x01;          // Turn ADC on

    // PWM Setup (CCP1 on RC2)
    TRISCbits.TRISC2 = 0;   // PWM pin as output
    PR2 = 0x9B;             // ~1.6 kHz PWM frequency at 10MHz
    CCP1CON = 0x0C;         // PWM mode
    T2CON = 0x05;           // Timer2 ON, Prescaler 4
}

unsigned int ADC_to_Temperature(unsigned int adc_value) {
    // Convert ADC value to temperature in °C
    // Temperature = (ADC × 500) / 1023
    unsigned long temp = ((unsigned long)adc_value * 500) / 1023;
    return (unsigned int)temp;
}
void Set_Heater_PWM(unsigned int duty_10bit) {
    // duty_10bit: 0 to 499 (for PR2=124)
    if(duty_10bit > 499) duty_10bit = 499;
    
    CCPR4L = duty_10bit >> 2;
    CCP4CONbits.DC4B0 = (duty_10bit & 0x01);
    CCP4CONbits.DC4B1 = ((duty_10bit >> 1) & 0x01);
}

void Set_Fan_PWM(unsigned int duty_10bit) {
    // duty_10bit: 0 to 499 (for PR2=124)
    if(duty_10bit > 499) duty_10bit = 499;
    
    CCPR5L = duty_10bit >> 2;
    CCP5CONbits.DC5B0 = (duty_10bit & 0x01);
    CCP5CONbits.DC5B1 = ((duty_10bit >> 1) & 0x01);
}

void Control_Temperature(unsigned int temp_adc) {
    /*
     * Temperature Control Logic for LM35:
     * 
     * ADC  | Temp | Heater | Fan  | Status
     * -----|------|--------|------|--------
     * <41  | <20°C| 100%   | 0%   | VERY COLD
     * 41-51| 20-25| 100?0% | 0%   | COLD
     * 51-61| 25-30| 0%     | 0%   | COMFORTABLE
     * 61-72| 30-35| 0%     | 0%   | COMFORTABLE
     * 72-82| 35-40| 0%     | 0?50%| WARM
     * 82+  | 40+  | 0%     | 100% | HOT
     */
    
    if(temp_adc < TEMP_VERY_COLD) {
        // VERY COLD (<20°C): Full heater, no fan
        Set_Heater_PWM(499);    // 100% heater
        Set_Fan_PWM(0);         // 0% fan
    }
    else if(temp_adc < TEMP_COLD) {
        // COLD (20-25°C): Gradually reduce heater
        unsigned int heater_duty = 499 - ((unsigned long)(temp_adc - TEMP_VERY_COLD) * 250 
                                   / (TEMP_COLD - TEMP_VERY_COLD));
        Set_Heater_PWM(heater_duty);
        Set_Fan_PWM(0);
    }
    else if(temp_adc < TEMP_COMFORT_LOW) {
        // APPROACHING COMFORT (25-30°C): Low heater
        unsigned int heater_duty = ((unsigned long)(TEMP_COMFORT_LOW - temp_adc) * 249 
                                   / (TEMP_COMFORT_LOW - TEMP_COLD));
        Set_Heater_PWM(heater_duty);
        Set_Fan_PWM(0);
    }
    else if(temp_adc <= TEMP_COMFORT_HIGH) {
        // COMFORTABLE ZONE (30-35°C): Both OFF
        Set_Heater_PWM(0);
        Set_Fan_PWM(0);
    }
    else if(temp_adc < TEMP_WARM) {
        // GETTING WARM (35-40°C): Start fan gradually
        unsigned int fan_duty = ((unsigned long)(temp_adc - TEMP_COMFORT_HIGH) * 249 
                                / (TEMP_WARM - TEMP_COMFORT_HIGH));
        Set_Heater_PWM(0);
        Set_Fan_PWM(fan_duty);
    }
    else if(temp_adc < TEMP_HOT) {
        // WARM (40-50°C): Increase fan more
        unsigned int fan_duty = 249 + ((unsigned long)(temp_adc - TEMP_WARM) * 250 
                                / (TEMP_HOT - TEMP_WARM));
        Set_Heater_PWM(0);
        Set_Fan_PWM(fan_duty);
    }
    else {
        // HOT (>50°C): Full fan, no heater
        Set_Heater_PWM(0);
        Set_Fan_PWM(499);       // 100% fan
    }
}
void Display_Temperature_LCD(unsigned int temperature_celsius)
{
    LCD_Command(0x01);
    LCD_Command(0x80);
    LCD_String("Temperature: ");
    LCD_Char((temperature_celsius / 10) + '0');
    LCD_Char((temperature_celsius % 10) + '0');
    LCD_String(" C");
}