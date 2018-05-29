#ifndef PCE_h
#define PCE_h

/**
 * Class for handling P/C Events.
 *
 * Maintains a single list of events, which could be
 * either produced or consumed (indicated by flags).
 *<p>
 * Basic state machine handles e.g. Request Producer/Consumer/event
 * messages for you, including at initialization.
 */

#include "EventID.h"
#include "Event.h"

class NodeID;
class OpenLcbCanBuffer;
class LinkControl;

class PCE {
  public:

  PCE(Event* events, int nEvents, OpenLcbCanBuffer* b, NodeID* nid, void (*callback)(int i), void (*store)(), LinkControl* link);
  
  /**
   * Produce the ith event
   * 
   * Return true if done; return false to require retry later.
   * Note: It's an error to do this to an event that's not
   * marked as for production.
   */
  void produce(int i);
  
  /**
   * Handle any routine processing that needs to be done.
   * Go through this in loop() to e.g. send pending messages
   */
  void check();
  
  /**
   * When a CAN frame is received, it should
   * be transferred to the PCER object via this method
   * so that it can handle the verification protocol.
   */
  bool receivedFrame(OpenLcbCanBuffer* rcv);
  
  /**
   * A new event has been defined, and we should
   * do the necessary notifications, etc on the OpenLCB link
   *
   * index is the 0-based index of the newly defined
   * event in the array provided to the ctor earlier.
   */
  void newEvent(int index, bool produce, bool consume);
    
  /**
   * Mark a particular slot to acquire the event 
   * from the next learn message.
   * true marks, false unmarks.
   *
   * index is the 0-based index of the desired
   * event in the array provided to the ctor earlier.
   *
   * ToDo: This doesn't force the non-volatile memory
   * to be stored after the learn message is received.
   */
  void markToLearn(int index, bool mark);
  
  /**
   * Send a learn frame for a particular slot's event.
   *
   * index is the 0-based index of the desired
   * event in the array provided to the ctor earlier.
   */
  void sendTeach(int index);
  void sendTeach(Event e);
  bool isMarkedToLearn(int index);

private:
  Event* events;  // array
  int nEvents;
  LinkControl* link;
  OpenLcbCanBuffer* buffer;
  NodeID* nid;
  void (*callback)(int i);   // void callback(int index) pointer
  void (*store)();           // void store() pointer for storing config changes
  void handlePCEventReport(OpenLcbCanBuffer* rcv);
  void handleLearnEvent(OpenLcbCanBuffer* rcv);
  int sendEvent; // index of next identified event to send, or -1
};

#endif
