#include "security.h"
#include "lcd.h"
#include "keypad.h"
#include "temp_control.h"
#include <delays.h>

// 1. Function Prototypes (Fixes "call without prototype" and "linkage" errors)
void delay_ms(unsigned int ms);
void delay_us(unsigned int us);
static void led_On(void);
static void led_Off(void);
static void buzzer_on(void);
static void buzzer_off(void);
static void Servo_Pulse(unsigned int ticks);
static void Servo_SetAngle(unsigned char angle);
static void Servo_Open(void);
static void Servo_Close(void);

// ================= Password =================
static const char correctPassword[PASSWORD_LENGTH + 1] = "1234";

// ================= Delay Functions =================
void delay_us(unsigned int us) {
    while (us--) {
        Delay10TCYx(1); Delay10TCYx(1); 
        Delay10TCYx(1); Delay10TCYx(1); 
        Delay10TCYx(1); 
    }
}

// ================= Alarm Functions =================

static void buzzer_on(void)
{
    BUZZER = 1;   // RC0 HIGH
}

static void buzzer_off(void)
{
    BUZZER = 0;   // RC0 LOW
}


static void led_on(void){LED = 1;}
static void led_off(void){LED = 0;}



// ================= Servo Control (10MHz) =================
#define SERVO_MIN_TICKS  250    
#define SERVO_MAX_TICKS  500    
#define SERVO_RANGE      90     

static void Servo_Pulse(unsigned int ticks) {
    unsigned int off_ticks;
    off_ticks = 5000 - ticks;

    SERVO = 1;
    if(ticks > 255) {
        Delay10TCYx(255);
        Delay10TCYx((unsigned char)(ticks - 255));
    } else {
        Delay10TCYx((unsigned char)ticks);
    }
    SERVO = 0;
    
    while(off_ticks > 255) {
        Delay10TCYx(255);
        off_ticks -= 255;
    }
    Delay10TCYx((unsigned char)off_ticks);
}

static void Servo_SetAngle(unsigned char angle) {
    unsigned char i;
    unsigned int ticks;
    ticks = SERVO_MIN_TICKS + ((unsigned long)angle * (SERVO_MAX_TICKS - SERVO_MIN_TICKS)) / SERVO_RANGE;

    for (i = 0; i < 50; i++) {
        Servo_Pulse(ticks);
    }
}

static void Servo_Open(void) {
    led_on();       // Now compiler knows this function exists
    Servo_SetAngle(90); 
}

static void Servo_Close(void) {
    Servo_SetAngle(0); 
    led_off();      
}

// ================= Init =================
void Security_Init(void)
{
    BUZZER_TRIS = 0;
    LED_TRIS    = 0;
    SERVO_TRIS  = 0;

    SERVO = 0;
    buzzer_off();
    led_off();
}

// ================= Main Logic =================
void Security_Run(void)
{
    char enteredPassword[PASSWORD_LENGTH + 1];
    unsigned char key;
    unsigned char index;
    unsigned char attempts = 0;
    unsigned char i;
    unsigned char accessGranted;
	unsigned int current_temp;
    while (1)
    {
        LCD_Command(0x01);
        LCD_String("Enter Password");
        LCD_Command(0xC0);

        index = 0;
        accessGranted = 1;

        while (index < PASSWORD_LENGTH)
        {
            key = Keypad_Read();

            if (key >= '0' && key <= '9')
            {
                enteredPassword[index++] = key;
                LCD_Char('*');
            }
        }
        enteredPassword[index] = '\0';

        // Check password
        for (i = 0; i < PASSWORD_LENGTH; i++)
        {
            if (enteredPassword[i] != correctPassword[i])
            {
                accessGranted = 0;
            }
        }

        // ================= Access Granted =================
        if (accessGranted)
        {
            attempts = 0;
            //Alarm_Off();
            

            LCD_Command(0x01);
            LCD_String("Access Granted");
            LCD_Command(0xC0);
            LCD_String("Welcome");

            Servo_Open();       // Open door
            //Alarm_On();
			delay_ms(1000);     // Door open time
			//Servo_SetAngle(0);
            Servo_Close();      // Close door
			//Alarm_Off();
			 return 1;           // Return success to main program
			

	
        }

        // ================= Access Denied =================
        else
        {
            attempts++;

            LCD_Command(0x01);
            LCD_String("Access Denied");
            delay_ms(1500);

            if (attempts >= MAX_ATTEMPTS)
            {
                LCD_Command(0x01);
                LCD_String("SYSTEM LOCKED");
                LCD_Command(0xC0);
                LCD_String("ALARM ON");
                buzzer_on();

                for (i = 5; i > 0; i--)
                {
                    LCD_Command(0xC0);
                    LCD_String("Wait ");
                    LCD_Char(i + '0');
                    LCD_String(" sec ");
                    delay_ms(100);
                }

                buzzer_off();
                attempts = 0;
            }
        }
    }
}
