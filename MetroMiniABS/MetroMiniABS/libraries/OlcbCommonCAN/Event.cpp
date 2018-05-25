
#include "logging.h"
#include "Event.h"
#include "EventID.h"

Event::Event() {
}

Event::Event(bool produce, bool consume) {
    if (produce) flags |= Event::CAN_PRODUCE_FLAG;
    if (consume) flags |= Event::CAN_CONSUME_FLAG;
}

Event::Event(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7) 
  : EventID(b0, b1, b2, b3, b4, b5, b6, b7), flags(0){
}

bool Event::equals(Event* n) {
    return  EventID::equals(n);  // just check ID for now
}

/**
* Check to see if this object is equal
* to any in an array of Events
*/
Event* Event::findEventInArray(Event* array, int len) {
  for (int i = 0; i<len; i++) {
      if (equals(array+i)) return array+i;
  }
  return 0;
}

int Event::findIndexInArray(Event* array, int len, int start) {
  for (int i = start; i<len; i++) {
      if (equals(array+i)) return i;
  }
  return -1;
}

