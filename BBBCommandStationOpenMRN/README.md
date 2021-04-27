# BBBCommandStationOpenMRN

OpenMRN program for the various Beagle Board Command Station boards.  This 
program should work for all of the Beagle Board Command Station boards.  Also 
included is an X11-based (written in Tcl/Tk) GUI, typically run on a separate 
system, typically on a RPi but could be run on a PC or Mac with an appropriate 
Tclkit runtime, etc.  This program uses the PRUs in the Beagle Board's AM335x 
processor to create the DCC signals.  One PRU for the OPS signal and the other 
for the PROG signal.
