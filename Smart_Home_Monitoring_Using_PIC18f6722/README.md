# Smart Home Security and Environmental Control System
##### (OKAY_MAN.zip) 
## Project Overview

 Smart Home Security and Environmental Control System is an embedded system project designed for the **PIC18F6722 microcontroller** that integrates multiple subsystems to create a comprehensive smart home security and environmental control solution.

### Key Features

1. **Security Access Control System**
   - 4-digit password authentication via 4x4 keypad
   - Servo motor-controlled door lock mechanism
   - LED status indicator
   - Buzzer alarm for failed attempts
   - Lockout protection (3 failed attempts = 5-second lockout)

2. **Temperature Control System**
   - LM35 temperature sensor (AN0) for real-time monitoring
   - Automatic heater control (PWM on CCP4)
   - Automatic fan control (PWM on CCP5)
   - LCD display of current temperature
   - Intelligent temperature zones (20-50°C range)

3. **Automatic Lighting System**
   - LDR (Light Dependent Resistor) sensor (AN1)
   - Automatic lamp brightness adjustment (PWM on CCP3)
   - Inverse control: darker room = brighter lamp
   - LCD display of light level percentage

4. **System Reset Functionality**
   - Hardware reset button (RC3)
   - Timer0-based interrupt handling
   - System status LED (RG1) with blink indicator
   - Complete system reinitialization

---

## Hardware Requirements

### Microcontroller
- **PIC18F6722** running at 10MHz (HS oscillator)

### Components

| Component | Connection | Function |
|-----------|-----------|----------|
| 16x2 LCD | PORTD (data), RB4-RB6 (control) | Display interface |
| 4x4 Keypad | PORTB (RB0-RB3: columns, RB4-RB7: rows) | Password input |
| LM35 Sensor | AN0 (RA0) | Temperature sensing |
| LDR | AN1 (RA1) | Light level sensing |
| Servo Motor | RC2 | Door lock control |
| Buzzer | RC0 | Alarm output |
| Status LED | RC4 | Access indicator |
| System LED | RG1 | System status (blink) |
| Heater | RG3 (CCP4/PWM) | Temperature control |
| Fan | RG4 (CCP5/PWM) | Temperature control |
| Lamp | RG0 (CCP3/PWM) | Automatic lighting |
| Reset Button | RC3 (active low) | System reset |

### Power Supply
- 5V regulated power supply
- Appropriate current ratings for motors and sensors

---

## Software Architecture

### Module Structure

```
OKAY_MAN/
├── Untitled.c          # Main program entry point
├── security.c/h        # Password authentication & door control
├── temp_control.c/h    # Temperature monitoring & control
├── ldr_control.c/h     # Light sensing & lamp control
├── Timer2.c/h          # PWM initialization & ADC functions
├── timer0.c/h          # System reset & status LED
├── LCD.c/h             # LCD driver functions
├── KEYPAD.c/h          # 4x4 Keypad driver
├── Makefile            # Build configuration
└── okay.hex            # Compiled firmware
```

### Key Design Patterns

1. **Modular Architecture**: Each subsystem is isolated in separate modules
2. **Interrupt-Driven**: Timer0 ISR handles reset button and LED blinking
3. **State Machine**: Security module implements authentication flow
4. **PWM Control**: Three independent PWM channels for smooth control

---

## System Workflow

### 1. System Initialization Flow

```
Power On
    ↓
Configure ADC (AN0, AN1 as analog)
    ↓
Initialize LCD Display
    ↓
Initialize Security Module (Buzzer, LED, Servo)
    ↓
Initialize Timer0 (Reset button monitoring)
    ↓
Run Security Authentication
    ↓
[Wait for correct password]
```

### 2. Security Authentication Flow

