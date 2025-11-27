
# 🔢 PIC18F6722 – 4-Digit 7-Segment Counter (Increment/Decrement)

This project implements a **4-digit multiplexed 7-segment counter** using a **PIC18F6722** microcontroller.  
Two push-buttons are used to **increment** and **decrement** the displayed value from **0000 to 9999**.  
A Proteus simulation model is included for testing and demonstration.

![Alt Text](Screen%20of%20run%20simulation.png)

---

## ⭐ Features
- 4-digit common-anode 7-segment display (multiplexing)
- Two push-buttons:
  - **INC (RC4)** → count +
  - **DEC (RC5)** → count –
- Counter wraps around 0 ↔ 9999
- Debouncing included
- Multiplexing using PORTB (segments) + RC0–RC3 (digit select)
- Full MPLAB C18 source code
- Proteus simulation file included (`.pdsprj`)

---

## 🛠 Hardware Requirements
- PIC18F6722 microcontroller  
- 4 × 7-segment displays  
- 8 × segment resistors  
- 4 × digit switching transistors (optional)  
- Push-buttons for increment & decrement  
- 5V DC supply  
- Proteus 8+ for simulation  

---

## 📁 Project Structure
/project
**│── README.md**

**│── MPLAB IDE files**

     │──**Digital_counter.c** 
   
     │──**Digital_counter.hex** (used in simulation)
   
     │──Digital_counter.cof
   
     │──Digital_counter.map
   
     │──Digital_counter.mcp
   
     │──Digital_counter.mcw
   
     │──Digital_counter.o
   
**│── 7Segment_Counter.pdsprj (A Proteus project file (.pdsprj)**

**│── Digital counter.pdf (Fully Explaination of code )**

---

## 🧪 Proteus Simulation

A Proteus project file (`.pdsprj`) is included.  
It contains:

- **PIC18F6722** wired to 7-segment displays  
- **Segment resistors**  
- **Multiplexing lines**  
- **INC/DEC buttons**  

Open it using **Proteus 8 Professional**.

---

## ▶️ How to Run the Project

### **1. In MPLAB**
1. Open MPLAB  
2. Create a new project for **PIC18F6722**  
3. Add `Digital_counter.c`  
4. Build the project → generate the `.hex` file  

---

### **2. In Proteus**
1. Open the provided `.pdsprj` file  
2. Double-click the PIC → load the **HEX file**  
3. Run the simulation  
4. Press **INC/DEC** buttons  

---

## 📌 Future Improvements
- Long-press acceleration  
- EEPROM storage of last value  
- Reset button  
- Buzzer click feedback  
- Interrupt-driven multiplexing  

