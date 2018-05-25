#ifndef PIP_h
#define PIP_h

/**
 * Class for handling Protocol Identification Protocol
 *
 */

class OpenLcbCanBuffer;
class LinkControl;

#include <stdint.h>

void PIP_setup(OpenLcbCanBuffer* b, LinkControl* link);

/**
 * Handle any routine processing that needs to be done.
 * Go through this in loop() to e.g. send pending messages
 */
void PIP_check();

/**
 * When a CAN frame is received, it should
 * be transferred to the PIP object via this method
 * so that it can handle the output protocol
 */
bool PIP_receivedFrame(OpenLcbCanBuffer* rcv);

#endif
