
# Project Title

RFID and Password based Dual Layer Door Locking System

As there is a growing usage of smart authentication in door locks this innovative project combines RFID technology and Password authentication to create a customizable door-locking system. 


## Overview

This system aims to provide a dual-layer security. First we need to scan our RFID tag/card then enter the correct password. If authentication fails the buzzer is activated for 2 seconds which acts as an alarm. The servo motor used here signifies a lock. The password can be typed from the keypad. The concerned messages are displayed on the LCD. ATMega328PU is used as a microcontroller unit in this system. The entire system uses 5V supply except the RFID reader which uses 3.3V supply.  


## Features

- Dual layer of security (RFID and 4-digit Password)
- More secure 
- Fast authentication


## Requirements

Hardware Requirements

- ATmega328PU microcontroller unit
- 16MHz crystal oscillator
- MFCR522 RFID Reader
- RFID tags
- 16x2 LCD Display
- I2C Serial Interface Adapter Module for LCD display
- 4x4 matrix keypad
- SG90 Servo motor
- 5V DC Adapter
- S1117-33PI voltage regulator
- 10k resistor
- 10uF electrolytic capacitors
- 22pF ceramic capacitors
- Breadboard
- Jumper wires

Software Requirements

- Arduino IDE


## Usage

On powering the system the LCD screen will display the message "Scan your card". Place your RFID card/tag near to the RFID reader. If authentication is successful then the LCD screen will display the message "Enter Password" otherwise the buzzer will be activated for 2 seconds. Then enter the correct password (Default is '1A2B'. Can be changed later) to unlock the servo. After 10 seconds, the servo will get locked automatically. If the entered password is not correct then buzzer will be activated for 2 seconds.


## Configurable Settings

- Password can be configured in line 36 of code.ino 
- Access UID can be changed in line 11 of code.ino 
