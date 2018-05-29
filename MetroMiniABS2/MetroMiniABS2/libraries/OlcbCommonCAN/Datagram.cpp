#include <string.h>

#include "OpenLcbCan.h"
#include "OpenLcbCanBuffer.h"
#include "LinkControl.h"
#include "Datagram.h"

#include<stdio.h>

#include "logging.h"


Datagram::Datagram(OpenLcbCanBuffer* b, unsigned int (*cb)(uint8_t tbuf[DATAGRAM_LENGTH], unsigned int length,  unsigned int from), LinkControl* ln) {
      buffer = b;
      link = ln;
      callback = cb; 
      reservedXmt = false;
      sendcount = -1;
      rptr = rbuf;
      receiving = false;
}


uint8_t* Datagram::getTransmitBuffer() {
    if (reservedXmt) return 0;
    reservedXmt = true;
    return tbuf;
}

void Datagram::sendTransmitBuffer(int length, unsigned int destNIDa) {
    if (!reservedXmt) {
        //logstr("error: Datagram::sendTransmitBuffer when not reserved");
        return;
    }
    sendcount = length;
    resendcount = length;
    tptr = tbuf;
    first = true;
    dest = destNIDa;
}

void Datagram::check() {
  // see if any datagram segments are waiting to send
  if (sendcount >= 0) { // 0 is valid length
    // check if can send now
    if (OpenLcb_can_xmt_ready(buffer)) {
      // load buffer
      if (first)
        buffer->setOpenLcbFormat(FRAME_FORMAT_ADDRESSED_DATAGRAM_FIRST);
      else
        buffer->setOpenLcbFormat(FRAME_FORMAT_ADDRESSED_DATAGRAM_MID);
      buffer->length = sendcount<8 ? sendcount : 8;
      for (int i = 0; i<buffer->length; i++)
          buffer->data[i] = *(tptr++);
      sendcount = sendcount - buffer->length;
      // if hit zero this time, done
      if (sendcount == 0) {
          sendcount = -1;
          if (first)
              buffer->setOpenLcbFormat(FRAME_FORMAT_ADDRESSED_DATAGRAM_ALL);
          else
              buffer->setOpenLcbFormat(FRAME_FORMAT_ADDRESSED_DATAGRAM_LAST);
      }
      // and send it
      buffer->setDestAlias(dest);
      first = false;
      OpenLcb_can_queue_xmt_wait(buffer);  // wait until buffer queued, but OK due to earlier check
      // and wait for reply
    }
  }
}

void Datagram::setDatagramReply(uint16_t srcAlias, uint16_t dstAlias) {
    buffer->init(srcAlias);
    buffer->setOpenLcbFormat(FRAME_FORMAT_NORMAL_MTI); // needed to set dest address
    buffer->setDestAlias(dstAlias);
}

void Datagram::setDatagramAck(uint16_t srcAlias, uint16_t dstAlias) {
    setDatagramReply(srcAlias, dstAlias);
    buffer->setOpenLcbMTI(MTI_DATAGRAM_RCV_OK);
}

void Datagram::setDatagramNak(uint16_t srcAlias, uint16_t dstAlias, uint16_t code) {
    setDatagramReply(srcAlias, dstAlias);
    buffer->setOpenLcbMTI(MTI_DATAGRAM_REJECTED);
    buffer->data[2] = (code>>8)&0xFF;
    buffer->data[3] = code&0xFF;
    buffer->length = 4;
}

