# Smart Home Security & Environmental Control System

<p align="center">
  <img src="./Smart_Home_Monitoring_Using_PIC18f6722/Diagram_images_PNG/Screenshot%202025-12-28%20122732.png" alt="Proteus Simulation" width="700"/>
</p>

#### Download the project ZIP: [OKAY_man_3.zip](./Smart_Home_Monitoring_Using_PIC18f6722/CODEs_MPlab_files/OKAY_man_3.zip)

A compact embedded project for the PIC18F6722 that combines access control, temperature regulation, and automatic lighting. The system uses a 4x4 keypad for authentication, LM35 and LDR sensors for environmental monitoring, PWM-driven actuators (heater, fan, lamp), and a servo for door locking. Designed for development with MPLAB / MCC18 and optionally simulated in Proteus.

---

## Table of contents
- Project overview
- Features
- Hardware requirements & pinout
- Software architecture
- System workflows
- Installation & build
- Programming the device
- Configuration
- Testing & troubleshooting
- API reference
- Future enhancements
- License & credits
- Version history

---

## Project overview
This project implements a small smart-home controller on the PIC18F6722 (10 MHz). It provides:
- Password-protected access control (4-digit password, keypad, servo lock, buzzer).
- Temperature monitoring and control (LM35, heater and fan PWM).
- Automatic lighting based on ambient light (LDR → lamp PWM).
- Reset and status management (hardware reset, timer-driven LED blink).

The design focuses on modularity, interrupt-driven tasks, and PWM-based actuators.

---

## Key features
- Security access control
  - 4-digit password via 4x4 keypad
  - Servo-controlled door lock
  - Status LED and buzzer for alerts
  - Lockout after 3 failed attempts (configurable)
- Temperature control
  - LM35 on AN0
  - Heater (CCP4) and fan (CCP5) controlled by PWM
  - Intelligent temperature zones (20–50 °C)
  - LCD temperature display
- Automatic lighting
  - LDR on AN1
  - Lamp brightness via CCP3 (inverse control)
  - LCD light-level display
- System controls
  - Hardware reset button (RC3)
  - Timer0-based interrupt and status LED (RG1)

---

## Hardware requirements

Microcontroller
- PIC18F6722 @ 10 MHz (HS oscillator)

Power
- 5 V regulated supply
- Ensure sufficient current for servo and other actuators (use separate supply for servo if needed)

Recommended components
- 16×2 character LCD
- 4×4 keypad
- LM35 temperature sensor
- LDR with voltage divider
- Standard hobby servo (door)
- Buzzer, LEDs
- Drivers/transistors for heater/fan/lamp as required
- PICkit 3/4 (programming)

Pin summary (wiring examples)
- PORT A
  - RA0 (AN0): LM35
  - RA1 (AN1): LDR
- PORT B
  - RB0–RB3: Keypad columns
  - RB4: LCD RS
  - RB5: LCD RW
  - RB6: LCD EN
  - RB7: Keypad row (shared)
- PORT C
  - RC0: Buzzer
  - RC2: Servo control (door)
  - RC3: Reset button (active low, with pull-up)
  - RC4: Status LED
- PORT D
  - RD0–RD7: LCD data bus (D0–D7)
- PORT G
  - RG0 (CCP3): Lamp PWM
  - RG1: System status LED
  - RG3 (CCP4): Heater PWM
  - RG4 (CCP5): Fan PWM

Wiring tips
- Use bypass caps (100 nF) near sensors and MCU supply pins.
- Use proper voltage dividers, references, and drivers for actuators.

---

## Software architecture

Module layout
```
OKAY_MAN/
├── Untitled.c          # Main program entry point
├── security.c / .h     # Password auth & door control
├── temp_control.c / .h # Temperature monitoring & control
├── ldr_control.c / .h  # Light sensing & lamp control
├── Timer2.c / .h       # PWM initialization & ADC helpers
├── timer0.c / .h       # System reset & status LED (ISR)
├── LCD.c / .h          # LCD driver
├── KEYPAD.c / .h       # 4×4 keypad driver
├── Makefile            # Build script
└── okay.hex            # Compiled firmware
```

