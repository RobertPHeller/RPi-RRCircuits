#include <string.h>

#include "PCE.h"

#include "NodeID.h"
#include "EventID.h"
#include "Event.h"
#include "LinkControl.h"
#include "OpenLcbCanBuffer.h"

#include "logging.h"

// Mark as waiting to have Identify sent
#define IDENT_FLAG 0x01
// Mark produced event for send
#define PRODUCE_FLAG 0x02
// Mark entry as really empty, ignore
#define EMPTY_FLAG 0x04
// Mark entry to written from next learn message
#define LEARN_FLAG 0x08
// Mark entry to send a learn message
#define TEACH_FLAG 0x10


PCE::PCE(Event* evts, int nEvt, OpenLcbCanBuffer* b, NodeID* node, void (*cb)(int i), void (*st)(), LinkControl* li) {
      events = evts;
      nEvents = nEvt;
      buffer = b;
      nid = node;
      callback = cb;
      store = st;
      link = li;
       
      // mark as needing transmit of IDs, otherwise not interesting
      // ToDo: Is this needed if requiring newEvent?
      for (int i = 0; i < nEvents; i++) {
         if (events[i].flags & ( Event::CAN_PRODUCE_FLAG | Event::CAN_CONSUME_FLAG ))
            events[i].flags = IDENT_FLAG;
      }
      sendEvent = 0;
      
  }
  
  void PCE::produce(int i) {
    // ignore if not producer
    if ((events[i].flags & Event::CAN_PRODUCE_FLAG) == 0) return;
    // mark for production
    events[i].flags |= PRODUCE_FLAG;
    sendEvent = sendEvent<i ? sendEvent : i;
  }
  
  void PCE::check() {
     // see in any replies are waiting to send
     while (sendEvent < nEvents) {
         // OK to send, see if marked for some cause
         // ToDo: This only sends _either_ producer ID'd or consumer ID'd, not both
         if ( (events[sendEvent].flags & (IDENT_FLAG | Event::CAN_PRODUCE_FLAG)) == (IDENT_FLAG | Event::CAN_PRODUCE_FLAG)) {
           events[sendEvent].flags &= ~IDENT_FLAG;    // reset flag
           buffer->setProducerIdentified(&events[sendEvent]);
           OpenLcb_can_queue_xmt_wait(buffer);  // wait until buffer queued, but OK due to earlier check
           break; // only send one from this loop
         } else if ( (events[sendEvent].flags & (IDENT_FLAG | Event::CAN_CONSUME_FLAG)) == (IDENT_FLAG | Event::CAN_CONSUME_FLAG)) {
           events[sendEvent].flags &= ~IDENT_FLAG;    // reset flag
           buffer->setConsumerIdentified(&events[sendEvent]);
           OpenLcb_can_queue_xmt_wait(buffer);  // wait until buffer queued, but OK due to earlier check
           break; // only send one from this loop
         } else if (events[sendEvent].flags & PRODUCE_FLAG) {
           events[sendEvent].flags &= ~PRODUCE_FLAG;    // reset flag
           buffer->setPCEventReport(&events[sendEvent]);
           handlePCEventReport(buffer);
           OpenLcb_can_queue_xmt_wait(buffer);  // wait until buffer queued, but OK due to earlier check
           break; // only send one from this loop
         } else if (events[sendEvent].flags & TEACH_FLAG) {
           events[sendEvent].flags &= ~TEACH_FLAG;    // reset flag
           buffer->setLearnEvent(&events[sendEvent]);
           handleLearnEvent(buffer);
           OpenLcb_can_queue_xmt_wait(buffer);  // wait until buffer queued, but OK due to earlier check
           break; // only send one from this loop
         } else {
           // just skip
           sendEvent++;
         }
     }
  }
  
  void PCE::newEvent(int index, bool p, bool c) {
    events[index].flags |= IDENT_FLAG;
    sendEvent = sendEvent < index ? sendEvent : index;
    if (p) events[index].flags |= Event::CAN_PRODUCE_FLAG;
    if (c) events[index].flags |= Event::CAN_CONSUME_FLAG;
  }
  
  void PCE::markToLearn(int index, bool mark) {
    if (mark)
        events[index].flags |= LEARN_FLAG;
    else
        events[index].flags &= ~LEARN_FLAG;
  }


