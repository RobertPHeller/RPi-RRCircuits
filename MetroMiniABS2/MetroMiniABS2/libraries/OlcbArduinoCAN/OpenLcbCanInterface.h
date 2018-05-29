#ifndef OpenLcbCanInterface_h
#define OpenLcbCanInterface_h

/**
 * Provide generic CAN interface for OpenLcb code
 *
 * Heavily modelled on can.h, but intended to abstract
 * CAN access so that other hardware and software solutions
 * can be provided in the future.
 *
 */

class OpenLcbCanBuffer;

/**
 * Specific definitions
 *
 * This section attaches the general interface (see below)
 * to the specific can.h implementation
 * 
 * This version is for the can.h library from 
 * Fabian Greif, Roboterclub Aachen e.V., adapted
 * and extended by Alex Shepherd.
 *
 * Because of the way the Arduino IDE creates the -I include
 * search list, you need to #include a file from each
 * needed library in the top level file.  In the case of
 * this implementation, this is the can.h file in the CAN library.
 */
#include <can.h>

/**
 * General interface
 *
 * This section defines the general interface used by
 * the OpenLcb sample code
 */

// Buffer structures
class CanInterfaceBuffer : public tCAN {};

// Initialize CAN system
void OpenLcb_can_init();

// Can a (the) CAN buffer be used?  
// Generally, indicates the buffer can be immediately
// queued for transmit, so it make sense to prepare it now
bool OpenLcb_can_xmt_ready(OpenLcbCanBuffer* b);

// Queue a CAN frame for sending, if possible
// Returns true if queued, false if not currently possible
bool OpenLcb_can_queue_xmt_immediate(OpenLcbCanBuffer* b);

// Queue a CAN frame for sending; waits until it can queue
void OpenLcb_can_queue_xmt_wait(OpenLcbCanBuffer* b);

// Send a CAN frame, waiting until it has been sent
void OpenLcb_can_send_xmt(OpenLcbCanBuffer* b);

// Check whether all frames have been sent,
// a proxy for the link having gone idle
bool OpenLcb_can_xmt_idle();

// Make the oldest received CAN frame available,
// in the process removing it from the CAN subsystem.
// Return false (zero) if no frame available.
bool OpenLcb_can_get_frame(OpenLcbCanBuffer* b);

// set true on any transmit; used to inform externals
extern bool OpenLcb_can_active;

#endif
