// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon Feb 25 11:37:34 2019
//  Last Modified : <190225.1822>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
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

#ifndef __LAMP_HXX
#define __LAMP_HXX

#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/ConfigRepresentation.hxx"
#include "utils/ConfigUpdateListener.hxx"
#include "utils/ConfigUpdateService.hxx"
#include "openlcb/RefreshLoop.hxx"
#include <os/Gpio.hxx>
#include <stdio.h>

#define LAMPCOUNT 4

static const char LampSelectMap[] = 
    "<relation><property>0</property><value>Unused</value></relation>"

    "<relation><property>1</property><value>A0</value></relation>"
    "<relation><property>2</property><value>A1</value></relation>"
    "<relation><property>3</property><value>A2</value></relation>"
    "<relation><property>4</property><value>A3</value></relation>"
    "<relation><property>5</property><value>A4</value></relation>"
    "<relation><property>6</property><value>A5</value></relation>"
    "<relation><property>7</property><value>A6</value></relation>"
    "<relation><property>8</property><value>A7</value></relation>"

    "<relation><property>9</property><value>B0</value></relation>"
    "<relation><property>10</property><value>B1</value></relation>"
    "<relation><property>11</property><value>B2</value></relation>"
    "<relation><property>12</property><value>B3</value></relation>"
    "<relation><property>13</property><value>B4</value></relation>"
    "<relation><property>14</property><value>B5</value></relation>"
    "<relation><property>15</property><value>B6</value></relation>"
    "<relation><property>16</property><value>B7</value></relation>";

static const char LampPhaseMap[] = 
    "<relation><property>0</property><value>Steady</value></relation>"

    "<relation><property>1</property><value>A - Slow</value></relation>"
    "<relation><property>2</property><value>A - Medium</value></relation>"
    "<relation><property>3</property><value>A - Fast</value></relation>"

    "<relation><property>4</property><value>None (not used)</value></relation>"

    "<relation><property>5</property><value>B - Slow</value></relation>"
    "<relation><property>6</property><value>B - Medium</value></relation>"
    "<relation><property>7</property><value>B - Fast</value></relation>";


/// CDI Configuration for a @ref Lamp
CDI_GROUP(LampConfig);
/// Specifies the lamp selection for this lamp
CDI_GROUP_ENTRY(selection, openlcb::Uint8ConfigEntry, 
                Name("Lamp Selection"), MapValues(LampSelectMap),
                Default(0));
CDI_GROUP_ENTRY(phase, openlcb::Uint8ConfigEntry,
                Name("Lamp Phase (A-B) - Flash Rate"),
                MapValues(LampPhaseMap), Default(0));
CDI_GROUP_END();


using LampGroup = openlcb::RepeatedGroup<LampConfig, LAMPCOUNT>;

class Lamp : public ConfigUpdateListener {
public:
    enum LampID {Unused, A0, A1, A2, A3, A4, A5, A6, A7, 
              B0, B1, B2, B3, B4, B5, B6, B7};
    enum LampPhase {Steady,A_Slow,A_Medium,A_Fast,None,B_Slow,B_Medium,
              C_Fast};
    Lamp(const LampConfig &cfg) : cfg_(cfg)
    {
        lampid_ = Unused;
        phase_  = Steady;
        ConfigUpdateService::instance()->register_update_listener(this);
    }
    void factory_reset(int fd) OVERRIDE
    {
        CDI_FACTORY_RESET(cfg_.selection);
        CDI_FACTORY_RESET(cfg_.phase);
    }
    UpdateAction apply_configuration(int fd, bool initial_load,
                                     BarrierNotifiable *done) override
    {
        AutoNotify n(done);
        lampid_ = (LampID)     cfg_.selection().read(fd);
        phase_  = (LampPhase)  cfg_.phase().read(fd);
        return UPDATED;
    }
    const Gpio* Pin() const       {return pinlookup_[(int)lampid_];}
    const LampPhase Phase() const {return phase_;}
    static const Gpio* PinLookup(LampID id) {
        return pinlookup_[(int)id];
    }
private:
    const static Gpio* pinlookup_[17];
    LampID lampid_;
    LampPhase  phase_;
    const LampConfig cfg_;
};

#endif // __LAMP_HXX

