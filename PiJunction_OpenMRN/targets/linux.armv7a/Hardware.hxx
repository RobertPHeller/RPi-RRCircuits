// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Oct 14 13:41:42 2018
//  Last Modified : <181014.1449>
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

#ifndef __HARDWARE_HXX
#define __HARDWARE_HXX

#include <os/LinuxGpio.hxx>
#include "utils/GpioInitializer.hxx"

// On chip GPIO:

#define MotorPin GpioOutputSafeLow
#define SensePin GpioInputActiveHigh

GPIO_PIN(Motor1, MotorPin, 17);
GPIO_PIN(Motor2, MotorPin, 18);
GPIO_PIN(Points1, SensePin, 27);
GPIO_PIN(Points2, SensePin, 22);

// Assumes Kernel config CONFIG_ARCH_NR_GPIO is 512!
#define GPIO_BASE 512

// MCP230xx expanders.  Assumes this order for dtoverlay calls:
//
// dtoverlay mcp23008 addr=0x20
// dtoverlay mcp23017 addr=0x21
// dtoverlay mcp23017 addr=0x22
//
// mcp23008 @ 0x20 == QuadSSSQuadIn:
//
//     0 Mad Hatter Lights
//     1 NC
//     2 NC
//     3 NC
//     4 MainEast OD
//     5 MainWest OD
//     6 Siding OD
//     7 S314159265358979323846 OS
//

#define QuadSSSQuadInBase (GPIO_BASE-8)

#define SSRPin GpioOutputSafeLow
#define ODPin  GpioInputActiveLow
#define LEDPin GpioOutputSafeLow


GPIO_PIN(MadHatterLights, SSRPin, QuadSSSQuadInBase+1);
GPIO_PIN(MainEast, ODPin, QuadSSSQuadInBase+4);
GPIO_PIN(MainWest, ODPin, QuadSSSQuadInBase+5);
GPIO_PIN(Siding,   ODPin, QuadSSSQuadInBase+6);
GPIO_PIN(S314159,  ODPin, QuadSSSQuadInBase+7);

// mcp23017 @ 0x21 == Raspberry Pi Tower Signals 1 (MCP23017Hat)
//
//     0-7 NC
//     8-11: CP314W (3/1 Interlocking: Black:L0, Red:L1, Yellow:L2, Green:L3)
//          8: Lower Red
//          9: Upper Red
//         10: Upper Yellow
//         11: Upper Green
//
//    12-14: MainWestSig (3 block: Red:L4, Yellow:L5, Green:L6)
//         12: Red
//         13: Yellow
//         14: Green
//    15: NC
//

#define MCP23017Hat1Base ((QuadSSSQuadInBase)-16)

GPIO_PIN(CP314WLowRed,      LEDPin, MCP23017Hat1Base+8);
GPIO_PIN(CP314WUpperRed,    LEDPin, MCP23017Hat1Base+9);
GPIO_PIN(CP314WUpperYellow, LEDPin, MCP23017Hat1Base+10);
GPIO_PIN(CP314WUpperGreen,  LEDPin, MCP23017Hat1Base+11);

GPIO_PIN(MainWestRed,       LEDPin, MCP23017Hat1Base+12);
GPIO_PIN(MainWestYellow,    LEDPin, MCP23017Hat1Base+13);
GPIO_PIN(MainWestGreen,     LEDPin, MCP23017Hat1Base+14);

// mcp23017 @ 0x22 == Raspberry Pi Tower Signals 2 (MCP23017Hat)
//
//     0-2:  SidingSig (3 block: Red:L0, Yellow:L1, Green:L2)
//          0: Red
//          1: Yellow
//          2: Green
//     3-6:  CP314S (1/3 Interlocking: Black:L3, White:L4, Blue:L5, Red:L6)
//          3: Lower Red
//          4: Lower Yellow
//          5: Lower Green
//          6: Upper Red
//
//     7: NC
//     8-10: MainEast Sig (3 block: Red:L0, Yellow:L1, Green:L2)
//          8: Red
//          9: Yellow
//         10: Green
//
//    11-25: CP314E (3/2 Interlocking: Black:L3, White:L4, Red:L5, Yellow:L6, Green:L7)
//         11: Lower Red
//         12: Lower Yellow
//         13: Upper Red
//         14: Upper Yellow
//         15: Upper Green
//

#define MCP23017Hat2Base ((MCP23017Hat1Base)-16)

GPIO_PIN(SidingRed,         LEDPin,MCP23017Hat2Base+0);
GPIO_PIN(SidingYellow,      LEDPin,MCP23017Hat2Base+1);
GPIO_PIN(SidingGreen,       LEDPin,MCP23017Hat2Base+2);

GPIO_PIN(CP314SLowerRed,    LEDPin,MCP23017Hat2Base+3);
GPIO_PIN(CP314SLowerYellow, LEDPin,MCP23017Hat2Base+3);
GPIO_PIN(CP314SLowerGreen,  LEDPin,MCP23017Hat2Base+3);
GPIO_PIN(CP314SUpperRed,    LEDPin,MCP23017Hat2Base+3);

GPIO_PIN(MainEastRed,       LEDPin,MCP23017Hat2Base+8);
GPIO_PIN(MainEastYellow,    LEDPin,MCP23017Hat2Base+9);
GPIO_PIN(MainEastGreen,     LEDPin,MCP23017Hat2Base+10);

GPIO_PIN(CP314ELowerRed,    LEDPin,MCP23017Hat2Base+11);
GPIO_PIN(CP314ELowerYellow, LEDPin,MCP23017Hat2Base+12);
GPIO_PIN(CP314EUpperRed,    LEDPin,MCP23017Hat2Base+13);
GPIO_PIN(CP314EUpperYellow, LEDPin,MCP23017Hat2Base+14);
GPIO_PIN(CP314EUpperGreen,  LEDPin,MCP23017Hat2Base+15);

typedef GpioInitializer<Motor1_Pin, Motor2_Pin, Points1_Pin, 
                        Points2_Pin, 
                        MadHatterLights_Pin, MainEast_Pin, 
                        MainWest_Pin, Siding_Pin, S314159_Pin,
                        CP314WLowRed_Pin, CP314WUpperRed_Pin, 
                        CP314WUpperYellow_Pin, CP314WUpperGreen_Pin,
                        MainWestRed_Pin, MainWestYellow_Pin, 
                        MainWestGreen_Pin,
                        SidingRed_Pin, SidingYellow_Pin, 
                        SidingGreen_Pin,
                        CP314SLowerRed_Pin, CP314SLowerYellow_Pin,
                        CP314SLowerGreen_Pin, CP314SUpperRed_Pin,
                        MainEastRed_Pin, MainEastYellow_Pin, 
                        MainEastGreen_Pin,
                        CP314ELowerRed_Pin, CP314ELowerYellow_Pin,
                        CP314EUpperRed_Pin, CP314EUpperYellow_Pin,
                        CP314EUpperGreen_Pin> GpioInit;

#endif // __HARDWARE_HXX

