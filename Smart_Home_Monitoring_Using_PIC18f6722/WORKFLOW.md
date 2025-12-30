# OKAY_MAN System Workflow & Brief Overview

## Executive Summary

The OKAY_MAN system is an intelligent embedded security and environmental control system built on the PIC18F6722 microcontroller. It integrates password-based access control with automated temperature and lighting management, creating a comprehensive smart home security solution.

---

## System Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│                    OKAY_MAN SYSTEM                          │
│                  PIC18F6722 @ 10MHz                         │
└─────────────────────────────────────────────────────────────┘
                             │
                ┌────────────┼────────────┐
                │            │            │
        ┌───────▼───────┐   │    ┌───────▼────────┐
        │   SECURITY    │   │    │  ENVIRONMENTAL │
        │   SUBSYSTEM   │   │    │   CONTROL      │
        └───────────────┘   │    └────────────────┘
                            │
                    ┌───────▼────────┐
                    │  USER INTERFACE│
                    │   & DISPLAY    │
                    └────────────────┘
```

---

## Main System Workflow

### High-Level Process Flow

```
                    ┌──────────────┐
                    │  POWER ON    │
                    └──────┬───────┘
                           │
                    ┌──────▼───────┐
                    │ Initialize   │
                    │ Hardware     │
                    │ - LCD        │
                    │ - Security   │
                    │ - Timer0     │
                    └──────┬───────┘
                           │
              ┌────────────▼─────────────┐
              │  SECURITY AUTHENTICATION │
              │  "Enter Password"        │
              └────────────┬─────────────┘
                           │
                    ┌──────▼───────┐
                    │ Read Keypad  │
                    │ (4 digits)   │
                    └──────┬───────┘
                           │
                    ┌──────▼───────┐
                    │  Validate    │
                    │  Password    │
                    └──────┬───────┘
                           │
                  ┌────────┴────────┐
                  │                 │
            ┌─────▼─────┐     ┌────▼──────┐
            │  CORRECT  │     │   WRONG   │
            └─────┬─────┘     └────┬──────┘
                  │                 │
            ┌─────▼─────┐     ┌────▼──────┐
            │Access     │     │Increment  │
            │Granted    │     │Attempts   │
            │LED ON     │     │Counter    │
            └─────┬─────┘     └────┬──────┘
                  │                 │
            ┌─────▼─────┐     ┌────▼──────┐
            │Open Door  │     │Attempts   │
            │(Servo 90°)│     │>= 3?      │
            └─────┬─────┘     └────┬──────┘
                  │                 │
            ┌─────▼─────┐          │
            │Wait 1 sec │     ┌────▼──────┐
            └─────┬─────┘     │YES: Lock  │
                  │           │System     │
            ┌─────▼─────┐     │Sound Alarm│
            │Close Door │     │5 sec      │
            │(Servo 0°) │     └────┬──────┘
            │LED OFF    │          │
            └─────┬─────┘     ┌────▼──────┐
                  │           │Reset      │
                  │           │Attempts   │
                  │           │Retry      │
                  │           └───────────┘
                  │
            ┌─────▼─────────────────────┐
            │   MAIN OPERATION LOOP     │
            │                           │
            │ ┌─────────────────────┐  │
            │ │ Check Reset Button  │  │
            │ │ (Timer0 ISR)        │  │
            │ └──────────┬──────────┘  │
            │            │              │
            │ ┌──────────▼──────────┐  │
            │ │ Read Temperature    │  │
            │ │ Sensor (AN0)        │  │
            │ └──────────┬──────────┘  │
            │            │              │
            │ ┌──────────▼──────────┐  │
            │ │ Control Temperature │  │
            │ │ - Heater PWM        │  │
            │ │ - Fan PWM           │  │
            │ └──────────┬──────────┘  │
            │            │              │
            │ ┌──────────▼──────────┐  │
            │ │ Read LDR            │  │
            │ │ Sensor (AN1)        │  │
            │ └──────────┬──────────┘  │
            │            │              │
            │ ┌──────────▼──────────┐  │
            │ │ Adjust Lamp         │  │
            │ │ Brightness (PWM)    │  │
            │ └──────────┬──────────┘  │
            │            │              │
            │ ┌──────────▼──────────┐  │
            │ │ Update LCD Display  │  │
            │ │ - Temperature       │  │
            │ │ - Light Level       │  │
            │ └──────────┬──────────┘  │
            │            │              │
            │ ┌──────────▼──────────┐  │
            │ │ Delay 2 seconds     │  │
            │ └──────────┬──────────┘  │
            │            │              │
            │            │              │
            └────────────┼──────────────┘
                         │
                    Loop Back ▲
