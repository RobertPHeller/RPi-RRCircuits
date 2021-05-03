// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon May 3 13:52:39 2021
//  Last Modified : <210503.1356>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2021  Robert Heller D/B/A Deepwoods Software
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

#ifndef __ESTOPHANDLER_HXX
#define __ESTOPHANDLER_HXX

#include <dcc/PacketSource.hxx>
#include <dcc/SimpleUpdateLoop.hxx>
#include <openlcb/Defs.hxx>
#include <openlcb/EventHandlerTemplates.hxx>
#include <utils/Atomic.hxx>
#include <utils/logging.h>

namespace BeagleCS
{

// Event handler for the E-Stop well known events. This will generate a
// continuous stream of e-stop DCC packets until the E-Stop event has been
// received or the state has been reset via API.
class EStopHandler : public openlcb::BitEventInterface
                   , public dcc::NonTrainPacketSource
                   , private Atomic
{
public:
  EStopHandler(openlcb::Node *node)
    : BitEventInterface(openlcb::Defs::EMERGENCY_STOP_EVENT
                      , openlcb::Defs::CLEAR_EMERGENCY_STOP_EVENT)
    , node_(node)
    , remaining_(0)
  {
    LOG(INFO, "[eStop] Registering emergency stop handler (On: %s, Off:%s)"
      , uint64_to_string_hex(openlcb::Defs::EMERGENCY_STOP_EVENT).c_str()
      , uint64_to_string_hex(openlcb::Defs::CLEAR_EMERGENCY_STOP_EVENT).c_str());
  }

  openlcb::EventState get_current_state() override
  {
    return openlcb::EventState::INVALID;
  }

  void set_state(bool new_value) override;

  void get_next_packet(unsigned code, dcc::Packet* packet);

  openlcb::Node *node() override
  {
    return node_;
  }

private:
  openlcb::BitEventPC pc_{this};
  openlcb::Node *node_;
  int16_t remaining_;
};

}

#endif // __ESTOPHANDLER_HXX

