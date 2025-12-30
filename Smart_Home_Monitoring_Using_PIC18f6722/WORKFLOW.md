# Automation Home System Workflow & Brief Overview

## Executive Summary

The Automation Home system is an intelligent embedded security and environmental control system built on the PIC18F6722 microcontroller. It integrates password-based access control with automated temperature and lighting management, creating a comprehensive smart home security solution.
---

## System Architecture Overview

![System Diagram](Diagram_images_PNG/System%20Architecture%20Overview.png)

---

## Main System Workflow

### High-Level Process Flow

![System Diagram](Diagram_images_PNG/System%20Architecture%20Overview.png)

---
## Subsystem Workflows

### 1. Security Authentication Flow
![System Diagram](Diagram_images_PNG/Security%20Authentication%20Flow.png)

---
### 2. Temperature Control Flow

![System Diagram](Diagram_images_PNG/Temperature%20Control%20Flow.png)

---
### 3. Light Control Flow
![System Diagram](Smart_Home_Monitoring_Using_PIC18f6722/Diagram_images_PNG/Light%20Control%20Flow.png)
 
 ---
### 4. System Reset Flow
![System Diagram](Smart_Home_Monitoring_Using_PIC18f6722/Diagram_images_PNG/System%20Reset%20Flow.png)

---

## Data Flow Diagram
![System Diagram](Diagram_images_PNG/Data%20Flow%20Diagram.png)

---

## State Diagram
![System Diagram](Smart_Home_Monitoring_Using_PIC18f6722/Diagram_images_PNG/State%20Diagram.png)
```
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
![System Diagram](Smart_Home_Monitoring_Using_PIC18f6722/Diagram_images_PNG/Timing%20Diagram.png)

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

The Automation Homw system demonstrates a complete embedded solution that integrates multiple subsystems:

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
