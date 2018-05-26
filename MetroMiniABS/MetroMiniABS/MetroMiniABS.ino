/**
 * MetroMiniABS.ino -- An OpenLCB node that implements a single Automatic Block
 * Signal.  This node presumes the hardware detailed at 
 *
 * This board uses an Adafruit Metro Mini, which is a Arduino Uno R3 clone on 
 * a mini form factor: 1.7"x.7", with 28 pins as two rows of 14 .6" with a .1" 
 * pitch (same footprint as a wide DIL-28).
 *
 * I/O pin usage:
 *
 * D3 Red signal lamp (PWM)
 * D4 Blue Button/Led
 * D5 Yellow signal lamp (PWM)
 * D6 Green signal lamp (PWM)
 * D7 Gold Button/Led
 * D8 Occupancy Detector
 *
 * Events used:
 *
 * Consumed:
 *   Next Block Occupied
 *   Next Block Clear
 * Produced:
 *   Block Occupied
 *   Block Clear
 *   Stop Aspect Triggered
 *   Stop Aspect Released
 *   Approach Aspect Triggered
 *   Approach Aspect Released
 *   Clear Aspect Triggered
 *   Clear Aspect Released
 *
 * Additional configuration:
 *
 * Common Anode: Yes/No
 * BiPolar Searchlight: Yes/No
 * Stop (red) Brightness: 0-255
 * Approach (yellow) Brightness: 0-255
 * Clear (green) Brightness: 0-255
 * Yellow Hue (ratio of red to green): 0-255
 *
 */
// OlcbBasicNode
//   A prototype of a basic 4-channel OpenLCB board
// 
//   setup() determines which are consumers and
//   which are producers
//
//   Bob Jacobsen 2010, 2012
//      based on examples by Alex Shepherd and David Harris
//==============================================================

// next line for stand-alone compile
#include <Arduino.h>

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "logging.h"

// The following lines are needed because the Arduino environment 
// won't search a library directory unless the library is included 
// from the top level file (this file)
#include <EEPROM.h>
#include <can.h>
#include <ButtonLed.h>
#include "ButtonLed_nobutton.h"
#include "ButtonLed_nc.h"
#include "ButtonLed_noled.h"
#include "SignalOptions.h"
#include "Mast.h"


#include "OlcbCommonVersion.h"
#include "NodeID.h"

// init for serial communications if used
#define         BAUD_RATE       57600

NodeID nodeid(0x05,0x01,0x01,0x01,0x22,0x80);    // This node's default ID; must be valid 

// Define pins
#define BLUE 4
#define GOLD 7

#define BLOCKOCC 8 // No led!
#define SIGNALGREEN 6 // No button 
#define SIGNALYELLOW 5 // No button
#define SIGNALRED 3 // No button

ButtonLed_noled blockocc(BLOCKOCC,LOW);
ButtonLed_nc nextblock(0); // No actual pin 
ButtonLed_nobutton signalgreen(SIGNALGREEN);
ButtonLed_nobutton signalyellow(SIGNALYELLOW);
ButtonLed_nobutton signalred(SIGNALRED);

struct {
    char nodename[20];
    char nodedescr[24];
    SignalOptions signalOptions;
} configOptions;

Mast mast(&blockocc,&nextblock,&signalgreen,&signalyellow,&signalred,
          &(configOptions.signalOptions));

#define NUM_EVENT 10

