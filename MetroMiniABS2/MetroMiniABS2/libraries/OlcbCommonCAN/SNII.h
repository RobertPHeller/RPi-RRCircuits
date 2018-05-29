#ifndef SNII_h
#define SNII_h

/**
 * Handle Simple Node Identification Information protocol
 *
 */

class OpenLcbCanBuffer;
class LinkControl;

#include <stdint.h>

/**
* Call once to record constants and do needed setup
*
*   count:              Length of constant section in 0xFC memory space
*   var_comment_offset: Starting address of "comment" user-defined value in 0xFB memory space
*
*/
void SNII_setup(uint8_t count, uint8_t var_comment_offset, OpenLcbCanBuffer* b, LinkControl* link);
  
/**
* Handle any routine processing that needs to be done.
* Go through this in loop() to e.g. send pending messages
*/
void SNII_check();
  
/**
* When a CAN frame for this node is received, it should
* be transferred to the SNII object via this method
* so that it can handle the output protocol
*
* Returns true if frame was SNII and processed.
* 
* Assumes destination address check has already passed previously
*/
bool SNII_receivedFrame(OpenLcbCanBuffer* rcv);
      
#endif
