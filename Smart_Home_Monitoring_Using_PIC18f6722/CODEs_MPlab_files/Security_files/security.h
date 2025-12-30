#ifndef SECURITY_H
#define SECURITY_H

#include <p18f6722.h>


// ===== Hardware Mapping =====
#define BUZZER LATCbits.LATC0
#define LED    LATCbits.LATC4
#define SERVO  LATCbits.LATC2

#define BUZZER_TRIS TRISCbits.TRISC0
#define LED_TRIS    TRISCbits.TRISC4
#define SERVO_TRIS  TRISCbits.TRISC2

// ===== System Parameters =====
#define PASSWORD_LENGTH 4
#define MAX_ATTEMPTS    3

// ===== Functions =====
void Security_Init(void);
void Security_Run(void);
//unsigned char Security_CheckAccess(void);  // New - returns status

#endif
