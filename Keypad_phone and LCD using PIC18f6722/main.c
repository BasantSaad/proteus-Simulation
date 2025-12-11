/*******************************************************************************
 * File:        Keypad_LCD_PIC18F6722.c
 * Description: 4x4 Keypad with 16x2 LCD Interface using PIC18F6722
 * Author:      Embedded System
 * Date:        2024
 * Compiler:    MPLAB v8.56 - C18 Compiler
 * MCU:         PIC18F6722
 * LCD:         LM016L (16x2) in 8-bit mode
 * Connections: LCD Data: PORTD, LCD RS: RA0, LCD E: RA1
 *              Keypad Rows: RB0-RB3, Keypad Columns: RB4-RB7
 *******************************************************************************/

#include <p18f6722.h>
#include <delays.h>

#pragma config OSC = HS, FCMEN = OFF, IESO = OFF
#pragma config PWRT = ON, BOREN = OFF, BORV = 3
#pragma config WDT = OFF, WDTPS = 32768
#pragma config MODE = MC, ADDRBW = ADDR20BIT, DATABW = DATA16BIT, WAIT = OFF
#pragma config CCP2MX = PORTC, ECCPMX = PORTE, LPT1OSC = OFF
#pragma config STVREN = ON, LVP = OFF, BBSIZ = BB2K, XINST = OFF
#pragma config DEBUG = OFF

// LCD Control Definitions
#define LCD_RS          LATAbits.LATA0    // Register Select
#define LCD_E           LATAbits.LATA1    // Enable
#define LCD_DATA        LATD              // Data Port
#define LCD_DATA_DIR    TRISD             // Data Direction
#define LCD_CTRL_DIR    TRISA             // Control Direction

// Keypad Definitions
#define KEYPAD_PORT     PORTB
#define KEYPAD_TRIS     TRISB
#define ROW1            PORTBbits.RB0
#define ROW2            PORTBbits.RB1
#define ROW3            PORTBbits.RB2
#define ROW4            PORTBbits.RB3
#define COL1            PORTBbits.RB4
#define COL2            PORTBbits.RB5
#define COL3            PORTBbits.RB6
#define COL4            PORTBbits.RB7

// LCD Commands
#define LCD_CLEAR       0x01
#define LCD_HOME        0x02
#define LCD_ENTRY_MODE  0x06
#define LCD_DISPLAY_ON  0x0C
#define LCD_DISPLAY_OFF 0x08
#define LCD_FUNCTION_SET 0x38  // 8-bit, 2 lines, 5x8 font
#define LCD_SET_CGRAM   0x40
#define LCD_SET_DDRAM   0x80

// Function Prototypes
void Initialize_System(void);
void LCD_Init(void);
void LCD_Cmd(unsigned char command);
void LCD_Write(unsigned char data);
void LCD_Write_String(const char *str);
void LCD_Set_Cursor(unsigned char row, unsigned char column);
void Delay_ms(unsigned int ms);
void Delay_us(unsigned int us);
unsigned char Keypad_Scan(void);
unsigned char Keypad_GetKey(void);

// Welcome Message
const char welcome_msg1[] = "4x4 Keypad + LCD ";
const char welcome_msg2[] = "Press any key... ";

