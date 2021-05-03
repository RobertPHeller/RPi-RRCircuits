// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon May 3 10:50:44 2021
//  Last Modified : <210503.1113>
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

#ifndef __DCCPROGRAMMER_HXX
#define __DCCPROGRAMMER_HXX

#include <stdint.h>
#include <dcc/ProgrammingTrackBackend.hxx>
#include <dcc/DccDebug.hxx>
#include <DuplexedTrackIf.hxx>
#include <utils/Uninitialized.hxx>
#include "utils/macros.h"

namespace BeagleCS
{

class DCCProgrammer 
{
public:
    enum CV_NAMES
    {
        SHORT_ADDRESS                       = 1
              , DECODER_VERSION                     = 7
              , DECODER_MANUFACTURER                = 8
              , ACCESSORY_DECODER_MSB_ADDRESS       = 9
              , LONG_ADDRESS_MSB_ADDRESS            = 17
              , LONG_ADDRESS_LSB_ADDRESS            = 18
              , CONSIST_ADDRESS                     = 19
              , CONSIST_FUNCTION_CONTROL_F1_F8      = 21
              , CONSIST_FUNCTION_CONTROL_FL_F9_F12  = 22
              , DECODER_CONFIG                      = 29
          };
    
    static constexpr uint8_t CONSIST_ADDRESS_REVERSED_ORIENTATION = 0x80;
    static constexpr uint8_t CONSIST_ADDRESS_NO_ADDRESS = 0x00;
    
    enum DECODER_CONFIG_BITS
    {
        LOCOMOTIVE_DIRECTION        = 0
              , FL_CONTROLLED_BY_SPEED      = 1
              , POWER_CONVERSION            = 2
              , BIDIRECTIONAL_COMMUNICATION = 3
              , SPEED_TABLE                 = 4
              , SHORT_OR_LONG_ADDRESS       = 5
              , ACCESSORY_ADDRESS_MODE      = 6
              , DECODER_TYPE                = 7
          };
    
    enum CONSIST_FUNCTION_CONTROL_F1_F8_BITS
    {
        F1_BIT = 0
              , F2_BIT = 1
              , F3_BIT = 2
              , F4_BIT = 3
              , F5_BIT = 4
              , F6_BIT = 5
              , F7_BIT = 6
              , F8_BIT = 7
          };
    
    enum CONSIST_FUNCTION_CONTROL_FL_F9_F12_BITS
    {
        FL_BIT  = 0
              , F9_BIT  = 1
              , F10_BIT = 2
              , F11_BIT = 3
              , F12_BIT = 4
          };
    
    int16_t readCV(const uint16_t);
    bool writeProgCVByte(const uint16_t, const uint8_t);
    bool writeProgCVBit(const uint16_t, const uint8_t, const bool);
    void writeOpsCVByte(const uint16_t, const uint16_t, const uint8_t);
    void writeOpsCVBit(const uint16_t, const uint16_t, const uint8_t, const bool);
private:
    // number of attempts the programming track will make to read/write a CV
    static constexpr uint8_t PROG_TRACK_CV_ATTEMPTS = 3;
    bool enterServiceMode()
    {
        BufferPtr<ProgrammingTrackRequest> req =
              invoke_flow(Singleton<ProgrammingTrackBackend>::instance()
                          , ProgrammingTrackRequest::ENTER_SERVICE_MODE);
        return req->data()->resultCode == 0;
    }
    
    void leaveServiceMode()
    {
        invoke_flow(Singleton<ProgrammingTrackBackend>::instance()
                    , ProgrammingTrackRequest::EXIT_SERVICE_MODE);
    }
    
    bool sendServiceModeDecoderReset()
    {
        BufferPtr<ProgrammingTrackRequest> req =
              invoke_flow(Singleton<ProgrammingTrackBackend>::instance()
                          , ProgrammingTrackRequest::SEND_RESET, 15);
        return (req->data()->resultCode == 0);
    }
    
    bool sendServiceModePacketWithAck(dcc::Packet pkt)
    {
        BufferPtr<ProgrammingTrackRequest> req =
              invoke_flow(Singleton<ProgrammingTrackBackend>::instance()
                          , ProgrammingTrackRequest::SEND_PROGRAMMING_PACKET, pkt
                          , 15);
        return req->data()->hasAck_;
    }
    
    bool executeProgTrackWriteRequest(dcc::Packet pkt)
    {
        if (enterServiceMode())
        {
            LOG(VERBOSE, "[PROG] Resetting DCC Decoder");
            if (!sendServiceModeDecoderReset())
            {
                leaveServiceMode();
                return false;
            }
            LOG(VERBOSE, "[PROG] Sending DCC packet: %s", dcc::packet_to_string(pkt).c_str());
            if (!sendServiceModePacketWithAck(pkt))
            {
                leaveServiceMode();
                return false;
            }
            LOG(VERBOSE, "[PROG] Resetting DCC Decoder (after PROG)");
            if (!sendServiceModeDecoderReset())
            {
                leaveServiceMode();
                return false;
            }
            leaveServiceMode();
            return true;
        }
        return false;
    }
};

}
#endif // __DCCPROGRAMMER_HXX

