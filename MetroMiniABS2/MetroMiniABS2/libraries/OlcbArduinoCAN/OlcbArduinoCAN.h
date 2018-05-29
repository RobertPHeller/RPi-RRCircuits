/** **********************************************
*  Provide core implementation for sketches using
*  the OlcbCommonCAN and OlcbArduinoCAN libraries.
*
*  This .h file should included in the main sketch
*  file _only_.
*
*  Conditional compilation symbols:
*   OLCB_USE_STREAMS   - #define to provide basic stream support
*   OLCB_USE_BLUE_GOLD - #define to include support for blue/gold buttons
*                                and programming; requires BLUE and GOLD
*                                to be defined as pin numbers
*
*
*/

// OpenLCB definitions
#include "OpenLcbCanInterface.h"
#include "OpenLcbCanBuffer.h"
#include "NodeID.h"
#include "EventID.h"
#include "Event.h"

// specific OpenLCB implementations
#include "LinkControl.h"

#ifndef OLCB_NO_DATAGRAM
#include "Datagram.h"
#endif

#ifndef OLCB_NO_STREAM
#include "OlcbStream.h"
#endif

#ifndef OLCB_NO_MEMCONFIG
#include "Configuration.h"
#endif

#include "NodeMemory.h"
#include "PCE.h"
#include "PIP.h"
#include "SNII.h"
#include "BG.h"
#include "ButtonLed.h"

OpenLcbCanBuffer     rxBuffer;	// CAN receive buffer
OpenLcbCanBuffer     txBuffer;	// CAN send buffer
OpenLcbCanBuffer*    ptxCAN;

LinkControl link(&txBuffer, &nodeid);

extern "C" {

extern const uint8_t getRead(uint32_t address, int space);
extern void getWrite(uint32_t address, int space, uint8_t val);
extern void restart();

};

#ifndef OLCB_NO_STREAM
unsigned int streamRcvCallback(uint8_t *rbuf, unsigned int length);
OlcbStream str(&txBuffer, streamRcvCallback, &link);
unsigned int resultcode = 1;  // dummy temp value

unsigned int streamRcvCallback(uint8_t *rbuf, unsigned int length){
  return resultcode;  // return pre-ordained result
}
#else
#define str 0
#endif

#ifndef OLCB_NO_DATAGRAM
unsigned int datagramCallback(uint8_t *rbuf, unsigned int length, unsigned int from);
Datagram dg(&txBuffer, datagramCallback, &link);
Configuration cfg(&dg, &str, &getRead, &getWrite, (void (*)())0);

unsigned int datagramCallback(uint8_t *rbuf, unsigned int length, unsigned int from){
  // invoked when a datagram arrives
  //logstr("consume datagram of length ");loghex(length); lognl();
  //for (int i = 0; i<length; i++) printf("%x ", rbuf[i]);
  //printf("\n");
  // pass to consumers
  return cfg.receivedDatagram(rbuf, length, from);
}
#endif

extern PCE pce;
extern BG bg;

// invoke from setup()
void Olcb_setup() {
  // Init protocol blocks
  PIP_setup(&txBuffer, &link);
  SNII_setup((uint8_t)sizeof(SNII_const_data), SNII_var_offset, &txBuffer, &link);

  // Initialize OpenLCB CAN connection
  OpenLcb_can_init();
  
  // Initialize OpenLCB CAN link controller
  link.reset();
}



extern "C" {
void produceFromInputs();
};

// invoke from loop()
// returns true is a frame was processed, to allow indicating activity
bool Olcb_loop() {  
  // check for input frames, acquire if present
  bool rcvFramePresent = OpenLcb_can_get_frame(&rxBuffer);
  // standard frames suppressed before here
  
  // process link control first
  link.check();
  bool handled = false;  // start accumulating whether it was processed or skipped
  if (rcvFramePresent) {
    // see if recieved frame changes link state
    handled = link.receivedFrame(&rxBuffer);
  }

  // if link is initialized, higher-level operations possible
  if (link.linkInitialized()) {
     // if frame present, pass to handlers
     if (rcvFramePresent && rxBuffer.isForHere(link.getAlias()) ) {
#ifndef OLCB_NO_DATAGRAM
        handled |= dg.receivedFrame(&rxBuffer);  // has to process frame level
#endif
        if(rxBuffer.isFrameTypeOpenLcb()) {  // skip if not OpenLCB message (already for here)
            
            handled |= pce.receivedFrame(&rxBuffer);
        
#ifndef OLCB_NO_STREAM
            handled |= str.receivedFrame(&rxBuffer); // suppress stream for space
#endif
            handled |= PIP_receivedFrame(&rxBuffer);
            handled |= SNII_receivedFrame(&rxBuffer);
            
            // if not handled, by default reject
            if (!handled && rxBuffer.isAddressedMessage()) link.rejectMessage(&rxBuffer, 0x2000);
        }
     }
     // periodic processing of any internal state change needs
     pce.check();
     
#ifndef OLCB_NO_DATAGRAM
     dg.check();
#endif
#ifndef OLCB_NO_STREAM
     str.check();
#endif
#ifndef OLCB_NO_MEMCONFIG
     cfg.check();
#endif
     bg.check();
     PIP_check();
     SNII_check();
     produceFromInputs();
  }
  return rcvFramePresent;
}

 