```

---

## Subsystem Workflows

### 1. Security Authentication Flow

```
┌───────────────────────────────────────────────────┐
│          PASSWORD AUTHENTICATION                  │
└───────────────────────────────────────────────────┘

START
  │
  ├─→ Display "Enter Password"
  │
  ├─→ Initialize: index = 0, attempts = 0
  │
  └─→ KEYPAD INPUT LOOP
      │
      ├─→ Read key from keypad
      │
      ├─→ Is key 0-9?
      │   │
      │   YES ──→ Store in buffer
      │   │       Display '*'
      │   │       Increment index
      │   │
      │   NO ───→ Ignore key
      │
      ├─→ Index < 4? ──YES─→ Loop back
      │   │
      │   NO
      │
      └─→ VALIDATION
          │
          ├─→ Compare buffer with "1234"
          │
          ├─→ Match?
          │   │
          │   YES ──→ ACCESS GRANTED
          │   │       │
          │   │       ├─→ Display "Access Granted"
          │   │       ├─→ Turn ON Status LED
          │   │       ├─→ Open Door (Servo 90°)
          │   │       ├─→ Wait 1000ms
          │   │       ├─→ Close Door (Servo 0°)
          │   │       ├─→ Turn OFF Status LED
          │   │       └─→ PROCEED TO MAIN LOOP
          │   │
          │   NO ───→ ACCESS DENIED
          │           │
          │           ├─→ Display "Access Denied"
          │           ├─→ Increment attempts
          │           ├─→ Wait 1500ms
          │           │
          │           ├─→ Attempts >= 3?
          │           │   │
          │           │   YES ──→ LOCKOUT
          │           │   │       │
          │           │   │       ├─→ Display "SYSTEM LOCKED"
          │           │   │       ├─→ Display "ALARM ON"
          │           │   │       ├─→ Turn ON Buzzer
          │           │   │       ├─→ Countdown 5 to 1
          │           │   │       │   (Display each second)
          │           │   │       ├─→ Turn OFF Buzzer
          │           │   │       ├─→ Reset attempts = 0
          │           │   │       └─→ Loop back
          │           │   │
          │           │   NO ───→ Loop back
          │           │
          │           └─→ START (Retry)
```

### 2. Temperature Control Flow

```
┌───────────────────────────────────────────────────┐
│        TEMPERATURE CONTROL LOGIC                  │
└───────────────────────────────────────────────────┘

Read ADC from AN0 (LM35)
  │
  ├─→ ADC Value = 0-1023
  │
  └─→ Determine Temperature Zone
      │
      ├─→ ADC < 41 (< 20°C) ──→ VERY COLD
      │   │
      │   └─→ Heater = 100% PWM (499)
      │       Fan = 0% PWM (0)
      │
      ├─→ ADC 41-50 (20-25°C) ──→ COLD
      │   │
      │   └─→ Heater = 100% → 50% (gradual decrease)
      │       Fan = 0%
      │
      ├─→ ADC 51-60 (25-30°C) ──→ APPROACHING COMFORT
      │   │
      │   └─→ Heater = 50% → 0% (gradual decrease)
      │       Fan = 0%
      │
      ├─→ ADC 61-72 (30-35°C) ──→ COMFORTABLE
      │   │
      │   └─→ Heater = 0%
      │       Fan = 0%
      │       [Optimal zone - no action needed]
      │
      ├─→ ADC 73-82 (35-40°C) ──→ GETTING WARM
      │   │
      │   └─→ Heater = 0%
      │       Fan = 0% → 50% (gradual increase)
      │
      ├─→ ADC 83-101 (40-50°C) ──→ WARM
      │   │
      │   └─→ Heater = 0%
      │       Fan = 50% → 100% (gradual increase)
      │
      └─→ ADC > 101 (> 50°C) ──→ HOT
          │
          └─→ Heater = 0%
              Fan = 100% PWM (499)

Apply PWM to CCP4 (Heater) and CCP5 (Fan)
  │
  └─→ PWM Frequency: 5 kHz
      Resolution: 10-bit (0-499)
```

### 3. Light Control Flow

```
┌───────────────────────────────────────────────────┐
│         AUTOMATIC LIGHTING CONTROL                │
└───────────────────────────────────────────────────┘

