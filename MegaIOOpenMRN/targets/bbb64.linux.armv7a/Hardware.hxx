#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "BBB Mega IO (64 I/O bits, 8 5x2 headers)"

// On chip GPIO:

// Assumes Kernel config CONFIG_ARCH_NR_GPIO is 512!
#define GPIO_BASE 512

// MCP230xx expanders.  Assumes this order for dtoverlay calls:
//
// mcp23017 @ 0x20 == GPIO header 1 & 2 (base is 448)
// mcp23017 @ 0x21 == GPIO header 3 & 4 (base is 464)
// mcp23017 @ 0x22 == GPIO header 5 & 6 (base is 480)
// mcp23017 @ 0x23 == GPIO header 7 & 8 (base is 496)


//
//   GPIO 7 Header:
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
//   GPIO 8 Header:
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

#define MCP23017_4_16DriverOutputBase ((GPIO_BASE)-16)

GPIO_PIN(GPIO7_1, GpioInputActiveLow, MCP23017_4_16DriverOutputBase+(8*0)+7);
GPIO_PIN(GPIO7_2, GpioInputActiveLow, MCP23017_4_16DriverOutputBase+(8*0)+6);
GPIO_PIN(GPIO7_3, GpioInputActiveLow, MCP23017_4_16DriverOutputBase+(8*0)+5);
GPIO_PIN(GPIO7_4, GpioInputActiveLow, MCP23017_4_16DriverOutputBase+(8*0)+4);
GPIO_PIN(GPIO7_7, GpioInputActiveLow, MCP23017_4_16DriverOutputBase+(8*0)+3);
GPIO_PIN(GPIO7_8, GpioInputActiveLow, MCP23017_4_16DriverOutputBase+(8*0)+2);
GPIO_PIN(GPIO7_9, GpioInputActiveLow, MCP23017_4_16DriverOutputBase+(8*0)+1);
GPIO_PIN(GPIO7_10, GpioInputActiveLow, MCP23017_4_16DriverOutputBase+(8*0)+0);

GPIO_PIN(GPIO8_1, GpioInputActiveLow, MCP23017_4_16DriverOutputBase+(8*1)+7);
GPIO_PIN(GPIO8_2, GpioInputActiveLow, MCP23017_4_16DriverOutputBase+(8*1)+6);
GPIO_PIN(GPIO8_3, GpioInputActiveLow, MCP23017_4_16DriverOutputBase+(8*1)+5);
GPIO_PIN(GPIO8_4, GpioInputActiveLow, MCP23017_4_16DriverOutputBase+(8*1)+4);
GPIO_PIN(GPIO8_7, GpioInputActiveLow, MCP23017_4_16DriverOutputBase+(8*1)+3);
GPIO_PIN(GPIO8_8, GpioInputActiveLow, MCP23017_4_16DriverOutputBase+(8*1)+2);
GPIO_PIN(GPIO8_9, GpioInputActiveLow, MCP23017_4_16DriverOutputBase+(8*1)+1);
GPIO_PIN(GPIO8_10, GpioInputActiveLow, MCP23017_4_16DriverOutputBase+(8*1)+0);

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

#define MCP23017_3_16DriverOutputBase ((MCP23017_4_16DriverOutputBase)-16)

GPIO_PIN(GPIO5_1, GpioInputActiveLow, MCP23017_3_16DriverOutputBase+(8*0)+7);
GPIO_PIN(GPIO5_2, GpioInputActiveLow, MCP23017_3_16DriverOutputBase+(8*0)+6);
GPIO_PIN(GPIO5_3, GpioInputActiveLow, MCP23017_3_16DriverOutputBase+(8*0)+5);
GPIO_PIN(GPIO5_4, GpioInputActiveLow, MCP23017_3_16DriverOutputBase+(8*0)+4);
GPIO_PIN(GPIO5_7, GpioInputActiveLow, MCP23017_3_16DriverOutputBase+(8*0)+3);
GPIO_PIN(GPIO5_8, GpioInputActiveLow, MCP23017_3_16DriverOutputBase+(8*0)+2);
GPIO_PIN(GPIO5_9, GpioInputActiveLow, MCP23017_3_16DriverOutputBase+(8*0)+1);
GPIO_PIN(GPIO5_10, GpioInputActiveLow, MCP23017_3_16DriverOutputBase+(8*0)+0);

GPIO_PIN(GPIO6_1, GpioInputActiveLow, MCP23017_3_16DriverOutputBase+(8*1)+7);
GPIO_PIN(GPIO6_2, GpioInputActiveLow, MCP23017_3_16DriverOutputBase+(8*1)+6);
GPIO_PIN(GPIO6_3, GpioInputActiveLow, MCP23017_3_16DriverOutputBase+(8*1)+5);
GPIO_PIN(GPIO6_4, GpioInputActiveLow, MCP23017_3_16DriverOutputBase+(8*1)+4);
GPIO_PIN(GPIO6_7, GpioInputActiveLow, MCP23017_3_16DriverOutputBase+(8*1)+3);
GPIO_PIN(GPIO6_8, GpioInputActiveLow, MCP23017_3_16DriverOutputBase+(8*1)+2);
GPIO_PIN(GPIO6_9, GpioInputActiveLow, MCP23017_3_16DriverOutputBase+(8*1)+1);
GPIO_PIN(GPIO6_10, GpioInputActiveLow, MCP23017_3_16DriverOutputBase+(8*1)+0);

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

#define MCP23017_2_16DriverOutputBase ((MCP23017_3_16DriverOutputBase)-16)

