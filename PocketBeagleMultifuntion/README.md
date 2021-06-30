# PocketBeagleMultifuntion

This is a circuit board that supports a PocketBeagle board to manage a range
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

- **GPIO7** Occupancy Detector 1.
- **GPIO19** Occupancy Detector 2.
- **GPIO20** Occupancy Detector 3.
- **GPIO23** Occupancy Detector 4.
- **GPIO26** Motor Select 1: select the position of stall motor 1.
- **GPIO27** Motor Select 1: select the position of stall motor 2.
- **GPIO45** Motor Select 1: select the position of stall motor 3.
- **GPIO46** Motor Select 1: select the position of stall motor 4.
- **GPIO47** Point Sense 1: return the state of the points for stall motor 1.
- **GPIO48** Point Sense 1: return the state of the points for stall motor 2.
- **GPIO50** Point Sense 1: return the state of the points for stall motor 3.
- **GPIO52** Point Sense 1: return the state of the points for stall motor 4.
- **GPIO57** Schmitt trigger input 1.
- **GPIO58** Schmitt trigger input 2.
- **GPIO59** Schmitt trigger input 3.
- **GPIO60** Schmitt trigger input 4.
- **GPIO64** Driver (LED) 1.
- **GPIO65** Driver (LED) 2.
- **GPIO86** Driver (LED) 3.
- **GPIO87** Driver (LED) 4.
- **GPIO110** GPIO Line 1.
- **GPIO111** GPIO Line 2.
- **GPIO112** GPIO Line 3.
- **GPIO113** GPIO Line 4.
- **GPIO114** GPIO Line 5.
- **GPIO115** GPIO Line 6.
- **GPIO116** GPIO Line 7.
- **GPIO117** GPIO Line 8.
- **I2C1 Address 0x40** A PCA9685 16-channel, 12-bit PWM LED Controller