Design patterns
- Modular code: each subsystem in its own module
- Interrupt-driven timer0 for reset monitoring and LED blinking
- State machine for authentication
- PWM via Timer2 for smooth actuator control

PWM and ADC notes
- PWM configured with PR2 = 124 (≈5 kHz @ 10 MHz, prescaler=4)
- ADC: 10-bit (0–1023)

---

## System workflows (summary)

Initialization
- Configure oscillator, ADC (AN0, AN1), and digital I/O
- Initialize LCD, PWM (Timer2), Timer0 interrupt, and security module
- Prompt for password on the LCD

Authentication flow
- Display "Enter Password"
- Read 4 digits from keypad
- If correct → grant access (LED on, servo opens for defined time, then closes) and proceed to main loop
- If wrong → increment attempt counter; after 3 wrong attempts, lock system for a configurable period and sound buzzer

Main control loop
- Monitor reset button via Timer0 ISR (if pressed → System_Reset())
- Read temperature (LM35) and call Control_Temperature()
  - Adjust heater/fan PWM according to temperature zones
- Read LDR and call Set_Lamp_Brightness()
- Update LCD with temperature and light status
- Wait (≈2 s) and repeat

Light control logic
- Read LDR (value range approx. 11–989)
- Brightness duty = (989 − LDR) × 499 / (989 − 11)  (inverted control: darker → brighter)
- Apply to CCP3 PWM

Temperature zones (ADC thresholds)
| ADC range | Temperature | Heater | Fan | Status |
|-----------|-------------|--------|-----|--------|
| < 41      | < 20 °C     | 100%   | 0%  | VERY COLD |
| 41–51     | 20–25 °C    | 100→50%| 0%  | COLD |
| 51–61     | 25–30 °C    | 50→0%  | 0%  | APPROACHING COMFORT |
| 61–72     | 30–35 °C    | 0%     | 0%  | COMFORTABLE |
| 72–82     | 35–40 °C    | 0%     | 0→50%| GETTING WARM |
| 82–102    | 40–50 °C    | 0%     | 50→100%| WARM |
| > 102     | > 50 °C     | 0%     | 100% | HOT |

---

## Installation & build

Prerequisites
- MPLAB IDE (v8.x or compatible)
- MCC18 C Compiler (for PIC18)
- MPLINK linker
- PICkit 3/4 or compatible programmer
- Proteus (optional for simulation)

Build with MPLAB
1. Open `okay.mcp` in MPLAB IDE
2. Select PIC18F6722 as target device
3. Recommended build options:
   - Optimization disabled (for easier debugging)
   - Debug mode enabled if you will use the debugger
4. Build → Project → Build All
5. Confirm `okay.hex` is generated

Build with Make (command line)
```bash
# Windows (MPLAB command-line tools)
make -f Makefile
# Clean build
make clean
make -f Makefile
```

---

## Programming the device
1. Connect PICkit programmer to ICSP pins (MCLR, VPP, VDD, VSS, PGD, PGC).
2. Load `okay.hex` into the programmer utility.
3. Program the PIC and verify the operation.

---

## Configuration

Change default password
- Edit `security.c`:
```c
static const char correctPassword[PASSWORD_LENGTH + 1] = "1234";
```
Change the string to your 4-digit password.

Adjust temperature thresholds
- Edit `temp_control.h`:
```c
#define TEMP_VERY_COLD    41    // <20°C
#define TEMP_COLD         51    // 20–25°C
#define TEMP_COMFORT_LOW  61    // 25–30°C
#define TEMP_COMFORT_HIGH 72    // 30–35°C
#define TEMP_WARM         82    // 35–40°C
#define TEMP_HOT          102   // 40–50°C
```