GPIO_PIN(GPIO3_1, GpioInputActiveLow, MCP23017_2_16DriverOutputBase+(8*0)+7);
GPIO_PIN(GPIO3_2, GpioInputActiveLow, MCP23017_2_16DriverOutputBase+(8*0)+6);
GPIO_PIN(GPIO3_3, GpioInputActiveLow, MCP23017_2_16DriverOutputBase+(8*0)+5);
GPIO_PIN(GPIO3_4, GpioInputActiveLow, MCP23017_2_16DriverOutputBase+(8*0)+4);
GPIO_PIN(GPIO3_7, GpioInputActiveLow, MCP23017_2_16DriverOutputBase+(8*0)+3);
GPIO_PIN(GPIO3_8, GpioInputActiveLow, MCP23017_2_16DriverOutputBase+(8*0)+2);
GPIO_PIN(GPIO3_9, GpioInputActiveLow, MCP23017_2_16DriverOutputBase+(8*0)+1);
GPIO_PIN(GPIO3_10, GpioInputActiveLow, MCP23017_2_16DriverOutputBase+(8*0)+0);

GPIO_PIN(GPIO4_1, GpioInputActiveLow, MCP23017_2_16DriverOutputBase+(8*1)+7);
GPIO_PIN(GPIO4_2, GpioInputActiveLow, MCP23017_2_16DriverOutputBase+(8*1)+6);
GPIO_PIN(GPIO4_3, GpioInputActiveLow, MCP23017_2_16DriverOutputBase+(8*1)+5);
GPIO_PIN(GPIO4_4, GpioInputActiveLow, MCP23017_2_16DriverOutputBase+(8*1)+4);
GPIO_PIN(GPIO4_7, GpioInputActiveLow, MCP23017_2_16DriverOutputBase+(8*1)+3);
GPIO_PIN(GPIO4_8, GpioInputActiveLow, MCP23017_2_16DriverOutputBase+(8*1)+2);
GPIO_PIN(GPIO4_9, GpioInputActiveLow, MCP23017_2_16DriverOutputBase+(8*1)+1);
GPIO_PIN(GPIO4_10, GpioInputActiveLow, MCP23017_2_16DriverOutputBase+(8*1)+0);

//
//   GPIO 1 Header:
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
//   GPIO 2 Header:
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

#define MCP23017_1_16DriverOutputBase ((MCP23017_2_16DriverOutputBase)-16)

GPIO_PIN(GPIO1_1, GpioInputActiveLow, MCP23017_1_16DriverOutputBase+(8*0)+7);
GPIO_PIN(GPIO1_2, GpioInputActiveLow, MCP23017_1_16DriverOutputBase+(8*0)+6);
GPIO_PIN(GPIO1_3, GpioInputActiveLow, MCP23017_1_16DriverOutputBase+(8*0)+5);
GPIO_PIN(GPIO1_4, GpioInputActiveLow, MCP23017_1_16DriverOutputBase+(8*0)+4);
GPIO_PIN(GPIO1_7, GpioInputActiveLow, MCP23017_1_16DriverOutputBase+(8*0)+3);
GPIO_PIN(GPIO1_8, GpioInputActiveLow, MCP23017_1_16DriverOutputBase+(8*0)+2);
GPIO_PIN(GPIO1_9, GpioInputActiveLow, MCP23017_1_16DriverOutputBase+(8*0)+1);
GPIO_PIN(GPIO1_10, GpioInputActiveLow, MCP23017_1_16DriverOutputBase+(8*0)+0);

GPIO_PIN(GPIO2_1, GpioInputActiveLow, MCP23017_1_16DriverOutputBase+(8*1)+7);
GPIO_PIN(GPIO2_2, GpioInputActiveLow, MCP23017_1_16DriverOutputBase+(8*1)+6);
GPIO_PIN(GPIO2_3, GpioInputActiveLow, MCP23017_1_16DriverOutputBase+(8*1)+5);
GPIO_PIN(GPIO2_4, GpioInputActiveLow, MCP23017_1_16DriverOutputBase+(8*1)+4);
GPIO_PIN(GPIO2_7, GpioInputActiveLow, MCP23017_1_16DriverOutputBase+(8*1)+3);
GPIO_PIN(GPIO2_8, GpioInputActiveLow, MCP23017_1_16DriverOutputBase+(8*1)+2);
GPIO_PIN(GPIO2_9, GpioInputActiveLow, MCP23017_1_16DriverOutputBase+(8*1)+1);
GPIO_PIN(GPIO2_10, GpioInputActiveLow, MCP23017_1_16DriverOutputBase+(8*1)+0);

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
GPIO6_7_Pin,GPIO6_8_Pin,GPIO6_9_Pin,GPIO6_10_Pin,

GPIO7_1_Pin,GPIO7_2_Pin,GPIO7_3_Pin,GPIO7_4_Pin,
GPIO7_7_Pin,GPIO7_8_Pin,GPIO7_9_Pin,GPIO7_10_Pin,

GPIO8_1_Pin,GPIO8_2_Pin,GPIO8_3_Pin,GPIO8_4_Pin,
GPIO8_7_Pin,GPIO8_8_Pin,GPIO8_9_Pin,GPIO8_10_Pin> GpioInit;

#define HAVEQUADMCP23017 1

//#define HAVE_TCP_GRIDCONNECT_HOST
//#define TCP_GRIDCONNECT_HOST "localhost"
//#define TCP_GRIDCONNECT_PORT 12021

//#define PRINT_ALL_PACKETS

//#define HAVE_SOCKET_CAN_PORT
//#define SOCKET_CAN_PORT "can1"

#define USE_OPENLCB_TCP_HOST 1
#define DEFAULT_OPENLCB_TCP_PORT 12000
#define DEFAULT_OPENLCB_TCP_HOST "localhost"

#endif // __HARDWARE_HXX
