#ifndef LDR_H
#define LDR_H

#include <p18f6722.h>

void LDR_Init(void);
unsigned int LDR_Read(void);
void Set_Lamp_Brightness(unsigned int ldr_value);
void Display_LDR_LCD(unsigned int ldr_value);
#endif
