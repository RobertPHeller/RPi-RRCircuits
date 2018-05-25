#ifndef BG_h
#define BG_h

#include <ButtonLed.h>

/**
 * Class for Blue/Gold configuration.
 * Works with a PCE instance to do the actual operations.
 * Takes an array of buttons and a corresponding array of patterns
 * to blink for each event producer/consumer to be programmed
 *
 * 1) Push the blue button to light blue and start indexing through event slots.
 *    At each index, a button is given a provided pattern to indicate what
 *    the event slot does. After cycling through, blue goes out.
 * 2) Press gold to select the current slot to learn. Blue goes out and 
 *    you can start again with another if desired.
 * 3) To teach from an event slot, start by pressing gold. Gold comes on.
 * 4) Press blue to select the appropriate event slot to teach. Press gold
 *    again to send the message and reset to start.
 *
 * The gold LED is used to indicate board health.  Before init complete, it's
 * slow blinking 50% on and 50% off.  After init complete, it winks on periodically.
 *
 * A long (>3 seconds) press of just the blue button erases any learn nominations made.
 *
 * A long (>5 seconds) press of blue _and_ gold is a factory reset.
 * Pressing both at the same time for less than 5 seconds sends an ident message.
 *
 */

class PCE;
class ButtonLed;
class OpenLcbCanBuffer;

class BG {
  public:

  BG(PCE* pce, ButtonLed** buttons, long* patterns, uint8_t nEvents, ButtonLed* blue, ButtonLed* gold, OpenLcbCanBuffer* buffer);
  
  void check();
  
  private:

  PCE* pce;
  ButtonLed** buttons;
  long* patterns;
  uint8_t nEvents;
  ButtonLed* blue;
  ButtonLed* gold;
  OpenLcbCanBuffer* buffer;

  int8_t index;
  bool lastBlue;
  bool lastGold;
  bool started;
  
  void sendIdent();
  void factoryReset();  // ToDo: better name!  Not really a true "factory reset"
  
};

#endif
