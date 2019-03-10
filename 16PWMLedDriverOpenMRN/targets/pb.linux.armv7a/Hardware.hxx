#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include <os/LinuxPWM.hxx>
#include "utils/GpioInitializer.hxx"

#define HARDWARE_IMPL "PB 16 Channel PWM LED Driver"

// On chip GPIO:

#define OEPin GpioOutputSafeLow

GPIO_PIN(OE, OEPin, 89); // GPIO89: P1-4


typedef GpioInitializer<OE_Pin> GpioInit;

static constexpr uint32_t PWMCHIP = 0; 

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


//#define HAVE_TCP_GRIDCONNECT_HOST
//#define TCP_GRIDCONNECT_HOST "127.0.0.1"
//#define TCP_GRIDCONNECT_PORT 12021

#define PRINT_ALL_PACKETS
#define HAVE_SOCKET_CAN_PORT
#define SOCKET_CAN_PORT "can1"

#endif // __HARDWARE_HXX