bool Datagram::receivedFrame(OpenLcbCanBuffer* rcv) {
    // conditionally check for link-level frames that stop reception
    if (receiving) {
        if (rcv->isAMR(fromAlias) || rcv->isAMD(fromAlias)) {
            // yes, abort reception
            rptr = rbuf;
            receiving = false;
            return false;
        }
    }
    // check for datagram reply, which can free buffer

     // TODO also need to check that this is _from_ the node that's sending the datagram
     // as a redundant check (shouldn't be anybody else sending a reply right now, though)

    // for this node, check meaning
    if (rcv->isOpenLcbMTI(MTI_DATAGRAM_RCV_OK) ) { // datagram ACK
        // release reserve
        reservedXmt = false;
        return true;
    } else if (rcv->isOpenLcbMTI(MTI_DATAGRAM_REJECTED) ) { // datagram NAK
        // check permanent or temporary
        if (rcv->length < 6 || rcv->data[2] & (DATAGRAM_REJECTED_RESEND_MASK >> 8)) {
            // temporary, set up for resend
            sendcount = resendcount;
            tptr = tbuf;
            first = true;    
            return true;
        } else {
            // permanent, drop; nothing else to do?
            // TODO signal permanent error somehow     
            // release reserve
            reservedXmt = false;
            return true;
        }
    }
    // check for datagram fragment received
    else if ( rcv->isFrameTypeOpenLcb() &&
              ((rcv->getOpenLcbFormat() == FRAME_FORMAT_ADDRESSED_DATAGRAM_ALL) 
            || (rcv->getOpenLcbFormat() == FRAME_FORMAT_ADDRESSED_DATAGRAM_FIRST)
            || (rcv->getOpenLcbFormat() == FRAME_FORMAT_ADDRESSED_DATAGRAM_MID)
            || (rcv->getOpenLcbFormat() == FRAME_FORMAT_ADDRESSED_DATAGRAM_LAST)
          )) {
         // can this start reception, e.g. we're not already receiving?
         if (
               (rcv->getOpenLcbFormat() == FRAME_FORMAT_ADDRESSED_DATAGRAM_ALL)
            || (rcv->getOpenLcbFormat() == FRAME_FORMAT_ADDRESSED_DATAGRAM_FIRST) 
            ) {
            if (receiving) {
                // already receiving, this is an error, reply if last
                if (rcv->getOpenLcbFormat() == FRAME_FORMAT_ADDRESSED_DATAGRAM_ALL ) {
                    // TODO: Need a more robust scheduling method here
                    setDatagramNak(link->getAlias(), rcv->getSourceAlias(), DATAGRAM_REJECTED_BUFFER_FULL);
                    OpenLcb_can_queue_xmt_wait(buffer);  // wait until buffer queued _WITHOUT_ prior check
                }
                return true;
                // TODO send error response some how; will return false be enough?
            } else {
                // not receiving, start reception and continue processing
                fromAlias = rcv->getSourceAlias();
                receiving = true;
            }
         } else {
            // not a start segment, make sure we're receiving
            if (!receiving) {
                // missed the start frame? if last, tell
                if ( (rcv->getOpenLcbFormat() == FRAME_FORMAT_ADDRESSED_DATAGRAM_ALL)
                    || (rcv->getOpenLcbFormat() == FRAME_FORMAT_ADDRESSED_DATAGRAM_LAST)
                    ) {
                    // TODO: Need a more robust scheduling method here
                    setDatagramNak(link->getAlias(), rcv->getSourceAlias(), DATAGRAM_REJECTED_OUT_OF_ORDER);
                    OpenLcb_can_queue_xmt_wait(buffer);  // wait until buffer queued _WITHOUT_ prior check
                }
                return true;
                // TODO send error response some how; will return false be enough?
            }
         }        
         // this is for us, is it part of a currently-being received datagram?
         if (fromAlias != rcv->getSourceAlias()) {
            // no - if last, send reject temporarily; done immediately with wait
            if ( (rcv->getOpenLcbFormat() == FRAME_FORMAT_ADDRESSED_DATAGRAM_ALL)
                || (rcv->getOpenLcbFormat() == FRAME_FORMAT_ADDRESSED_DATAGRAM_LAST)
                ) {
                // TODO: Need a more robust scheduling method here
                setDatagramNak(link->getAlias(), rcv->getSourceAlias(), DATAGRAM_REJECTED_BUFFER_FULL);
                OpenLcb_can_queue_xmt_wait(buffer);  // wait until buffer queued _WITHOUT_ prior check
            }
            return true;
         }
         // this is a datagram fragment, store into the buffer
         for (int i=0; i<rcv->length; i++) {
            *(rptr++) = rcv->data[i];
         }
         // is the end of the datagram?
         if ( ( rcv->getOpenLcbFormat() == FRAME_FORMAT_ADDRESSED_DATAGRAM_ALL) 
            || ( rcv->getOpenLcbFormat() == FRAME_FORMAT_ADDRESSED_DATAGRAM_LAST )
            ) {
            // 
            unsigned int length = rptr-rbuf;
            // callback; result is error code or zero
            unsigned int result = (*callback)(rbuf, length, rcv->getSourceAlias());
            rptr = rbuf;
            receiving = false;
            if (result == 0) {
                // send OK; done immediately with wait
                // TODO: Need a more robust method here
                // load buffer
                setDatagramAck(link->getAlias(), rcv->getSourceAlias());
            } else {
                // not OK, send reject; done immediately with wait
                // TODO: Need a more robust method here
                setDatagramNak(link->getAlias(), rcv->getSourceAlias(), DATAGRAM_REJECTED_DATAGRAM_TYPE_NOT_ACCEPTED);
            }
            OpenLcb_can_queue_xmt_wait(buffer);  // wait until buffer queued _WITHOUT_ prior check
         }
         return true;
    }
    return false;
}