```
Display "Enter Password"
    ↓
Read 4 digits from keypad
    ↓
Validate password (default: "1234")
    ↓
┌─────────────┴─────────────┐
│                           │
CORRECT                  WRONG
    ↓                        ↓
Display "Access Granted"  Display "Access Denied"
    ↓                        ↓
Turn ON Status LED        Increment attempt counter
    ↓                        ↓
Open Door (Servo 90°)     Check attempts ≥ 3?
    ↓                        │
Wait 1 second             YES    NO
    ↓                        │     │
Close Door (Servo 0°)      │     └─→ Retry
    ↓                        ↓
Turn OFF Status LED    SYSTEM LOCKED
    ↓                   Turn ON Buzzer
Proceed to Main Loop    Display countdown
                        Turn OFF Buzzer
                        Reset attempts
                        Retry
```

### 3. Main Operating Loop

```
┌─────────────────────────────────────┐
│         Main Control Loop           │
└─────────────────────────────────────┘
    ↓
Check Reset Button (via Timer0 ISR)
    │
    │ [If pressed]
    ├──────────→ System_Reset() → Re-authenticate
    │
    ↓ [Continue]
Read Temperature Sensor (AN0)
    ↓
Control_Temperature()
    ├─→ Set Heater PWM (CCP4)
    └─→ Set Fan PWM (CCP5)
    ↓
Read LDR Sensor (AN1)
    ↓
Set_Lamp_Brightness() (CCP3)
    ↓
Display Temperature on LCD
    ↓
Display Light Level on LCD
    ↓
Delay 2 seconds
    ↓
Loop back to start
```

### 4. Temperature Control Logic

| ADC Range | Temperature | Heater | Fan | Status |
|-----------|------------|--------|-----|--------|
| < 41 | < 20°C | 100% | 0% | VERY COLD |
| 41-51 | 20-25°C | 100→50% | 0% | COLD |
| 51-61 | 25-30°C | 50→0% | 0% | APPROACHING COMFORT |
| 61-72 | 30-35°C | 0% | 0% | COMFORTABLE |
| 72-82 | 35-40°C | 0% | 0→50% | GETTING WARM |
| 82-102 | 40-50°C | 0% | 50→100% | WARM |
| > 102 | > 50°C | 0% | 100% | HOT |

### 5. Light Control Logic

```
Read LDR Value (11-989)
    ↓
Calculate Brightness = (989 - LDR) × 499 / (989 - 11)
    ↓
Darker Room (LDR low) → Brighter Lamp (PWM high)
Brighter Room (LDR high) → Dimmer Lamp (PWM low)
    ↓
Set CCP3 PWM Duty Cycle
```

---

## Installation & Setup

### Prerequisites

1. **MPLAB IDE** (v8.x or compatible)
2. **MCC18 C Compiler** for PIC18 series
3. **MPLINK Linker**
4. **PICkit 3/4** or compatible programmer
5. **Proteus** (optional, for simulation)

### Compilation Steps

#### Using MPLAB IDE

1. Open `okay.mcp` project file in MPLAB IDE
2. Select **PIC18F6722** as target device
3. Configure build options:
   - Optimization: Disabled (`-Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-`)
   - Debug mode: Enabled (`-D__DEBUG`)
4. Build project: **Project → Build All**
5. Verify `okay.hex` is generated

#### Using Command Line (Make)

```bash
# Windows (MPLAB command line)
make -f Makefile

# Clean build
make clean
make -f Makefile
```

### Programming the Device

1. Connect PICkit programmer to ICSP pins
2. Load `okay.hex` into programmer software
3. Program the device
4. Verify programming success

### Hardware Setup

1. **Power Connections**
   - Connect 5V and GND to all components
   - Ensure common ground

2. **LCD Connections**
   - Data: RD0-RD7
   - RS: RB4
   - RW: RB5  
   - EN: RB6

3. **Sensor Connections**
   - LM35 output → AN0 (RA0)
   - LDR voltage divider → AN1 (RA1)