Read ADC from AN1 (LDR)
  │
  ├─→ ADC Value = 11-989
  │
  └─→ Calculate Lamp Brightness (Inverse)
      │
      ├─→ Formula: duty = (989 - LDR) × 499 / (989 - 11)
      │
      └─→ Examples:
          │
          ├─→ LDR = 11 (Dark)
          │   └─→ duty = (989-11) × 499/978 = 499 (100%)
          │       Result: LAMP FULL BRIGHT
          │
          ├─→ LDR = 500 (Medium Light)
          │   └─→ duty = (989-500) × 499/978 = 249 (50%)
          │       Result: LAMP MEDIUM
          │
          └─→ LDR = 989 (Bright)
              └─→ duty = (989-989) × 499/978 = 0 (0%)
                  Result: LAMP OFF

Apply PWM to CCP3 (Lamp)
  │
  └─→ PWM Frequency: 5 kHz
      Resolution: 10-bit (0-499)

RELATIONSHIP:
  Darker Room → Higher LDR Resistance → Lower ADC
                                       → Higher PWM
                                       → Brighter Lamp

  Brighter Room → Lower LDR Resistance → Higher ADC
                                        → Lower PWM
                                        → Dimmer Lamp
```

### 4. System Reset Flow

```
┌───────────────────────────────────────────────────┐
│           SYSTEM RESET WORKFLOW                   │
└───────────────────────────────────────────────────┘

Timer0 Interrupt (every ~10ms)
  │
  ├─→ Check Reset Button (RC3)
  │   │
  │   └─→ Button Pressed (RC3 = LOW)?
  │       │
  │       YES ──→ Set reset_flag = 1
  │       │
  │       NO ───→ Continue
  │
  └─→ Toggle Status LED (every 200ms)

Main Loop checks reset_flag
  │
  └─→ reset_flag = 1?
      │
      YES ──→ SYSTEM RESET SEQUENCE
      │       │
      │       ├─→ Disable Timer0 interrupt
      │       │
      │       ├─→ Turn OFF all outputs:
      │       │   • Lamp → 989 (full brightness)
      │       │   • Heater → 0%
      │       │   • Fan → 0%
      │       │
      │       ├─→ Display "System Reset"
      │       │
      │       ├─→ Wait 1000ms
      │       │
      │       ├─→ Reinitialize:
      │       │   • LCD_Init()
      │       │   • Security_Init()
      │       │   • Security_Run() [Wait for password]
      │       │   • Three_PWM_Init()
      │       │   • ADC_Init()
      │       │
      │       ├─→ Status LED ON for 1 second
      │       │
      │       ├─→ Status LED OFF
      │       │
      │       ├─→ Re-enable Timer0 interrupt
      │       │
      │       └─→ Clear reset_flag = 0
      │
      NO ───→ Continue normal operation
```

---

## Data Flow Diagram

```
                        SENSORS
                           │
          ┌────────────────┼────────────────┐
          │                │                │
    ┌─────▼─────┐    ┌────▼────┐    ┌─────▼─────┐
    │   LM35    │    │   LDR   │    │  KEYPAD   │
    │(Temp Sensor)│  │(Light)  │    │  (Input)  │
    └─────┬─────┘    └────┬────┘    └─────┬─────┘
          │               │                │
          │ Analog        │ Analog         │ Digital
          │ (0-1.2V)      │ (0-5V)         │ (Scan)
          │               │                │
    ┌─────▼─────┐    ┌────▼────┐    ┌─────▼─────┐
    │    AN0    │    │   AN1   │    │  PORTB    │
    │  (ADC)    │    │  (ADC)  │    │  (GPIO)   │
    └─────┬─────┘    └────┬────┘    └─────┬─────┘
          │               │                │
          │               │                │
          └───────────────┼────────────────┘
                          │
                  ┌───────▼───────┐
                  │  PIC18F6722   │
                  │  PROCESSOR    │
                  │               │
                  │ • Control     │
                  │ • Calculate   │
                  │ • Decide      │
                  └───────┬───────┘
                          │
          ┌───────────────┼───────────────┐
          │               │               │
    ┌─────▼─────┐    ┌────▼────┐    ┌────▼─────┐
    │   PWM     │    │  LCD    │    │  SERVO   │
    │ OUTPUTS   │    │ DISPLAY │    │  MOTOR   │
    └─────┬─────┘    └────┬────┘    └────┬─────┘
          │               │               │
    ┌─────┼─────┐         │               │
    │     │     │         │               │
