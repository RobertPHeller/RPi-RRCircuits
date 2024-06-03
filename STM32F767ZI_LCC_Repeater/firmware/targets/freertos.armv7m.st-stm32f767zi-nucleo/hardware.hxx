
#include "Stm32Gpio.hxx"
#include "utils/GpioInitializer.hxx"
#include "BlinkerGPIO.hxx"

GPIO_PIN(LED1_RAW, LedPin, B, 0);

typedef GpioInitializer<LED1_RAW_Pin> GpioInit;

typedef LED1_RAW_Pin BLINKER_RAW_Pin;