4. **Actuator Connections**
   - Servo signal → RC2
   - Buzzer → RC0
   - LED indicators → RC4, RG1
   - Heater driver → RG3
   - Fan driver → RG4
   - Lamp driver → RG0

5. **Input Connections**
   - 4x4 Keypad → PORTB (RB0-RB7)
   - Reset button → RC3 (with pull-up)

---

## Configuration

### Changing Default Password

Edit `security.c`, line 20:

```c
static const char correctPassword[PASSWORD_LENGTH + 1] = "1234";
```

Replace `"1234"` with your desired 4-digit code.

### Adjusting Temperature Thresholds

Edit `temp_control.h`:

```c
#define TEMP_VERY_COLD    41    // <20°C
#define TEMP_COLD         51    // 20-25°C
#define TEMP_COMFORT_LOW  61    // 25-30°C
#define TEMP_COMFORT_HIGH 72    // 30-35°C
#define TEMP_WARM         82    // 35-40°C
#define TEMP_HOT          102   // 40-50°C
```

### PWM Frequency

Configured in `Timer2.c`:

```c
PR2 = 124;  // ~5kHz @ 10MHz, prescaler=4
```

Formula: `PWM_freq = Fosc / (4 × Prescaler × (PR2 + 1))`

---

## Testing & Debugging

### Initial Testing Sequence

1. **Power-On Test**
   - LCD should display "Enter Password"
   - System LED (RG1) should blink every 200ms

2. **Security Test**
   - Enter correct password ("1234")
   - Verify: LED ON → Servo opens → Delay → Servo closes → LED OFF
   - Enter wrong password 3 times
   - Verify: Buzzer sounds for countdown

3. **Temperature Control Test**
   - Heat LM35 sensor gently
   - Verify: Fan PWM increases, heater PWM decreases
   - Cool LM35 sensor
   - Verify: Heater PWM increases, fan PWM decreases

4. **Light Control Test**
   - Cover LDR sensor (simulate darkness)
   - Verify: Lamp brightness increases
   - Expose LDR to light
   - Verify: Lamp brightness decreases

5. **Reset Test**
   - Press reset button (RC3)
   - Verify: System returns to password entry

### Common Issues

| Problem | Possible Cause | Solution |
|---------|---------------|----------|
| LCD blank | Wrong contrast | Adjust 10K pot |
| No keypad response | Wrong port mapping | Check PORTB connections |
| Servo jittery | Power supply insufficient | Use external 5V for servo |
| ADC readings unstable | No decoupling caps | Add 100nF near sensors |
| PWM not working | Timer2 not started | Verify T2CON = 0x05 |

---

## API Reference

### Security Module

```c
void Security_Init(void);
// Initialize security hardware (buzzer, LED, servo)

void Security_Run(void);
// Run password authentication loop
// Blocks until correct password entered
```

### Temperature Control

```c
void Three_PWM_Init(void);
// Initialize CCP3, CCP4, CCP5 for PWM output

void ADC_Init(void);
// Configure ADC for AN0 and AN1

unsigned int ADC_Read(unsigned char channel);
// Read ADC value from specified channel (0 or 1)
// Returns: 10-bit ADC value (0-1023)

unsigned int ADC_to_Temperature(unsigned int adc_value);
// Convert ADC reading to temperature in Celsius
// Returns: Temperature in °C

void Control_Temperature(unsigned int temp_adc);
// Adjust heater and fan PWM based on temperature

void Set_Heater_PWM(unsigned int duty_10bit);
// Set heater PWM duty cycle (0-499)

void Set_Fan_PWM(unsigned int duty_10bit);
// Set fan PWM duty cycle (0-499)

void Display_Temperature_LCD(unsigned int temperature_celsius);
// Display temperature on LCD
```

### Light Control

```c
void Set_Lamp_Brightness(unsigned int ldr_value);
// Adjust lamp brightness based on LDR reading
// Inverse control: dark = bright

void Display_LDR_LCD(unsigned int ldr_value);
// Display light level percentage on LCD
```

