#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "RPi ProgRelay Hat"

// On chip GPIO:

GPIO_PIN(Relay, GpioOutputSafeLow, 4);
GPIO_PIN(BlueLEDS, GpioOutputSafeLow, 17);
GPIO_PIN(ProgSelect, GpioInputActiveLow, 18);

typedef GpioInitializer<Relay_Pin, BlueLEDS_Pin, 
                        ProgSelect_Pin> GpioInit;



#define USE_OPENLCB_TCP_HOST 1
#define DEFAULT_OPENLCB_TCP_PORT 12000
#define DEFAULT_OPENLCB_TCP_HOST "127.0.0.1"

//#define HAVE_TCP_GRIDCONNECT_HOST
//#define TCP_GRIDCONNECT_HOST "127.0.0.1"
//#define TCP_GRIDCONNECT_PORT 12021

//#define PRINT_ALL_PACKETS
//#define HAVE_SOCKET_CAN_PORT
//#define SOCKET_CAN_PORT "vcan0"

#endif // __HARDWARE_HXX