// next lines get "warning: only initialized variables can be placed into program memory area" due to GCC bug
extern "C" {
 const char configDefInfo[] PROGMEM = {
   60, 63, 120, 109, 108, 32, 118, 101, 114, 115, 105, 111, 110, 61, 34, 49, 46, 48, 34, 63, 62, 10, 60, 63, 120, 109, 108, 45, 115, 116, 121, 108, 101, 115, 104, 101, 101, 116, 32, 116, 121, 112, 101, 61, 39, 116, 101, 120, 116, 47, 120, 115, 108, 39, 32, 104, 114, 101, 102, 61, 39, 104, 116, 116, 112, 58, 47, 47, 111, 112, 101, 110, 108, 99, 98, 46, 111, 114, 103, 47, 116, 114, 117, 110, 107, 47, 112, 114, 111, 116, 111, 116, 121, 112, 101, 115, 47, 120, 109, 108, 47, 120, 115, 108, 116, 47, 99, 100, 105, 46, 120, 115, 108, 39, 63, 62, 10, 60, 99, 100, 105, 32, 120, 109, 108, 110, 115, 58,    // | <?xml version="1.0"?><?xml-stylesheet type='text/xsl' href='http://openlcb.org/trunk/prototypes/xml/xslt/cdi.xsl'?><cdi xmlns:|
   120, 115, 105, 61, 39, 104, 116, 116, 112, 58, 47, 47, 119, 119, 119, 46, 119, 51, 46, 111, 114, 103, 47, 50, 48, 48, 49, 47, 88, 77, 76, 83, 99, 104, 101, 109, 97, 45, 105, 110, 115, 116, 97, 110, 99, 101, 39, 32, 120, 115, 105, 58, 110, 111, 78, 97, 109, 101, 115, 112, 97, 99, 101, 83, 99, 104, 101, 109, 97, 76, 111, 99, 97, 116, 105, 111, 110, 61, 39, 104, 116, 116, 112, 58, 47, 47, 111, 112, 101, 110, 108, 99, 98, 46, 111, 114, 103, 47, 116, 114, 117, 110, 107, 47, 112, 114, 111, 116, 111, 116, 121, 112, 101, 115, 47, 120, 109, 108, 47, 115, 99, 104, 101, 109, 97, 47, 99, 100,    // | xsi='http://www.w3.org/2001/XMLSchema-instance' xsi:noNamespaceSchemaLocation='http://openlcb.org/trunk/prototypes/xml/schema/cd|
   105, 46, 120, 115, 100, 39, 62, 10, 10, 32, 32, 60, 105, 100, 101, 110, 116, 105, 102, 105, 99, 97, 116, 105, 111, 110, 62, 10, 32, 32, 32, 32, 60, 109, 97, 110, 117, 102, 97, 99, 116, 117, 114, 101, 114, 62, 68, 101, 101, 112, 119, 111, 111, 100, 115, 32, 83, 111, 102, 116, 119, 97, 114, 101, 60, 47, 109, 97, 110, 117, 102, 97, 99, 116, 117, 114, 101, 114, 62, 10, 32, 32, 32, 32, 60, 109, 111, 100, 101, 108, 62, 77, 101, 116, 114, 111, 77, 105, 110, 105, 65, 66, 83, 60, 47, 109, 111, 100, 101, 108, 62, 10, 32, 32, 32, 32, 60, 104, 97, 114, 100, 119, 97, 114, 101, 86, 101, 114,    // | i.xsd'>  <identification>    <manufacturer>Deepwoods Software</manufacturer>    <model>MetroMiniABS</model>    <hardwareVer|
   115, 105, 111, 110, 62, 49, 46, 48, 60, 47, 104, 97, 114, 100, 119, 97, 114, 101, 86, 101, 114, 115, 105, 111, 110, 62, 10, 32, 32, 32, 32, 60, 115, 111, 102, 116, 119, 97, 114, 101, 86, 101, 114, 115, 105, 111, 110, 62, 49, 46, 48, 60, 47, 115, 111, 102, 116, 119, 97, 114, 101, 86, 101, 114, 115, 105, 111, 110, 62, 10, 32, 32, 60, 47, 105, 100, 101, 110, 116, 105, 102, 105, 99, 97, 116, 105, 111, 110, 62, 10, 10, 32, 32, 10, 32, 32, 60, 115, 101, 103, 109, 101, 110, 116, 32, 111, 114, 105, 103, 105, 110, 61, 39, 49, 49, 50, 39, 32, 115, 112, 97, 99, 101, 61, 39, 50, 53, 51,    // | sion>1.0</hardwareVersion>    <softwareVersion>1.0</softwareVersion>  </identification>    <segment origin='112' space='253|
   39, 62, 10, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 78, 111, 100, 101, 32, 73, 68, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 60, 100, 101, 115, 99, 114, 105, 112, 116, 105, 111, 110, 62, 85, 115, 101, 114, 45, 112, 114, 111, 118, 105, 100, 101, 100, 32, 100, 101, 115, 99, 114, 105, 112, 116, 105, 111, 110, 32, 111, 102, 32, 116, 104, 101, 32, 110, 111, 100, 101, 60, 47, 100, 101, 115, 99, 114, 105, 112, 116, 105, 111, 110, 62, 10, 32, 32, 32, 32, 60, 103, 114, 111, 117, 112, 62, 10, 32, 32, 32, 32, 32, 32, 60, 115, 116, 114, 105, 110, 103, 32, 115, 105, 122, 101, 61,    // | '>    <name>Node ID</name>    <description>User-provided description of the node</description>    <group>      <string size=|
   39, 50, 48, 39, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 78, 111, 100, 101, 32, 78, 97, 109, 101, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 115, 116, 114, 105, 110, 103, 62, 10, 32, 32, 32, 32, 32, 32, 60, 115, 116, 114, 105, 110, 103, 32, 115, 105, 122, 101, 61, 39, 50, 52, 39, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 78, 111, 100, 101, 32, 68, 101, 115, 99, 114, 105, 112, 116, 105, 111, 110, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 115, 116, 114, 105,    // | '20'>        <name>Node Name</name>      </string>      <string size='24'>        <name>Node Description</name>      </stri|
   110, 103, 62, 10, 32, 32, 32, 32, 60, 47, 103, 114, 111, 117, 112, 62, 10, 32, 32, 32, 32, 60, 103, 114, 111, 117, 112, 62, 10, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 83, 105, 103, 110, 97, 108, 32, 73, 109, 112, 108, 101, 109, 101, 110, 116, 97, 116, 105, 111, 110, 32, 79, 112, 116, 105, 111, 110, 115, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 105, 110, 116, 32, 115, 105, 122, 101, 61, 39, 49, 39, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 67, 111, 109, 109, 111, 110, 32, 65, 110, 111, 100, 101, 60, 47, 110,    // | ng>    </group>    <group>      <name>Signal Implementation Options</name>      <int size='1'>        <name>Common Anode</n|
   97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 109, 97, 112, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 60, 114, 101, 108, 97, 116, 105, 111, 110, 62, 60, 112, 114, 111, 112, 101, 114, 116, 121, 62, 49, 60, 47, 112, 114, 111, 112, 101, 114, 116, 121, 62, 60, 118, 97, 108, 117, 101, 62, 89, 101, 115, 60, 47, 118, 97, 108, 117, 101, 62, 60, 47, 114, 101, 108, 97, 116, 105, 111, 110, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 60, 114, 101, 108, 97, 116, 105, 111, 110, 62, 60, 112, 114, 111, 112, 101, 114, 116, 121, 62, 48, 60, 47, 112, 114, 111, 112,    // | ame>        <map>          <relation><property>1</property><value>Yes</value></relation>          <relation><property>0</prop|
   101, 114, 116, 121, 62, 60, 118, 97, 108, 117, 101, 62, 78, 111, 60, 47, 118, 97, 108, 117, 101, 62, 60, 47, 114, 101, 108, 97, 116, 105, 111, 110, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 47, 109, 97, 112, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 105, 110, 116, 62, 10, 32, 32, 32, 32, 32, 32, 60, 105, 110, 116, 32, 115, 105, 122, 101, 61, 39, 49, 39, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 66, 105, 80, 111, 108, 97, 114, 32, 83, 101, 97, 114, 99, 104, 108, 105, 103, 104, 116, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32,    // | erty><value>No</value></relation>        </map>      </int>      <int size='1'>        <name>BiPolar Searchlight</name>    |
   32, 32, 32, 32, 60, 109, 97, 112, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 60, 114, 101, 108, 97, 116, 105, 111, 110, 62, 60, 112, 114, 111, 112, 101, 114, 116, 121, 62, 49, 60, 47, 112, 114, 111, 112, 101, 114, 116, 121, 62, 60, 118, 97, 108, 117, 101, 62, 89, 101, 115, 60, 47, 118, 97, 108, 117, 101, 62, 60, 47, 114, 101, 108, 97, 116, 105, 111, 110, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 60, 114, 101, 108, 97, 116, 105, 111, 110, 62, 60, 112, 114, 111, 112, 101, 114, 116, 121, 62, 48, 60, 47, 112, 114, 111, 112, 101, 114, 116, 121, 62, 60, 118, 97, 108,    // |     <map>          <relation><property>1</property><value>Yes</value></relation>          <relation><property>0</property><val|
   117, 101, 62, 78, 111, 60, 47, 118, 97, 108, 117, 101, 62, 60, 47, 114, 101, 108, 97, 116, 105, 111, 110, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 47, 109, 97, 112, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 105, 110, 116, 62, 10, 32, 32, 32, 32, 32, 32, 60, 105, 110, 116, 32, 115, 105, 122, 101, 61, 34, 49, 34, 32, 109, 105, 110, 61, 34, 48, 34, 32, 109, 97, 120, 61, 34, 50, 53, 53, 34, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 83, 116, 111, 112, 32, 40, 114, 101, 100, 41, 32, 66, 114, 105, 103, 104, 116, 110, 101, 115, 115, 60,    // | ue>No</value></relation>        </map>      </int>      <int size="1" min="0" max="255">        <name>Stop (red) Brightness<|
   47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 105, 110, 116, 62, 10, 32, 32, 32, 32, 32, 32, 60, 105, 110, 116, 32, 115, 105, 122, 101, 61, 34, 49, 34, 32, 109, 105, 110, 61, 34, 48, 34, 32, 109, 97, 120, 61, 34, 50, 53, 53, 34, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 65, 112, 112, 114, 111, 97, 99, 104, 32, 40, 121, 101, 108, 108, 111, 119, 41, 32, 66, 114, 105, 103, 104, 116, 110, 101, 115, 115, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 105, 110, 116, 62, 10, 32, 32, 32, 32, 32, 32,    // | /name>      </int>      <int size="1" min="0" max="255">        <name>Approach (yellow) Brightness</name>      </int>      |
   60, 105, 110, 116, 32, 115, 105, 122, 101, 61, 34, 49, 34, 32, 109, 105, 110, 61, 34, 48, 34, 32, 109, 97, 120, 61, 34, 50, 53, 53, 34, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 67, 108, 101, 97, 114, 32, 40, 103, 114, 101, 101, 110, 41, 32, 66, 114, 105, 103, 104, 116, 110, 101, 115, 115, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 105, 110, 116, 62, 10, 32, 32, 32, 32, 32, 32, 60, 105, 110, 116, 32, 115, 105, 122, 101, 61, 34, 49, 34, 32, 109, 105, 110, 61, 34, 48, 34, 32, 109, 97, 120, 61, 34, 50, 53, 53,    // | <int size="1" min="0" max="255">        <name>Clear (green) Brightness</name>      </int>      <int size="1" min="0" max="255|
   34, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 89, 101, 108, 108, 111, 119, 32, 72, 117, 101, 32, 40, 114, 97, 116, 105, 111, 32, 111, 102, 32, 114, 101, 100, 32, 116, 111, 32, 103, 114, 101, 101, 110, 41, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 105, 110, 116, 62, 10, 32, 32, 32, 32, 60, 47, 103, 114, 111, 117, 112, 62, 10, 32, 32, 60, 47, 115, 101, 103, 109, 101, 110, 116, 62, 10, 32, 32, 60, 115, 101, 103, 109, 101, 110, 116, 32, 111, 114, 105, 103, 105, 110, 61, 39, 49, 50, 39, 32, 115, 112, 97, 99, 101, 61, 39,    // | ">        <name>Yellow Hue (ratio of red to green)</name>      </int>    </group>  </segment>  <segment origin='12' space='|
   50, 53, 51, 39, 62, 10, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 69, 118, 101, 110, 116, 115, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 60, 100, 101, 115, 99, 114, 105, 112, 116, 105, 111, 110, 62, 68, 101, 102, 105, 110, 101, 32, 101, 118, 101, 110, 116, 115, 32, 97, 115, 115, 111, 99, 105, 97, 116, 101, 100, 32, 119, 105, 116, 104, 32, 105, 110, 112, 117, 116, 32, 97, 110, 100, 32, 111, 117, 116, 112, 117, 116, 32, 112, 105, 110, 115, 60, 47, 100, 101, 115, 99, 114, 105, 112, 116, 105, 111, 110, 62, 10, 32, 32, 32, 32, 60, 103, 114, 111, 117, 112, 62, 10, 32, 32, 32,    // | 253'>    <name>Events</name>    <description>Define events associated with input and output pins</description>    <group>   |
   32, 32, 32, 60, 110, 97, 109, 101, 62, 66, 108, 111, 99, 107, 32, 69, 118, 101, 110, 116, 115, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 78, 101, 120, 116, 32, 66, 108, 111, 99, 107, 32, 79, 99, 99, 117, 112, 105, 101, 100, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 60, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60,    // |    <name>Block Events</name>      <eventid>        <name>Next Block Occupied</name>      </eventid>      <eventid>        <|
   110, 97, 109, 101, 62, 78, 101, 120, 116, 32, 66, 108, 111, 99, 107, 32, 67, 108, 101, 97, 114, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 60, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 66, 108, 111, 99, 107, 32, 79, 99, 99, 117, 112, 105, 101, 100, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 60, 101, 118, 101, 110, 116, 105,    // | name>Next Block Clear</name>      </eventid>      <eventid>        <name>Block Occupied</name>      </eventid>      <eventi|
   100, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 66, 108, 111, 99, 107, 32, 67, 108, 101, 97, 114, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 60, 47, 103, 114, 111, 117, 112, 62, 10, 32, 32, 32, 32, 60, 103, 114, 111, 117, 112, 62, 10, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 83, 105, 103, 110, 97, 108, 32, 65, 115, 112, 101, 99, 116, 32, 69, 118, 101, 110, 116, 115, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 101, 118, 101,    // | d>        <name>Block Clear</name>      </eventid>    </group>    <group>      <name>Signal Aspect Events</name>      <eve|
   110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 83, 116, 111, 112, 32, 65, 115, 112, 101, 99, 116, 32, 84, 114, 105, 103, 103, 101, 114, 101, 100, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 60, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 83, 116, 111, 112, 32, 65, 115, 112, 101, 99, 116, 32, 82, 101, 108, 101, 97, 115, 101, 100, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32,    // | ntid>        <name>Stop Aspect Triggered</name>      </eventid>      <eventid>        <name>Stop Aspect Released</name>    |
   32, 32, 60, 47, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 60, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 65, 112, 112, 114, 111, 97, 99, 104, 32, 65, 115, 112, 101, 99, 116, 32, 84, 114, 105, 103, 103, 101, 114, 101, 100, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 60, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 65, 112, 112, 114, 111,    // |   </eventid>      <eventid>        <name>Approach Aspect Triggered</name>      </eventid>      <eventid>        <name>Appro|
   97, 99, 104, 32, 65, 115, 112, 101, 99, 116, 32, 82, 101, 108, 101, 97, 115, 101, 100, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 60, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 67, 108, 101, 97, 114, 32, 65, 115, 112, 101, 99, 116, 32, 84, 114, 105, 103, 103, 101, 114, 101, 100, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 60,    // | ach Aspect Released</name>      </eventid>      <eventid>        <name>Clear Aspect Triggered</name>      </eventid>      <|
   101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 32, 32, 32, 32, 60, 110, 97, 109, 101, 62, 67, 108, 101, 97, 114, 32, 65, 115, 112, 101, 99, 116, 32, 82, 101, 108, 101, 97, 115, 101, 100, 60, 47, 110, 97, 109, 101, 62, 10, 32, 32, 32, 32, 32, 32, 60, 47, 101, 118, 101, 110, 116, 105, 100, 62, 10, 32, 32, 32, 32, 60, 47, 103, 114, 111, 117, 112, 62, 10, 32, 32, 60, 47, 115, 101, 103, 109, 101, 110, 116, 62, 10, 60, 47, 99, 100, 105, 62, 10,    // | eventid>        <name>Clear Aspect Released</name>      </eventid>    </group>  </segment></cdi>|
   0
 };

 const char SNII_const_data[] PROGMEM = "\001MetroMiniABS\000Metro Mini Automatic Block Signal\0001.0\000" OlcbCommonVersion ; // last zero in double-quote

}; // end extern "C"

