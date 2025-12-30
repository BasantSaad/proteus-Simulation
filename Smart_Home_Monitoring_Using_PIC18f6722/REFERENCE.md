# Automation Home Technical Reference Manual

## Table of Contents
1. [Hardware Specifications](#hardware-specifications)
2. [Register Configuration](#register-configuration)
3. [Memory Map](#memory-map)
4. [Function Reference](#function-reference)
5. [Timing Diagrams](#timing-diagrams)
6. [Error Codes](#error-codes)
7. [Electrical Characteristics](#electrical-characteristics)

---

## Hardware Specifications

### Microcontroller: PIC18F6722

| Parameter | Value |
|-----------|-------|
| Architecture | 8-bit RISC |
| Program Memory | 128 KB Flash |
| Data Memory | 3,936 bytes RAM |
| EEPROM | 1024 bytes |
| Operating Frequency | 10 MHz (HS Oscillator) |
| Instruction Cycle | 400 ns (Fosc/4) |
| I/O Pins | 70 |
| ADC Channels | 16 (10-bit) |
| CCP Modules | 5 (all PWM capable) |
| Timers | Timer0, Timer1, Timer2, Timer3, Timer4 |
| Interrupts | 20 sources |

### Clock Configuration

```
Crystal Frequency: 10 MHz
Instruction Cycle: Fosc/4 = 2.5 MHz (400 ns per instruction)
ADC Clock: Fosc/32 = 312.5 kHz
PWM Frequency: Fosc / (4 × Prescaler × (PR2 + 1))
              = 10MHz / (4 × 4 × 125) = 5 kHz
```

---

## Register Configuration

### ADCON0 - ADC Control Register 0

```
Bit 7-6: Unimplemented
Bit 5-2: CHS[3:0] - Channel Select
         0000 = AN0 (Temperature Sensor)
         0001 = AN1 (LDR Sensor)
Bit 1:   GO/DONE - ADC Conversion Status
         1 = Conversion in progress
         0 = Idle
Bit 0:   ADON - ADC Enable
         1 = ADC module enabled
```

**Configuration**: `0x01` (AN0, ADC ON)

### ADCON1 - ADC Control Register 1

```
Bit 7-6: Unimplemented
Bit 5:   VCFG1 - Negative Reference
         0 = VSS
Bit 4:   VCFG0 - Positive Reference
         0 = VDD
Bit 3-0: PCFG[3:0] - Port Configuration
         1101 = AN0, AN1 are analog, rest digital
```

**Configuration**: `0x0D` (AN0, AN1 analog)

### ADCON2 - ADC Control Register 2

```
Bit 7:   ADFM - Result Format
         1 = Right justified
Bit 6:   Unimplemented
Bit 5-3: ACQT[2:0] - Acquisition Time
         101 = 12 TAD
Bit 2-0: ADCS[2:0] - Conversion Clock
         010 = Fosc/32
```

**Configuration**: `0xBE` (Right justified, 12 TAD, Fosc/32)

### T0CON - Timer0 Control Register

```
Bit 7:   TMR0ON - Timer0 Enable
         1 = Enabled
Bit 6:   T08BIT - Timer0 Mode
         0 = 16-bit mode
Bit 5:   T0CS - Clock Source
         0 = Internal clock (Fosc/4)
Bit 4:   T0SE - Source Edge
         0 = Increment on low-to-high
Bit 3:   PSA - Prescaler Assignment
         0 = Prescaler assigned to Timer0
Bit 2-0: T0PS[2:0] - Prescaler Select
         111 = 1:256
```

**Configuration**: `0x07` (16-bit, internal, 1:256)

**Timer0 Reload Value**: `0xFA47` (~10ms interrupt at 10MHz)

### T2CON - Timer2 Control Register

```
Bit 7:   Unimplemented
Bit 6-3: TOUTPS[3:0] - Postscaler (not used in PWM)
Bit 2:   TMR2ON - Timer2 Enable
         1 = Enabled
Bit 1-0: T2CKPS[1:0] - Prescaler
         01 = 1:4
```

**Configuration**: `0x05` (1:4 prescaler, Timer2 ON)

**PR2 Value**: `124` (PWM period)

### CCPxCON - CCP Module Control Registers

```
Bit 7-6: DCxB[1:0] - PWM Duty Cycle LSBs
Bit 5-4: CCPxM[3:2] - CCP Mode Select
Bit 3-0: CCPxM[1:0] - CCP Mode Select
         1100 = PWM mode
```

**CCP3 (Lamp)**: `0x0C` (PWM mode)  
**CCP4 (Heater)**: `0x0C` (PWM mode)  
**CCP5 (Fan)**: `0x0C` (PWM mode)

### INTCON - Interrupt Control Register

```
Bit 7:   GIE - Global Interrupt Enable
         1 = Enabled
Bit 6:   PEIE - Peripheral Interrupt Enable
         1 = Enabled
Bit 5:   TMR0IE - Timer0 Overflow Interrupt Enable
         1 = Enabled
Bit 4:   INT0IE - External Interrupt 0 Enable
Bit 3:   RBIE - PORTB Change Interrupt Enable
Bit 2:   TMR0IF - Timer0 Overflow Flag
Bit 1:   INT0IF - External Interrupt 0 Flag
Bit 0:   RBIF - PORTB Change Interrupt Flag
```

**Configuration**: `0xA0` (GIE, TMR0IE enabled)

---

## Memory Map

### RAM Usage (Approximate)

| Module | Variables | Size (bytes) |
|--------|-----------|--------------|
| Security | enteredPassword, key, index, attempts | 8 |
| Temperature | temp_reading, temperature_celsius | 4 |
| LDR | ldr_reading, ldr_percent | 4 |
| Timer0 ISR | tick, reset_flag | 2 |
| PWM | duty_cycle calculations | 4 |
| LCD | buffer, command variables | 20 |
| **Total Estimated** | | **~42 bytes** |

### Program Memory Distribution

| Module | Approximate Size |
|--------|------------------|
| security.o | ~22 KB |
| temp_control.o | ~24 KB |
| LCD.o | ~19 KB |
| ldr_control.o | ~10 KB |
| Timer2.o | ~5 KB |
| timer0.o | ~7 KB |
| KEYPAD.o | ~5 KB |
| Untitled.o | ~7 KB |
| **Total** | **~99 KB** |

---

## Function Reference

### Security Module (security.c)

#### `void Security_Init(void)`

**Purpose**: Initialize security hardware pins

**Registers Modified**:
- `TRISC0 = 0` (Buzzer output)
- `TRISC4 = 0` (LED output)
- `TRISC2 = 0` (Servo output)

**Initial States**:
- Buzzer: OFF
- LED: OFF
- Servo: 0° (closed)

**Execution Time**: ~10 μs

---

#### `void Security_Run(void)`

**Purpose**: Handle password authentication flow

**Algorithm**:
```
1. Display "Enter Password"
2. Read 4 keypad digits
3. Validate against stored password
4. IF correct:
     - Display "Access Granted"
     - Open door (servo 90°)
     - Wait 1 second
     - Close door (servo 0°)
     - Return to main program
   ELSE:
     - Increment attempts counter
     - Display "Access Denied"
     - IF attempts >= 3:
         - Lock system
         - Sound alarm
         - 5-second countdown
         - Reset attempts
     - Retry password entry
```

**Blocking**: Yes (waits for correct password)

**Variables**:
- `enteredPassword[5]`: User input buffer
- `attempts`: Failed attempt counter
- `accessGranted`: Boolean flag

**Constants**:
- `correctPassword = "1234"`
- `PASSWORD_LENGTH = 4`
- `MAX_ATTEMPTS = 3`

---

#### `static void Servo_SetAngle(unsigned char angle)`

**Purpose**: Position servo motor

**Parameters**:
- `angle`: Desired angle (0-90 degrees)

**PWM Timing** (at 10MHz):
```
Pulse Width:
  0°:  0.5ms → ticks = 250
  90°: 1.0ms → ticks = 500

Calculation:
  ticks = 250 + (angle × (500-250) / 90)
```

**Servo Pulse Train**:
- 50 pulses sent for stable positioning
- 20ms period per pulse
- Total positioning time: ~1 second

---

### Temperature Control (temp_control.c)

#### `void Control_Temperature(unsigned int temp_adc)`

**Purpose**: Automatic climate control based on temperature

**Input Range**: 0-1023 (10-bit ADC)

**Temperature Mapping**:
```
ADC = (Temperature_°C × 1023) / 500
Temperature_°C = (ADC × 500) / 1023
```

**Control Zones**:

| Zone | ADC Range | Temp (°C) | Heater PWM | Fan PWM |
|------|-----------|-----------|------------|---------|
| VERY_COLD | 0-40 | 0-20 | 499 (100%) | 0 (0%) |
| COLD | 41-50 | 20-25 | 499→249 | 0 |
| APPROACHING | 51-60 | 25-30 | 249→0 | 0 |
| COMFORTABLE | 61-72 | 30-35 | 0 | 0 |
| GETTING_WARM | 73-82 | 35-40 | 0 | 0→249 |
| WARM | 83-101 | 40-50 | 0 | 249→499 |
| HOT | 102+ | 50+ | 0 | 499 (100%) |

**PWM Duty Cycle Range**: 0-499 (10-bit, for PR2=124)

**Calculation Example** (COLD zone):
```c
heater_duty = 499 - ((temp_adc - 41) × 250) / (51 - 41)
            = 499 - ((temp_adc - 41) × 25)
```

---

#### `void Set_Heater_PWM(unsigned int duty_10bit)`

**Purpose**: Set heater PWM duty cycle

**Parameters**:
- `duty_10bit`: 0-499 (10-bit PWM value)

**Register Updates**:
```c
CCPR4L = duty_10bit >> 2;              // Upper 8 bits
CCP4CONbits.DC4B0 = (duty_10bit & 0x01);    // LSB bit 0
CCP4CONbits.DC4B1 = ((duty_10bit >> 1) & 0x01); // LSB bit 1
```

**PWM Resolution**: 10 bits (0-1023 possible, 0-499 used)

**Output Frequency**: 5 kHz

---

### Light Control (ldr_control.c)

#### `void Set_Lamp_Brightness(unsigned int ldr_value)`

**Purpose**: Adjust lamp brightness inversely to ambient light

**Input Range**: 11-989 (typical LDR ADC range)

**Mapping Formula**:
```
duty_cycle = (989 - ldr_value) × 499 / (989 - 11)
duty_cycle = (989 - ldr_value) × 0.51
```

**Control Logic**:
- Dark room (ldr_value = 11) → Bright lamp (duty = 499)
- Bright room (ldr_value = 989) → Dim lamp (duty = 0)

**Examples**:
| LDR ADC | Light Level | Lamp Duty | Brightness |
|---------|-------------|-----------|------------|
| 11 | 0% (dark) | 499 | 100% |
| 500 | 50% | 249 | 50% |
| 989 | 100% (bright) | 0 | 0% |

---

### Timer Functions (Timer2.c)

#### `unsigned int ADC_Read(unsigned char channel)`

**Purpose**: Read ADC value from specified channel

**Parameters**:
- `channel`: 0 (AN0/Temperature) or 1 (AN1/LDR)

**Timing**:
1. Channel selection: ~1 μs
2. Acquisition delay: 5 ms (for sensor stabilization)
3. Conversion time: ~50 μs (12 TAD at Fosc/32)

**Total Time**: ~5.05 ms per reading

**Return**: 10-bit ADC value (0-1023)

**Algorithm**:
```c
1. Select ADC channel (ADCON0bits.CHS)
2. Wait acquisition time (5ms)
3. Start conversion (ADCON0bits.GO = 1)
4. Wait for completion (while ADCON0bits.GO)
5. Combine ADRESH:ADRESL
6. Return 10-bit result
```

---

### System Reset (timer0.c)

#### Interrupt Service Routine: `high_isr(void)`

**Trigger**: Timer0 overflow (~10ms)

**Operations**:
1. Clear Timer0 interrupt flag
2. Reload Timer0 (TMR0H:TMR0L = 0xFA47)
3. Check reset button (RC3)
   - If pressed: Set `reset_flag`
4. Blink status LED (200ms period)

**Interrupt Latency**: ~10 instruction cycles (~4 μs)

**Priority**: High

---

#### `void System_Reset(void)`

**Purpose**: Complete system reinitialization

**Sequence**:
```
1. Disable Timer0 interrupt
2. Turn OFF all outputs:
   - Lamp → Full brightness (LDR=989)
   - Heater → 0% PWM
   - Fan → 0% PWM
3. Display "System Reset"
4. Reinitialize all modules:
   - LCD_Init()
   - Security_Init()
   - Security_Run()  [blocks until password]
   - Three_PWM_Init()
   - ADC_Init()
5. Status LED: ON for 1 second
6. Re-enable Timer0 interrupt
```

**Total Reset Time**: ~2-3 seconds (plus password entry time)

---

### LCD Functions (LCD.c)

#### `void LCD_Command(unsigned char cmd)`

**Purpose**: Send command to LCD

**Timing**:
- RS = 0 (Command mode)
- Data setup: ~1 μs
- EN pulse: ~2 μs (high + low)
- Command execution: 40 μs - 1.64 ms (depends on command)

**Common Commands**:
| Command | Hex | Function |
|---------|-----|----------|
| Clear Display | 0x01 | Clear screen, home cursor |
| Return Home | 0x02 | Cursor to home |
| Entry Mode | 0x06 | Auto-increment cursor |
| Display ON | 0x0C | Display ON, cursor OFF |
| Set DDRAM (Line 1) | 0x80 | First line position 0 |
| Set DDRAM (Line 2) | 0xC0 | Second line position 0 |

---

#### `void LCD_String(const char *str)`

**Purpose**: Display string on LCD

**Implementation**:
```c
while(*str) {
    LCD_Char(*str++);
}
```

**Timing**: ~50 μs per character

---

### Keypad Functions (KEYPAD.c)

#### `unsigned char Keypad_Read(void)`

**Purpose**: Scan 4x4 keypad and return pressed key

**Scanning Method**: Column-by-column scanning

**Algorithm**:
```
FOR each column (0-3):
    1. Set column LOW, others HIGH
    2. Read rows (RB4-RB7)
    3. IF row LOW:
         - Debounce (10ms delay)
         - Wait for key release
         - Return character
    4. Next column
RETURN 0xFF (no key pressed)
```

**Debounce Time**: 10 ms

**Keypad Matrix**:
```
     Col0  Col1  Col2  Col3
Row0:  1     2     3     A
Row1:  4     5     6     B
Row2:  7     8     9     C
Row3:  *     0     #     D
```

**Port Mapping**:
- Columns: RB0-RB3 (outputs)
- Rows: RB4-RB7 (inputs with pull-ups)

---

## Timing Diagrams

### PWM Waveform (CCP3/CCP4/CCP5)

```
Period = (PR2 + 1) × 4 × Tosc × Prescaler
       = 125 × 4 × 100ns × 4
       = 200 μs (5 kHz)

High Time (for duty=249):
  = 249 × Tosc × Prescaler
  = 249 × 100ns × 4
  = 99.6 μs (49.8% duty cycle)

     _____        _____
____|     |______|     |____
    <---->
    99.6μs
    <--------------->
       200 μs
```

### Servo Pulse Timing

```
20ms Period (50 Hz)

0° Position (0.5ms pulse):
  ___                                    ___
_|   |__________________________________|   |__
  0.5ms         19.5ms                 0.5ms

90° Position (1.0ms pulse):
  _____                                _____
_|     |______________________________|     |__
  1.0ms         19.0ms                1.0ms
```

### ADC Conversion Timing

```
Total Conversion = Acquisition + Conversion
                 = 5ms + ~50μs
                 ≈ 5.05ms

Timeline:
Select     Acquire              Convert   Read
Channel    (5ms)                (50μs)    Result
   |-------------|-------------------|
   0            5ms              5.05ms
```

### LCD Write Cycle

```
RS/Data Setup  EN High   EN Low    Process
   |---------|-------|-------|---------|
   0        1μs     3μs     5μs     50μs

             ________
EN: ________|        |________
            <------->
             2μs min
```

### Keypad Scan Timing

```
Set Column   Read Rows   Debounce   Wait Release
   |------------|-----------|-------------|
   1μs         10μs        10ms         Variable

For each column:
    ____
COL |    |____________
    <--->
    ~1μs

    Scan time per key press: ~10ms
    Full matrix scan: ~40μs (no key pressed)
```

---

## Error Codes

### System Status Indicators

| LED State | Meaning |
|-----------|---------|
| RG1 Blinking (200ms) | Normal operation |
| RC4 Solid ON | Access granted / Door open |
| RC4 OFF | Door closed / Access denied |
| RG1 ON (2 seconds) | System reset complete |

### Buzzer Patterns

| Pattern | Meaning |
|---------|---------|
| Continuous tone | System locked (3 failed attempts) |
| Silent | Normal operation |

### LCD Messages

| Message | Status | Action Required |
|---------|--------|----------------|
| "Enter Password" | Waiting for input | Enter 4-digit code |
| "Access Granted" | Authenticated | Door opening |
| "Access Denied" | Wrong password | Retry |
| "SYSTEM LOCKED" | Max attempts reached | Wait for countdown |
| "ALARM ON" | Security alert | Wait 5 seconds |
| "Wait X sec" | Cooldown timer | Countdown active |
| "System Reset" | Reinitialization | System restarting |
| "Temperature: XX C" | Normal display | Monitoring active |
| "LDR: XX%" | Light level display | Monitoring active |

---

## Electrical Characteristics

### Power Supply

| Parameter | Min | Typ | Max | Unit |
|-----------|-----|-----|-----|------|
| VDD | 4.5 | 5.0 | 5.5 | V |
| IDD (active) | - | 30 | 50 | mA |
| IDD (idle) | - | 10 | 20 | mA |

### I/O Pin Specifications

| Parameter | Value | Unit |
|-----------|-------|------|
| VOH (high-level output) | 4.3 | V |
| VOL (low-level output) | 0.6 | V |
| VIH (high-level input) | 2.0 | V |
| VIL (low-level input) | 0.8 | V |
| IOH (source current) | 25 | mA |
| IOL (sink current) | 25 | mA |

### ADC Specifications

| Parameter | Value | Unit |
|-----------|-------|------|
| Resolution | 10 | bits |
| Absolute accuracy | ±2 | LSB |
| Conversion time | 11-12 | TAD |
| Input impedance | 2.5 | kΩ |
| Input voltage range | 0 to VDD | V |

### PWM Specifications

| Parameter | Value | Unit |
|-----------|-------|------|
| Resolution (at 5kHz) | 10 | bits |
| Frequency range | 1.2 - 200 | kHz |
| Duty cycle range | 0 - 100 | % |
| Output impedance | 50 | Ω |

### Sensor Specifications

#### LM35 Temperature Sensor
| Parameter | Value | Unit |
|-----------|-------|------|
| Operating range | -55 to +150 | °C |
| Scale factor | 10 | mV/°C |
| Accuracy (25°C) | ±0.5 | °C |
| Quiescent current | 60 | μA |

#### LDR (Light Dependent Resistor)
| Parameter | Value | Unit |
|-----------|-------|------|
| Dark resistance | 1 | MΩ |
| Light resistance | 1-10 | kΩ |
| Response time | 20-30 | ms |

#### Servo Motor (Generic)
| Parameter | Value | Unit |
|-----------|-------|------|
| Operating voltage | 4.8 - 6.0 | V |
| Operating current | 100-500 | mA |
| Stall current | 1-2 | A |
| Pulse width (0°) | 0.5 | ms |
| Pulse width (90°) | 1.0 | ms |
| Pulse period | 20 | ms |

---

## Timing Specifications

### Instruction Timing (at 10MHz)

| Operation | Cycles | Time |
|-----------|--------|------|
| Single instruction | 1 | 400 ns |
| Branch instruction | 2 | 800 ns |
| Table read/write | 2 | 800 ns |
| Multiply | 1 | 400 ns |

### Peripheral Timing

| Module | Parameter | Value |
|--------|-----------|-------|
| Timer0 | Overflow period | 10 ms |
| Timer2 | PWM period | 200 μs |
| ADC | Conversion time | 50 μs |
| LCD | Character write | 50 μs |
| Keypad | Debounce time | 10 ms |

### System Response Times

| Event | Response Time |
|-------|---------------|
| Reset button press | 10 ms (max) |
| Temperature change | 2 seconds |
| Light level change | 2 seconds |
| Password validation | ~500 ms |
| Door operation | ~1 second |

---

## Component Interfacing

### LCD Interface Timing

```
Timing Requirements (HD44780):
- EN cycle time: ≥500ns
- EN pulse width (high): ≥230ns
- EN pulse width (low): ≥500ns
- Data setup time: ≥80ns
- Data hold time: ≥10ns
- RS/RW setup time: ≥60ns
```

### Keypad Pull-up Resistors

```
Recommended: 10kΩ on each row line
Purpose: Define logic HIGH when no key pressed
Current when key pressed: VDD/10kΩ ≈ 0.5mA
```

### PWM Output Filtering

```
For smooth DC output:
- Add RC low-pass filter
- Cutoff frequency: 500 Hz (< PWM freq / 10)
- Components: R=1kΩ, C=330nF
```

### ADC Input Protection

```
Sensor → 10kΩ resistor → ADC pin → 100nF to GND
Purpose:
- Current limiting
- Noise filtering
- ESD protection
```

---

## Calibration Procedures

### Temperature Sensor Calibration

```c
// Measure actual temperature with reference thermometer
// Adjust scaling factor in code:

unsigned int ADC_to_Temperature(unsigned int adc_value) {
    // Standard: temp = (adc × 500) / 1023
    // Calibrated: temp = (adc × CALIB_FACTOR) / 1023
    #define CALIB_FACTOR 500  // Adjust based on measurements
    return ((unsigned long)adc_value * CALIB_FACTOR) / 1023;
}
```

### LDR Calibration

```c
// Measure LDR voltage in dark and bright conditions
// Adjust mapping in Set_Lamp_Brightness():

#define LDR_DARK  11   // ADC value in complete darkness
#define LDR_BRIGHT 989 // ADC value in bright light

// Then use these values in mapping formula
```

### Servo Calibration

```c
// Adjust pulse width constants for your specific servo:
#define SERVO_MIN_TICKS  250  // 0.5ms pulse width
#define SERVO_MAX_TICKS  500  // 1.0ms pulse width

// Some servos may need:
// #define SERVO_MIN_TICKS  200  // 0.4ms
// #define SERVO_MAX_TICKS  600  // 1.2ms
```

---

## Performance Benchmarks

### CPU Utilization (Estimated)

| Task | Time per cycle | % CPU |
|------|----------------|-------|
| Main loop | 2 seconds | - |
| ADC reading | 10ms | 0.5% |
| PWM control | Continuous (hardware) | 0% |
| LCD update | 5ms | 0.25% |
| Temperature control | 100μs | <0.01% |
| Timer0 ISR | 10ms period, 50μs exec | 0.5% |
| **Total Active** | | **~1.5%** |
| **Idle Time** | | **~98.5%** |

### Memory Utilization

| Resource | Used | Total | % Used |
|----------|------|-------|--------|
| Program Flash | ~99 KB | 128 KB | 77% |
| RAM | ~42 bytes | 3,936 bytes | 1% |
| EEPROM | 0 bytes | 1,024 bytes | 0% |

### Power Consumption (Typical at 5V)

| Component | Current (mA) |
|-----------|--------------|
| PIC18F6722 (active) | 30 |
| LCD backlight | 20 |
| LEDs (2×) | 40 |
| Sensors (LM35, LDR) | 1 |
| Buzzer (when on) | 30 |
| Servo (moving) | 500 |
| Heater (100% PWM) | varies |
| Fan (100% PWM) | varies |

**Total (max)**: ~650mA + actuators

---

## Compiler Optimizations

### Current Settings (No Optimization)

```
-Ou-  // Disable optimize for speed/space
-Ot-  // Disable optimize for speed
-Ob-  // Disable optimize for branch
-Op-  // Disable optimize for size
-Or-  // Disable optimize redundant
-Od-  // Disable optimize for debug
-Opa- // Disable optimize procedural abstraction
```

**Reason**: Debug mode enabled (`-D__DEBUG`)

### Recommended Production Settings

```
-Ou+  // Enable general optimizations
-Ot+  // Optimize for speed
-Os   // Optimize for size
```

**Expected Improvements**:
- Code size: 15-25% reduction
- Execution speed: 10-20% improvement

---

## Development Tips

### Debugging Techniques

1. **Use LED Indicators**
```c
#define DEBUG_LED LATDbits.LATD0
DEBUG_LED = 1;  // Mark code execution point
```

2. **UART Debug Output** (if available)
```c
printf("Temp ADC: %u\n", temp_reading);
```

3. **MPLAB Simulator**
- Set breakpoints in critical functions
- Watch window for variables
- Stopwatch for timing analysis

4. **Logic Analyzer**
- Monitor PWM signals
- Verify timing specifications
- Check communication protocols

### Common Pitfalls

1. **Integer Overflow**
```c
// Wrong:
unsigned int result = adc_value * 500 / 1023;  // Overflow!

// Correct:
unsigned long temp = (unsigned long)adc_value * 500;
unsigned int result = temp / 1023;
```

2. **Floating Point Avoidance**
```c
// Slow:
float temp_c = adc * 0.488;  // Uses FP library

// Fast:
unsigned int temp_c = (adc * 500) / 1023;  // Integer math
```

3. **Busy-Wait Timing**
```c
// Blocking:
delay_ms(1000);  // CPU locked for 1 second

// Better: Use timer interrupts for non-critical delays
```

4. **ADC Channel Switching**
```c
// Always allow acquisition time after channel change:
ADCON0bits.CHS = new_channel;
delay_ms(5);  // Wait for stabilization
ADCON0bits.GO = 1;
```

---

## Version Information

| Item | Version |
|------|---------|
| Firmware | 1.0 |
| Compiler | MCC18 v3.x+ |
| MPLAB IDE | v8.x |
| Target Device | PIC18F6722 |
| Date Compiled | December 2025 |

---

## Acronyms and Abbreviations

| Term | Full Form |
|------|-----------|
| ADC | Analog-to-Digital Converter |
| CCP | Capture/Compare/PWM |
| COF | Common Object File Format |
| DAC | Digital-to-Analog Converter |
| EEPROM | Electrically Erasable Programmable Read-Only Memory |
| GIE | Global Interrupt Enable |
| HEX | Intel Hex Format |
| HS | High-Speed (oscillator mode) |
| ISR | Interrupt Service Routine |
| LCD | Liquid Crystal Display |
| LDR | Light Dependent Resistor |
| LSB | Least Significant Bit |
| MSB | Most Significant Bit |
| MPLAB | Microchip Programmable Logic Application Building |
| PIC | Peripheral Interface Controller |
| PWM | Pulse Width Modulation |
| RAM | Random Access Memory |
| RISC | Reduced Instruction Set Computer |
| ROM | Read-Only Memory |
| TAD | ADC Conversion Time Unit |
| TMR | Timer |
| TRIS | Tri-State Register |
| UART | Universal Asynchronous Receiver/Transmitter |
| VDD | Positive Supply Voltage |
| VSS | Ground (0V) |

---

**End of Technical Reference**