┌───▼───┐ │ ┌───▼───┐    │           ┌───▼───┐
│ Lamp  │ │ │ Fan   │    │           │ Door  │
│(CCP3) │ │ │(CCP5) │    │           │ Lock  │
└───────┘ │ └───────┘    │           └───────┘
          │              │
      ┌───▼────┐    ┌────▼─────┐
      │ Heater │    │ Temperature│
      │ (CCP4) │    │ Light Level│
      └────────┘    │ Status    │
                    └───────────┘
```

---

## State Diagram

```
┌─────────────────────────────────────────────────────────┐
│              OKAY_MAN SYSTEM STATES                     │
└─────────────────────────────────────────────────────────┘

                    ┌──────────────┐
                    │  POWER_OFF   │
                    └──────┬───────┘
                           │ Power Applied
                           │
                    ┌──────▼───────┐
                    │ INITIALIZING │
                    │              │
                    │ • Setup GPIO │
                    │ • Setup ADC  │
                    │ • Setup PWM  │
                    │ • Setup LCD  │
                    └──────┬───────┘
                           │
                    ┌──────▼───────┐
             ┌──────┤   LOCKED     │
             │      │              │
             │      │ Waiting for  │
             │      │ Password     │
             │      └──────┬───────┘
             │             │
             │             │ Correct Password
             │             │
             │      ┌──────▼───────┐
             │      │ AUTHENTICATING│
             │      │              │
             │      │ Opening Door │
             │      └──────┬───────┘
             │             │
             │             │ Door Sequence Complete
             │             │
             │      ┌──────▼───────┐
      Reset │      │  OPERATING   │
      Button│◄─────┤              │
             │      │ • Monitor    │
             │      │ • Control    │
             │      │ • Display    │
             │      └──────┬───────┘
             │             │
             │             │ Reset Button Pressed
             │             │
             │      ┌──────▼───────┐
             └─────►│  RESETTING   │
                    │              │
                    │ • Stop PWM   │
                    │ • Clear LCD  │
                    │ • Reinit     │
                    └──────┬───────┘
                           │
                           └─→ Back to LOCKED

STATE TRANSITIONS:

POWER_OFF → INITIALIZING
  Trigger: Power applied
  Duration: ~100ms

INITIALIZING → LOCKED
  Trigger: Init complete
  Duration: Immediate

LOCKED → AUTHENTICATING
  Trigger: Correct password entered
  Duration: ~2 seconds (door operation)

AUTHENTICATING → OPERATING
  Trigger: Door sequence complete
  Duration: Continuous

OPERATING → RESETTING
  Trigger: Reset button pressed (RC3)
  Duration: ~2 seconds

RESETTING → LOCKED
  Trigger: Reinitialization complete
  Duration: Immediate

OPERATING → OPERATING (steady state)
  Continuous monitoring and control
  2-second update cycle
```

---

## Timing Diagram

```
TIME →

ISR (Timer0) ────┬───────┬───────┬───────┬───────┬───────┬──
Every 10ms       │       │       │       │       │       │
                 └──┐    └──┐    └──┐    └──┐    └──┐    └──┐
                    │       │       │       │       │       │
System LED ─────────┴───────────────┴───────────────┴───────────
(RG1 Blink)    ON  OFF         ON  OFF         ON  OFF
                <--------200ms------><--------200ms-------->

PWM Signals ────┐  ┐  ┐  ┐  ┐  ┐  ┐  ┐  ┐  ┐  ┐  ┐  ┐  ┐  ┐
(5 kHz)         └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──
CCP3/4/5        <--200μs--><--200μs--><--200μs--><--200μs-->

Servo Pulse ────┐        ┌────────────┐        ┌────────────┐
(50 Hz)         └────────┘            └────────┘            └──
                <--20ms--><---20ms---><---20ms---><---20ms--->
                  ^0.5-1ms

Main Loop ──────────────────────────┬─────────────────────────
(2 sec cycle)                       │ Read Sensors
                                    │ Control Outputs
                                    │ Update Display
                                    └─ Delay 2 sec

ADC Conv. ───────────┬─────────┬─────────┬─────────┬──────────
(~5ms each)          └──┐      └──┐      └──┐      └──┐
                        │ AN0     │ AN1     │ AN0     │ AN1
                        └─────────┘         └─────────┘

