#ifndef OpenLcbCan_h
#define OpenLcbCan_h

/**
 * OpenLCB CAN Frame Transport
 */

#define RIM_VAR_FIELD 0x0700
#define AMD_VAR_FIELD 0x0701
#define AME_VAR_FIELD 0x0702
#define AMR_VAR_FIELD 0x0703

/**
 * OpenLCB CAN Frame format (3 bits)
 */
#define FRAME_FORMAT_NORMAL_MTI               1    
//
#define FRAME_FORMAT_ADDRESSED_DATAGRAM_ALL   2    
#define FRAME_FORMAT_ADDRESSED_DATAGRAM_FIRST 3    
#define FRAME_FORMAT_ADDRESSED_DATAGRAM_MID   4    
#define FRAME_FORMAT_ADDRESSED_DATAGRAM_LAST  5    

#define FRAME_FORMAT_STREAM                   7    


/**
 * Basic 12-bit header MTI definitions for OpenLCB on CAN.
 * See the MtiAllocations.ods document for allocations.
 *
 * Note: This is just the low 12 bits, and does not include
 * 0-7 format MTI format field just above this.
 */

#define MTI_OPTION_INT_REJECTED             0x068

#define MTI_INITIALIZATION_COMPLETE         0x100

#define MTI_VERIFY_NID_GLOBAL               0x490
#define MTI_VERIFY_NID_ADDRESSED            0x488

#define MTI_VERIFIED_NID                    0x170

#define MTI_IDENTIFY_CONSUMERS              0x8F4
#define MTI_IDENTIFY_CONSUMERS_RANGE        0x4A4

// modifier is "unknown"
#define MTI_CONSUMER_IDENTIFIED             0x4C7

#define MTI_IDENTIFY_PRODUCERS              0x914
#define MTI_IDENTIFY_PRODUCERS_RANGE        0x524

// modifier is "unknown"
#define MTI_PRODUCER_IDENTIFIED             0x547

#define MTI_IDENTIFY_EVENTS_GLOBAL          0x970
#define MTI_IDENTIFY_EVENTS_ADDRESSED       0x968

#define MTI_LEARN_EVENT                     0x594
#define MTI_PC_EVENT_REPORT                 0x5B4

#define MTI_DATAGRAM_RCV_OK                 0xA28

#define MTI_DATAGRAM_REJECTED               0xA48

/**
 * Define some decoding bits relative to 
 * the 12-bit MTI
 */

#define MTI_ADDRESS_PRESENT_MASK  0x008

#endif
