/* -*- C -*- ****************************************************************
 *
 *  System        : 
 *  Module        : 
 *  Object Name   : $RCSfile$
 *  Revision      : $Revision$
 *  Date          : $Date$
 *  Author        : $Author$
 *  Created By    : Robert Heller
 *  Created       : Thu Feb 8 20:24:17 2018
 *  Last Modified : <180210.1047>
 *
 *  Description	
 *
 *  Notes
 *
 *  History
 *	
 ****************************************************************************
 *
 *    Copyright (C) 2018  Robert Heller D/B/A Deepwoods Software
 *			51 Locke Hill Road
 *			Wendell, MA 01379-9728
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * 
 *
 ****************************************************************************/

static const char rcsid[] = "@(#) : $Id$";



#include "driverlib/pin_map.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"

#include "DCCHardware.h"

/// An enumerated type for keeping track of the state machine used in the timer1 ISR
/** Given the structure of a DCC packet, the ISR can be in one of 5 states.
      *dos_idle: there is nothing to put on the rails. In this case, the only legal thing
                 to do is to put a '1' on the rails.  The ISR should almost never be in this state.
      *dos_send_premable: A packet has been made available, and so we should broadcast the preamble: 14 '1's in a row
      *dos_send_bstart: Each data uint8_t is preceded by a '0'
      *dos_send_uint8_t: Sending the current data uint8_t
      *dos_end_bit: After the final uint8_t is sent, send a '0'.
*/                 
typedef enum  {
  dos_idle,
  dos_send_preamble,
  dos_send_bstart,
  dos_send_uint8_t,
  dos_end_bit
} DCC_output_state_t;

DCC_output_state_t DCC_state = dos_idle; //just to start out

/// The pin to send PWM on
static uint8_t dcc_pin = 0;
static volatile uint32_t currentTimerCount;
/// The currently queued packet to be put on the rails. Default is a reset packet.
uint8_t current_packet[6] = {0,0,0,0,0,0};
/// How many data uint8_ts in the queued packet?
volatile uint8_t current_packet_size = 0;
/// How many uint8_ts remain to be put on the rails?
volatile uint8_t current_uint8_t_counter = 0;
/// How many bits remain in the current data uint8_t/preamble before changing states?
volatile uint8_t current_bit_counter = 14; //init to 14 1's for the preamble
/// A fixed-content packet to send when idle
//uint8_t DCC_Idle_Packet[3] = {255,0,255};
/// A fixed-content packet to send to reset all decoders on layout
//uint8_t DCC_Reset_Packet[3] = {0,0,0};



/** S 9.1 A specifies that '1's are represented by a square wave with a
 * 
 *    half-period of 58us (valid range: 55-61us) and '0's with a half-period
 *    of >100us (valid range: 95-9900us) Because '0's are stretched to provide
 *    DC power to non-DCC locos, we need two zero counters, one for the top
 *    half, and one for the bottom half.
 *
 * 
 *    ticks per microsec is F_CPU / 1000000, so
 * 
 *     58us is (F_CPU / 1000000) * 58 is 4640 for a 80Mhz TivaC (tm4c123)
 *     100us is (F_CPU / 1000000) * 100 is 8000 for a 80Mhz TivaC (tm4c123)
 * and 9900us is (F_CPU / 1000000) * 9900 is 792000 for a 80Mhz TivaC (tm4c123)
 * 
 */

const uint32_t one_count=(F_CPU / 1000000) * 58; //58us
const uint32_t zero_high_count=(F_CPU / 1000000) * 100; //100us
const uint32_t zero_low_count=(F_CPU / 1000000) * 100; //100us

// Aliases for timer config and loading
#define DCC_TIMER            TIMER2_BASE
#define DCC_TIMER_CFG        TIMER_CFG_PERIODIC
#define DCC_TIMER_TRIGGER    TIMER_TIMA_TIMEOUT
#define DCC_TIMER_INTERRUPT  INT_TIMER2A
#define DCC_TIMER_A          TIMER_A
#define DCC_TIMER_PERIPH     SYSCTL_PERIPH_TIMER2

static void DCCIntHandler(void);

/// Setup phase: configure and enable timer1 CTC interrupt, set OC1A and OC1B to toggle on CTC
void setup_DCC_waveform_generator(int pin) {
    
    // Enable timer
    ROM_SysCtlPerPeripheralEnable(DCC_TIMER_PERIPH);
    
    //Enable processor interrupts
    ROM_IntMasterEnable();
    
    TimerIntRegister(DCC_TIMER, DCC_TIMER_A, DCCIntHandler);
    // Configure the TIMER
    ROM_TimerConfigure(DCC_TIMER, DCC_TIMER_CFG);
    
    // Initially load the timer with '1'
    ROM_TimerLoadSet(DCC_TIMER, DCC_TIMER_A, one_count);
    currentTimerCount = one_count;
    
    //Setup the interrupt for the TIMER1A timeout
    ROM_IntEnable(DCC_TIMER_INTERRUPT);
    ROM_TimerIntEnable(DCC_TIMER, DCC_TIMER_TRIGGER);
    
    // Initialize the pin
    dcc_pin = pin;
    pinMode(dcc_pin,OUTPUT);
    digitalWrite(dcc_pin,LOW);
}

