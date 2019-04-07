#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "PB Mega IO (40 I/O bits, 6 5x2 headers)"



// On chip GPIO:

// Extra 2 headers from "on chip" I/O pins.

// GPIO 1 Header:
// GPIO48   1
// GPIO47   2
// GPIO46   3
// GPIO45   4
// GND      5
// +3.3V    6
// GPIO27   7
// GPIO26   8
// GPIO23   9
// GPIO20   10
// 

GPIO_PIN(GPIO1_1, GpioOutputSafeLow, 48);
GPIO_PIN(GPIO1_2, GpioOutputSafeLow, 47);
GPIO_PIN(GPIO1_3, GpioOutputSafeLow, 46);
GPIO_PIN(GPIO1_4, GpioOutputSafeLow, 45);
GPIO_PIN(GPIO1_7, GpioOutputSafeLow, 27);
GPIO_PIN(GPIO1_8, GpioOutputSafeLow, 26);
GPIO_PIN(GPIO1_9, GpioOutputSafeLow, 23);
GPIO_PIN(GPIO1_10, GpioOutputSafeLow, 20);

// GPIO 2 Header:
// GPIO87   1
// GPIO65   2
// GPIO64   3
// GPIO60   4
// GND      5
// +3.3V    6
// GPIO59   7
// GPIO58   8
// GPIO57   9
// GPIO52   10
// 

GPIO_PIN(GPIO2_1, GpioOutputSafeLow, 87);
GPIO_PIN(GPIO2_2, GpioOutputSafeLow, 65);
GPIO_PIN(GPIO2_3, GpioOutputSafeLow, 64);
GPIO_PIN(GPIO2_4, GpioOutputSafeLow, 60);
GPIO_PIN(GPIO2_7, GpioOutputSafeLow, 59);
GPIO_PIN(GPIO2_8, GpioOutputSafeLow, 58);
GPIO_PIN(GPIO2_9, GpioOutputSafeLow, 57);
GPIO_PIN(GPIO2_10, GpioOutputSafeLow, 52);

// Assumes Kernel config CONFIG_ARCH_NR_GPIO is 512!
#define GPIO_BASE 512

// MCP230xx expanders.  Assumes this order for dtoverlay calls:
//
// Assuming no address jumpers in place. (B11)
// dtoverlay mcp23017 addr=0x26
// dtoverlay mcp23017 addr=0x27
//
// mcp23017 @ 0x26 == GPIO header 1 & 2
// mcp23017 @ 0x27 == GPIO header 3 & 4

//
//   GPIO 3 Header:
//   GPIO_0.0  10
//   GPIO_0.1   9
//   GPIO_0.2   8
//   GPIO_0.3   7
//   +3.3V      6
//   GND        5
//   GPIO_0.4   4
//   GPIO_0.5   3
//   GPIO_0.6   2
//   GPIO_0.7   1
//
//   GPIO 4 Header:
//   GPIO_1.0  10
//   GPIO_1.1   9
//   GPIO_1.2   8
//   GPIO_1.3   7
//   +3.3V      6
//   GND        5
//   GPIO_1.4   4
//   GPIO_1.5   3
//   GPIO_1.6   2
//   GPIO_1.7   1

#define MCP23017_1_16DriverOutputBase ((GPIO_BASE)-16)

GPIO_PIN(GPIO3_1, GpioOutputSafeLow, MCP23017_1_16DriverOutputBase+(8*0)+7);
GPIO_PIN(GPIO3_2, GpioOutputSafeLow, MCP23017_1_16DriverOutputBase+(8*0)+6);
GPIO_PIN(GPIO3_3, GpioOutputSafeLow, MCP23017_1_16DriverOutputBase+(8*0)+5);
GPIO_PIN(GPIO3_4, GpioOutputSafeLow, MCP23017_1_16DriverOutputBase+(8*0)+4);
GPIO_PIN(GPIO3_7, GpioOutputSafeLow, MCP23017_1_16DriverOutputBase+(8*0)+3);
GPIO_PIN(GPIO3_8, GpioOutputSafeLow, MCP23017_1_16DriverOutputBase+(8*0)+2);
GPIO_PIN(GPIO3_9, GpioOutputSafeLow, MCP23017_1_16DriverOutputBase+(8*0)+1);
GPIO_PIN(GPIO3_10, GpioOutputSafeLow, MCP23017_1_16DriverOutputBase+(8*0)+0);

GPIO_PIN(GPIO4_1, GpioOutputSafeLow, MCP23017_1_16DriverOutputBase+(8*1)+7);
GPIO_PIN(GPIO4_2, GpioOutputSafeLow, MCP23017_1_16DriverOutputBase+(8*1)+6);
GPIO_PIN(GPIO4_3, GpioOutputSafeLow, MCP23017_1_16DriverOutputBase+(8*1)+5);
GPIO_PIN(GPIO4_4, GpioOutputSafeLow, MCP23017_1_16DriverOutputBase+(8*1)+4);
GPIO_PIN(GPIO4_7, GpioOutputSafeLow, MCP23017_1_16DriverOutputBase+(8*1)+3);
GPIO_PIN(GPIO4_8, GpioOutputSafeLow, MCP23017_1_16DriverOutputBase+(8*1)+2);
GPIO_PIN(GPIO4_9, GpioOutputSafeLow, MCP23017_1_16DriverOutputBase+(8*1)+1);
GPIO_PIN(GPIO4_10, GpioOutputSafeLow, MCP23017_1_16DriverOutputBase+(8*1)+0);

