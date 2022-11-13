# ESP32-PWMHalfSidingSMD

This is a circuit board that supports an ESP32 Dev Kit board or TTGO-T1 board 
to manage one half (one end) of a siding.  This board can also be used to 
manage two bi-directional single track ABS blocks or one bi-directional dual 
track ABS block.  There are other trackwork cases this board can handle as 
well.  

[Order this board as a kit from The Country Robot](https://www.thecountryrobot.com/product/esp32-pwm-half-siding-node-kit/)


The board contains these I/O sections:

- Two occupancy detectors.  These are optoisolator based, so they will 
  work for both DC and DCC systems.
- Two stall-motor with point sense.
- Sixteen PWM Led drivers.  These are meant to light lamps in signal 
  heads.

This board uses six GPIO pins and one I2C channel:

- **GPIO0** Motor Select 1: select the position of stall motor 1.
- **GPIO12** Motor Select 2: select the position of stall motor 2.
- **GPIO34** Point Sense 1: return the state of the points for 
  stall motor 1. 
- **GPIO35** Point Sense 2: return the state of the points for 
  stall motor 2. 
- **GPIO26** Occupancy Detector 1.
- **GPIO27** Occupancy Detector 2.
- **GPIO16** (Optional) Output enable for the PWM LED Controller.
- **I2C Address 0x40** A PCA9685 16-channel, 12-bit PWM LED Controller.

Each of the motor drive circuits is through a TC4428, which can drive up to
1.5A, which is way more needed to drive a typical stall motor. It is enough to
drive a pair of stall motors, wired in parallel as would be the case for a
cross over. 

## Circuit Description

[ESP32-PWMHalfSidingSMD.pdf](https://github.com/RobertPHeller/RPi-RRCircuits/blob/master/ESP32-PWMHalfSidingSMD/ESP32-PWMHalfSidingSMD.pdf
)

### Turnout Control

The turnout control has two parts. There is an output section that contains
two TC4428 chips. Each chip has a non-inverting and an inverting driver. The
inputs of both drivers are connected to one of the motor GPIO pins. The output
are wired to the terminal block for a one of the motors. For any given logic
state of the motor control output, one of the drivers is ``on'' and the other
is ``off'', thus one motor terminal is ground and one is raised to the 12V
supply. This means alternative states of the logic line will drive the stall
motor in alternative directions. 

The other section is a pair of flip-flop debounce circuits, one for each of
two SPDT switch contacts that report the position of the turnout points. The
output of these flip-flops goes to a pair of GPIO input pins.

### CAN Transceiver

This section contains the CAN Transceiver, along with a termination jumper 
block. Power insertion and pick off and the RJ45 Jacks.

### Power Supply

This section contains a 5V power supply that takes the nominal 12V on the CAN 
power bus and regulates it down to 5V to supply the ESP32 MCU board.

### Occupancy Detectors

The occupancy detectors use optoisolators in series with the track power 
supply.  There is a heavy duty bridge rectifier in parallel with the 
optoisolators to carry the bulk of the current to the track.  They will work 
with either DC or DCC track power.

### PWM LED Driver

The PWM LED Driver uses a PCA9685 which is a 16 channel, 12 bit PWD LED 
driver.  A pair of octal MOSFET drivers and series load resistors are also 
included on the board.  The MOSFET drivers come in both inverting (low-side 
drive) and non-inverting (high-side drive), so it is possible to support both 
common anode and common cathode LED signals.  

## User Installed Parts

|Index|Qty|Refs|Mouser Part Number|Description|   
|-------|----|----------|-------------------|-------------------------|
1|2|J1, J2|710-615008144221|RJ45 Jacks|
2|1|JP1|649-67997-404HLF|Termination Jumper|
3|2|P601, P602|651-1725724|Signal Lamp Terminals|
4|3|T1, T2, T601|651-1725656|LCC and Signal power|
5|2|T3, T6|490-TB007-508-02BE|Occupancy Terminals|
6|2|T4, T5|651-1725685|Turnout terminals|
7|2|U0|517-929974-01-19-RK|MCU Headers|
8|1|U0|517-929850-01-18-10|MCU Headers|


