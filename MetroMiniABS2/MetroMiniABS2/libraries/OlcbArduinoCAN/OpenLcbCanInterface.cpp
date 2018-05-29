
/**
 * Specific implementation for CAN MP2515
 */
#include <can.h>

//#include "OpenLcbCanInterface.h"
#include "OpenLcbCanBuffer.h"

void OpenLcb_can_init() {
    can_init(BITRATE_125_KBPS);

    // From Don Goodman-Wilson's libraries/OpenLCB/OLCB_CAN_Link.cpp
    // Filter out standard can frames using hardware to avoid random crash
    // upon receipt of large numbers of them.
#if defined (__AVR_AT90CAN128__) || defined (__AVR_AT90CAN64__) || defined (__AVR_AT90CAN32__)
	for(uint8_t i = 0; i < 15; ++i)
    {
    	CANPAGE = (i << 4);
    	CANIDM4 |= (1<<IDEMSK);
	}
#else
	//TODO figure out how to set up filters for MCP2515
#endif

}

// Can a (the) CAN buffer be used?  
// Generally, indicates the buffer can be immediately
// queued for transmit, so it make sense to prepare it now
bool OpenLcb_can_xmt_ready(OpenLcbCanBuffer* b) {
  // use only MP2515 buffer 0 and 1 to ensure that
  // tranmissions take place in order
#if defined(SUPPORT_MCP2515) && (SUPPORT_MCP2515 == 1)  
  uint8_t status = can_buffers_status();
  // Check to see if Tx Buffer 0 or 1 is free	
  if ((status & (ST_TX0REQ|ST_TX1REQ)) == (ST_TX0REQ|ST_TX1REQ))
    return false;  //  Both at full
  else
    return true;   // at least one has space
#elif defined(SUPPORT_AT90CAN) && (SUPPORT_AT90CAN == 1)
    return can_check_free_buffer();  //  Both at full
#endif
}

// Queue a CAN frame for sending, if possible
// Returns true if queued, false if not currently possible
bool OpenLcb_can_queue_xmt_immediate(OpenLcbCanBuffer* b) {
  if (!OpenLcb_can_xmt_ready(b)) return false;
  // buffer available, queue for send
  OpenLcb_can_active = true;
  can_send_message(b);
  return true;
}

// Queue a CAN frame for sending; spins until it can queue
void OpenLcb_can_queue_xmt_wait(OpenLcbCanBuffer* b) {
  while (!OpenLcb_can_queue_xmt_immediate(b)) {};
}

// Send a CAN frame, waiting until it has been sent
void OpenLcb_can_send_xmt(OpenLcbCanBuffer* b) {
  OpenLcb_can_queue_xmt_wait(b);
  // wait for sent
  while (!OpenLcb_can_xmt_idle()) {}
}

// Check whether all frames have been sent,
// a proxy for the link having gone idle
bool OpenLcb_can_xmt_idle() {
#if defined(SUPPORT_MCP2515) && (SUPPORT_MCP2515 == 1)  
  uint8_t status = can_buffers_status();
  // Check to see if Tx Buffer 0,1 and 2 are all free	
  if ((status & (ST_TX0REQ|ST_TX1REQ|ST_TX2REQ)) == 0)
    return true;   // All empty, nothing to send
  else
    return false;  // Any full
#elif defined(SUPPORT_AT90CAN) && (SUPPORT_AT90CAN == 1)
    return can_check_free_buffer();  //  Both at full
#endif
}

// Make the oldest received CAN frame available,
// in the process removing it from the CAN subsystem.
// Return false (zero) if no frame available.
bool OpenLcb_can_get_frame(OpenLcbCanBuffer* b) {
  return can_get_message(b);
}

bool OpenLcb_can_active;