PWM frequency
- Configured in `Timer2.c`:
```c
PR2 = 124;  // ~5 kHz @ 10 MHz, prescaler = 4
```
Formula:
```
PWM_freq = Fosc / (4 × Prescaler × (PR2 + 1))
```

---

## Testing & debugging

Initial checks
- Power-on: LCD displays "Enter Password" and RG1 blinks
- Security: correct password opens servo and toggles status LED; 3 wrong attempts triggers buzzer and lockout
- Temperature: heating and cooling behavior should reflect temperature changes on LM35
- Lighting: covering LDR should increase lamp brightness (PWM duty)
- Reset button: pressing RC3 returns system to password prompt

Common issues and fixes
- LCD blank: adjust contrast potentiometer
- Keypad unresponsive: verify PORTB wiring
- Servo jitter: use a separate 5V supply and common ground
- Unstable ADC: add 100 nF decoupling capacitors near sensors
- PWM not working: ensure Timer2 is configured (T2CON)

---

## API reference (selected)

Security
```c
void Security_Init(void);    // Init buzzer, LED, servo
void Security_Run(void);     // Blocking password auth loop
```

Temperature & ADC
```c
void Three_PWM_Init(void);                    // Init CCP3/4/5
void ADC_Init(void);                          // Configure ADC for AN0, AN1
unsigned int ADC_Read(unsigned char channel); // Read ADC (0 or 1)
unsigned int ADC_to_Temperature(unsigned int adc_value);
void Control_Temperature(unsigned int temp_adc);
void Set_Heater_PWM(unsigned int duty_10bit); // duty: 0–499
void Set_Fan_PWM(unsigned int duty_10bit);    // duty: 0–499
```

Light control
```c
void Set_Lamp_Brightness(unsigned int ldr_value); // inverse mapping
void Display_LDR_LCD(unsigned int ldr_value);
```

System reset / Timer0
```c
void Timer0_Init(void);
void System_Reset(void); // Reinitialize system and return to auth
```

LCD / Keypad
```c
void LCD_Init(void);
void LCD_Command(unsigned char cmd);
void LCD_Char(unsigned char data);
void LCD_String(const char *str);

unsigned char Keypad_Read(void); // Returns '0'-'9', 'A'-'D', '*', '#'
```

---

## Troubleshooting (quick reference)

- System won't start: check 5 V supply, crystal connections, MCLR pull-up
- LCD shows garbage: verify initialization and contrast pot
- Servo not moving: confirm servo supply and RC2 signal
- ADC incorrect: verify VREF, bypass caps, and ADCON1 settings
- Reset button not detected: check pull-up resistor and Timer0 ISR

---

## Performance & limits
- CPU frequency: 10 MHz
- PWM frequency: ~5 kHz
- ADC resolution: 10-bit
- ADC conversion: ~50 μs
- Typical LCD update: every ~2 seconds
- Servo pulse range used: ~0.5 ms (0°) to ~1.0 ms (90°)
- Temperature measurement: 0–50 °C (LM35)

---

## Future enhancements
- Store password and settings in EEPROM
- Add RTC for time-based access
- GSM/Wi‑Fi alerts and remote monitoring
- Multiple user accounts
- Event data logging
- Voice prompts or TTS
- Biometric authentication (fingerprint)
- Energy monitoring integration

---

## License
Provided as-is for educational use. No warranty.

---

## Credits
- Developer: [Basant Saad Eldin]
- Target device: Microchip PIC18F6722
- IDE: MPLAB IDE with MCC18
- Date: December 2025

---

## Support & contribution
If you encounter problems:
1. Re-read this README and verify wiring/power.
2. Check compiler and Makefile settings.
3. Test modules individually (LCD, keypad, ADC, PWM).
4. Use the MPLAB debugger when available.

Contributions are welcome — open an issue or fork the repository.

---

## Version history
- v1.0 (Dec 2025): Initial release — access control, temperature control, automatic lighting, reset functionality.

**End of documentation**
