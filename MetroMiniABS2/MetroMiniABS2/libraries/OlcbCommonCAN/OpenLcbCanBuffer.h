#ifndef OpenLcbCanBuffer_h
#define OpenLcbCanBuffer_h

class NodeID;
class EventID;
#include "OpenLcbCanInterface.h"

/**
 * Class to handle transforming OpenLCB (S9.6) frames to/from std CAN frames.
 * <p>
 * We're trying to localize the formating of frames to/from the node here,
 * so that only this class needs to change when/if the wire protocol changes.
 */
 class OpenLcbCanBuffer : public CanInterfaceBuffer {
  public: 
  
  // Initialize a buffer for transmission
  void init(uint16_t a);
  
  // Start of basic message structure

  void setFrameTypeCAN();
  bool isFrameTypeCAN();
  
  void setFrameTypeOpenLcb();
  bool isFrameTypeOpenLcb();
  
  void setVariableField(uint16_t f);
  uint16_t getVariableField();
  
  void setSourceAlias(uint16_t a);
  uint16_t getSourceAlias();

  // End of basic message structure
  
  // Start of CAN-level messages
  void setFrameTypeCAN(uint16_t alias, uint16_t varField);  
  void setCIM(uint8_t i, uint16_t testval, uint16_t alias);
  bool isCIM();
  
  void setRIM(uint16_t alias);
  bool isRIM();

  void setAMD(uint16_t alias,NodeID* nid);
  bool isAMD(uint16_t alias);
  
  void setAMR(uint16_t alias,NodeID* nid);
  bool isAMR(uint16_t alias);

  // End of CAN-level messages
  
  // Start of OpenLCB format support
  
  uint8_t getOpenLcbFormat();  // 0-7 value
  void setOpenLcbFormat(uint8_t i);   // 0-7 value

  void setDestAlias(uint16_t a);  // needs format already set; sets length >= 2 if needed
  uint16_t getDestAlias();
  
  void setOpenLcbMTI(uint16_t mti);  // 12 bit MTI, but can take 16
  uint16_t getOpenLcbMTI(); 
  bool isOpenLcbMTI(uint16_t mti);  // efficient check
  
  bool isForHere(uint16_t thisAlias);  // include OpenLCB messages and CAN control frames

  bool isMsgForHere(uint16_t thisAlias);  // OpenLCB messages only
  
  bool isAddressedMessage();  // OpenLCB messages only

  void getEventID(EventID* evt); // loads return value into given variable
  void getNodeID(NodeID* nid); // loads return value into given variable
  bool matchesNid(NodeID* nid);

  // End of OpenLCB format support
  
  // Start of OpenLCB messages
  //
  // These neither set nor test the source/destination aliases.
  // Check separately for whether this frame is addressed to 
  // the current node (or global).
  //
  void setInitializationComplete(NodeID* nid);
  bool isInitializationComplete();

  void setPCEventReport(EventID* eid);
  bool isPCEventReport();
  
  void setLearnEvent(EventID* eid);
  bool isLearnEvent();
    
  bool isVerifyNID();

  void setVerifiedNID(NodeID* nid);
  bool isVerifiedNID();

  void setOptionalIntRejected(OpenLcbCanBuffer* rcv, uint16_t code);
  
  bool isIdentifyConsumers();
  
  void setConsumerIdentified(EventID* eid);
  
  // Mask uses an EventID data structure; 1 bit means mask out when routing
  void setConsumerIdentifyRange(EventID* eid, EventID* mask);

  bool isIdentifyProducers();

  void setProducerIdentified(EventID* eid);

  // Mask uses an EventID data structure; 1 bit means mask out when routing
  void setProducerIdentifyRange(EventID* eid, EventID* mask);

  bool isIdentifyEvents();

  bool isDatagramFrame();
  bool isLastDatagramFrame();
    
  private: 
  unsigned int nodeAlias;   // Initialization complete sets, all later use

  // service routines

  // copy content (0-7) to a previously-allocated Eid
  void loadFromEid(EventID* eid);
};

#endif
