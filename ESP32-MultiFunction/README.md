# ESP32-MultiFunction

This is a circuit board that supports an ESP32 Dev Kit board or TTGO-T1 board
to manage a range of possible interlocking plants or other model railroad
situations. It contains four stall motor drivers with point sense, four CT
Coil type occupancy detectors, four Schmitt-Trigger inputs (for push buttons),
four driver outputs, and 16 PWM LED drivers.

The board contains these I/O sections:

- Four occupancy detectors.  These are current transformer based for DCC.
- Four stall-motor with point sense.
- Four Schmitt-Trigger inputs, typically for facia push buttons
- Four driver outputs, typically for facia lamps (LEDs).
- Sixteen PWM Led drivers.  These are meant to light lamps in signal heads. 

This board uses 16 GPIO pins and one I2C channel:

- **GPIO34** Occupancy Detector 1.
- **GPIO35** Occupancy Detector 2.
- **GPIO36** Occupancy Detector 3.
- **GPIO39** Occupancy Detector 4.
- **GPIO25** Motor Select 1: select the position of stall motor 1.
- **GPIO26** Motor Select 1: select the position of stall motor 2.
- **GPIO32** Motor Select 1: select the position of stall motor 3.
- **GPIO33** Motor Select 1: select the position of stall motor 4.
- **GPIO12** Point Sense 1: return the state of the points for stall motor 1.
- **GPIO13** Point Sense 1: return the state of the points for stall motor 2.
- **GPIO14** Point Sense 1: return the state of the points for stall motor 3.
- **GPIO27** Point Sense 1: return the state of the points for stall motor 4.
- **GPIO0** Schmitt trigger input 1.
- **GPIO2** Schmitt trigger input 2.
- **GPIO15** Schmitt trigger input 3.
- **GPIO16** Schmitt trigger input 4.
- **GPIO17** Driver (LED) 1.
- **GPIO18** Driver (LED) 2.
- **GPIO19** Driver (LED) 3.
- **GPIO23** Driver (LED) 4.
- **I2C Address 0x40** A PCA9685 16-channel, 12-bit PWM LED Controller


