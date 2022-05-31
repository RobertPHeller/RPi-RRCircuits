# TivaMultiFunctionBoard

This is a circuit board that supports a Tiva LaunchPad MCU to manage a range
of possible interlocking plants or other model railroad situations. It
contains four stall motor drivers with point sense, four CT Coil type
occupancy detectors, four Schmitt-Trigger inputs (for push buttons), four
driver outputs, 8 GPIO pins in a 5x2 header, and 16 PWM LED drivers. 
                                                  
The board contains these I/O sections:

- Four occupancy detectors.  These are current transformer based for DCC.
- Four stall-motor with point sense.
- Four Schmitt-Trigger inputs, typically for facia push buttons.
- Four driver outputs, typically for facia lamps (LEDs).
- Eight GPIO pins brought out to a 5x2 header.
- Sixteen PWM Led drivers.  These are meant to light lamps in signal heads. 

This board uses 24 GPIO pins and one I2C channel:

- **PD7** OD1
- **PF4** OD2
- **PA5** OD3
- **PB2** OD4
- **PC5** Motor1
- **PC6** Motor2
- **PC7** Motor3
- **PD6** Motor4
- **PF2** Points1
- **PF3** Points2
- **PB3** Points3
- **PC4** Points4
- **PD0** LED1
- **PD1** LED2
- **PD2** LED3
- **PD3** LED4
- **PE1** Button1
- **PE2** Button2
- **PE3** Button3
- **PF1** Button4
- **PE0** GPIO8
- **PF0** GPIO7
- **PB4** GPIO6
- **PB7** GPIO5
- **PB6** GPIO4
- **PA4** GPIO3
- **PA3** GPIO2
- **PA2** GPIO1
- **I2C1 Address 0x40** A PCA9685 16-channel, 12-bit PWM LED Controller