#define SNII_var_data 94
#define SNII_var_offset 20



#define ShortBlinkOn   0x00010001L
#define ShortBlinkOff  0xFFFEFFFEL

// The patterns are not really used.  The only LEDs are powered by PWM and the
// Other "ButtonLeds" are input only (no led) or not connected at all and are 
// just a state.  But the BG class wants them...
long patterns[] = {
    ShortBlinkOff,ShortBlinkOn,
    ShortBlinkOff,ShortBlinkOn,
    ShortBlinkOff,ShortBlinkOn,
    ShortBlinkOff,ShortBlinkOn,
    ShortBlinkOff,ShortBlinkOn    
};

// Non of these are actual buttons with LEDs.  One input, one state, and three 
// PWM outputs.
ButtonLed* buttons[] = {
    &nextblock,&nextblock,
    &blockocc,&blockocc,
    &signalred,&signalred,
    &signalyellow,&signalyellow,
    &signalgreen,&signalgreen
};

// These are actual buttons w/LEDs
ButtonLed blue(BLUE, LOW);
ButtonLed gold(GOLD, LOW);

#include "OlcbArduinoCAN.h"

/* *************************************************
 * EEPROM memory layout
 *     See NodeMemory.h for background info
 *
 * Internal data, not to be reset by user:
 *     0 - 3        Memory state flag
 *     4 - 5        Cycle count
 *     6 - 11       Node ID
 *
 * User configuration data:
 *     12 - 12+10*sizeof(Event)  EventID storage
 * 
 *     112 - 132     Node name (zero-term string, 20 bytes total)
 *     132 - 156     User comment (zero-term string, 24 bytes total)
 *
 *     12+44 - 12+44+10*sizeof(Event)  Events plus description strings
 *     SignalOptions follow...
 *
 *************************************************** */

