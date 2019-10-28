#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "BBB Command Station"

#define FANPin GpioOutputSafeLow
#define EnablePin GpioOutputSafeHigh
#define ThermFlagPin GpioInputActiveLow
#define CurrentFromAIN(val) ((val)*.0007326007)
#define TempFromAIN(val) ((((val)*.0004395604)/.01)+(-50))

// On chip GPIO:
//
// Mains:
//
// DCC P8_12 (PRU 0, R30.14) [config-pin P8_12 pruout]
// EN  P8-10 GPIO_68 (2.04)  [config-pin P8_10 gpio]
// CSense P9-39 AIN0         [/sys/bus/iio/devices/iio:device0/in_voltage0_raw]
// TF  P8-11 GPIO_45 (1.13)  [config-pin P8_11 gpio_input]

#define CSenseMainAnalogChannel 0
GPIO_PIN(MainEN, EnablePin, 68);
GPIO_PIN(MainTF, ThermFlagPin, 45);


//
// Prog Track:
//
// DCC P8-21 (PRU 1, R30.12) [config-pin P8_21 pruout]
// EN  P8-16 GPIO_46 (1.14)  [config-pin P8_16 gpio]
// CSense P9-40 AIN1         [/sys/bus/iio/devices/iio:device0/in_voltage1_raw]
// TF  P8-18 GPIO_65 (2.01)  [config-pin P8_18 gpio_input]
//

#define CSenseProgAnalogChannel 1
GPIO_PIN(ProgEN, EnablePin, 46);
GPIO_PIN(ProgTF, ThermFlagPin, 65);
// Tempsensor:
//
// P9-37 AIN2                [/sys/bus/iio/devices/iio:device0/in_voltage2_raw]
//

#define TempsensorChannel 2

// Fan Control (out):
//
// P8-14 GPIO_26 (0.26)      [config-pin P8_14 gpio]
//

GPIO_PIN(FanControl, FANPin, 26);

typedef GpioInitializer<MainEN_Pin, MainTF_Pin, ProgEN_Pin, 
                        ProgTF_Pin, FanControl_Pin> GpioInit;

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
