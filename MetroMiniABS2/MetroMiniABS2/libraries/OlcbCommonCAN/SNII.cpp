#include <string.h>

#include "SNII.h"

#include "LinkControl.h"
#include "OpenLcbCanBuffer.h"
#include "OpenLcbCan.h"

#include "logging.h"

static OpenLcbCanBuffer* buffer;
static uint8_t const_count;
static uint8_t var_offset;
static LinkControl* link;
static uint16_t dest;
static uint8_t ptr;

static uint8_t state;
#define STATE_CONST 1
#define STATE_FLAG 2
#define STATE_NAME 3
#define STATE_COMMENT 4
#define STATE_DONE 5

#define MTI_SNII_REQUEST 0xDE8
#define MTI_SNII_REPLY   0xA08

/**
 * Handle Simple Node Identification Information protocol
 *
 * This implementation relies on the getRead(address, space) call
 * to find the specific value, including any special handling of
 * program space, EEPROM, etc as needed inside the target node.
 */

extern "C" const uint8_t getRead(uint32_t address, int space);

void SNII_setup(uint8_t count, uint8_t offset, OpenLcbCanBuffer* b, LinkControl* li) {
      const_count = count;
      var_offset = offset;
      buffer = b;
      link = li;
      state = STATE_DONE;
  }
  
const uint8_t SNII_nextByte() { 
    uint8_t c;
    switch (state) {
    case STATE_CONST:
        c = getRead(ptr++, 0xFC);
        if (ptr >= const_count) state = STATE_FLAG;
        return c;
    case STATE_FLAG:
        state = STATE_NAME;
        ptr = 0;
        return 1;
    case STATE_NAME:
        c = getRead(ptr++, 0xFB);
        if (c==0) {
            state = STATE_COMMENT;
            ptr = var_offset;
        }
        return c;
    case STATE_COMMENT:
        c = getRead(ptr++, 0xFB);
        if (c==0) state = STATE_DONE;
        return c;
    default:
        return 0;
    }
}

void SNII_check() {
    if ( state != STATE_DONE ) {
        if (OpenLcb_can_xmt_ready(buffer)) {
            buffer->setOpenLcbMTI(MTI_SNII_REPLY);
            buffer->setDestAlias(dest);
            uint8_t i;
            for (i = 2; i<8; i++ ) {
                buffer->data[i] = SNII_nextByte();
                if (state == STATE_DONE) {
                    i++;
                    break;
                }
            }
            buffer->length = i;
            OpenLcb_can_queue_xmt_immediate(buffer);  // checked previously
        }
    }
    return;
}


bool SNII_receivedFrame(OpenLcbCanBuffer* rcv) {
    if ( rcv->isOpenLcbMTI(MTI_SNII_REQUEST) )  { 
        // check if available to send
        if (state == STATE_DONE) {
            // OK, start process
            ptr = 0;
            state = STATE_CONST;
            dest = rcv->getSourceAlias();
            
            return true;
        } else {
            // busy already, skip & ask for resend
            link->rejectMessage(rcv,0x1000);
            return true;
        }
    }
    return false;
}

