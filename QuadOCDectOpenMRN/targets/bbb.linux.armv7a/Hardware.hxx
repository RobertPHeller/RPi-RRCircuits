#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "BBB QuadOCDectCape"



// On chip GPIO:

#define MotorPin GpioOutputSafeLow
#define SensePin GpioInputActiveHigh
#define ODPin  GpioInputActiveLow
#define LEDPin GpioOutputSafeLow


GPIO_PIN(OD1, ODPin, (32*1)+12); // GPIO1_12: P8-12
GPIO_PIN(OD2, ODPin, (32*1)+2); // GPIO1_2: P8-5
GPIO_PIN(OD3, ODPin, (32*1)+3); // GPIO1_3: P8-6
GPIO_PIN(OD4, ODPin, (32*1)+13); // GPIO1_13: P8-11

typedef GpioInitializer<OD1_Pin, OD2_Pin, OD3_Pin, OD4_Pin> GpioInit;


//#define HAVE_TCP_GRIDCONNECT_HOST
//#define TCP_GRIDCONNECT_HOST "localhost"
//#define TCP_GRIDCONNECT_PORT 12021

//#define PRINT_ALL_PACKETS

#define HAVE_SOCKET_CAN_PORT
#define SOCKET_CAN_PORT "can1"


#endif // __HARDWARE_HXX
