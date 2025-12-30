#ifndef TIMER0_H
#define TIMER0_H

#include <p18f6722.h>

/* ================= GLOBAL VARIABLES ================= */
extern volatile unsigned char tick;
extern volatile unsigned char reset_flag;

/* ================= FUNCTION PROTOTYPES ================= */
void Timer0_Init(void);
void System_Reset(void);

/* ================= INTERRUPT PROTOTYPES ================= */
void high_isr(void);
void high_vector(void);

#endif /* TIMER0_H */
