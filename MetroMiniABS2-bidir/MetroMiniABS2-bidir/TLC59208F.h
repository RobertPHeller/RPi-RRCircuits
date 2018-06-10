// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Jun 10 08:54:18 2018
//  Last Modified : <180610.1023>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2018  Robert Heller D/B/A Deepwoods Software
//			51 Locke Hill Road
//			Wendell, MA 01379-9728
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// 
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __TLC59208F_H
#define __TLC59208F_H

class TLC59208F {
public:
    void begin(uint8_t addr);
    void begin(void);
    
    void dimblinkmode(bool mode);
    void set_pwm(uint8_t channel, uint8_t dutycycle);
    void group_dutycycle(uint8_t dutycycle);
    void group_freq(uint8_t groupfreq);
    void led_driver_state_control(uint8_t channel,bool on,bool off,
                                  bool indivual,bool group);
private:
    uint8_t i2caddr;
    uint8_t read_register(uint8_t addr);
    void write_register(uint8_t addr, uint8_t data);
};

#define TLC59208F_ADDRESS 0x40

// registers
#define TLC59208F_MODE1      0x00
#define TLC59208F_MODE1_AI2     7
#define TLC59208F_MODE1_AI1     6
#define TLC59208F_MODE1_AI0     5
#define TLC59208F_MODE1_SLEEP   4
#define TLC59208F_MODE1_SUB1    3
#define TLC59208F_MODE1_SUB2    2
#define TLC59208F_MODE1_SUB3    1
#define TLC59208F_MODE1_ALLCALL 0
#define TLC59208F_MODE2      0x01
#define TLC59208F_MODE2_DMBLNK  5
#define TLC59208F_MODE2_OCH     3
#define TLC59208F_MODE2_WDTEN   2
#define TLC59208F_MODE2_WDTPR   0
#define TLC59208F_MODE2_WDTPR_M 0x03
#define TLC59208F_PWM0       0x02
#define TLC59208F_PWM1       0x03
#define TLC59208F_PWM2       0x04
#define TLC59208F_PWM3       0x05
#define TLC59208F_PWM4       0x06
#define TLC59208F_PWM5       0x07
#define TLC59208F_PWM6       0x08
#define TLC59208F_PWM7       0x09
#define TLC59208F_GRPPWM     0x0A
#define TLC59208F_GRPFREQ    0x0B
#define TLC59208F_LEDOUT0    0x0C
#define TLC59208F_LEDOUT1    0x0D
#define TLC59208F_LEDOUTn_LDR3 6
#define TLC59208F_LEDOUTn_LDR2 4
#define TLC59208F_LEDOUTn_LDR1 2
#define TLC59208F_LEDOUTn_LDR0 0
#define TLC59208F_LEDOUTn_LDRm_M 0x03
#define TLC59208F_SUBADR1    0x0E
#define TLC59208F_SUBADR2    0x0F
#define TLC59208F_SUBADR3    0x10
#define TLC59208F_ALLCALLADR 0x11

#endif // __TLC59208F_H

