# BBBCommandStationOpenMRN

This is the program for the Beagle Board LCC/DCC/Railcom command 
station.  It uses OpenMRN to interface with tha LCC/OpenLCB network
to implement a LCC/OpenLCB aware command station node that can 
operate DCC/Railcom locomotives.  It should work with any of the
Beagle Board addon boards I have designed.  It uses the AM335X's
PRUs to generate the DCC signals.  It uses the OpenMRN Console
class to communicate over a Tcp/Ip channel with a Tcl/Tk coded
GUI program to provide a user friendly point-and-click high level
user interface.  

## Supported boards:

* [PocketBeagleCommandStation_SMD](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/PocketBeagleCommandStation_SMD) (targets/pbNTF.linux.armv7a)
* [PocketBeagleCommandStation](https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/PocketBeagleCommandStation) (targets/pb.linux.armv7a)
* [BBBCommandStation](https://github.com/RobertPHeller/RPi-RRCircuits/https://github.com/RobertPHeller/RPi-RRCircuits/tree/master/BBBCommandStation) (targets/bbb.linux.armv7a)

## PDF Reference Manual:

* [refman.pdf](https://github.com/RobertPHeller/RPi-RRCircuits/blob/master/BBBCommandStationOpenMRN/refman.pdf)

## SYNOPSIS

`BBBCommandStationOpenMRN [options]`

## OPTIONS

- -e EEPROM_file_path is the path to use to implement the EEProm 
     device.
- -t Persistent_Train_file_path is the path to use to the implement
     the train persistent data.
- -M The path to the Main (PRU0) firmware.
- -P The path to the Prog (PRU1) firmware.
- -u upstream_host   is the host name for an upstream hub.
- -q upstream_port   is the port number for the upstream hub
- -c can_socketname   is the name of the CAN socket.
- -n nodeid is the node id, as a 12 hex digit number (optionally with colons 
     between pairs of hex digits.
- -W name[:port] Start a WiThrottle named name on port (if :port is ommited, 
     on the default port).

The -u and -q options are only available if the program was built
to support either a OpenLCB Tcp host or a GRIDCONNECT host.
The -c option is only available if the program was built to
support CAN Sockets.


## Building

This program needs the OpenMRN library installed in a "standard"
place: /opt/openmrn or ~/openmrn or else in the location specified
by the environment variable OPENMRNPATH.
 
Various build options can be controlled in the Hardware.hxx header
file in the target directory.  Specificly, the GPIO pin 
assignments, whether to build a binary OpenLCB Tcp/Ip connected 
node, a GridConnect connected node, or a LCC CAN connected node. 
And if a network connected node the default port and host to 
connect to, also the console port to or to use a terminal console 
(for debugging).

There are separate Makefiles for the GUI Front End and the PRU firmware 
programs.

## Configuration
 
There are three configuration sections, one for each of the DCC
outputs (Main and Programming) and one for the fan control.
 
The two DCC outputs have these configuration options:
 
- The event to send when there is a short.
- The event to send when short is cleared.
- The event to send when the command station is shutdown
  due to over current.
- The event to send when the shutdown is cleared.
- The event to send when the thermal flag goes on.
- The event to send when the thermal flag goes off.
 
The fan control section has these configuration options:

- The alarm temperature threshold, in tenths of degree 
  centitrade.
- The event to send when the temperature excedes the alarm 
  temperature threshold.
- The event to send when the temperature drops below the alarm 
  temperature threshold.
- The fan temperature threshold, in tenths of degree
  centitrade.
- The event to send when the temperature excedes the fan
  temperature threshold.
- The event to send when the temperature drops below the fan
  temperature threshold.

## Features / Functionallities

### Persistent train database

This program provides a persistent train database (as a file on the Beagle 
board file system).

### WiThrottle server

This program can start a WiThrottle server and handle WiThrottle connections.

### Concurrent main line and programming track operation

The main line can remain functioning while the programming track is used to 
program a loco.

## Subdirectories

### GUIFrontEnd

This subsirectory contains the Tcl/Tk coded GUI Front end.  The GUI front end 
is optional -- the Command Station will run without it.  GUI front end 
provides a way to monitor the Command Station and to provide an interface to 
to access the programming track.

### PRUProgs

This subdirectory contains the PRU firmware programs.  There is one main 
program for both PRUs.  The main program has compiler preprocessor macros to 
control compile time options to generate separate versions for the two PRUs.  
The firmware is just a loop generating a DCC packet waveform on a PRU GPIO 
pin.  The firmware creates a remote process message FIFO device which the 
Track IF objects (see CommandStationDCCPRUTrack.hxx in the main Command 
Station code) write packets to. The firmware peels off these packets to its 
local packet buffer and then sends the packet over and over again until a new 
packet is sent down the FIFO.