bool PCE::isMarkedToLearn(int index) {
	return events[index].flags==LEARN_FLAG;
}

void PCE::sendTeach(Event e) {   /// DPH added for Clock
	buffer->setLearnEvent(&e);
	handleLearnEvent(buffer);
	OpenLcb_can_queue_xmt_wait(buffer);  // wait until buffer queued, but OK due to earlier check
}	

  
  void PCE::sendTeach(int index) {
    events[index].flags |= TEACH_FLAG;
    sendEvent = sendEvent < index ? sendEvent : index;
  }
  
  bool PCE::receivedFrame(OpenLcbCanBuffer* rcv) {
    if (rcv->isIdentifyConsumers()) {
        // see if we consume the listed event
        Event event;
        rcv->getEventID(&event);
        int index = 0;
        // find consumers of event
        while (-1 != (index = event.findIndexInArray(events, nEvents, index))) {
           // yes, we have to reply with ConsumerIdentified
           if (events[index].flags & Event::CAN_CONSUME_FLAG) {
               events[index].flags |= IDENT_FLAG;
               sendEvent = sendEvent < index ? sendEvent : index;
           }
           index++;
           if (index>=nEvents) break;
        }
    } else if (rcv->isIdentifyProducers()) {
        // see if we produce the listed event
        Event event;
        rcv->getEventID(&event);
        int index = 0;
        // find producers of event
        while (-1 != (index = event.findIndexInArray(events, nEvents, index))) {
           // yes, we have to reply with ProducerIdentified
           if (events[index].flags & Event::CAN_PRODUCE_FLAG) {
               events[index].flags |= IDENT_FLAG;
               sendEvent = sendEvent < index ? sendEvent : index;
           }
           index++;
           if (index>=nEvents) break;
        }
        // ToDo: add identify flags so that events that are both produced and consumed
        // have only one form sent in response to a specific request.
    } else if (rcv->isIdentifyEvents()) {
        // if so, send _all_ ProducerIdentified, ConsumerIdentified
        // via the "check" periodic call
        for (int i = 0; i < nEvents; i++) {
          events[i].flags |= IDENT_FLAG;
        }
        sendEvent = 0;  
    } else if (rcv->isPCEventReport()) {
        // found a PC Event Report, see if we consume it
        handlePCEventReport(rcv);
    } else if (rcv->isLearnEvent()) {
        // found a teaching frame, apply to selected
        handleLearnEvent(rcv);
    } else return false;
    return true;
  }

  void PCE::handlePCEventReport(OpenLcbCanBuffer* rcv) {
        Event event;
        rcv->getEventID(&event);
        int index = 0;
        while (-1 != (index = event.findIndexInArray(events, nEvents, index))) {
            if (events[index].flags & Event::CAN_CONSUME_FLAG) {
                // yes, notify our own code
                (*callback)(index);
            }
            index++;
            if (index>=nEvents) break;
        }
  }
  
  void PCE::handleLearnEvent(OpenLcbCanBuffer* rcv) {
        bool save = false;
        for (int i=0; i<nEvents; i++) {
            if ( (events[i].flags & LEARN_FLAG ) != 0 ) {
                rcv->getEventID(events+i);
                events[i].flags |= IDENT_FLAG; // notify new eventID
                events[i].flags &= ~LEARN_FLAG; // enough learning
                sendEvent = sendEvent < i ? sendEvent : i;
                save = true;
            }
        }
        if (save) (*store)();
  }  

