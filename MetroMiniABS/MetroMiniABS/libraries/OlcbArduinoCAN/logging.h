// define some rudimentary logging
// intended for inclusion in .cpp files for Arduino

#ifndef lognl
#define logstr(...) Serial.print(__VA_ARGS__)
#define loghex(...) Serial.print(__VA_ARGS__, HEX)
#define lognl() Serial.println()
#include "HardwareSerial.h"
#endif  // logln
