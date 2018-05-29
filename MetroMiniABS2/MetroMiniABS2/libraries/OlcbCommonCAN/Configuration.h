#ifndef Configuration_h
#define Configuration_h

/**
 * Class for handling Configuration protocol
 *
 * 
 */
#include <stdint.h>

#include "Datagram.h"

class OlcbStream;

class Configuration {
  public:
  
  //*****
  // The "getRead", "getWrite" and "reset" methods
  // are a very simple interface to the actual
  // device.  Redo them for the specific memory
  // map being used.
  // (We're trying for now to avoid virtual methods)
  // ****
  
  Configuration(Datagram* datagramHandler, OlcbStream *streamHandler,
                        const uint8_t (*getRead)(uint32_t address, int space),
                        void (*getWrite)(uint32_t address, int space, uint8_t val),
                        void (*restart)()
            );
            
  void check(); 
  int receivedDatagram(uint8_t* data, int length, unsigned int from);
  
  private:
  void processRead(uint8_t* data, int length);
  void processWrite(uint8_t* data, int length);
  void processCmd(uint8_t* data, int length);
  
  Datagram* dg;
  OlcbStream* str;
  uint8_t buffer[DATAGRAM_LENGTH];
  int length;
  unsigned int from;
  bool request;
  
  uint32_t getAddress(uint8_t* data);
  int decodeLen(uint8_t* data);
  int decodeSpace(uint8_t* data);
  
  const uint8_t (*getRead)(uint32_t address, int space);
  void (*getWrite)(uint32_t address, int space, uint8_t val);
  void (*restart)();
 
};

// optional members, 0 if not appearing; does this work with clang compiler?
extern "C" {
uint32_t spaceUpperAddr(uint8_t space) __attribute__((weak));
};

#endif
