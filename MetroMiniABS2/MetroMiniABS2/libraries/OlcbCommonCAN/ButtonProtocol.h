#ifndef BUTTONPROTOCOL_h
#define BUTTONPROTOCOL_h

#include <ButtonLed.h>

/**
 * Class for Remote Button Protocol.
 *
 * Takes pointer to Blue and Gold ButtonLeds, plus an array of ButtonLeds to serve
 *
 * check() assumes that address has already been checked, and does not
 * verify it again.
 */

class ButtonLed;
class OpenLcbCanBuffer;

class ButtonProtocol {
  public:

  ButtonProtocol(ButtonLed** buttons, uint8_t nButtons, ButtonLed* blue, ButtonLed* gold);
  
  void check();
  
  bool receivedFrame(OpenLcbCanBuffer* rcv);
  
  private:

  ButtonLed** buttons;
  uint8_t nButtons;
  ButtonLed* blue;
  ButtonLed* gold;
  
};

#endif