//
//   GPIO 5 Header:
//   GPIO_0.0  10
//   GPIO_0.1   9
//   GPIO_0.2   8
//   GPIO_0.3   7
//   +3.3V      6
//   GND        5
//   GPIO_0.4   4
//   GPIO_0.5   3
//   GPIO_0.6   2
//   GPIO_0.7   1
//
//   GPIO 6 Header:
//   GPIO_1.0  10
//   GPIO_1.1   9
//   GPIO_1.2   8
//   GPIO_1.3   7
//   +3.3V      6
//   GND        5
//   GPIO_1.4   4
//   GPIO_1.5   3
//   GPIO_1.6   2
//   GPIO_1.7   1

#define MCP23017_2_16DriverOutputBase ((MCP23017_1_16DriverOutputBase)-16)

GPIO_PIN(GPIO5_1, GpioOutputSafeLow, MCP23017_2_16DriverOutputBase+(8*0)+7);
GPIO_PIN(GPIO5_2, GpioOutputSafeLow, MCP23017_2_16DriverOutputBase+(8*0)+6);
GPIO_PIN(GPIO5_3, GpioOutputSafeLow, MCP23017_2_16DriverOutputBase+(8*0)+5);
GPIO_PIN(GPIO5_4, GpioOutputSafeLow, MCP23017_2_16DriverOutputBase+(8*0)+4);
GPIO_PIN(GPIO5_7, GpioOutputSafeLow, MCP23017_2_16DriverOutputBase+(8*0)+3);
GPIO_PIN(GPIO5_8, GpioOutputSafeLow, MCP23017_2_16DriverOutputBase+(8*0)+2);
GPIO_PIN(GPIO5_9, GpioOutputSafeLow, MCP23017_2_16DriverOutputBase+(8*0)+1);
GPIO_PIN(GPIO5_10, GpioOutputSafeLow, MCP23017_2_16DriverOutputBase+(8*0)+0);

GPIO_PIN(GPIO6_1, GpioOutputSafeLow, MCP23017_2_16DriverOutputBase+(8*1)+7);
GPIO_PIN(GPIO6_2, GpioOutputSafeLow, MCP23017_2_16DriverOutputBase+(8*1)+6);
GPIO_PIN(GPIO6_3, GpioOutputSafeLow, MCP23017_2_16DriverOutputBase+(8*1)+5);
GPIO_PIN(GPIO6_4, GpioOutputSafeLow, MCP23017_2_16DriverOutputBase+(8*1)+4);
GPIO_PIN(GPIO6_7, GpioOutputSafeLow, MCP23017_2_16DriverOutputBase+(8*1)+3);
GPIO_PIN(GPIO6_8, GpioOutputSafeLow, MCP23017_2_16DriverOutputBase+(8*1)+2);
GPIO_PIN(GPIO6_9, GpioOutputSafeLow, MCP23017_2_16DriverOutputBase+(8*1)+1);
GPIO_PIN(GPIO6_10, GpioOutputSafeLow, MCP23017_2_16DriverOutputBase+(8*1)+0);

typedef GpioInitializer<
GPIO1_1_Pin,GPIO1_2_Pin,GPIO1_3_Pin,GPIO1_4_Pin,
GPIO1_7_Pin,GPIO1_8_Pin,GPIO1_9_Pin,GPIO1_10_Pin,
GPIO2_1_Pin,GPIO2_2_Pin,GPIO2_3_Pin,GPIO2_4_Pin,
GPIO2_7_Pin,GPIO2_8_Pin,GPIO2_9_Pin,GPIO2_10_Pin,
GPIO3_1_Pin,GPIO3_2_Pin,GPIO3_3_Pin,GPIO3_4_Pin,
GPIO3_7_Pin,GPIO3_8_Pin,GPIO3_9_Pin,GPIO3_10_Pin,
GPIO4_1_Pin,GPIO4_2_Pin,GPIO4_3_Pin,GPIO4_4_Pin,
GPIO4_7_Pin,GPIO4_8_Pin,GPIO4_9_Pin,GPIO4_10_Pin,
GPIO5_1_Pin,GPIO5_2_Pin,GPIO5_3_Pin,GPIO5_4_Pin,
GPIO5_7_Pin,GPIO5_8_Pin,GPIO5_9_Pin,GPIO5_10_Pin,
GPIO6_1_Pin,GPIO6_2_Pin,GPIO6_3_Pin,GPIO6_4_Pin,
GPIO6_7_Pin,GPIO6_8_Pin,GPIO6_9_Pin,GPIO6_10_Pin> GpioInit;

#define EXTRAHEADERS

//#define HAVE_TCP_GRIDCONNECT_HOST
//#define TCP_GRIDCONNECT_HOST "localhost"
//#define TCP_GRIDCONNECT_PORT 12021

//#define PRINT_ALL_PACKETS

#define HAVE_SOCKET_CAN_PORT
#define SOCKET_CAN_PORT "can1"


#endif // __HARDWARE_HXX
