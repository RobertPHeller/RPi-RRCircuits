//==============================================================
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

ButtonLed blockocc(BLOCKOCC,LOW);
ButtonLed_nc nextblock(0); // No actual pin 
ButtonLed_nobutton signalgreen(SIGNALGREEN);
ButtonLed_nobutton signalyellow(SIGNALYELLOW);
ButtonLed_nobutton signalred(SIGNALRED);

Mast mast(&blockocc,&nextblock,&signalgreen,&signalyellow,&signalred);

#define NUM_EVENT 10

// next lines get "warning: only initialized variables can be placed into program memory area" due to GCC bug
extern "C" {
 const char configDefInfo[] PROGMEM = {
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
    &blockocc,&blockocc,
    &signalgreen,&signalgreen,
    &signalyellow,&signalyellow,
    &signalred,&signalred,
    &nextblock,&nextblock
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
 *     12 - 12+8*sizeof(Event)  EventID storage
 * 
 *     94 - 113     Node name (zero-term string, 20 bytes total)
 *     114 - 136     User comment (zero-term string, 24 bytes total)
 *
 *     12+150 - 12+150+8*sizeof(Event)  Event description strings
 *
 *************************************************** */

#define LAST_EEPROM 12+150+NUM_EVENT*sizeof(Event)

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
void store() { nm.store(&nodeid, events, NUM_EVENT); }

PCE pce(events, NUM_EVENT, &txBuffer, &nodeid, pceCallback, store, &link);

// Set up Blue/Gold configuration
BG bg(&pce, buttons, patterns, NUM_EVENT, &blue, &gold, &txBuffer);

bool states[] = {false, false, false, false}; // current input states; report when changed

int scanIndex = 0;
// On the assumption that the producers (inputs) and consumers (outputs) are consecutive, 
// these are used later to label the individual channels as producer or consumer
#define FIRST_PRODUCER_CHANNEL_INDEX    0
#define LAST_PRODUCER_CHANNEL_INDEX     0
#define FIRST_CONSUMER_CHANNEL_INDEX    1
#define LAST_CONSUMER_CHANNEL_INDEX     4

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
  nm.setup(&nodeid, events, NUM_EVENT, (uint8_t*) 0, (uint16_t)0, (uint16_t)LAST_EEPROM);  
  
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
