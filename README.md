# RPi-RRCircuits

This is a collection of Model Railroad open hardware and open source projects,
using LCC, OpenMRN, and OpenMRN-lite, for the following MCUs and SBCs:


Raspberry Pi, generally as HATs for the "B" flavors (RPi2B, RPi3B and RPi4B).
  (Directories with "HAT" at the end are for these boards.)

Beagle boards, including capes for BeagleBone Black and baseboards for
  PocketBeagles.
  (Directories with "CAPE" at the end are for BeagleBone Blacks,
   directories starting with "BBB" are also boards for the BeagleBone Black,
   directories starting with "PB" or "PocketBeagle" are for the PocketBeagles -
   these boards assume male header pins soldered to the bottom of the 
   PocketBeagle that insert into female header sockets on the base board.)

ESP32 (mostly meant for the Lilygo TTGO-T1 board)
  (Directories starting with "ESP32" are for this board.)

There are a few projects for other boards, including MetroMini (an Adafruit 
mini board that is a Arduino UNO clone), Tiva, and STM's Nucleo board.  The 
directory names will include the name of the board.

Directories ending in OpenMRN contain the Raspberry Pi and Beagle board
software for the various boards.  


These projects were created using Kicad 2013.06.11
(https://launchpad.net/kicad) under CentOS 6 and Kicad 4.0.7 under Ubuntu
18.04. Each subdirectory is one project, and contains a schematic and PCB
design or contains a program for one (or more) projects. There is also
software for the boards using OpenMRN (C++ library for OpenLCB/LCC) for these
projects.

There is a README in each project dir.

## Availble Projects:

1. [16DriverOutputCape](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/16DriverOutputCape)

    > This is a MCP23017 with two octal buffers meant to drive 16 LEDs (such as 
    > signal lamps). No PWM or special effects.  This board is a Cape for a 
    > BeagleBone Black or Green.  Has address jumpers for the MCP23017 and can be 
    > stacked -- up to 8 of these capes for up to 128 LEDS -- each board has power 
    > terminals for the LEDs.

1. [16DriverOutputHAT](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/16DriverOutputHAT)

    > This is a MCP23017 with two octal buffers meant to drive 16 LEDs (such as 
    > signal lamps). No PWM or special effects.  This board is a HAT for a 
    > Raspberry Pi 2, 3, or 4.  Has address jumpers for the MCP23017 and can be 
    > stacked -- up to 8 of these hats for up to 128 LEDS -- each board has power 
    > terminals for the LEDs.

1. [16DriverOutputOpenMRN](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/16DriverOutputOpenMRN)

    > The is the OpenMRN (OpenLCB/LCC) program code for the 16DriverOutput HAT or 
    > Cape boards. Can be build on either a BeagleBoard or a Raspberry Pi for 
    > either. 

1. [16LEDDriver23017SMD](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/16LEDDriver23017SMD)

    > This is a SMD variant of 16DriverOutputHAT.

1. [16PWMLedDriverCape](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/16PWMLedDriverCape)

    > This is a PCA9685 with two octal buffers meant to drive 16 LEDs (such as 
    > signal lamps).  This board is much like 16DriverOutputHat, but uses a PWM chip 
    > to allow for special effects and allows for brightness adjustment.  Has 
    > address jumpers for the PCA9685 and can be stacked -- up to 64 boards (WOW) 
    > for up to 1024 LEDs (WOW) -- each board has power terminals for the LEDs.

1. [16PWMLedDriverHat](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/16PWMLedDriverHat)

    > This is a PCA9685 with two octal buffers meant to drive 16 LEDs (such as 
    > signal lamps).  This board is much like 16DriverOutputHat, but uses a PWM chip 
    > to allow for special effects and allows for brightness adjustment.  Has 
    > address jumpers for the PCA9685 and can be stacked -- up to 64 boards (WOW) 
    > for up to 1024 LEDs (WOW) -- each board has power terminals for the LEDs.

1. [16PWMLedDriverOpenMRN](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/16PWMLedDriverOpenMRN)

    > This is the OpenMRN (OpenLCB/LCC) program for the 16PWMLedDriver* boards (RPi
    > Hat, BBB Cape, and PocketBeagle board). Can be build on either a BeagleBoard 
    > or a Raspberry Pi for either.

1. [16PWMLedDriverPocketBeagle](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/16PWMLedDriverPocketBeagle)

    > This is a PCA9685 with two octal buffers meant to drive 16 LEDs (such as
    > signal lamps). This board is much like 16DriverOutputCape, but uses a PWM chip
    > to allow for special effects and allows for brightness adjustment. Has address
    > jumpers for the PCA9685, includes a CAN transceiver with LCC network
    > connectors (RJ45s), 5V power supply for the PocketBeagle, and uses LCC power
    > for the LEDS. It uses female headers for the PocketBeagle (presumes male pin
    > headers soldered to the underside of the PocketBeagle).

1. [BBBCommandStation](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/BBBCommandStation)

    > Early BeagleBone Black LCC/DCC Command Station board set -- two boards, a cape 
    > and a power board.  Does not include RailComm support.

1. [BBBCommandStationOpenMRN](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/BBBCommandStationOpenMRN)

    > This is the program for the Beagle Board LCC/DCC/Railcom command 
    > station.  It uses OpenMRN to interface with tha LCC/OpenLCB network
    > to implement a LCC/OpenLCB aware command station node that can 
    > operate DCC/Railcom locomotives.  It should work with any of the
    > Beagle Board addon boards I have designed.  It uses the AM335X's
    > PRUs to generate the DCC signals.  It ises the OpenMRN Console
    > class to communicate over a Tcp/Ip channel with a Tcl/Tk coded
    > GUI program to provide a user friendly point-and-click high level
    > user interface.  

1. [BBBHalfSiding](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/BBBHalfSiding)

    > Early HalfSiding cape for the Beagle Bone Black. A somewhat all-in-one board
    > for the Beagle Bone Black to manage one half of a siding. This something of a
    > "kitchen sink" board and proably awkard in practice. Probably better off
    > stacking several separate special purpose capes.  I mostly did this as a 
    > "thought experiment" to see what this would look like.  Oh, if it used SMD 
    > parts it might be more compact and might be practical as a single board.

1. [ButtonHat](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ButtonHat)

    > This is a RPi HAT board to interface up to 12 push buttons with hardware 
    > debouncing (using 74ahc14 Hex Schmitt-Trigger Inverters) to a RPi.  Jumpers 
    > are used to map the buttons to GPIO pins.  This was a simple "thought 
    > experiment" to see if this was possible.

1. [CANHat](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/CANHat)

    > This is the original CAN Hat, using the MCP2515 CAN IF Chip and a MCP2551 
    > transceiver chip.

1. [CANHat2](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/CANHat2)

    > This is the second incarnation of the CAN Hat, also using the MCP2515 and 
    > MCP2551 chips.

1. [CANHat3](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/CANHat3)

    > This is the third incarnation of the CAN Hat, also using the MCP2515 and 
    > MCP2551 chips.

1. [DCCOD_SMD](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/DCCOD_SMD)

    > Single current sensing occupancy detector, meant for ESP32-16xDCCOD or 
    > simular.  Meant as a thought experiment to see what this would cost, etc.

1. [DCCShield](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/DCCShield)

    > Simple DCC driver shield for Arduino.

1. [DCCTiva](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/DCCTiva)

    > Simple DCC Driver board for Tiva.

1. [DualUncouplerHAT](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/DualUncouplerHAT)

    > This a dual driver for a electromagnetic uncoupler coil.  It features a 
    > pair of 555 type timers (in the form of a 556 dual timer) driving a TIP120 
    > power darlington.  It uses GPIO pins 21 and 22 (BCM 5 and 6, physical pins 
    > 29 and 31).

1. [DualUncouplerHAT1](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/DualUncouplerHAT1)

    > This is the same as DualUncouplerHAT, except it uses GPIO pins 23 and 24 
    > (BCM 13 and 19, physical pins 33 and 35).

1. [ESP32-16LedDriver](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32-16LedDriver)

    > 16 on/off LED Driver board using a MCP23017.

1. [ESP32-16PWMLedDriver](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32-16PWMLedDriver)

    > 16 LED PWM driver board using a PCA9685.

1. [ESP32-16xDCCOD](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32-16xDCCOD)

    > Backplane for up to 16 DCCOD_SMD's using a ESP32 MCU.  Meant as a thought 
    > experiment to see what this would cost, etc.

1. [ESP32-C3-BoosterOpenMRNIDF](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32-C3-BoosterOpenMRNIDF)

    > 

1. [ESP32-D0WD-V3-MultiFunctionOpenMRNIDF](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32-D0WD-V3-MultiFunctionOpenMRNIDF)

    > 

1. [ESP32-HalfSiding](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32-HalfSiding)

    > First draft of a ESP32 HalfSiding node using a MCP23017 on/off signal lamps.

1. [ESP32-MultiFunction](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32-MultiFunction)

    > This is a circuit board that supports an ESP32 Dev Kit board or TTGO-T1 board
    > to manage a range of possible interlocking plants or other model railroad
    > situations. It contains four stall motor drivers with point sense, four CT
    > Coil type occupancy detectors, four Schmitt-Trigger inputs (for push buttons),
    > four driver outputs, and 16 PWM LED drivers.

1. [ESP32-PWMHalfSiding](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32-PWMHalfSiding)

    > This is a circuit board that supports an ESP32 Dev Kit board or TTGO-T1 board 
    > to manage one half (one end) of a siding.  This board can also be used to 
    > manage two bi-directional single track ABS blocks or one bi-directional dual 
    > track ABS block.  There are other trackwork cases this board can handle as 
    > well.  

1. [ESP32-PWMHalfSidingSMD-T7](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32-PWMHalfSidingSMD-T7)

    > This is a circuit board that supports an ESP32 Dev Kit board or TTGO-T1 board 
    > to manage one half (one end) of a siding.  This board can also be used to 
    > manage two bi-directional single track ABS blocks or one bi-directional dual 
    > track ABS block.  There are other trackwork cases this board can handle as 
    > well.  

1. [ESP32-PWMHalfSidingSMD](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32-PWMHalfSidingSMD)

    > This is a circuit board that supports an ESP32 Dev Kit board or TTGO-T1 board 
    > to manage one half (one end) of a siding.  This board can also be used to 
    > manage two bi-directional single track ABS blocks or one bi-directional dual 
    > track ABS block.  There are other trackwork cases this board can handle as 
    > well.  

1. [ESP32-S3-MultiFunction](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32-S3-MultiFunction)

    > This is a LCC node circuit board that is based around an Espressif ESP32-S3FN8
    > MCU to manage and operate a collection of model railroad sensors and
    > actuators. 

1. [ESP32-S3-MultiFunctionOpenMRNIDF](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32-S3-MultiFunctionOpenMRNIDF)

    > 

1. [ESP32_mini_KiCad_Library](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32_mini_KiCad_Library)

    > KiCad symbol and footprint for the ESP32 Mini, the ESP32 clone of the Wemos D1 mini with more pins and more power!
    > There is some info on the boards [here](https://www.reddit.com/r/esp8266/comments/a93raj/wemos_ttgo_mini_d1_esp32/). It seems to be a Wemos clone, but is def. not made by them. I really like the boards for the small size and cheap price while giving me the power of an ESP32 and much more IO compared to the Wemos D1 mini.

1. [ESP32ABSBoard](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32ABSBoard)

    > ESP32 ABS (Automatic Block Signal) board.  Very simple board: one block 
    > detector, 8 LED drivers.

1. [ESP32BaseBoard](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32BaseBoard)

    > ESP32 Base board.  Base starting point for a ESP32 node.

1. [ESP32MRNSketches](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32MRNSketches)

    > ESP32 Arduino / OpenMRNLite sketches for the ESP32 boards.

1. [ESP32QuadOCDect](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32QuadOCDect)

    > ESP32 Quad Occupancy Detector node.

1. [ESP32QuadSMCSense](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/ESP32QuadSMCSense)

    > ESP32 Quad Stall Motor w/ sense node.

1. [LCCCANCape](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/LCCCANCape)

    > LCC/CAN Transceiver Cape for the Beagle Bone.

1. [LEDDriverHat](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/LEDDriverHat)

    > Eight LED drivers.  Jumpers to select GPIO pins.

1. [MCP23017Hat](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/MCP23017Hat)

    > This is a 16 pin I2C port expander using a MCP230017.

1. [OpenMRNIDF](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/OpenMRNIDF)

    > This is an exported version of the OpenMRN library for the ESP32 which is
    > usable as an ESP-IDF component.

1. [PBCommandStation](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/PBCommandStation)

    > Original through hole version of the Pocket Beagle LCC/DCC Command Station.
    > This design is depreciated -- see PocketBeagleCommandStation and 
    > PocketBeagleCommandStation_SMD for a current design.

1. [PocketBeagleCommandStation](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/PocketBeagleCommandStation)

    > Through hole version of the PocketBeagle LCC/DCC Command Station.  See 
    > PocketBeagleCommandStation_SMD for a SMD version.

1. [PocketBeagleCommandStation_SMD](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/PocketBeagleCommandStation_SMD)

    > SMD version of the PocketBeagle LCC/DCC Command Station.

1. [PocketBeagleMultifuntion](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/PocketBeagleMultifuntion)

    > This is a circuit board that supports a PocketBeagle board to manage a range
    > of possible interlocking plants or other model railroad situations. It
    > contains four stall motor drivers with point sense, four CT Coil type
    > occupancy detectors, four Schmitt-Trigger inputs (for push buttons), four
    > driver outputs, 8 GPIO pins in a 5x2 header, and 16 PWM LED drivers. 

1. [PocketBeagleMultifuntionOpenMRN](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/PocketBeagleMultifuntionOpenMRN)

    > OpenMRN program for the PocketBeagleMultifuntion board.

1. [PocketBeagleQuadOCDect](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/PocketBeagleQuadOCDect)

    > PocketBeagle Quad Occupancy Detector.

1. [PocketBeagleQuadSMCSense](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/PocketBeagleQuadSMCSense)

    > PocketBeagle Quad Stall Motor w/ Sense.

1. [QuadOCDectCape](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/QuadOCDectCape)

    > Quad Occupancy detector Cape (for a Beagle Bone).

1. [QuadOCDectHat](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/QuadOCDectHat)

    > Quad Occupancy detector Hat for Raspberry Pi.

1. [QuadOCDectOpenMRN](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/QuadOCDectOpenMRN)

    > OpenMRN (LCC / OpenLCB) program for Quad Occupancy Detector boards for Linux
    > SBCs.

1. [QuadSMC](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/QuadSMC)

    > This is a generic quad verison of SMCSenseHAT.

1. [QuadSMCSenseCape](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/QuadSMCSenseCape)

    > Quad Stall Motor w/ Sense Cape for a Beagle Bone.

1. [QuadSMCSenseHat](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/QuadSMCSenseHat)

    > Quad Stall Motor w/ Sense Hat for a Raspberry Pi.

1. [QuadSMCSenseOpenMRN](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/QuadSMCSenseOpenMRN)

    > OpenMRN (LCC / OpenLCB) program for Linux SBC with QuadSMCSense addon boards.

1. [QuadUncoupler](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/QuadUncoupler)

    > This is a generic qad version of DualUncouplerHAT.

1. [SignalHAT](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/SignalHAT)

    > This is a hat containing a MAX7221 driven off level shifted SPI.  It can 
    > drive up to 8 common-cathode signals of up to 8 LEDs each.

1. [SMCSenseHAT](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/SMCSenseHAT)

    > This is a dual stall motor driver and point sense circuit.  The stall 
    > motor driver uses a MPQ3725A transistor to allow driving either multiple 
    > Tortoises in parallel (eg for a crossover) or newer Switchmaster or 
    > Torquemaster which draw more current than the LM324 can handle.  Also 
    > included is a point sense circuit built from a RS Flip Flop made from 
    > 2-input NAND gates.  The point sense circuit expects to be connected to a 
    > single pole double throw switch, either a built in one in the Tortoise or 
    > an external one attached to the points themselves.  Uses GPIO 0, 1, 2, and 
    > 3.

1. [SMCSenseHAT1](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/SMCSenseHAT1)

    > This is the same as SMCSenseHAT, except uses GPIO 4, 5, 6, and 7.

1. [TivaMultiFunctionBoard](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/TivaMultiFunctionBoard)

    > This is a circuit board that supports a Tiva LaunchPad MCU to manage a range
    > of possible interlocking plants or other model railroad situations. It
    > contains four stall motor drivers with point sense, four CT Coil type
    > occupancy detectors, four Schmitt-Trigger inputs (for push buttons), four
    > driver outputs, 8 GPIO pins in a 5x2 header, and 16 PWM LED drivers. 

