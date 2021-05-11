# BBBCommandStationOpenMRN

This is the program for the Beagle Board LCC/DCC/Railcom command 
station.  It uses OpenMRN to interface with tha LCC/OpenLCB network
to implement a LCC/OpenLCB aware command station node that can 
operate DCC/Railcom locomotives.  It should work with any of the
Beagle Board addon boards I have designed.  It uses the AM335X's
PRUs to generate the DCC signals.  It ises the OpenMRN Console
class to communicate over a Tcp/Ip channel with a Tcl/Tk coded
GUI program to provide a user friendly point-and-click high level
user interface.  

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

Also this program uses scripts in the ../CommonOpenMRNExtras 
directory.  These scripts relate to the NODEID.txt.

The NODEID.txt file contains the seed for the node id generation.
It contains the node id of the most recent build.  The next build
will have a node id one more than this.

The scripts used are:
 
- GenerateNODEID.tcl -- generates and increments the node id
- NodeIDBin.mk -- contains the Makefile includes to auto
                  generate successive node ids.
 

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

## Subdirectories

### GUIFrontEnd

This subsirectory contains the Tcl/Tk coded GUI Front end.

### PRUProgs

This subsirectory contains the PRU firmware programs.