void DCC_waveform_generation_hasshin()
{
    // Enable the timer
    ROM_TimerEnable(DCC_TIMER, DCC_TIMER_A);
}

static void DCCIntHandler(void)
{
    // Clear the timer interrupt
    ROM_TimerIntClear(DCC_TIMER, DCC_TIMER_TRIGGER);
    
    //in CTC mode, timer TCINT1 automatically resets to 0 when it matches OCR1A. Depending on the next bit to output,
    //we may have to alter the value in OCR1A, maybe.
    //to switch between "one" waveform and "zero" waveform, we assign a value to OCR1A.
    
    //if the pin is low, we need to use a different zero counter to enable streched-zero DC operation
    if (!digitalRead(dcc_pin)) {
        if (currentTimerCount == zero_high_count) {
            currentTimerCount = zero_low_count;
            ROM_TimerLoadSet(DCC_TIMER, DCC_TIMER_A, currentTimerCount);
        }
        digitalWrite(dcc_pin,1);
    } else //the pin is high. New cycle is begining. Here's where the real work goes.
    {
        //time to switch things up, maybe. send the current bit in the current packet.
        //if this is the last bit to send, queue up another packet (might be the idle packet).
        digitalWrite(dcc_pin,0);
        switch(DCC_state)
        {
            /// Idle: Check if a new packet is ready. If it is, fall through to dos_send_premable. Otherwise just stick a '1' out there.
        case dos_idle:
            if(!current_uint8_t_counter) //if no new packet
            {
                //          Serial.println("X");
                currentTimerCount = one_count;//just send ones if we don't know what else to do. safe bet.
                ROM_TimerLoadSet(DCC_TIMER, DCC_TIMER_A, currentTimerCount);
                
                break;
            }
            //looks like there's a new packet for us to dump on the wire!
            //for debugging purposes, let's print it out
            //        if(current_packet[1] != 0xFF)
            //        {
            //          Serial.print("Packet: ");
            //          for(uint8_t j = 0; j < current_packet_size; ++j)
            //          {
            //            Serial.print(current_packet[j],HEX);
            //            Serial.print(" ");
            //          }
            //          Serial.println("");
            //        }
            DCC_state = dos_send_preamble; //and fall through to dos_send_preamble
            /// Preamble: In the process of producing 14 '1's, counter by current_bit_counter; when complete, move to dos_send_bstart
        case dos_send_preamble:
            currentTimerCount = one_count;//just send ones if we don't know what else to do. safe bet.
            ROM_TimerLoadSet(DCC_TIMER, DCC_TIMER_A, currentTimerCount);
            //        Serial.print("P");
            if(!--current_bit_counter)
                DCC_state = dos_send_bstart;
            break;
            /// About to send a data uint8_t, but have to peceed the data with a '0'. Send that '0', then move to dos_send_uint8_t
        case dos_send_bstart:
            currentTimerCount  = zero_high_count;
            ROM_TimerLoadSet(DCC_TIMER, DCC_TIMER_A, currentTimerCount);
            DCC_state = dos_send_uint8_t;
            current_bit_counter = 8;
            //        Serial.print(" 0 ");
            break;
            /// Sending a data uint8_t; current bit is tracked with current_bit_counter, and current uint8_t with current_uint8_t_counter
        case dos_send_uint8_t:
            if(((current_packet[current_packet_size-current_uint8_t_counter])>>(current_bit_counter-1)) & 1) //is current bit a '1'?
            {
                currentTimerCount = one_count;
                ROM_TimerLoadSet(DCC_TIMER, DCC_TIMER_A, currentTimerCount);
                //          Serial.print("1");
            }
            else //or is it a '0'
            {
                currentTimerCount = zero_high_count;
                ROM_TimerLoadSet(DCC_TIMER, DCC_TIMER_A, currentTimerCount);
                //          Serial.print("0");
            }
            if(!--current_bit_counter) //out of bits! time to either send a new uint8_t, or end the packet
            {
                if(!--current_uint8_t_counter) //if not more uint8_ts, move to dos_end_bit
                {
                    DCC_state = dos_end_bit;
                }
                else //there are more uint8_ts…so, go back to dos_send_bstart
                {
                    DCC_state = dos_send_bstart;
                }
            }
            break;
            /// Done with the packet. Send out a final '1', then head back to dos_idle to check for a new packet.
        case dos_end_bit:
            currentTimerCount = one_count;
            ROM_TimerLoadSet(DCC_TIMER, DCC_TIMER_A, currentTimerCount);
            DCC_state = dos_idle;
            current_bit_counter = 14; //in preparation for a premable...
            //        Serial.println(" 1");
            break;
        }
    }
}

            
                                                   
        
