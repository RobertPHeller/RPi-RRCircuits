#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "BBB Command Station"



// On chip GPIO:

typedef GpioInitializer<> GpioInit;

//#define USE_OPENLCB_TCP_HOST
//#define DEFAULT_OPENLCB_TCP_HOST "localhost"
//#define DEFAULT_OPENLCB_TCP_HOST 12000

//#define USE_GRIDCONNECT_HOST
//#define DEFAULT_TCP_GRIDCONNECT_HOST "localhost"
//#define DEFAULT_TCP_GRIDCONNECT_PORT 12021

//#define PRINT_ALL_PACKETS

#define USE_SOCKET_CAN_PORT
#define DEFAULT_CAN_SOCKET "can1"


#endif // __HARDWARE_HXX
