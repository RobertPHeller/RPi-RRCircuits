#ifndef Datagram_h
#define Datagram_h

/**
 * Class for handling Datagrams
 *
 * This combines Receiver and Transmitter for now;
 * perhaps they need to be refactored separately later.
 *<p>
 * Basic state machine handles transmission and reception.
 */

#define DATAGRAM_LENGTH 70

// some error messages for datagram rejected response messages
// These top 16 bits after the datagram-rejected MTI
// The bottom are reserved, send as zero, ignore on receipt

#define DATAGRAM_REJECTED_PERMANENT_ERROR        0x1000
#define DATAGRAM_REJECTED_INFORMATION_LOGGED     0x1010
#define DATAGRAM_REJECTED_SOURCE_NOT_PERMITTED   0x1020
#define DATAGRAM_REJECTED_DATAGRAM_TYPE_NOT_ACCEPTED 0x1040
#define DATAGRAM_REJECTED_BUFFER_FULL            0x2000
#define DATAGRAM_REJECTED_OUT_OF_ORDER           0x6000

#define DATAGRAM_REJECTED_NO_RESEND_MASK         0x1000
#define DATAGRAM_REJECTED_RESEND_MASK            0x2000
#define DATAGRAM_REJECTED_TRANSPORT_ERROR_MASK   0x4000

class OpenLcbCanBuffer;
class LinkControl;

class Datagram {
  public:

  /**
   * Ask for the transmit buffer. If
   * it's in use, indicated by returning a 0 pointer,
   * you must repeat the request later
   */
  uint8_t* getTransmitBuffer();
  
  /**
   * Send the filled transmit buffer.
   */
  void sendTransmitBuffer(int length, unsigned int destID);
  
  /**
   * Handle any routine processing that needs to be done.
   * Go through this in loop() to e.g. send pending messages
   */
  void check();
  
  /**
   * When a CAN frame is received, it should
   * be transferred to the PCER object via this method
   * so that it can handle the verification protocol.
   */
  bool receivedFrame(OpenLcbCanBuffer* rcv);
  
  Datagram(OpenLcbCanBuffer* b, unsigned int (*callback)(uint8_t tbuf[DATAGRAM_LENGTH], unsigned int length, unsigned int from), LinkControl* link);
  
  private:
  OpenLcbCanBuffer* buffer;
  LinkControl* link;
  
  uint8_t tbuf[DATAGRAM_LENGTH];
  int sendcount;
  int resendcount;
  unsigned int dest;
  uint8_t* tptr;
  bool first;
  bool reservedXmt;  // buffer reserved for transmission
  bool receiving; 
  
  unsigned int fromAlias;
  
  uint8_t rbuf[DATAGRAM_LENGTH];
  uint8_t* rptr;
  
  void setDatagramReply(uint16_t srcAlias, uint16_t dstAlias);
  void setDatagramAck(uint16_t srcAlias, uint16_t dstAlias);
  void setDatagramNak(uint16_t srcAlias, uint16_t dstAlias, uint16_t code);
  
  // returns 0 for used, OK; > 0 for specific error message
  unsigned int (*callback)(uint8_t tbuf[DATAGRAM_LENGTH], unsigned int length, unsigned int from);   // void callback(int index) pointer
};

#endif
