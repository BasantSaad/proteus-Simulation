#include "Timer2.h"
volatile unsigned char softPWM_duty = 0;
volatile unsigned char softPWM_counter = 0;

void Three_PWM_Init(void) {
    // Pin configurations
    TRISGbits.TRISG0 = 0;      // CCP3/RG0 - Lamp OUTPUT
    TRISGbits.TRISG3 = 0;      // CCP4/RG3 - Heater OUTPUT
    TRISGbits.TRISG4 = 0;      // CCP5/RG4 - Fan OUTPUT
    
    // Set PWM period (same for all using Timer2)
    PR2 = 124;                  // ~5kHz @ 10MHz, prescaler=4
    
    // CCP3 - Lamp (controlled by LDR on AN1)
    CCP3CON = 0b00001100;       // PWM mode
    CCPR3L = 0;                 // 0% initially
    CCP3CONbits.DC3B0 = 0;
    CCP3CONbits.DC3B1 = 0;
    
    // CCP4 - Heater (controlled by temp on AN0)
    CCP4CON = 0b00001100;       // PWM mode
    CCPR4L = 0;                 // 0% initially
    CCP4CONbits.DC4B0 = 0;
    CCP4CONbits.DC4B1 = 0;
    
    // CCP5 - Fan (controlled by temp on AN0)
    CCP5CON = 0b00001100;       // PWM mode
    CCPR5L = 0;                 // 0% initially
    CCP5CONbits.DC5B0 = 0;
    CCP5CONbits.DC5B1 = 0;
    
    // Start Timer2 (shared by all PWM)
    T2CON = 0b00000101;         // Prescaler=4, Timer2 ON
}

void ADC_Init(void) {
    // Set RA0 and RA1 as inputs
    TRISAbits.TRISA0 = 1;       // AN0 - LM35 Temperature sensor
    TRISAbits.TRISA1 = 1;       // AN1 - LDR
    
    // Configure ADCON1 - Both AN0 and AN1 as analog
    ADCON1bits.PCFG0 = 1;       // PCFG = 1101
    ADCON1bits.PCFG1 = 0;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG3 = 1;
    ADCON1bits.VCFG0 = 0;       // Vref+ = VDD
    ADCON1bits.VCFG1 = 0;       // Vref- = VSS
    
    // Configure ADCON2
    ADCON2bits.ADFM = 1;        // Right justified result
    ADCON2bits.ACQT0 = 1;       // Acquisition time = 12 TAD (101)
    ADCON2bits.ACQT1 = 0;
    ADCON2bits.ACQT2 = 1;
    ADCON2bits.ADCS0 = 0;       // FOSC/32 (010)
    ADCON2bits.ADCS1 = 1;
    ADCON2bits.ADCS2 = 0;
    
    // Configure ADCON0
    ADCON0bits.CHS0 = 0;        // Channel AN0 (0000)
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS3 = 0;
    ADCON0bits.ADON = 1;        // Turn on ADC
    
    //Delay_Ms(1);                // Allow ADC to stabilize
}

unsigned int ADC_Read(unsigned char channel) {
    // Select channel
    ADCON0bits.CHS0 = (channel & 0x01);
    ADCON0bits.CHS1 = ((channel >> 1) & 0x01);
    ADCON0bits.CHS2 = ((channel >> 2) & 0x01);
    ADCON0bits.CHS3 = ((channel >> 3) & 0x01);
    
    //Delay10TCYx(5);             // Acquisition time (~20us at 10MHz)
    delay_ms(5);
    ADCON0bits.GO = 1;          // Start conversion
    while(ADCON0bits.GO);       // Wait for completion
    
    return ((unsigned int)ADRESH << 8) | ADRESL;
}


