#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "PB Command Station"

#define FANPin GpioOutputSafeLow
#define EnablePin GpioOutputSafeHigh
#define ThermFlagPin GpioInputActiveLow
#define Railcom
// Computed experimentally.
#define CurrentFromAIN(val) ((val)*.002185075)
// MCP9700T-E/LT: .5V == 0C, .01V/Degree C, 125C == 1.75V, 1.9V Vref,
// 12bit ADC (0 <= val <= 4095).
#define TempFromAIN(val) ((((val)*.0004639448)/.01)+(-50))

// On chip GPIO:
//
// Mains:
//
// DCC P8_12 (PRU 0, R30.14) [config-pin P8_12 pruout]
//                           BeagleBone: P8_12, PocketBeagle: P2_24,
//                           GPIO_44/1.12
// EN  P8-10 GPIO_86 (2.22)  [config-pin P8_29 gpio]
//                           PocketBeagle: P2_35
// CSense P9-39 AIN0         [/sys/bus/iio/devices/iio:device0/in_voltage0_raw]
//                           PocketBeagle: P1_19
// TF  P8-11 GPIO_45 (1.13)  [config-pin P8_11 gpio_input]
//                           PocketBeagle: P2_33
// BRAKE P9-12 GPIO_60 (1.28) [config-pin P9-12 gpio_input]
//                           PocketBeagle: P2_8

#define CSenseMainAnalogChannel 0
#define MAIN_MAX_MILLIAMPS 2800
#define MAIN_LIMIT_MILLIAMPS 2800
GPIO_PIN(MainEN, EnablePin, 86);
#define NO_THERMFAULT 1
GPIO_PIN(MainBRAKE, EnablePin, 60);

//
// Prog Track:
//
// DCC P8-21 (PRU 1, R30.11) [config-pin P8_30 pruout] [disable video!]
//                           BeagleBone: P8_30, PocketBeagle: P1_4,
//                           GPIO_89/2.25
// EN  P8-16 GPIO_87 (2.23)  [config-pin P8_29 gpio]
//                           PocketBeagle: P1_2
// CSense P9-40 AIN1         [/sys/bus/iio/devices/iio:device0/in_voltage1_raw]
//                           Pocketbeagle: P1_21

#define CSenseProgAnalogChannel 1
#define PROG_MAX_MILLIAMPS 2800
GPIO_PIN(ProgEN, EnablePin, 87);

// Tempsensor:
//
// P9-37 AIN2                [/sys/bus/iio/devices/iio:device0/in_voltage2_raw]
//                           Pocketbeagle: P1_23
//

#define TempsensorChannel 2

// Fan Control (out):
//
// P8-14 GPIO_26 (0.26)      [config-pin P8_14 gpio]
//                           Pocketbeagle: P1_34

GPIO_PIN(FanControl, FANPin, 26);

// Railcom:
//
// RAILCOM-ENABLE P8_15 GPIO_47 (1.15)  [config-pin P8_15 gpio output]
//                                      PocketBeagle: P2_18
// RAILCOM-SHORT P8_16 GPIO_46 (1.14)   [config-pin P8_15 gpio input]
//                                      PocketBeagle: P2_22
// RAILCOM-DIR   P8_13 GPIO_23 (0.23)   [config-pin P8_13 gpio input]
//                                      PocketBeagle: P2_3
// RAILCOM-DATA  P9_11 /dev/ttyS4       [config-pin P9_11 uart4 rx ?]
//                                      PocketBeagle: P2_5

GPIO_PIN(RailcomEN, EnablePin, 47);
GPIO_PIN(RailcomShort, ThermFlagPin, 46);
GPIO_PIN(RailcomDir, GpioInputActiveLow, 23);

typedef GpioInitializer<MainEN_Pin, ProgEN_Pin, 
                        MainBRAKE_Pin, FanControl_Pin, RailcomEN_Pin,
                        RailcomShort_Pin, RailcomDir_Pin> GpioInit;

#define RAILCOM_DATA_PORT "/dev/ttyS4"
#define RAILCOM_FEEDBACK_QUEUE 4

//#define USE_OPENLCB_TCP_HOST
//#define DEFAULT_OPENLCB_TCP_HOST "localhost"
//#define DEFAULT_OPENLCB_TCP_HOST 12000

//#define USE_GRIDCONNECT_HOST
//#define DEFAULT_TCP_GRIDCONNECT_HOST "localhost"
//#define DEFAULT_TCP_GRIDCONNECT_PORT 12021

#define START_GRIDCONNECT_SERVER

//#define PRINT_ALL_PACKETS

#define USE_SOCKET_CAN_PORT
#define DEFAULT_CAN_SOCKET "can1"

#define START_GCTCP_HUB
#define DEFAULT_GRIDCONNECT_HUB_PORT 12021

//#define TERMINALCONSOLE
#define CONSOLEPORT 9900

#endif // __HARDWARE_HXX
