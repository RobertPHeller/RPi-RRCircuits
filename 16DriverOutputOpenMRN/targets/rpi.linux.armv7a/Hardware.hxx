#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "RPi 16 Driver Output."

// On chip GPIO:

// (none)

// Assumes Kernel config CONFIG_ARCH_NR_GPIO is 512!
#define GPIO_BASE 512

// MCP230xx expanders.  Assumes this order for dtoverlay calls:
//
// dtoverlay mcp23017 addr=0x27
//
// mcp23017 @ 0x27 == Signals 

//
//   GPIO_0.0    A0
//   GPIO_0.1    A1
//   GPIO_0.2    A2
//   GPIO_0.3    A3
//   GPIO_0.4    A4
//   GPIO_0.5    A5
//   GPIO_0.6    A6
//   GPIO_0.7    A7
//   GPIO_1.0    B0
//   GPIO_1.1    B1
//   GPIO_1.2    B2
//   GPIO_1.3    B3
//   GPIO_1.4    B4
//   GPIO_1.5    B5
//   GPIO_1.6    B6
//   GPIO_1.7    B7

#define MCP23017_16DriverOutputBase ((GPIO_BASE)-16)

#define LEDPin GpioOutputSafeLow

GPIO_PIN(A0, LEDPin, MCP23017_16DriverOutputBase+(8*0)+0);
GPIO_PIN(A1, LEDPin, MCP23017_16DriverOutputBase+(8*0)+1);
GPIO_PIN(A2, LEDPin, MCP23017_16DriverOutputBase+(8*0)+2);
GPIO_PIN(A3, LEDPin, MCP23017_16DriverOutputBase+(8*0)+3);
GPIO_PIN(A4, LEDPin, MCP23017_16DriverOutputBase+(8*0)+4);
GPIO_PIN(A5, LEDPin, MCP23017_16DriverOutputBase+(8*0)+5);
GPIO_PIN(A6, LEDPin, MCP23017_16DriverOutputBase+(8*0)+6);
GPIO_PIN(A7, LEDPin, MCP23017_16DriverOutputBase+(8*0)+7);

GPIO_PIN(B0, LEDPin, MCP23017_16DriverOutputBase+(8*1)+0);
GPIO_PIN(B1, LEDPin, MCP23017_16DriverOutputBase+(8*1)+1);
GPIO_PIN(B2, LEDPin, MCP23017_16DriverOutputBase+(8*1)+2);
GPIO_PIN(B3, LEDPin, MCP23017_16DriverOutputBase+(8*1)+3);
GPIO_PIN(B4, LEDPin, MCP23017_16DriverOutputBase+(8*1)+4);
GPIO_PIN(B5, LEDPin, MCP23017_16DriverOutputBase+(8*1)+5);
GPIO_PIN(B6, LEDPin, MCP23017_16DriverOutputBase+(8*1)+6);
GPIO_PIN(B7, LEDPin, MCP23017_16DriverOutputBase+(8*1)+7);

typedef GpioInitializer<A0_Pin,A1_Pin,A2_Pin,A3_Pin,A4_Pin,A5_Pin,
                        A6_Pin,A7_Pin,
                        B0_Pin,B1_Pin,B2_Pin,B3_Pin,B4_Pin,B5_Pin,
                        B6_Pin,B7_Pin> GpioInit;



#define HAVE_TCP_GRIDCONNECT_HOST
#define TCP_GRIDCONNECT_HOST "127.0.0.1"
#define TCP_GRIDCONNECT_PORT 12021

//#define PRINT_ALL_PACKETS
//#define HAVE_SOCKET_CAN_PORT
//#define SOCKET_CAN_PORT "vcan0"

#endif // __HARDWARE_HXX
