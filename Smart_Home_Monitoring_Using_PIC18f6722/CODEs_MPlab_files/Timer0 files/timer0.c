#include "timer0.h"
#include "lcd.h"
#include "Timer2.h"
#include "security.h"
#include "temp_control.h"
#include "ldr_control.h"
/* ================= GLOBAL VARIABLES ================= */
volatile unsigned char tick = 0;
volatile unsigned char reset_flag = 0;
static unsigned char i;

/* ================= INTERRUPT ================= */
#pragma interrupt high_isr
void high_isr(void)
{
    if (INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0;

        /* Reload Timer0 (~10 ms) */
        TMR0H = 0xFA;
        TMR0L = 0x47;

        /* ---- RESET BUTTON ON RC3 (ACTIVE LOW) ---- */
        if (PORTCbits.RC3 == 0)
        {
            reset_flag = 1;
        }

        /* ---- LED BLINK ---- */
        tick++;
        if (tick >= 20)   // ~200 ms
        {
            LATGbits.LATG1 ^= 1;  // Toggle LED
            tick = 0;
        }
    }
}

/* ================= INTERRUPT VECTOR ================= */
#pragma code high_vector = 0x08
void high_vector(void)
{
    _asm GOTO high_isr _endasm
}
#pragma code

/* ================= TIMER0 INIT ================= */
void Timer0_Init(void)
{
    TRISGbits.TRISG1 = 0;       // RG1 as output (LED)
    T0CON = 0b00000111;         // 16-bit, internal clock, prescaler 1:256
    TMR0H = 0xFA;
    TMR0L = 0x47;

    INTCONbits.TMR0IE = 1;      // Enable Timer0 interrupt
    INTCONbits.GIE = 1;         // Enable global interrupt
    T0CONbits.TMR0ON = 1;       // Turn on Timer0
    reset_flag = 0;
}

/* ================= SYSTEM RESET ================= */
void System_Reset(void)
{
    // Stop Timer0 LED toggle temporarily
    INTCONbits.TMR0IE = 0;

    // Turn off devices
    Set_Lamp_Brightness(989);
    Set_Heater_PWM(0);
    Set_Fan_PWM(0);


    // LCD message
    LCD_Command(0x01);
    LCD_String("System Reset");
	delay_ms(1000);
    LCD_Init();
	Security_Init();
    Security_Run(); 
    Three_PWM_Init();
	ADC_Init();
    // Turn LED ON for 2 sec
    LATGbits.LATG1 = 1;
    delay_ms(1000);
    LATGbits.LATG1 = 0;

    // Restart Timer0 LED toggle
    INTCONbits.TMR0IE = 1;
}
