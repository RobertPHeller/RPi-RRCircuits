#include "OpenLcbCan.h"
#include "OpenLcbCanBuffer.h"
#include "LinkControl.h"
#include "OlcbStream.h"

#include "logging.h"

// ToDo: Implementation incomplete

OlcbStream::OlcbStream(OpenLcbCanBuffer* b, unsigned int (*cb)(uint8_t *tbuf, unsigned int length), LinkControl* ln) {
}

void OlcbStream::check() {
    // see if can send.
}

bool OlcbStream::receivedFrame(OpenLcbCanBuffer* rcv) {
    // check for init stream
    // check for stream data transfer headed here
    
    // check for init stream reply
    // check for stream ack to send more
    return false;
}