LCD Update ──────────────────────────────┬────────────────────
(Every 2 sec)                            │ Temperature: XX C
                                         │ LDR: XX%
                                         └───────────────────
```

---

## Resource Usage

### Processor Utilization

```
┌────────────────────────────────────┐
│    CPU Time Distribution           │
├────────────────────────────────────┤
│ Main Loop (Active)      1.5%      ██
│ Timer0 ISR              0.5%      █
│ Idle Time              98.0%      ████████████████████████
└────────────────────────────────────┘
```

### Memory Allocation

```
┌────────────────────────────────────┐
│    Flash Memory (128 KB)           │
├────────────────────────────────────┤
│ Program Code          ~99 KB       ████████████████████
│ Free                  ~29 KB       ██████
└────────────────────────────────────┘

┌────────────────────────────────────┐
│    RAM (3,936 bytes)               │
├────────────────────────────────────┤
│ Variables             ~42 bytes    █
│ Stack                 ~100 bytes   ██
│ Free                  ~3794 bytes  ███████████████████████
└────────────────────────────────────┘
```

---

## Key System Parameters

### Performance Metrics

| Parameter | Value | Unit |
|-----------|-------|------|
| **System Response Time** |
| Password Entry | <500 | ms |
| Door Operation | 1000 | ms |
| Temperature Update | 2000 | ms |
| Light Level Update | 2000 | ms |
| Reset Response | <10 | ms |
| **Control Loop** |
| Main Loop Period | 2 | seconds |
| PWM Update Rate | Continuous | - |
| LCD Refresh Rate | 0.5 | Hz |
| **Timing** |
| Timer0 Overflow | 10 | ms |
| PWM Frequency | 5 | kHz |
| Servo Pulse Period | 20 | ms |
| ADC Conversion | 5 | ms |

### Operating Ranges

| Parameter | Min | Typical | Max | Unit |
|-----------|-----|---------|-----|------|
| Supply Voltage | 4.5 | 5.0 | 5.5 | V |
| Operating Current | 50 | 150 | 500 | mA |
| Temperature Range | 0 | 25 | 50 | °C |
| Light Level | 0 | 50 | 100 | % |

---

## Project Summary

### What This System Does

1. **Secures Entry**
   - Password-protected access
   - Automatic door lock control
   - Intrusion alarm (after 3 failed attempts)

2. **Maintains Comfort**
   - Automatic temperature regulation
   - Heater control when cold
   - Fan control when hot

3. **Manages Lighting**
   - Automatic brightness adjustment
   - Responds to ambient light
   - Energy-efficient operation

4. **Provides Feedback**
   - Real-time temperature display
   - Light level indication
   - System status indicators

### Key Technologies

- **Microcontroller**: PIC18F6722 (8-bit, 10 MHz)
- **Programming**: Embedded C (MCC18)
- **Interfaces**: ADC, PWM, GPIO, LCD
- **Sensors**: LM35, LDR, 4x4 Keypad
- **Actuators**: Servo, Buzzer, PWM-controlled devices

### Development Tools

- MPLAB IDE v8.x
- MCC18 C Compiler
- PICkit 3/4 Programmer
- Proteus (simulation)

---

## Quick Start Guide

### For End Users

1. **Power On** → System LED blinks
2. **Enter Password** → Type: 1-2-3-4
3. **Wait for Access** → Door opens automatically
4. **System Active** → Temperature and lighting controlled automatically
5. **Reset if Needed** → Press reset button to restart

### For Developers

1. **Setup IDE** → Install MPLAB + MCC18
2. **Open Project** → Load okay.mcp
3. **Connect Programmer** → PICkit to ICSP
4. **Program Device** → Upload okay.hex
5. **Test & Debug** → Verify all functions
6. **Customize** → Modify thresholds as needed

---

## Conclusion

The OKAY_MAN system demonstrates a complete embedded solution that integrates multiple subsystems:

- ✓ Security with password authentication
- ✓ Environmental sensing and control
- ✓ User interface and feedback
- ✓ Interrupt-driven design
- ✓ PWM-based output control
- ✓ Modular software architecture

This makes it an excellent reference design for smart home applications and embedded systems education.

---

**For detailed information, refer to:**
- `README.md` - Complete documentation
- `REFERENCE.md` - Technical specifications
- `INSTALLATION.md` - Setup instructions

**End of Workflow & Overview**
