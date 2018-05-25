#include <string.h>

#include "OpenLcbCanBuffer.h"
#include "ButtonProtocol.h"

#define MTI_BP_REQUEST 0x948
#define MTI_BP_REPLY   0x949

ButtonProtocol::ButtonProtocol(ButtonLed** bC, uint8_t n, ButtonLed* bptr, ButtonLed* gptr) {
      buttons = bC;
      nButtons = n;
      blue = bptr;
      gold = gptr;
}
  
void ButtonProtocol::check() {
}

bool ButtonProtocol::receivedFrame(OpenLcbCanBuffer* rcv) {
    if ( rcv->isOpenLcbMTI(MTI_BP_REQUEST) )  { 
        // OK, start process
        return true;
    }
    return false;
}
