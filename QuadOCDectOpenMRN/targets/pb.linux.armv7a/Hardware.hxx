#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "PocketBeagleQuadOCDect board"



// On chip GPIO:

#define MotorPin GpioOutputSafeLow
#define SensePin GpioInputActiveHigh
#define ODPin  GpioInputActiveLow
#define LEDPin GpioOutputSafeLow


GPIO_PIN(OD1, ODPin, 52); // GPIO52: P2-10
GPIO_PIN(OD2, ODPin, 57); // GPIO57: P2-6
GPIO_PIN(OD3, ODPin, 58); // GPIO58: P2-4
GPIO_PIN(OD4, ODPin, 59); // GPIO59: P2-2

typedef GpioInitializer<OD1_Pin, OD2_Pin, OD3_Pin, OD4_Pin> GpioInit;


//#define HAVE_TCP_GRIDCONNECT_HOST
//#define TCP_GRIDCONNECT_HOST "localhost"
//#define TCP_GRIDCONNECT_PORT 12021

//#define PRINT_ALL_PACKETS

#define HAVE_SOCKET_CAN_PORT
#define SOCKET_CAN_PORT "can1"


#endif // __HARDWARE_HXX