#define LAST_EEPROM 12+44+NUM_EVENT*sizeof(Event)

extern "C" {
    uint32_t spaceUpperAddr(uint8_t space) {  // return last valid address
      switch (space) {
          case 255: return sizeof(configDefInfo) - 1; // CDI (data starts at zero)
          case 254: return RAMEND; // RAM from Arduino definition
          case 253: return LAST_EEPROM; // Configuration
      }
      return (uint32_t)3;
    }
const uint8_t getRead(uint32_t address, int space) {
  if (space == 0xFF) {
    // Configuration definition information
    return pgm_read_byte(configDefInfo+address);
  } else if (space == 0xFE) {
    // All memory reads from RAM starting at first location in this program
    return *(((uint8_t*)&rxBuffer)+address);
  } else if (space == 0xFD) {
    // Configuration space is entire EEPROM
    return EEPROM.read(address);
  } else if (space == 0xFC) { // 
    // used by ADCDI/SNII for constant data
    return pgm_read_byte(SNII_const_data+address);
  } else if (space == 0xFB) { // 
    // used by ADCDI/SNII for variable data
    return EEPROM.read(SNII_var_data+address);
  } else {
    // unknown space
    return 0; 
  }
}

void getWrite(uint32_t address, int space, uint8_t val) {
  if (space == 0xFE) {
    // All memory
    *(((uint8_t*)&rxBuffer)+address) = val;
  } else if (space == 0xFD) {
    // Configuration space
    EEPROM.write(address, val);
  } 
  // all other spaces not written
}

uint8_t protocolIdentValue[6] = {0xD7,0x58,0x00,0,0,0};
    // PIP, Datagram, MemConfig, P/C, ident, teach/learn, 
    // ACDI, SNIP, CDI


// Events this node can produce or consume, used by PCE and loaded from EEPROM by NM
Event events[] = { // should be NUM_EVENT of these
    Event(), Event(), Event(), Event(), 
    Event(), Event(), Event(), Event(),
    Event(), Event()
};

void pceCallback(int index){
    // invoked when an event is consumed; drive pins as needed
    // from index
    //
    // sample code uses inverse of low bit of pattern to drive pin all on or all off
    // (pattern is mostly one way, blinking the other, hence inverse)
    //
    //buttons[index]->on(patterns[index]&0x1 ? 0x0L : ~0x0L );
    buttons[index]->state = !(index & 0x01);
    // Process mast...
}

} // extern "C"

