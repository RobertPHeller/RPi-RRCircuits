#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "RPi "

// On chip GPIO:

#define MotorPin GpioOutputSafeLow
#define SensePin GpioInputActiveHigh
#define ODPin  GpioInputActiveLow
#define LEDPin GpioOutputSafeLow


GPIO_PIN(OD1, ODPin, 5);
GPIO_PIN(OD2, ODPin, 6);
GPIO_PIN(OD3, ODPin, 13);
GPIO_PIN(OD4, ODPin, 19);

typedef GpioInitializer<OD1_Pin, OD2_Pin, OD3_Pin, OD4_Pin> GpioInit;



#define HAVE_TCP_GRIDCONNECT_HOST
#define TCP_GRIDCONNECT_HOST "127.0.0.1"
#define TCP_GRIDCONNECT_PORT 12021

//#define PRINT_ALL_PACKETS
//#define HAVE_SOCKET_CAN_PORT
//#define SOCKET_CAN_PORT "vcan0"

#endif // __HARDWARE_HXX
