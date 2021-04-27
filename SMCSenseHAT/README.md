# SMCSenseHAT

This is a dual stall motor driver and point sense circuit.  The stall 
motor driver uses a MPQ3725A transistor to allow driving either multiple 
Tortoises in parallel (eg for a crossover) or newer Switchmaster or 
Torquemaster which draw more current than the LM324 can handle.  Also 
included is a point sense circuit built from a RS Flip Flop made from 
2-input NAND gates.  The point sense circuit expects to be connected to a 
single pole double throw switch, either a built in one in the Tortoise or 
an external one attached to the points themselves.  Uses GPIO 0, 1, 2, and 
3.
