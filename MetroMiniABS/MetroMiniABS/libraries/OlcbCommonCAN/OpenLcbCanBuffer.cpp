#include <string.h>

// The following line is needed because the Arduino environment 
// won't search a library directory unless the library is included 
// from the top level file (this file)
#include "can.h"

#include "OpenLcbCan.h"
#include "OpenLcbCanInterface.h"
#include "OpenLcbCanBuffer.h"
#include "NodeID.h"
#include "EventID.h"

// for definiton, see
// http://openlcb.sf.net/trunk/documents/can/index.html
// 
// In the following masks, bit 0 of the frame is 0x10000000L
//

// bit 1
#define MASK_FRAME_TYPE 0x08000000L

// bit 17-28
#define MASK_SRC_ALIAS 0x00000FFFL

// bit 2-16
#define MASK_VARIABLE_FIELD 0x07FFF000L
#define SHIFT_VARIABLE_FIELD 12

// bit 2-4, at the top of the variable field
#define MASK_OPENLCB_FORMAT 0x07000L
#define SHIFT_OPENLCB_FORMAT 12

  void OpenLcbCanBuffer::init(uint16_t alias) {
    // set default header: extended frame w low priority
    flags.extended = 1;
    // no data yet
    length = 0;
    // all bits in header default to 1 except MASK_SRC_ALIAS
    id = 0x1FFFF000 | (alias & MASK_SRC_ALIAS);
  }

  // start of basic message structure

  void OpenLcbCanBuffer::setSourceAlias(uint16_t a) {
    id &= ~MASK_SRC_ALIAS;
    id = id | (a & MASK_SRC_ALIAS);
  }
  
  uint16_t OpenLcbCanBuffer::getSourceAlias() {
      return id & MASK_SRC_ALIAS;
  }

  void OpenLcbCanBuffer::setFrameTypeCAN() {
    id &= ~MASK_FRAME_TYPE;     
  }
  
  bool OpenLcbCanBuffer::isFrameTypeCAN() {
    return (id & MASK_FRAME_TYPE) == 0x00000000L;
  }

  void OpenLcbCanBuffer::setFrameTypeOpenLcb() {
    id |= MASK_FRAME_TYPE;     
  }
  
  bool OpenLcbCanBuffer::isFrameTypeOpenLcb() {
    return (id & MASK_FRAME_TYPE) == MASK_FRAME_TYPE;
  }

  void OpenLcbCanBuffer::setVariableField(uint16_t f) {
    id &= ~MASK_VARIABLE_FIELD;
    uint32_t temp = f;  // ensure 32 bit arithmetic
    id |=  ((temp << SHIFT_VARIABLE_FIELD) & MASK_VARIABLE_FIELD);
  }

  uint16_t OpenLcbCanBuffer::getVariableField() {
    return (id & MASK_VARIABLE_FIELD) >> SHIFT_VARIABLE_FIELD;
  }
  
  // end of basic message structure
  
  // start of CAN-level messages
 
  void OpenLcbCanBuffer::setFrameTypeCAN(uint16_t alias, uint16_t var) {
    init(alias);
    setFrameTypeCAN();
    setVariableField(var);
    length=0;
  }

  void OpenLcbCanBuffer::setAMD(uint16_t alias,NodeID* nid) {
    setFrameTypeCAN(alias, AMD_VAR_FIELD);
    length=6;
    memcpy(data, nid->val, 6);
  }

  bool OpenLcbCanBuffer::isAMD(uint16_t alias) {
    return isFrameTypeCAN() && (getVariableField() == AMD_VAR_FIELD)
                && (alias == getSourceAlias());
  }

  void OpenLcbCanBuffer::setAMR(uint16_t alias,NodeID* nid) {
    setFrameTypeCAN(alias, AMR_VAR_FIELD);
    length=6;
    memcpy(data, nid->val, 6);
  }

  bool OpenLcbCanBuffer::isAMR(uint16_t alias) {
    return isFrameTypeCAN() && (getVariableField() == AMR_VAR_FIELD)
                && (alias == getSourceAlias());
  }

  void OpenLcbCanBuffer::setCIM(uint8_t i, uint16_t testval, uint16_t alias) {
    uint16_t var =  (( (0x7-i) & 7) << 12) | (testval & 0xFFF); 
    setFrameTypeCAN(alias, var);
  }

  bool OpenLcbCanBuffer::isCIM() {
    return isFrameTypeCAN() && (getVariableField()&0x7000) >= 0x4000;
  }

  void OpenLcbCanBuffer::setRIM(uint16_t alias) {
    setFrameTypeCAN(alias, RIM_VAR_FIELD);
  }

  bool OpenLcbCanBuffer::isRIM() {
      return isFrameTypeCAN() && getVariableField() == RIM_VAR_FIELD;
  }


  // end of CAN-level messages
  
  // start of OpenLCB format support
  
  void OpenLcbCanBuffer::setDestAlias(uint16_t a) {
     uint8_t format = getOpenLcbFormat();
     if (format == FRAME_FORMAT_NORMAL_MTI) {
        data[0] = (a>>8)&0xFF;
        data[1] = a&0xFF;
        if (length < 2) length = 2;
    } else {
        setVariableField(a | (format << SHIFT_OPENLCB_FORMAT));
    }
  }
  uint16_t OpenLcbCanBuffer::getDestAlias() {
     uint8_t format = getOpenLcbFormat();
     if (format == FRAME_FORMAT_NORMAL_MTI)
        return ((data[0]&0xF)<<8)|(data[1]&0xFF);
     else 
        return getVariableField() & 0xFFF;
  }

  void OpenLcbCanBuffer::setOpenLcbMTI(uint16_t mti) {
        setFrameTypeOpenLcb();
        setVariableField(mti | (FRAME_FORMAT_NORMAL_MTI<<SHIFT_OPENLCB_FORMAT));
  }
  
  uint16_t OpenLcbCanBuffer::getOpenLcbMTI() {
      return getVariableField() & 0xFFF;
  }

  bool OpenLcbCanBuffer::isOpenLcbMTI(uint16_t mti) {
      return isFrameTypeOpenLcb() 
                && ( getVariableField() == (mti | (FRAME_FORMAT_NORMAL_MTI<<SHIFT_OPENLCB_FORMAT) ) );
  }


  bool OpenLcbCanBuffer::isForHere(uint16_t alias) {  // includes frame level, the more common test
    if (!isFrameTypeOpenLcb()) return true; // CAN level is always for here
    // check for global
    uint8_t format = getOpenLcbFormat();
    if (format == FRAME_FORMAT_NORMAL_MTI && (getVariableField() & MTI_ADDRESS_PRESENT_MASK) == 0 ) {
        return true;
    } else {
        // else check addressed here
        return alias == getDestAlias();
    }
  }

  bool OpenLcbCanBuffer::isMsgForHere(uint16_t alias) {
    if (!isFrameTypeOpenLcb()) return false;
    return isForHere(alias);
  }
  
  bool OpenLcbCanBuffer::isAddressedMessage() {  // checks for message carries address, not that it's for here
    if (!isFrameTypeOpenLcb()) return false;
    // check for global
    uint8_t format = getOpenLcbFormat();
    if (format == FRAME_FORMAT_NORMAL_MTI && (getVariableField() & MTI_ADDRESS_PRESENT_MASK) == 0 ) {
        return false;
    } else {
        // all other formats are addressed
        return true;
    }
  }

  void OpenLcbCanBuffer::getEventID(EventID* evt) {
    memcpy(evt->val, data, 8);
  }
  
  void OpenLcbCanBuffer::getNodeID(NodeID* nid) {
    memcpy(nid->val, data, 6);
  }
  
  bool OpenLcbCanBuffer::matchesNid(NodeID* nid) {
        return 
            nid->val[0] == data[0] &&
            nid->val[1] == data[1] &&
            nid->val[2] == data[2] &&
            nid->val[3] == data[3] &&
            nid->val[4] == data[4] &&
            nid->val[5] == data[5];
  }


  // end of OpenLCB format and decode support
  
  // start of OpenLCB messages
  
  void OpenLcbCanBuffer::setInitializationComplete(NodeID* nid) {
    setOpenLcbMTI(MTI_INITIALIZATION_COMPLETE);
    length=6;
    memcpy(data, nid->val, 6);
  }
  
  bool OpenLcbCanBuffer::isInitializationComplete() {
      return isOpenLcbMTI(MTI_INITIALIZATION_COMPLETE);
  }
  
  void OpenLcbCanBuffer::setPCEventReport(EventID* eid) {
    setOpenLcbMTI(MTI_PC_EVENT_REPORT);
    length=8;
    loadFromEid(eid);
  }
  
  bool OpenLcbCanBuffer::isPCEventReport() {
      return isOpenLcbMTI(MTI_PC_EVENT_REPORT);
  }

  void OpenLcbCanBuffer::setLearnEvent(EventID* eid) {
    setOpenLcbMTI(MTI_LEARN_EVENT);
    length=8;
    loadFromEid(eid);
  }

  bool OpenLcbCanBuffer::isLearnEvent() {
      return isOpenLcbMTI(MTI_LEARN_EVENT);
  }


  bool OpenLcbCanBuffer::isVerifyNID() {
      if (isOpenLcbMTI(MTI_VERIFY_NID_GLOBAL)) return true;
      else return isOpenLcbMTI(MTI_VERIFY_NID_ADDRESSED);   
  }

  void OpenLcbCanBuffer::setVerifiedNID(NodeID* nid) {
    setOpenLcbMTI(MTI_VERIFIED_NID);
    length=6;
    memcpy(data, nid->val, 6);
  }

  bool OpenLcbCanBuffer::isVerifiedNID() {
    return isOpenLcbMTI(MTI_VERIFIED_NID);
  }

  void OpenLcbCanBuffer::setOptionalIntRejected(OpenLcbCanBuffer* rcv, uint16_t code) {
    setOpenLcbMTI(MTI_OPTION_INT_REJECTED);
    setDestAlias(rcv->getSourceAlias());
    length=6;

    data[2] = (code>>8)&0xFF;
    data[3] =  code    &0xFF;

    uint16_t mti = rcv->getOpenLcbMTI();
    data[4] = ((mti>>8)&0xFF);
    data[5] = mti&0xFF;

  }

  bool OpenLcbCanBuffer::isIdentifyConsumers() {
      return isOpenLcbMTI(MTI_IDENTIFY_CONSUMERS);
  }

  void OpenLcbCanBuffer::setConsumerIdentified(EventID* eid) {
    setOpenLcbMTI(MTI_CONSUMER_IDENTIFIED);
    length=8;
    loadFromEid(eid);
  }

  void OpenLcbCanBuffer::setConsumerIdentifyRange(EventID* eid, EventID* mask) {
    // does send a message, but not complete yet - RGJ 2009-06-14
    setOpenLcbMTI(MTI_IDENTIFY_CONSUMERS_RANGE);
    length=8;
    loadFromEid(eid);
  }

  bool OpenLcbCanBuffer::isIdentifyProducers() {
      return isOpenLcbMTI(MTI_IDENTIFY_PRODUCERS);
  }

  void OpenLcbCanBuffer::setProducerIdentified(EventID* eid) {
    setOpenLcbMTI(MTI_PRODUCER_IDENTIFIED);
    length=8;
    loadFromEid(eid);
  }

  void OpenLcbCanBuffer::setProducerIdentifyRange(EventID* eid, EventID* mask) {
    // does send a message, but not complete yet - RGJ 2009-06-14
    setOpenLcbMTI(MTI_IDENTIFY_PRODUCERS_RANGE);
    length=8;
    loadFromEid(eid);
  }

  bool OpenLcbCanBuffer::isIdentifyEvents() {
      if  (isOpenLcbMTI(MTI_IDENTIFY_EVENTS_GLOBAL)) return true;
      return isOpenLcbMTI(MTI_IDENTIFY_EVENTS_ADDRESSED);
  }

  // general, but not efficient
  bool OpenLcbCanBuffer::isDatagramFrame() {
    uint16_t fmt = getOpenLcbFormat();
      return isFrameTypeOpenLcb() 
                && ( (fmt == FRAME_FORMAT_ADDRESSED_DATAGRAM_ALL)
                        || (fmt == FRAME_FORMAT_ADDRESSED_DATAGRAM_FIRST)
                        || (fmt == FRAME_FORMAT_ADDRESSED_DATAGRAM_MID)
                        || (fmt == FRAME_FORMAT_ADDRESSED_DATAGRAM_LAST) );
  }
  
  // just checks 1st, assumes datagram already checked.
  bool OpenLcbCanBuffer::isLastDatagramFrame() {
      return (getOpenLcbFormat() == FRAME_FORMAT_ADDRESSED_DATAGRAM_LAST)
            || (getOpenLcbFormat() == FRAME_FORMAT_ADDRESSED_DATAGRAM_ALL);
  }

  uint8_t OpenLcbCanBuffer::getOpenLcbFormat() {
      return (getVariableField() & MASK_OPENLCB_FORMAT) >> SHIFT_OPENLCB_FORMAT;
  }

  void OpenLcbCanBuffer::setOpenLcbFormat(uint8_t i) {
      uint16_t now = getVariableField() & ~MASK_OPENLCB_FORMAT;
      setVariableField( ((i << SHIFT_OPENLCB_FORMAT) & MASK_OPENLCB_FORMAT) | now);
  }

  void OpenLcbCanBuffer::loadFromEid(EventID* eid) {
    memcpy(data, eid->val, 8);
  }
  
