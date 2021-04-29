# ESP32-PWMHalfSiding

This is a circuit board that supports an ESP32 Dev Kit board or TTGO-T1 board 
to manage one half (one end) of a siding.  This board can also be used to 
manage two bi-directional single track ABS blocks or one bi-directional dual 
track ABS block.  There are other trackwork cases this board can handle as 
well.  

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

[ESP32-PWMHalfSiding.pdf](https://github.com/RobertPHeller/RPi-RRCircuits/blob/master/ESP32-PWMHalfSiding/ESP32-PWMHalfSiding.pdf
)

### Turnout Control

The turnout control has two parts. There is an output section that contains
four TC4428 chips. Each chip has a non-inverting and an inverting driver. The
inputs of both drivers are connected to one of the motor GPIO pins. The output
are wired to the terminal block for a one of the motors. For any given logic
state of the motor control output, one of the drivers is ``on'' and the other
is ``off'', thus one motor terminal is ground and one is raised to the 12V
supply. This means alternative states of the logic line will drive the stall
motor in alternative directions. 

The other section is a pair of flip-flop debounce circuits, one for each of
two SPDT switch contacts that report the position of the turnout points. The
output of these flip-flops goes to a quartet of GPIO input pins.

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
common anode and common cathode LED signals.  There are also (solder) jumpers 
on the board to support selecting the proper common wiring.

## Parts List

|Value|Qty|Refs|Mouser Part Number|
|-------|----|----------|------------------------------|  
|.1 uf|3|C1 C7 C601|21RZ310-RC|
|15 uf 15V|1|C2|710-860080773002|
|47 nf|1|C3|75-1C10Z5U473M050B|
|220 uf 25V|1|C4|140-REA221M1EBK0811P|
|22 uf 100V|1|C5|140-REA220M2ABK0811P|
|SB240E|3|D1 D2 D601|625-SB240-E3|
|SB160-E3/54|1|D3|625-SB160-E3|
|BRIDGEX|2|D4 D5|750-KBL401-G|
|MCT6H|2|IC1 IC2|782-MCT6H|
|RJ45|2|J1 J2|710-615008144221|
|Termination|1|JP1|649-67997-404HLF|
|330 uh|1|L1|673-PE-52627NL|
|Signal Terminals|2|P601 P602|651-1725724|
|60 Ohms|2|R1 R2|71-RN60C60R0B/R|
|10 Ohms|2|R3 R5|603-CFR-25JR-5210R|
|10K Ohms|2|R4 R6|603-CFR25SJT-52-10K|
|1.5K Ohms|2|RP601 RP602|652-4116R-1LF-1.5K|
|10K Ohms|1|RR2|652-4605X-AP1-103LF|
|Power Terminals|3|T1 T2 T601|651-1725656|
|Track Terminals|2|T3 T6|490-TB007-508-02BE|
|Turnout Terminals|2|T4 T5|651-1725685|
|ESP32_DEVKIT_C or TTGO-T1|1|U0|2x517-929974-01-19-RK (ESP32_DEVKIT_C) or 2x517-929850-01-18-10 (TTGO-T1)|
|TCAN332DR|1|U1|595-TCAN332DR|
|LM2574N-5.0|1|U3|926-LM2574N-5.0/NOPB|
|PCA9685|1|U601|771-PCA9685PW|
|TBD62X83A|2|U602 U603|757-TBD62083APG or 757-TBD62783APG|
|TC4428|2|U7 U8|579-TC4428VPA|
|74HCT00|1|U9|595-SN74AHC00N|



