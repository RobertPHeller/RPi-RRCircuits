#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "BBB Command Station"

// On chip GPIO:
//
// Mains:
//
// DCC P8-8  GPIO_67 (2.03)
// EN  P8-19 GPIO_68 (2.04)
// CSense P9-39 AIN0
// TF  P8-12 GPIO_44 (1.12)
//
// Prog Track:
//
// DCC P8-14 GPIO_26 (0.26)
// EN  P8-16 GPIO_46 (1.14)
// CSense P9-40 AIN1
// TF  P8-18 GPIO_65 (2.01)
//


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

//#define TERMINALCONSOLE
#define CONSOLEPORT 9900

#endif // __HARDWARE_HXX
