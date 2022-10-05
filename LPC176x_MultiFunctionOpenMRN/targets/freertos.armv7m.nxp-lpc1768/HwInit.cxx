/** \copyright
 * Copyright (c) 2015, Stuart W Baker
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are  permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \file HwInit.cxx
 * This file represents the hardware initialization for the LPC1769 MCU.
 *
 * @author Stuart W. Baker
 * @date 12 April 2015
 */

#include <new>
#include <cstdint>

#include "chip.h"

#include "os/OS.hxx"
#include "Lpc17xx40xxUart.hxx"
#include "Lpc17xx40xxCan.hxx"
#include "Lpc17xx40xxGPIO.hxx"
#include "Lpc17xx40xxEEPROMEmulation.hxx"
#include "LPC17xx_40xxI2C.hxx"

/** override stdin */
const char *STDIN_DEVICE = "/dev/ser0";

/** override stdout */
const char *STDOUT_DEVICE = "/dev/ser0";

/** override stderr */
const char *STDERR_DEVICE = "/dev/ser0";

/** UART 0 serial driver instance */
static LpcUart uart0("/dev/ser0", LPC_UART3, UART3_IRQn);

/** CAN 0 driver instance */
static LpcCan can0("/dev/can0", LPC_CAN1);

/** I2C 0 driver instance */
static LPC17xx_40xxI2C i2c2("/dev/i2c0", I2C2);

extern const size_t EEPROMEmulation::SECTOR_SIZE = 0x8000;


/** EEPROM file */
static LpcEEPROMEmulation eeprom("/dev/eeprom", 16383);

extern "C" {
const uint32_t OscRateIn = 16000000;
const uint32_t RTCOscRateIn = 32768;

/** put hardware in a safe state.
 */
void hw_set_to_safe(void)
{
}

void resetblink(uint32_t pattern)
{
}


/** Fault handler for assert.
 * @param pattern LED pattern for error
 */
void diewith(uint32_t pattern)
{
    vPortClearInterruptMask(0x20);
    asm("cpsie i\n");

    while (1)
        ;
}

uint32_t clock_speed = 0;

/** Initialize the processor hardware.
 */
void hw_preinit(void)
{
    /* Globally disables interrupts until the FreeRTOS scheduler is up. */
    asm("cpsid i\n");

    /* setup clock */
    Chip_SetupXtalClocking();
    Chip_SYSCTL_SetFLASHAccess(FLASHTIM_100MHZ_CPU);

    /* enable GPIO */
    Chip_GPIO_Init(LPC_GPIO);

    /* enable pinmux */
    Chip_IOCON_Init(LPC_IOCON);
    
    /* set up CAN1 pins */
    Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 0, IOCON_FUNC1 | IOCON_MODE_INACT); 
    Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 1, IOCON_FUNC1 | IOCON_MODE_INACT); 
    
    /* Set up I2C2 pins */
    Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 10, IOCON_FUNC2 | IOCON_MODE_INACT); 
    Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 11, IOCON_FUNC2 | IOCON_MODE_INACT); 
    Chip_IOCON_EnableOD(LPC_IOCON, 0, 10);
    Chip_IOCON_EnableOD(LPC_IOCON, 0, 11);
}

} /* extern "C" */
