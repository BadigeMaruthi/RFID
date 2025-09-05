# RFID-Based Attendance System with Database Integration
## 🎯 Aim
The project automates attendance using **RFID technology** with a **Linux-based database**. It uses the LPC2148 ARM7 microcontroller for attendance logging, user management, and PC communication.
## 🛠️ Hardware Requirements
- LPC2148 Microcontroller  
- RFID Reader & RFID Cards  
- LCD Display  
- 4x4 Keypad  
- Switches  
- MAX232  
- AT25LC512 EEPROM  
- USB-to-UART Converter 
## 💻 Software Requirements
- Embedded C  
- Keil µVision / ARM-GCC Toolchain  
- Flash Magic  
- Linux OS (for database integration)  
## ⚙️ Workflow
1. **RFID Reader** scans card → sends data via UART.  
2. **Microcontroller** checks card type:
   - **Admin Card** → Add/Delete/Edit users in database.  
   - **User Card** → Log IN/OUT times with RTC.  
3. **PC (Linux)** maintains `.csv` file with attendance logs.  
4. **LCD & Keypad** allow user interaction.  
5. **EEPROM** stores Admin card details.  
6. **Interrupts** handle Admin/RTC updates.  
## 📂 Repository Structure
├── lcd.c / lcd.h # LCD driver
├── delay.c / delay.h # Delay routines
├── uart.c / uart.h # UART drivers
├── spi.c / spi.h # SPI driver
├── spi_eeprom.c / .h # EEPROM interface
├── kpm.c / kpm.h # Keypad driver
├── rtc.c / rtc.h # RTC functions
├── interrupt.c / .h # Interrupt handling
├── projectmain.c # Main project logic
├── readme5.csv # Example attendance log
└── userDetails1.txt # Sample user data
## 🗂️ CSV Example
S.No, USER ID, USER NAME, DATE, IN TIME, OUT TIME, WORKING HOURS, STATUS
1, 12345, Maruthi, 2025-09-05, 09:00, 17:30, 8.5, OUT
2, 67890, chari, 2025-09-05, 09:15, 18:00, 8.75, OUT
##🚀 Features
   1.RFID authentication (Admin/User)
   2.IN/OUT logging with RTC
   3.Working hours calculation
   4.Admin options (Add, Delete, Edit users)
   5.EEPROM storage for Admin card
   6.UART communication with PC
   7.Interrupt-driven Admin/RTC updates
##🔮 Future Scope
1.Cloud-based attendance
2.Web dashboard visualization
3.Hybrid RFID + Biometric
4.Mobile notifications

