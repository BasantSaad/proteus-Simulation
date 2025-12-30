# Automation Home Installation Guide

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Software Installation](#software-installation)
3. [Hardware Assembly](#hardware-assembly)
4. [Firmware Installation](#firmware-installation)
5. [Initial Configuration](#initial-configuration)
6. [Testing & Verification](#testing--verification)
7. [Troubleshooting](#troubleshooting)

---

## Prerequisites

### Required Tools

#### Software Tools
- [ ] MPLAB IDE v8.x or later
- [ ] MCC18 C Compiler for PIC18
- [ ] MPLINK Linker
- [ ] PICkit 3/4 Programmer Software
- [ ] Text Editor (Notepad++, VS Code, etc.)

#### Hardware Tools
- [ ] PICkit 3 or PICkit 4 Programmer
- [ ] Digital Multimeter
- [ ] Breadboard or PCB
- [ ] Wire strippers
- [ ] Soldering iron and solder (if using PCB)
- [ ] Wire cutters
- [ ] Small screwdrivers

#### Optional Tools
- [ ] Logic Analyzer
- [ ] Oscilloscope
- [ ] Proteus Simulation Software
- [ ] USB-to-Serial adapter (for debugging)

---

## Software Installation

### Step 1: Install MPLAB IDE

1. **Download MPLAB IDE**
   - Visit: https://www.microchip.com/mplab/mplab-integrated-development-environment
   - Download MPLAB IDE v8.92 (or latest compatible version)
   - File size: ~150 MB

2. **Run Installer**
   ```
   - Double-click: MPLAB_IDE_8_92.exe
   - Accept license agreement
   - Choose installation path: C:\Program Files\Microchip\MPLAB IDE
   - Select "Full Installation"
   - Install time: ~5 minutes
   ```

3. **Verify Installation**
   ```
   - Launch MPLAB IDE
   - Go to: Help → About MPLAB IDE
   - Confirm version number
   ```

### Step 2: Install MCC18 Compiler

1. **Download MCC18**
   - Visit: https://www.microchip.com/development-tools/pic-and-dspic-downloads-archive
   - Download: MPLAB C18 Compiler v3.47 or later
   - File size: ~50 MB

2. **Run Installer**
   ```
   - Double-click: mplabc18-v3_47.exe
   - Accept license agreement
   - Installation path: C:\MCC18
   - Install time: ~2 minutes
   ```

3. **Configure MPLAB to Use MCC18**
   ```
   - Open MPLAB IDE
   - Go to: Project → Set Language Toolsuite
   - Select: Microchip C18 Toolsuite
   - Set Toolsuite Contents:
     * Compiler: C:\MCC18\bin\mcc18.exe
     * Linker: C:\MCC18\bin\mplink.exe
     * Assembler: C:\MCC18\mpasmwin\mpasmwin.exe
   - Click OK
   ```

### Step 3: Install Programmer Software

1. **Install PICkit Driver**
   - Connect PICkit to USB
   - Windows will detect device
   - Install drivers from MPLAB IDE directory
   - Path: C:\Program Files\Microchip\MPLAB IDE\Drivers

2. **Test Programmer Connection**
   ```
   - Open MPLAB IDE
   - Go to: Programmer → Select Programmer → PICkit 3
   - Status should show: "PICkit 3 Connected"
   ```

---

## Hardware Assembly

### Component Checklist

Print this checklist and verify each component:

```
□ PIC18F6722 Microcontroller (DIP or TQFP package)
□ 10 MHz Crystal Oscillator
□ 2× 22pF Ceramic Capacitors (for crystal)
□ 16x2 LCD Display (HD44780 compatible)
□ 10kΩ Potentiometer (for LCD contrast)
□ 4x4 Matrix Keypad
□ LM35 Temperature Sensor (TO-92 package)
□ LDR (Light Dependent Resistor)
□ 10kΩ Resistor (for LDR voltage divider)
□ SG90 Servo Motor (or compatible)
□ 5V Buzzer (active type)
□ 2× LEDs (Red/Green)
□ 2× 220Ω Resistors (for LEDs)
□ Push Button (for reset)
□ 10kΩ Resistor (for pull-up)
□ 0.1μF Ceramic Capacitors (×5 for decoupling)
□ 5V Regulated Power Supply (2A minimum)
□ Breadboard or Custom PCB
□ Jumper Wires / Hookup Wire
□ MOSFET or Motor Driver IC (for heater/fan, if needed)
```

### Circuit Assembly

#### Step 1: Power Supply Section

1. **Connect Power Rails**
   ```
   Connect +5V rail to breadboard power rail
   Connect GND to breadboard ground rail
   ```

2. **Add Decoupling Capacitors**
   ```
   Place 0.1μF capacitor near PIC18F6722:
   - VDD (Pin 31) to VSS (Pin 32)
   - AVDD (Pin 28) to AVSS (Pin 27)
   - VDDCORE (Pin 42) to VSS (Pin 32)
   ```

#### Step 2: Microcontroller Setup

1. **Insert PIC18F6722**
   ```
   - Place PIC in center of breadboard (if DIP package)
   - Ensure Pin 1 identifier is at top-left
   - Press firmly but gently
   ```

2. **Connect Power Pins**
   ```
   Pin 31 (VDD)    → +5V
   Pin 32 (VSS)    → GND
   Pin 28 (AVDD)   → +5V
   Pin 27 (AVSS)   → GND
   Pin 42 (VDDCORE)→ +5V
   ```

3. **Connect Crystal Oscillator**
   ```
   10 MHz Crystal:
   - One leg → OSC1 (Pin 13)
   - Other leg → OSC2 (Pin 14)
   
   Load Capacitors (22pF each):
   - OSC1 → 22pF → GND
   - OSC2 → 22pF → GND
   ```

4. **Connect MCLR (Reset)**
   ```
   Pin 1 (MCLR) → 10kΩ resistor → +5V
   ```

#### Step 3: LCD Display Connection

**LCD Pin → PIC Pin Mapping:**

```
LCD Pin | Function | PIC Pin | Notes
--------|----------|---------|-------
1       | VSS      | GND     | Ground
2       | VDD      | +5V     | Power
3       | V0       | Wiper   | Contrast (from 10kΩ pot)
4       | RS       | RB4     | Register Select
5       | RW       | RB5     | Read/Write
6       | EN       | RB6     | Enable
7-10    | D0-D3    | NC      | Not connected (4-bit mode)
11      | D4       | RD4     | Data bit 4
12      | D5       | RD5     | Data bit 5
13      | D6       | RD6     | Data bit 6
14      | D7       | RD7     | Data bit 7
15      | A        | +5V     | Backlight Anode
16      | K        | GND     | Backlight Cathode
```

**Contrast Adjustment:**
```
10kΩ Potentiometer:
- Pin 1 → +5V
- Pin 2 (wiper) → LCD V0 (Pin 3)
- Pin 3 → GND
```

#### Step 4: Keypad Connection

**4x4 Keypad → PIC Pin Mapping:**

```
Keypad Pin | Function | PIC Pin
-----------|----------|--------
1          | Row 0    | RB4
2          | Row 1    | RB5
3          | Row 2    | RB6
4          | Row 3    | RB7
5          | Col 0    | RB0
6          | Col 1    | RB1
7          | Col 2    | RB2
8          | Col 3    | RB3
```

**Add Pull-up Resistors:**
```
Each Row Pin (RB4-RB7):
- Add 10kΩ resistor to +5V
```

#### Step 5: Temperature Sensor (LM35)

**LM35 Connections:**

```
LM35 Pin | Function | Connection
---------|----------|------------
1        | VCC      | +5V
2        | VOUT     | AN0 (RA0) - Pin 2
3        | GND      | GND
```

**Add Filtering:**
```
- 0.1μF capacitor: LM35 VOUT → GND (near sensor)
- 10kΩ resistor: LM35 VOUT → AN0 (current limiting)
```

#### Step 6: LDR Light Sensor

**Voltage Divider Circuit:**

```
+5V
 |
 |
[10kΩ Resistor]
 |
 +----→ AN1 (RA1) - Pin 3
 |
[LDR]
 |
GND
```

**Add Filtering:**
```
- 100nF capacitor: AN1 → GND (near PIC)
```

#### Step 7: Servo Motor

**Servo Connections:**

```
Servo Wire | Color  | Connection
-----------|--------|------------
Power      | Red    | +5V (separate supply recommended)
Ground     | Brown  | GND (common with PIC)
Signal     | Orange | RC2 (Pin 24)
```

**Important Notes:**
- Servo can draw 500mA+ under load
- Use separate 5V supply with shared ground
- Add 470μF capacitor across servo power

#### Step 8: Buzzer

**Buzzer Connection:**

```
Buzzer Positive → RC0 (Pin 23)
Buzzer Negative → GND
```

**Optional: Add transistor driver for louder volume**

```
RC0 → 1kΩ → NPN Transistor Base
Transistor Collector → Buzzer (+) → +5V
Transistor Emitter → GND
Buzzer (-) → GND
```

#### Step 9: Status LEDs

**LED Connections:**

```
LED 1 (Access Status):
- Anode (+) → 220Ω resistor → RC4 (Pin 26)
- Cathode (-) → GND

LED 2 (System Status):
- Anode (+) → 220Ω resistor → RG1 (Pin 46)
- Cathode (-) → GND
```

#### Step 10: Reset Button

**Push Button Connection:**

```
One side → RC3 (Pin 25)
Other side → GND
```

**Internal pull-up will be configured in software**

#### Step 11: PWM Outputs (Heater, Fan, Lamp)

**Direct Connection (for LEDs/small loads):**

```
RG0 (CCP3 - Lamp)   - Pin 45 → LED/Lamp driver
RG3 (CCP4 - Heater) - Pin 49 → Heater driver
RG4 (CCP5 - Fan)    - Pin 50 → Fan driver
```

**For High-Power Loads (using MOSFET):**

```
PIC PWM Pin → 1kΩ → MOSFET Gate (e.g., IRF520)
MOSFET Source → GND
MOSFET Drain → Load → +12V (or appropriate voltage)

Add:
- 10kΩ Gate-to-Source resistor (pull-down)
- Flyback diode across inductive loads
```

#### Step 12: Programmer Connection (ICSP)

**PICkit Pinout:**

```
PICkit Pin | Function | PIC Pin
-----------|----------|--------
1          | MCLR     | Pin 1
2          | VDD      | Pin 31
3          | VSS      | Pin 32
4          | PGD      | Pin 69 (RB7)
5          | PGC      | Pin 68 (RB6)
6          | NC       | -
```

**Programming Header (recommended):**
```
Install 6-pin header on board for easy programming access
```

### Assembly Verification Checklist

Before powering on:

```
□ All VDD pins connected to +5V
□ All VSS/GND pins connected to ground
□ No short circuits between power rails (check with multimeter)
□ Crystal and capacitors properly connected
□ MCLR has pull-up resistor
□ All decoupling capacitors in place
□ LCD contrast potentiometer installed
□ Keypad pull-ups present
□ Servo has separate power supply
□ All wire connections are secure
□ No loose components
```

---

## Firmware Installation

### Step 1: Prepare Firmware Files

1. **Extract Project Files**
   ```
   - Unzip OKAY_man.zip
   - Contents:
     * Source files (.c, .h)
     * Makefile
     * Project file (okay.mcp)
     * Pre-compiled HEX (okay.hex)
   ```

2. **Verify File Integrity**
   ```
   Files should include:
   - Untitled.c (main program)
   - security.c/h
   - temp_control.c/h
   - ldr_control.c/h
   - Timer2.c/h
   - timer0.c/h
   - LCD.c/h
   - KEYPAD.c/h
   - Makefile
   - okay.hex
   ```

### Step 2: Open Project in MPLAB

1. **Launch MPLAB IDE**

2. **Open Existing Project**
   ```
   File → Open Project
   Navigate to: OKAY_man folder
   Select: okay.mcp
   Click: Open
   ```

3. **Verify Target Device**
   ```
   Configure → Select Device
   Device: PIC18F6722
   Click: OK
   ```

4. **Set Configuration Bits**
   ```
   Already configured in code:
   #pragma config OSC = HS      // High-Speed oscillator
   #pragma config WDT = OFF     // Watchdog Timer disabled
   ```

### Step 3: Compile Firmware (Optional)

**If you want to compile from source:**

1. **Clean Previous Build**
   ```
   Project → Clean
   ```

2. **Build Project**
   ```
   Project → Build All
   
   Expected output:
   ------
   Building...
   Compiling Untitled.c
   Compiling LCD.c
   Compiling KEYPAD.c
   Compiling security.c
   Compiling temp_control.c
   Compiling ldr_control.c
   Compiling Timer2.c
   Compiling timer0.c
   Linking...
   BUILD SUCCEEDED
   ------
   ```

3. **Verify Output Files**
   ```
   Files generated:
   - okay.hex (main firmware file)
   - okay.cof (debug symbols)
   - okay.map (memory map)
   ```

### Step 4: Connect Programmer

1. **Connect PICkit to Computer**
   - Plug PICkit into USB port
   - Wait for driver installation
   - Green LED should light on PICkit

2. **Connect PICkit to Target**
   - Connect 6-pin ICSP cable
   - Match Pin 1 (arrow) on cable to Pin 1 on target
   - Ensure all connections are secure

3. **Power Target Circuit**
   - Option 1: Power from PICkit (if load < 100mA)
   - Option 2: External 5V supply (recommended)

4. **Select Programmer in MPLAB**
   ```
   Programmer → Select Programmer → PICkit 3
   
   Output window should show:
   "Connecting to PICkit 3..."
   "PICkit 3 Connected"
   "Target Device: PIC18F6722"
   "Target Vdd: 5.0V"
   ```

### Step 5: Program the Device

1. **Import HEX File**
   ```
   File → Import → okay.hex
   ```

2. **Program Device**
   ```
   Programmer → Program
   
   Programming sequence:
   1. Erase chip
   2. Program Flash memory
   3. Verify programming
   4. Program configuration bits
   5. Verify configuration
   
   Time: ~30 seconds
   
   Success message:
   "Programming/Verify complete"
   ```

3. **Verify Programming**
   ```
   Programmer → Verify
   
   Should display:
   "Device ID: 0x2C20"
   "Verify passed"
   ```

### Alternative: Command-Line Programming

**Using pk2cmd (PICkit 2 Command Line):**

```bash
pk2cmd -P PIC18F6722 -F okay.hex -M -R
```

**Using IPE (Integrated Programming Environment):**

```
1. Launch IPE
2. Select Device: PIC18F6722
3. Select Tool: PICkit 3
4. Browse to okay.hex
5. Click "Program"
```

---

## Initial Configuration

### Step 1: First Power-On

1. **Apply Power**
   ```
   - Connect 5V supply
   - Check current draw: ~50-100mA (idle)
   - System LED (RG1) should start blinking
   ```

2. **LCD Check**
   ```
   - Display should show: "Enter Password"
   - Adjust contrast pot if display is blank or too dark
   - Backlight should be ON
   ```

3. **Test Keypad**
   ```
   - Press any key
   - Should hear a click (if keypad has tactile switches)
   - No visible change yet (waiting for password)
   ```

### Step 2: Initial Access

1. **Enter Default Password**
   ```
   Default password: 1-2-3-4
   
   Press keys: 1, 2, 3, 4
   Display should show: ****
   ```

2. **Verify Access Granted**
   ```
   After entering 1234:
   - LCD: "Access Granted"
   - LCD: "Welcome"
   - Status LED (RC4): ON
   - Servo: Should move to 90° (open)
   - Wait ~1 second
   - Servo: Should return to 0° (close)
   - Status LED: OFF
   ```

3. **Observe Main Operation**
   ```
   After door closes:
   - LCD Line 1: "Temperature: XX C"
   - LCD Line 2: "LDR: XX%"
   - Display updates every 2 seconds
   ```

### Step 3: Change Default Password (Optional)

**To change password, edit security.c:**

1. **Open security.c**

2. **Locate line 20:**
   ```c
   static const char correctPassword[PASSWORD_LENGTH + 1] = "1234";
   ```

3. **Change to desired password:**
   ```c
   static const char correctPassword[PASSWORD_LENGTH + 1] = "5678";
   ```

4. **Recompile and reprogram**

### Step 4: Calibrate Sensors

#### Temperature Calibration

1. **Room Temperature Test**
   ```
   - Place LM35 at room temperature
   - Compare LCD reading with reference thermometer
   - Should be within ±2°C
   ```

2. **If calibration needed:**
   ```c
   // Edit temp_control.c, line 22:
   unsigned long temp = ((unsigned long)adc_value * 500) / 1023;
   
   // Adjust multiplier (500) if needed:
   // If reading too high: decrease to 490
   // If reading too low: increase to 510
   ```

#### Light Sensor Calibration

1. **Dark Room Test**
   ```
   - Cover LDR completely
   - LCD should show: "LDR: 0-5%"
   - Lamp should be at full brightness
   ```

2. **Bright Light Test**
   ```
   - Shine bright light on LDR
   - LCD should show: "LDR: 95-100%"
   - Lamp should be off or very dim
   ```

3. **If calibration needed:**
   ```c
   // Edit ldr_control.c, adjust range values
   ```

---

## Testing & Verification

### Comprehensive Test Sequence

#### Test 1: Power-On Self Test

```
✓ LCD displays startup message
✓ System LED blinks at 200ms intervals
✓ No smoke, unusual smells, or excessive heat
✓ Current draw is reasonable (~50-100mA idle)
```

#### Test 2: Security System Test

**Test Case 2.1: Correct Password**
```
Action: Enter "1234"
Expected:
  ✓ Display "Access Granted"
  ✓ Status LED turns ON
  ✓ Servo moves to 90°
  ✓ Wait 1 second
  ✓ Servo returns to 0°
  ✓ Status LED turns OFF
  ✓ Enter main operation mode
```

**Test Case 2.2: Wrong Password (1 attempt)**
```
Action: Enter "9999"
Expected:
  ✓ Display "Access Denied"
  ✓ Return to password entry
  ✓ No alarm
```

**Test Case 2.3: Multiple Wrong Attempts**
```
Action: Enter wrong password 3 times
Expected:
  ✓ After 3rd wrong entry:
  ✓ Display "SYSTEM LOCKED"
  ✓ Display "ALARM ON"
  ✓ Buzzer sounds continuously
  ✓ Display countdown: "Wait 5 sec", "Wait 4 sec", etc.
  ✓ After countdown, buzzer stops
  ✓ Return to password entry
```

#### Test 3: Temperature Control Test

**Test Case 3.1: Cold Environment**
```
Action: Cool LM35 to <20°C (use ice pack)
Expected:
  ✓ LCD shows temperature <20°C
  ✓ Heater output (RG3) active (measure with multimeter)
  ✓ Fan output (RG4) inactive
  ✓ PWM duty cycle high on heater
```

**Test Case 3.2: Hot Environment**
```
Action: Heat LM35 to >40°C (use heat gun or warm water)
Expected:
  ✓ LCD shows temperature >40°C
  ✓ Heater output (RG3) inactive
  ✓ Fan output (RG4) active
  ✓ PWM duty cycle high on fan
```

**Test Case 3.3: Comfortable Range**
```
Action: Keep LM35 at 30-35°C (room temperature)
Expected:
  ✓ LCD shows temperature 30-35°C
  ✓ Both heater and fan inactive
  ✓ No PWM activity on RG3 or RG4
```

#### Test 4: Light Control Test

**Test Case 4.1: Dark Environment**
```
Action: Cover LDR completely
Expected:
  ✓ LCD shows "LDR: 0-10%"
  ✓ Lamp output (RG0) at high PWM
  ✓ Lamp is brightest
```

**Test Case 4.2: Bright Environment**
```
Action: Shine bright light on LDR
Expected:
  ✓ LCD shows "LDR: 90-100%"
  ✓ Lamp output (RG0) at low/zero PWM
  ✓ Lamp is dimmest or OFF
```

**Test Case 4.3: Gradual Change**
```
Action: Slowly cover/uncover LDR
Expected:
  ✓ Lamp brightness changes smoothly
  ✓ Inverse relationship: darker room = brighter lamp
```

#### Test 5: Reset Function Test

**Test Case 5.1: Reset Button**
```
Action: Press reset button (RC3)
Expected:
  ✓ All outputs turn OFF
  ✓ Display "System Reset"
  ✓ System LED solid ON for 1 second
  ✓ Return to password entry
  ✓ Main operation resumes after correct password
```

#### Test 6: Long-Term Stability Test

```
Action: Run system for 24 hours
Monitor:
  ✓ Temperature readings stable
  ✓ Light readings stable
  ✓ No system crashes
  ✓ LCD display remains correct
  ✓ PWM outputs remain functional
  ✓ No excessive heating of components
```

### Measurement Points

**Key voltage measurements:**

```
Point           | Expected | Tolerance
----------------|----------|----------
VDD (Pin 31)    | 5.0V     | ±0.25V
AN0 (LM35 out)  | 0.3-1.2V | -
AN1 (LDR out)   | 0.5-4.5V | -
RC2 (Servo)     | 0.5-5V   | PWM signal
RG0,3,4 (PWM)   | 0-5V     | PWM signal
```

**Key signal checks with oscilloscope:**

```
Signal | Frequency | Duty Cycle Range
-------|-----------|------------------
CCP3   | 5 kHz     | 0-100%
CCP4   | 5 kHz     | 0-100%
CCP5   | 5 kHz     | 0-100%
Servo  | 50 Hz     | 0.5-1.0ms pulse
```

---

## Troubleshooting

### Issue 1: LCD Display Problems

**Symptom: Blank LCD**
```
Possible Causes:
1. No power to LCD
2. Wrong contrast setting
3. Incorrect wiring

Solutions:
□ Check VDD and GND connections
□ Adjust contrast potentiometer
□ Verify LCD enable (EN) signal with scope
□ Test LCD with known-good circuit
```

**Symptom: Garbage Characters**
```
Possible Causes:
1. Timing issue
2. Incorrect initialization
3. Loose connections

Solutions:
□ Add delays in LCD_Init()
□ Check all data lines (RD4-RD7)
□ Verify RS, RW, EN signals
□ Reduce clock speed temporarily to test
```

### Issue 2: Keypad Not Responding

**Symptom: No response to key presses**
```
Possible Causes:
1. Missing pull-up resistors
2. Wrong pin configuration
3. Keypad defective

Solutions:
□ Verify 10kΩ pull-ups on rows (RB4-RB7)
□ Check column output signals with scope
□ Test keypad with multimeter (continuity)
□ Swap with known-good keypad
```

**Symptom: Random key detections**
```
Possible Causes:
1. Insufficient debounce time
2. Electrical noise

Solutions:
□ Increase debounce delay in KEYPAD.c
□ Add 100nF capacitors across keypad pins
□ Check for loose connections
```

### Issue 3: Servo Not Moving

**Symptom: Servo doesn't respond**
```
Possible Causes:
1. Insufficient power
2. Wrong signal
3. Defective servo

Solutions:
□ Provide separate 5V power supply for servo
□ Check signal on RC2 with oscilloscope
□ Verify 50Hz, 0.5-1.0ms pulses
□ Test servo with Arduino or servo tester
□ Check servo power supply current capability (>1A)
```

**Symptom: Servo jittery or noisy**
```
Solutions:
□ Add 470μF capacitor across servo power
□ Use separate power supply for servo
□ Shorten servo signal wire
□ Add series resistor (100Ω) on signal line
```

### Issue 4: Temperature Reading Errors

**Symptom: Incorrect temperature reading**
```
Solutions:
□ Verify LM35 pinout (common mistake: reversed)
□ Check LM35 power supply (must be 5V)
□ Add 0.1μF bypass capacitor near sensor
□ Compare with calibrated thermometer
□ Adjust calibration factor in code
```

**Symptom: Temperature reading jumps around**
```
Solutions:
□ Add 100nF filter capacitor on AN0
□ Keep sensor wires short (<6 inches)
□ Route sensor wires away from PWM lines
□ Add 10kΩ series resistor to AN0
```

### Issue 5: LDR Not Working

**Symptom: LDR reading stuck**
```
Solutions:
□ Check voltage divider circuit
□ Verify 10kΩ resistor value
□ Test LDR resistance with multimeter:
  - Dark: >100kΩ
  - Light: <10kΩ
□ Ensure AN1 is configured as analog
```

### Issue 6: Programming Errors

**Error: "Device ID mismatch"**
```
Solutions:
□ Verify PIC18F6722 is installed (not wrong chip)
□ Check ICSP connections
□ Try slower programming speed
□ Replace PICkit if damaged
```

**Error: "Target device not found"**
```
Solutions:
□ Check target power (must be 4.5-5.5V)
□ Verify MCLR connection and pull-up
□ Check PGC and PGD connections
□ Ensure no shorts on power rails
```

### Issue 7: System Crashes or Resets

**Symptom: Random resets**
```
Possible Causes:
1. Power supply issues
2. Brown-out
3. Watchdog timer

Solutions:
□ Use regulated 5V supply (>500mA)
□ Add 100μF bulk capacitor near PIC
□ Check all ground connections
□ Verify WDT is disabled in config bits
□ Reduce load on power supply
```

**Symptom: Program freezes**
```
Possible Causes:
1. Infinite loop
2. Stack overflow
3. Interrupt issue

Solutions:
□ Enable watchdog timer for testing
□ Check for blocking code (long delays)
□ Verify interrupt vectors
□ Review ISR code for correctness
```

### Issue 8: PWM Outputs Not Working

**Symptom: No PWM signal**
```
Solutions:
□ Verify Timer2 is running (T2CON = 0x05)
□ Check PR2 value (should be 124)
□ Verify CCPxCON register (should be 0x0C)
□ Measure with oscilloscope (should be 5kHz)
□ Check pin configuration (TRIS = 0 for output)
```

**Symptom: Wrong frequency**
```
Solutions:
□ Verify oscillator frequency (10MHz)
□ Check Timer2 prescaler setting
□ Recalculate PR2 value if needed
```

### Quick Diagnostics

**Use this LED-based diagnostic code:**

```c
// Add to main() for debugging
void diagnostic_blink(unsigned char count) {
    while(count--) {
        LATGbits.LATG1 = 1;
        delay_ms(200);
        LATGbits.LATG1 = 0;
        delay_ms(200);
    }
}

// Example usage:
diagnostic_blink(3);  // Blink 3 times = reached this point
```

---

## Additional Resources

### Online Resources
- Microchip PIC18F6722 Datasheet: https://www.microchip.com/PIC18F6722
- MPLAB IDE Documentation: https://www.microchip.com/mplab
- MCC18 Compiler User Guide: Included with installation

### Community Support
- Microchip Forums: https://www.microchip.com/forums
- Stack Overflow: Tag [pic] [mplab]
- Reddit: r/embedded, r/AskElectronics

### Recommended Learning
- Embedded C Programming (book)
- PIC Microcontroller Projects in C (book)
- Microchip University online courses

---

## Safety Precautions

⚠️ **Always follow these safety guidelines:**

1. **Power off before connecting/disconnecting**
2. **Never touch components while powered**
3. **Use ESD protection (wrist strap)**
4. **Double-check polarity before powering**
5. **Start with low power (use current-limited supply)**
6. **Keep workspace clean and organized**
7. **Have fire extinguisher nearby when testing**
8. **Ensure proper ventilation when soldering**

---

## Warranty and Support

This project is provided as-is for educational purposes.

For technical support:
- Review this documentation
- Check troubleshooting section
- Search online forums
- Contact [basant.saadeldeen@gmail.com]

---

**End of Installation Guide**
