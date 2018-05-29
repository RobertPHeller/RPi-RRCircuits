#ifndef NodeMemory_h
#define NodeMemory_h

/**
 * Class for persisting node memory
 * in a non-volative memory, e.g. EEPROM.
 *
 * This class loads from and stores to EEPROM
 * a NodeID and two arrays of Events; it's up to you 
 * to store everything else.
 * 
 * The memory can either be 
 * blank, needing complete initialization or
 * OK at startup. Call setup(...) with a valid NodeID
 * to either load or create EventIDs.
 * 
 * If you change those, call store(...) when done.
 * 
 * When you "reset" the memory, you're 
 * putting _new_ unique EventIDs in place.
 *
 * The first four bytes of memory contain flag values:
 * 0xEE 0x55 0x5E 0xE5 - all memory valid, use as is
 * 0xEE 0x55 0x33 0xCC - Node ID valid, rest must be initialized
 * Any other flag means no memory valid.
 *
 * TODO: Add a "dirty" bit to make store logic easier for external code?
 *
 */
#include <stdint.h>

class NodeID;
class Event;
class EventID;

class NodeMemory {
  public:

  /**
   * Define starting address in EEPROM
   */
  NodeMemory(int startAddress);  // doesn't do anything
    
  /**
   * Make sure ready to go.  NodeID should have a default
   * value already in case this is the first time.
   *
   * events is address in RAM to load from EEPROM
   * copy N=extraBytes of memory from end of EEPROM to RAM.
   *
   * If the EEPROM is corrupt, all that is reloaded after  
   * N=clearBytes of memory is cleared to e.g. 
   * clear name strings. This count EEPROM address 0 (e.g. _not_ starting
   * at end of event strings; this is the full memory clear)   
   */
  void setup(NodeID* nid, Event* events, uint8_t numEvents, uint8_t* data, 
            uint16_t extraBytes, uint16_t clearBytes);
  
  /*
   * Move to a completely new set of values, e.g. a "default" reset
   * for OpenLCB. NodeID is not changed. EventIDs are loaded from
   * a new range.  N=clearBytes of memory is cleared to e.g. 
   * clear name strings counting from zero (e.g. _not_ starting
   * at end of event strings; this is the full memory clear)
   */
  void reset(NodeID* nid, Event* events, uint8_t numEvents, uint16_t clearBytes); 

  /** 
   * For debug and test, this forces the next
   * setup() to do a complete reload from initial
   * event values
   */
  static void forceInitAll();
    
  /** 
   * Reload a complete set of events via call to reset on next restart.
   */
  static void forceInitEvents();
    
  /*
   * Put modified contents back into memory.
   */
  void store(NodeID* nid, Event* events, uint8_t nunEvents); 
  void store(NodeID* nid, Event* events, uint8_t nunEvents, uint8_t* data, int extraBytes); 
  
  /*
   * Get a new, forever unique EventID and put in 
   * given EventID location. Does not do a EEPROM store,
   * which must be done separately. unique ID build using
   * this node's nodeID. 
   */
  void setToNewEventID(NodeID* nodeID, EventID* eventID);
  
  private:
  bool checkNidOK(); // check if memory tag says NID OK
  bool checkAllOK(); // check if memory tag says all OK
  void writeByte(int address, uint8_t value); // write only if needed
  
  int startAddress; // address of 1st byte in EEPROM
  uint8_t count; // count of EventIDs provided to date
};

#endif
