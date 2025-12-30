#ifndef TEMP_CONTROL_H
#define TEMP_CONTROL_H

#include <p18f6722.h>
#define TEMP_VERY_COLD   41     // 20°C: Full heater ON
#define TEMP_COLD        51     // 25°C: Start reducing heater
#define TEMP_COMFORT_LOW 61     // 30°C: Heater OFF, comfortable zone
#define TEMP_COMFORT_HIGH 72    // 35°C: Still comfortable
#define TEMP_WARM        82     // 40°C: Start fan
#define TEMP_HOT         102    // 50°C: Full fan ON
void Control_Temperature(unsigned int temp_value);
void Set_Heater_PWM(unsigned int duty_10bit);
void Set_Fan_PWM(unsigned int duty_10bit);
unsigned int ADC_to_Temperature(unsigned int adc_value);
void Display_Temperature_LCD(unsigned int temperature_celsius);
#endif