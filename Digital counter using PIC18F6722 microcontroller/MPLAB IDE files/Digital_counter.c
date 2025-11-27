#include <p18f6722.h>
#include <delays.h>

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

const rom unsigned char seg_patterns[10] = {
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F
};

#define BUTTON_INC PORTCbits.RC4   // ???? ???????
#define BUTTON_DEC PORTCbits.RC5   // ???? ???????

void display(unsigned int num)
{
    unsigned char d0 = num % 10;
    unsigned char d1 = (num / 10) % 10;
    unsigned char d2 = (num / 100) % 10;
    unsigned char d3 = (num / 1000) % 10;

    char i;
    for(i=0; i<4; i++)
    {
        LATB = seg_patterns[d0];
        LATC = 0b11111110;
        Delay1KTCYx(3);
        LATC = 0xFF;

        LATB = seg_patterns[d1];
        LATC = 0b11111101;
        Delay1KTCYx(3);
        LATC = 0xFF;

        LATB = seg_patterns[d2];
        LATC = 0b11111011;
        Delay1KTCYx(3);
        LATC = 0xFF;

        LATB = seg_patterns[d3];
        LATC = 0b11110111;
        Delay1KTCYx(3);
        LATC = 0xFF;
    }
}

void main(void)
{
    unsigned int counter = 0;

    TRISB = 0x00;
    TRISC = 0b11110000;   // RC0–RC3 output, RC4 + RC5 inputs
    LATB = 0x00;
    LATC = 0xFF;

    while(1)
    {
        display(counter);

        // ======================
        //  ???? ??????? +1
        // ======================
        if(BUTTON_INC == 0)
        {
            Delay1KTCYx(20);
            if(BUTTON_INC == 0)
            {
                counter++;
                if(counter > 9999) counter = 0;
                while(BUTTON_INC == 0);
            }
        }

        // ======================
        //  ???? ??????? -1
        // ======================
        if(BUTTON_DEC == 0)
        {
            Delay1KTCYx(20);
            if(BUTTON_DEC == 0)
            {
                if(counter == 0) counter = 9999;
                else counter--;

                while(BUTTON_DEC == 0);
            }
        }
    }
}
