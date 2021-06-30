#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include <os/LinuxPWM.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "PB Multifunction board"

// On chip GPIO:

// OD:       7,  19,  20,  23,
// Motor:   26,  27,  45,  46,
// Points:  47,  48,  50,  52,
// Buttons: 57,  58,  59   60,
// LEDs:    64,  65,  86   87,
// Signals: I2C1
// GPIO Header: 110, 111, 112, 113,
//              114, 115, 116, 117

#define ODPin  GpioInputActiveLow

GPIO_PIN(OD1, ODPin,  7);
GPIO_PIN(OD2, ODPin, 19);
GPIO_PIN(OD3, ODPin, 20);
GPIO_PIN(OD4, ODPin, 23);

#define MotorPin GpioOutputSafeLow
#define SensePin GpioInputActiveHigh

GPIO_PIN(Motor1, MotorPin, 26);
GPIO_PIN(Motor2, MotorPin, 27);
GPIO_PIN(Motor3, MotorPin, 45);
GPIO_PIN(Motor4, MotorPin, 46);

GPIO_PIN(Points1, SensePin, 47);
GPIO_PIN(Points2, SensePin, 48);
GPIO_PIN(Points3, SensePin, 50);
GPIO_PIN(Points4, SensePin, 52);

#define ButtonPin GpioInputActiveLow

GPIO_PIN(Button1, ButtonPin, 57);
GPIO_PIN(Button2, ButtonPin, 58);
GPIO_PIN(Button3, ButtonPin, 59);
GPIO_PIN(Button4, ButtonPin, 60);

#define LEDPin GpioOutputSafeLow

GPIO_PIN(LED1, LEDPin, 64);
GPIO_PIN(LED2, LEDPin, 65);
GPIO_PIN(LED3, LEDPin, 86);
GPIO_PIN(LED4, LEDPin, 87);

GPIO_PIN(GPIO1, GpioOutputSafeLow, 110);
GPIO_PIN(GPIO2, GpioOutputSafeLow, 111);
GPIO_PIN(GPIO3, GpioOutputSafeLow, 112);
GPIO_PIN(GPIO4, GpioOutputSafeLow, 113);
GPIO_PIN(GPIO5, GpioOutputSafeLow, 114);
GPIO_PIN(GPIO6, GpioOutputSafeLow, 115);
GPIO_PIN(GPIO7, GpioOutputSafeLow, 116);
GPIO_PIN(GPIO8, GpioOutputSafeLow, 117);

typedef GpioInitializer<OD1_Pin, OD2_Pin, OD3_Pin, OD4_Pin,
Motor1_Pin, Motor2_Pin, Motor3_Pin, Motor4_Pin,
Points1_Pin, Points2_Pin, Points3_Pin, Points4_Pin,
Button1_Pin, Button2_Pin, Button3_Pin, Button4_Pin,
LED1_Pin, LED2_Pin, LED3_Pin, LED4_Pin,
GPIO1_Pin, GPIO2_Pin, GPIO3_Pin, GPIO4_Pin,
GPIO5_Pin, GPIO6_Pin, GPIO7_Pin, GPIO8_Pin> GpioInit;

static constexpr uint32_t PWMCHIP = 8; //guess...

static constexpr uint32_t A0 = 0;
extern LinuxPWM A0_Pin;
static constexpr uint32_t A1 = 1;
extern LinuxPWM A1_Pin;
static constexpr uint32_t A2 = 2;
extern LinuxPWM A2_Pin;
static constexpr uint32_t A3 = 3;
extern LinuxPWM A3_Pin;
static constexpr uint32_t A4 = 4;
extern LinuxPWM A4_Pin;
static constexpr uint32_t A5 = 5;
extern LinuxPWM A5_Pin;
static constexpr uint32_t A6 = 6;
extern LinuxPWM A6_Pin;
static constexpr uint32_t A7 = 7;
extern LinuxPWM A7_Pin;

static constexpr uint32_t B0 = 8;
extern LinuxPWM B0_Pin;
static constexpr uint32_t B1 = 9;
extern LinuxPWM B1_Pin;
static constexpr uint32_t B2 = 10;
extern LinuxPWM B2_Pin;
static constexpr uint32_t B3 = 11;
extern LinuxPWM B3_Pin;
static constexpr uint32_t B4 = 12;
extern LinuxPWM B4_Pin;
static constexpr uint32_t B5 = 13;
extern LinuxPWM B5_Pin;
static constexpr uint32_t B6 = 14;
extern LinuxPWM B6_Pin;
static constexpr uint32_t B7 = 15;
extern LinuxPWM B7_Pin;

#define USE_SOCKET_CAN_PORT 1
#define DEFAULT_CAN_SOCKET "can1"
//#define USE_GRIDCONNECT_HOST 1
//#define DEFAULT_TCP_GRIDCONNECT_PORT 12021
//#define DEFAULT_TCP_GRIDCONNECT_HOST localhost
//#define USE_OPENLCB_TCP_HOST 1
//#define DEFAULT_OPENLCB_TCP_PORT 12000
//#define DEFAULT_OPENLCB_TCP_HOST localhost

//#define PRINT_ALL_PACKETS
//#define HAVE_SOCKET_CAN_PORT
//#define SOCKET_CAN_PORT "vcan0"

#endif // __HARDWARE_HXX
