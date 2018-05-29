#include "Configuration.h"
#include "Datagram.h"
#include "OlcbStream.h"

#include "logging.h"


#define CONFIGURATION_DATAGRAM_CODE 0x20

// define the operation codes, full byte
#define CFG_CMD_WRITE                       0x00
#define CFG_CMD_READ                        0x40
#define CFG_CMD_OPERATION                   0x80

#define CFG_CMD_READ_REPLY                  0x50

#define CFG_CMD_GET_CONFIG                  0x80
#define CFG_CMD_GET_CONFIG_REPLY            0x82
#define CFG_CMD_GET_ADD_SPACE_INFO          0x84
#define CFG_CMD_GET_ADD_SPACE_INFO_REPLY    0x86
#define CFG_CMD_LOCK                        0x88
#define CFG_CMD_LOCK_REPLY                  0x8A
#define CFG_CMD_GET_UNIQUEID                0x8C
#define CFG_CMD_GET_UNIQUEID_REPLY          0x8E

#define CFG_CMD_FREEZE                      0xA0
#define CFG_CMD_INDICATE                    0xA4
#define CFG_CMD_RESETS                      0xA8

/** 
 * Structure: Requests come in via 
 * receiveDatagram.  Results (if any) are stored in a 
 * buffer, and check() then sends it when possible.
 *
 * TODO: No stream implementation yet.
 * TODO: Uses an extra buffer, which it would be good to avoid,
 *       but I've included it now to not make assumptions about
 *       Datagram use & structure
 */
 
  Configuration::Configuration( Datagram *d, OlcbStream *s,
                        const uint8_t (*gr)(uint32_t address, int space),
                        void (*gw)(uint32_t address, int space, uint8_t value),
                        void (*res)()
                 ){
    dg = d;
    str = s;
    request = false;
    getWrite = gw;
    getRead = gr;
    restart = res;
}

void Configuration::check() {
    if (!request) return;
    // have a request pending
    switch (buffer[1]&0xC0) {
        case CFG_CMD_READ:
            processRead(buffer, length);
            break;
        case CFG_CMD_WRITE:
            processWrite(buffer, length);
            break;
        case CFG_CMD_OPERATION:
            processCmd(buffer, length);
            break;
    }
}

int Configuration::receivedDatagram(uint8_t* data, int ln, unsigned int f) {
    // decode whether this is a configuration request
    if (data[0] != CONFIGURATION_DATAGRAM_CODE) return 1;  // 1 is error
    // yes, copy to our buffer
    length = ln;
    from = f;
    for (int i = 0; i<length; i++) 
        buffer[i] = *(data++);
    // mark as ready
    request = true;
    return 0;
}

uint32_t Configuration::getAddress(uint8_t* data) {
    // AVR+GCC is byte little-endian, so can use cast method.
    uint32_t val = 0;
    val =  data[2];
    val = val << 8;
    val |= data[3];
    val = val << 8;
    val |= data[4];
    val = val << 8;
    val |= data[5];
    return val;
}

// -1 means stream
int Configuration::decodeLen(uint8_t* data) {
    // ToDo:  Add stream
    return data[6];
}
int Configuration::decodeSpace(uint8_t* data) {
    int val = 0xFF;  // default
    switch (data[1]&0x03) {
        case 0x03:
            val = 0xFF;
            break;
        case 0x02:
            val = 0xFE;
            break;
        case 0x01:
            val = 0xFD;
            break;
        case 0x00:
            val = data[6];
            break;
    }
    return val;
}

// length is the datagram data length
void Configuration::processRead(uint8_t* data, int length) {
    // see if we can get datagram buffer to reply
    uint8_t* d = dg->getTransmitBuffer();
    if (d == 0) return; // skip and return again later
    // will reply, mark as done.
    request = false;
    // copy front matter
    for (int i=0; i<6; i++)
        d[i]=data[i];
    d[1] = CFG_CMD_READ_REPLY | (d[1]&0x0F);
    // get length, space
    int len = decodeLen(data);
    uint32_t address = getAddress(data);
    int space = decodeSpace(data);
    for (int i=0; i<len; i++)
        d[i+6] = getRead(address+i, space);
    // send
    dg->sendTransmitBuffer(6+len, from);
}

// length is the datagram data length
void Configuration::processWrite(uint8_t* data, int length) {
    // will reply, mark as done.
    request = false;
    uint32_t address = getAddress(data);
    int space = decodeSpace(data);
    for (int i=0; i<length-6; i++) {
        getWrite(address+i, space, data[i+6]);
    }
}

void Configuration::processCmd(uint8_t* data, int length) {
    //logstr("  processCmd\n");
        
    switch (data[1]&0xFC) {
        case CFG_CMD_GET_CONFIG: {  // to partition local variable below
            // reply with canned message
            uint8_t* d = dg->getTransmitBuffer();
            if (d==0) return; // skip and return again later
            // will handle, mark as done.
            request = false;
            d[0]=CONFIGURATION_DATAGRAM_CODE; d[1]=CFG_CMD_GET_CONFIG_REPLY;
            d[2]=0x6E;d[3]=0x00;d[4]=0xF2;d[5]=0xFF;d[6]=0xFB;
            dg->sendTransmitBuffer(7, from);
            break;
          }
        case CFG_CMD_GET_ADD_SPACE_INFO: {  // to partition local variable below
            // reply with canned message
            uint8_t* d = dg->getTransmitBuffer();
            if (d==0) return; // skip and return again later
            // will handle, mark as done.
            request = false;
            // will reply, mark as done.
            d[0]=CONFIGURATION_DATAGRAM_CODE; d[1]=CFG_CMD_GET_ADD_SPACE_INFO_REPLY;
            d[2]=data[2]; // return space number requested
            if (spaceUpperAddr) {
                uint32_t a = spaceUpperAddr(data[2]);
                d[6] = (uint8_t)(a&0xFF);  a = a>>8;
                d[5] = (uint8_t)(a&0xFF);  a = a>>8;
                d[4] = (uint8_t)(a&0xFF);  a = a>>8;
                d[3] = (uint8_t)(a&0xFF);
            } else {
                d[3]=0x00;d[4]=0xFF;d[5]=0xFF;d[6]=0xFF;
            }
            d[7]=0x00;
            if (data[2] >= 0xFB) d[1]|= 0x1; // indicates is present
            dg->sendTransmitBuffer(8, from);
            break;
          }
        case CFG_CMD_RESETS: {
            // will handle, mark as done.
            request = false;
            // force restart (may not reply?)
            if ((data[1]&0x03) == 0x01) { // restart/reboot?
                (*restart)();
            }
            // TODO: Handle other cases
            break;
          }
        //case CFG_CMD_CFG_CMD_GET_CONFIG_REPLY :
        //case CFG_CMD_CFG_CMD_GET_ADD_SPACE_INFO_REPLY:
        //case CFG_CMD_LOCK:
        //case CFG_CMD_LOCK_REPLY:
        //case CFG_CMD_GET_UNIQUEID:
        //case CFG_CMD_GET_UNIQUEID_REPLY:
        //case CFG_CMD_FREEZE:
        //case CFG_CMD_INDICATE:
        default:
            // these do nothing in this implementation
            break;
    }
}

