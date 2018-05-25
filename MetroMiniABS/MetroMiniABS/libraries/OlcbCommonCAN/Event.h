#ifndef Event_h
#define Event_h

#include "EventID.h"

class Event : public EventID {
  public: 

  Event();

  Event(bool produce, bool consume);
  
  Event(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7); 
  
  bool equals(Event* n);
  
  /**
   * Check to see if this object is equal
   * to any in an array of Events
   */
  Event* findEventInArray(Event* array, int len);
  
  int findIndexInArray(Event* array, int len, int start);
  
  // bit mask local flags
  int flags;
  
  // Mark entry as consumer
  static const int CAN_CONSUME_FLAG = 0x20;
  // Mark entry as producer
  static const int CAN_PRODUCE_FLAG = 0x40;
};

#endif