NodeMemory nm(0);  // allocate from start of EEPROM
void store() { nm.store(&nodeid, events, NUM_EVENT,(uint8_t *)&configOptions,sizeof(configOptions)); }

PCE pce(events, NUM_EVENT, &txBuffer, &nodeid, pceCallback, store, &link);

// Set up Blue/Gold configuration
BG bg(&pce, buttons, patterns, NUM_EVENT, &blue, &gold, &txBuffer);

bool states[] = {false, false, false, false}; // current input states; report when changed

int scanIndex = 0;
// On the assumption that the producers (inputs) and consumers (outputs) are consecutive, 
// these are used later to label the individual channels as producer or consumer
#define FIRST_PRODUCER_CHANNEL_INDEX    0
#define LAST_PRODUCER_CHANNEL_INDEX     4
#define FIRST_CONSUMER_CHANNEL_INDEX    5
#define LAST_CONSUMER_CHANNEL_INDEX     5

extern "C" {
void produceFromInputs() {
    // called from loop(), this looks at changes in input pins and 
    // and decides which events to fire
    // with pce.produce(i);
    // The first event of each pair is sent on button down,
    // and second on button up.
    // 
  
  // To reduce latency, only MAX_INPUT_SCAN inputs are scanned on each loop
  //    (Should not exceed the total number of inputs, nor about 4)
#define MAX_INPUT_SCAN 4
  //
  
  for (int i = 0; i<(MAX_INPUT_SCAN); i++, scanIndex++) { // simply a counter of how many to scan
      if (scanIndex > (LAST_PRODUCER_CHANNEL_INDEX)) scanIndex = (FIRST_PRODUCER_CHANNEL_INDEX);
      if (states[scanIndex] != buttons[scanIndex*2]->state) {
          states[scanIndex] = buttons[scanIndex*2]->state;
          if (states[scanIndex]) {
              pce.produce(scanIndex*2);
          } else {
              pce.produce(scanIndex*2+1);
          }
          if (scanIndex == 0) {
              // Process mast logic...
          }
      }
  }
}

} // extern "C"


