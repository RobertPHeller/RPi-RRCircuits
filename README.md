# RPi-RRCircuits

These are a collection of model railroad circuits I designed for the Raspberry 
Pi.  All but two use the "HAT" footprint and are meant to be stacked onto a 
RPi.  The other two are "generic" and expect to be connected to 3.3V GPIO 
pins.

These projects were created using Kicad 2013.06.11 
(https://launchpad.net/kicad) under CentOS 6.  Each subdirectory is one 
project, and contains a schematic and PCB design.

There are eight problems, six of them are "HATs" and the other two generic 
boards meant to be connected to 3.3V logic GPIO pins (typically a batch of 8 
with a 3.3V power and ground connection).

The projects are:

 1. DualUncouplerHAT
    This a dual driver for a electromagnetic uncoupler coil.  It features a 
    pair of 555 type timers (in the form of a 556 dual timer) driving a TIP120 
    power darlington.  It uses GPIO pins 21 and 22 (BCM 5 and 6, physical pins 
    29 and 31).
 1. DualUncouplerHAT1
    This is the same as DualUncouplerHAT, except it uses GPIO pins 23 and 24 
    (BCM 13 and 19, physical pins 33 and 35).
 1. MCP23017Hat
    This is a 16 pin I2C port expander using a MCP230017.
 1. SMCSenseHAT
    This is a dual stall motor driver and point sense circuit.  The stall 
    motor driver uses a MPQ3725A transistor to allow driving either multiple 
    Tortoises in parallel (eg for a crossover) or newer Switchmaster or 
    Torquemaster which draw more current than the LM324 can handle.  Also 
    included is a point sense circuit built from a RS Flip Flop made from 
    2-input NAND gates.  The point sense circuit expects to be connected to a 
    single pole double throw switch, either a built in one in the Tortoise or 
    an external one attached to the points themselves.  Uses GPIO 0, 1, 2, and 
    3.
 1. SMCSenseHAT1
    This is the same as SMCSenseHAT, except uses GPIO 4, 5, 6, and 7.
 1. SignalHAT
    This is a hat containing a MAX7221 driven off level shifted SPI.  It can 
    drive up to 8 common-cathode signals of up to 8 LEDs each.
 1. QuadSMC
    This is a generic quad verison of SMCSenseHAT.
 1. QuadUncoupler
    This is a generic qad version of DualUncouplerHAT.
                                    