### System Reset

```c
void Timer0_Init(void);
// Initialize Timer0 for button monitoring and LED blink

void System_Reset(void);
// Reset entire system and return to authentication
```

### LCD Functions

```c
void LCD_Init(void);
void LCD_Command(unsigned char cmd);
void LCD_Char(unsigned char data);
void LCD_String(const char *str);
```

### Keypad Functions

```c
unsigned char Keypad_Read(void);
// Read pressed key, returns character ('0'-'9', 'A'-'D', '*', '#')
```

---

## Pin Configuration Summary

### PORT A (Analog Inputs)
- **RA0 (AN0)**: LM35 Temperature Sensor
- **RA1 (AN1)**: LDR Light Sensor

### PORT B (Keypad & LCD Control)
- **RB0-RB3**: Keypad Columns
- **RB4**: LCD RS
- **RB5**: LCD RW
- **RB6**: LCD EN
- **RB7**: Keypad Row (shared)

### PORT C (Control Outputs)
- **RC0**: Buzzer
- **RC2**: Servo Motor (also CCP1, not used for PWM here)
- **RC3**: Reset Button (Input)
- **RC4**: Status LED

### PORT D (LCD Data)
- **RD0-RD7**: LCD Data Bus

### PORT G (PWM & Status)
- **RG0 (CCP3)**: Lamp PWM
- **RG1**: System Status LED
- **RG3 (CCP4)**: Heater PWM
- **RG4 (CCP5)**: Fan PWM

---

## Performance Characteristics

- **CPU Frequency**: 10 MHz
- **PWM Frequency**: ~5 kHz
- **ADC Resolution**: 10-bit (0-1023)
- **ADC Conversion Time**: ~50 μs
- **LCD Update Rate**: ~2 seconds
- **Servo Pulse Width**: 0.5ms - 1.0ms (0° - 90°)
- **Temperature Range**: 0°C - 50°C
- **Light Sensor Range**: 0-100% (ADC: 11-989)

---

## Future Enhancements

1. **EEPROM Storage**: Save password and settings
2. **RTC Module**: Add time-based access control
3. **GSM/WiFi**: Remote monitoring and alerts
4. **Multiple Users**: Support for multiple passwords
5. **Data Logging**: Log temperature and access events
6. **Voice Feedback**: Audio prompts for status
7. **Biometric Access**: Fingerprint sensor integration
8. **Energy Monitoring**: Track power consumption

---

## Troubleshooting

### System Won't Start
- Check power supply (5V stable)
- Verify oscillator circuit (10MHz crystal + caps)
- Check MCLR pin (pulled high)

### LCD Shows Garbage
- Adjust contrast potentiometer
- Check LCD connections
- Verify initialization sequence

### Servo Not Moving
- Check power supply current capacity
- Verify RC2 connection
- Test servo separately with known good signal

### ADC Readings Incorrect
- Verify VREF connections (VDD/VSS)
- Check sensor power supply
- Add bypass capacitors (100nF)
- Verify ADCON1 configuration

### Reset Button Not Working
- Check pull-up resistor (10K)
- Verify RC3 connection
- Check Timer0 interrupt enable

---

## License

This project is provided as-is for educational purposes.

---

## Credits

**Developer**: [Your Name]  
**Target Device**: Microchip PIC18F6722  
**IDE**: MPLAB IDE with MCC18  
**Date**: December 2025

---

## Support

For questions or issues:
1. Review this documentation thoroughly
2. Check hardware connections against pin configuration
3. Verify compiler settings match Makefile
4. Test individual modules separately
5. Use MPLAB debugger with breakpoints

---

## Version History

- **v1.0** (Dec 2025): Initial release
  - Security access control
  - Temperature monitoring and control
  - Automatic lighting system
  - System reset functionality

---

**End of Documentation**
