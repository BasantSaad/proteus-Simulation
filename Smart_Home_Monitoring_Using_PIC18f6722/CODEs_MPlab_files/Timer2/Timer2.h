#ifndef DEVICE_CONTROL_H
#define DEVICE_CONTROL_H

#include <p18f6722.h>

// Declare global variables (define them in a .c file)
extern volatile unsigned char softPWM_duty;
extern volatile unsigned char softPWM_counter;

// Function prototypes
void Three_PWM_Init(void);
void ADC_Init(void);
unsigned int ADC_Read(unsigned char channel);



#endif