/**
 * Setup does initial configuration
 */
void setup()
{
  // set up serial comm; may not be space for this!
  //delay(250);Serial.begin(BAUD_RATE);logstr("\nOlcbBasicNode\n");
  
  // read OpenLCB from EEPROM
  //nm.forceInitAll(); // uncomment if need to go back to initial EEPROM state
  nm.setup(&nodeid, events, NUM_EVENT, (uint8_t*) &configOptions, (uint16_t)sizeof(configOptions), (uint16_t)LAST_EEPROM);  
    if (configOptions.signalOptions.clear_brite == 0) {
        configOptions.signalOptions.reset();
        store();
    }
  
  // set event types, now that IDs have been loaded from configuration
  // newEvent arguments are (event index, producer?, consumer?)
  for (int i=2*(FIRST_PRODUCER_CHANNEL_INDEX); i<2*(LAST_PRODUCER_CHANNEL_INDEX+1); i++) {
      pce.newEvent(i,true,false); // producer
  }
  for (int i=2*(FIRST_CONSUMER_CHANNEL_INDEX); i<2*(LAST_CONSUMER_CHANNEL_INDEX+1); i++) {
      pce.newEvent(i,false,true); // consumer
  }
  
  Olcb_setup();
}

void loop() {
    bool activity = Olcb_loop();
    if (activity) {
        // blink blue to show that the frame was received
        blue.blink(0x1);
    }
    if (OpenLcb_can_active) { // set when a frame sent
        gold.blink(0x1);
        OpenLcb_can_active = false;
    }
    // handle the status lights  
    blue.process();
    gold.process();
}