// Keypad Mapping (4x4 matrix)
const char keypad_map[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Variables
unsigned char key_pressed = 0;
unsigned char display_position = 0;
unsigned char lcd_line = 0;

/*******************************************************************************
 * Main Function
 *******************************************************************************/
void main(void) {
    Initialize_System();
    LCD_Init();
    
    // Display welcome message
    LCD_Write_String(welcome_msg1);
    LCD_Set_Cursor(2, 1);
    LCD_Write_String(welcome_msg2);
    Delay_ms(2000);
    LCD_Cmd(LCD_CLEAR);
    
    // Main loop
    while(1) {
        key_pressed = Keypad_GetKey();
        
        if(key_pressed != 0) {
            // Check if we need to clear or move to next line
            if(display_position >= 16) {
                if(lcd_line == 0) {
                    LCD_Set_Cursor(2, 1);
                    lcd_line = 1;
                    display_position = 0;
                } else {
                    LCD_Cmd(LCD_CLEAR);
                    LCD_Set_Cursor(1, 1);
                    lcd_line = 0;
                    display_position = 0;
                }
            }
            
            // Display the pressed key
            LCD_Write(key_pressed);
            display_position++;
            
            // Short delay after key press
            Delay_ms(200);
        }
    }
}

/*******************************************************************************
 * System Initialization
 *******************************************************************************/
void Initialize_System(void) {
    // Configure PORTA for LCD control (RA0, RA1 as outputs)
    TRISAbits.TRISA0 = 0;  // RS as output
    TRISAbits.TRISA1 = 0;  // E as output
    ADCON1 = 0x0F;         // Disable analog inputs on PORTA
    
    // Configure PORTD for LCD data (all as outputs)
    TRISD = 0x00;          // All PORTD as output for LCD data
    
    // Configure PORTB for keypad
    // Rows (RB0-RB3) as output, Columns (RB4-RB7) as input with pull-ups
    TRISB = 0xF0;          // RB0-3: Output, RB4-7: Input
    INTCON2bits.RBPU = 0;  // Enable PORTB pull-ups
}

/*******************************************************************************
 * LCD Functions
 *******************************************************************************/
void LCD_Init(void) {
    Delay_ms(50);           // Wait for LCD power up
    
    // Function Set (8-bit interface, 2 lines, 5x8 font)
    LCD_Cmd(0x30);
    Delay_ms(5);
    LCD_Cmd(0x30);
    Delay_us(100);
    LCD_Cmd(0x30);
    Delay_us(100);
    
    LCD_Cmd(LCD_FUNCTION_SET);
    Delay_us(100);
    
    // Display off
    LCD_Cmd(LCD_DISPLAY_OFF);
    Delay_us(100);
    
    // Clear display
    LCD_Cmd(LCD_CLEAR);
    Delay_ms(2);
    
    // Entry mode set
    LCD_Cmd(LCD_ENTRY_MODE);
    Delay_us(100);
    
    // Display on, cursor off, blink off
    LCD_Cmd(LCD_DISPLAY_ON);
    Delay_us(100);
}

void LCD_Cmd(unsigned char command) {
    LCD_RS = 0;            // Command mode
    LCD_DATA = command;    // Send command
    LCD_E = 1;             // Enable pulse
    Delay_us(1);
    LCD_E = 0;
    Delay_us(100);         // Wait for command to execute
    
    if(command == LCD_CLEAR || command == LCD_HOME)
        Delay_ms(2);       // Longer delay for clear/home
}

void LCD_Write(unsigned char data) {
    LCD_RS = 1;            // Data mode
    LCD_DATA = data;       // Send data
    LCD_E = 1;             // Enable pulse
    Delay_us(1);
    LCD_E = 0;
    Delay_us(100);         // Wait for data write
}

void LCD_Write_String(const char *str) {
    while(*str != '\0') {
        LCD_Write(*str);
        str++;
    }
}

void LCD_Set_Cursor(unsigned char row, unsigned char column) {
    unsigned char address;
    
    if(row == 1)
        address = 0x80 + (column - 1);  // First line
    else
        address = 0xC0 + (column - 1);  // Second line
        
    LCD_Cmd(address);
}

/*******************************************************************************
 * Delay Functions
 *******************************************************************************/
void Delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1000; j++);
}

void Delay_us(unsigned int us) {
    unsigned int i;
    for(i = 0; i < us; i++);
}

/*******************************************************************************
 * Keypad Functions with Debouncing
 *******************************************************************************/
unsigned char Keypad_Scan(void) {
    unsigned char row, col;
    
    // Scan each row
    for(row = 0; row < 4; row++) {
        // Set all rows high initially
        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 1;
        LATBbits.LATB2 = 1;
        LATBbits.LATB3 = 1;
        
        // Set current row low
        switch(row) {
            case 0: LATBbits.LATB0 = 0; break;
            case 1: LATBbits.LATB1 = 0; break;
            case 2: LATBbits.LATB2 = 0; break;
            case 3: LATBbits.LATB3 = 0; break;
        }
        
        Delay_us(10);  // Small delay for stabilization
        
        // Check columns
        if(COL1 == 0) return keypad_map[row][0];
        if(COL2 == 0) return keypad_map[row][1];
        if(COL3 == 0) return keypad_map[row][2];
        if(COL4 == 0) return keypad_map[row][3];
    }
    
    return 0;  // No key pressed
}

unsigned char Keypad_GetKey(void) {
    static unsigned char last_key = 0;
    unsigned char current_key;
    
    current_key = Keypad_Scan();
    
    if(current_key != 0) {
        Delay_ms(20);  // Debounce delay
        
        // Check again after debounce
        if(current_key == Keypad_Scan()) {
            if(current_key != last_key) {
                last_key = current_key;
                return current_key;
            }
        }
    }
    else {
        last_key = 0;
    }
    
    return 0;
}