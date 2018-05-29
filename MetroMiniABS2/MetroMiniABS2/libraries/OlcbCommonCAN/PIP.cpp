#include <string.h>

#include "PIP.h"

#include "LinkControl.h"
#include "OpenLcbCanBuffer.h"
#include "OpenLcbCan.h"

static LinkControl* link;
static OpenLcbCanBuffer* buffer;
static uint16_t dest;
static bool queued;

#define MTI_PIP_REQUEST 0x828
#define MTI_PIP_REPLY   0x668

extern "C" {
extern uint8_t protocolIdentValue[6];
}

void PIP_setup(OpenLcbCanBuffer* b, LinkControl* li) {
      buffer = b;
      link = li;
      queued = false;
  }
  
  void PIP_check() {
    if (queued) {
        if (OpenLcb_can_xmt_ready(buffer)) {
            buffer->setOpenLcbMTI(MTI_PIP_REPLY);
            buffer->setDestAlias(dest);
            memcpy( &(buffer->data[0])+2, protocolIdentValue, 6); 
            buffer->length = 8;
            OpenLcb_can_queue_xmt_immediate(buffer);  // checked previously
            queued = false;
        }
    }
    return;
  }
    
  bool PIP_receivedFrame(OpenLcbCanBuffer* rcv) {
    if ( rcv->isOpenLcbMTI(MTI_PIP_REQUEST) )  { 
        // previously checked for this node
        // check to start bit set active, ignore if not
        if ( ((rcv->data[0]) & 0x20) != 0) return true;
        queued = true;
        dest = rcv->getSourceAlias();
        return true;
    }
    return false;
  }

