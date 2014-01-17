STM32F4Template
===============

template for some STM32F4 periphericals for STM32F407 using Keil CC compiler. 

This module depends strongly of STM32F4 Peripheral Library V1.1.0

The last version of STM32F4 Peripheral Library is present in http://www.st.com/web/en/catalog/tools/PF257901

- implement the RTC code and put support for libc time functions
- implement SPI hardware interface using STM32F4 Peripheral Library
- implement support to serial SPI nor flash memory (M25PXXX)
- put support to log functionalities
- put support to RS-485 communication suit
  - implement industrial protocols like ModBus
- for a near future, I want to put support to lwip
  - implement some WebServices with functions of reading sensors and change status of actuators
  - implement WebServices to notify the clients about changes os internal state of operation 
- implement support to CC3000 wireless Wi-Fi dongle
- implement support to USB Host





