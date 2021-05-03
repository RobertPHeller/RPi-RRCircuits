// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon May 3 11:20:10 2021
//  Last Modified : <210503.1132>
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

static const char rcsid[] = "@(#) : $Id$";


#include "DCCProgrammer.hxx"

#include <dcc/ProgrammingTrackBackend.hxx>
#include <dcc/DccDebug.hxx>
#include <DuplexedTrackIf.hxx>
#include <utils/Uninitialized.hxx>


namespace BeagleCS
{

int16_t DCCProgrammer::readCV(const uint16_t cv)
{
  int16_t value = -1;
  if (enterServiceMode())
  {
    for(int attempt = 0; attempt < PROG_TRACK_CV_ATTEMPTS && value == -1; attempt++) {
      LOG(INFO, "[PROG %d/%d] Attempting to read CV %d", attempt+1, PROG_TRACK_CV_ATTEMPTS, cv);
      // reset cvValue to all bits OFF
      value = 0;
      for(uint8_t bit = 0; bit < 8; bit++) {
        LOG(VERBOSE, "[PROG %d/%d] CV %d, bit [%d/7]", attempt+1, PROG_TRACK_CV_ATTEMPTS, cv, bit);
        dcc::Packet pkt;
        pkt.start_dcc_svc_packet();
        pkt.add_dcc_prog_command(0x78, cv, 0xE8 + bit);
        if (sendServiceModePacketWithAck(pkt))
        {
          LOG(VERBOSE, "[PROG %d/%d] CV %d, bit [%d/7] ON", attempt+1, PROG_TRACK_CV_ATTEMPTS, cv, bit);
          value &= (1 << bit);
        } else {
          LOG(VERBOSE, "[PROG %d/%d] CV %d, bit [%d/7] OFF", attempt+1, PROG_TRACK_CV_ATTEMPTS, cv, bit);
        }
      }
      dcc::Packet pkt;
      pkt.set_dcc_svc_verify_byte(cv, value);
      if (sendServiceModePacketWithAck(pkt))
      {
        LOG(INFO, "[PROG %d/%d] CV %d, verified as %d", attempt+1, PROG_TRACK_CV_ATTEMPTS, cv, value);
      }
      else
      {
        LOG(WARNING, "[PROG %d/%d] CV %d, could not be verified", attempt+1, PROG_TRACK_CV_ATTEMPTS, cv);
        value = -1;
      }
    }
    LOG(INFO, "[PROG] CV %d value is %d", cv, value);
    leaveServiceMode();
  }
  else
  {
    LOG_ERROR("[PROG] Failed to enter programming mode!");
  }
  return value;
}

bool DCCProgrammer::writeProgCVByte(const uint16_t cv, const uint8_t value)
{
  bool writeVerified = false;
  dcc::Packet pkt, verifyPkt;
  pkt.set_dcc_svc_write_byte(cv, value);
  verifyPkt.set_dcc_svc_verify_byte(cv, value);
  
  for(uint8_t attempt = 1;
      attempt <= PROG_TRACK_CV_ATTEMPTS && !writeVerified;
      attempt++)
  {
    LOG(INFO, "[PROG %d/%d] Attempting to write CV %d as %d", attempt
      , PROG_TRACK_CV_ATTEMPTS, cv, value);

    if (executeProgTrackWriteRequest(pkt) &&
        executeProgTrackWriteRequest(verifyPkt))
    {
      // write byte and verify byte were successful
      writeVerified = true;
    }

    if (!writeVerified)
    {
      LOG(WARNING, "[PROG %d/%d] CV %d write value %d could not be verified."
        , attempt, PROG_TRACK_CV_ATTEMPTS, cv, value);
    }
    else
    {
      LOG(INFO, "[PROG %d/%d] CV %d write value %d verified.", attempt
        , PROG_TRACK_CV_ATTEMPTS, cv, value);
    }
  }
  return writeVerified;
}

bool DCCProgrammer::writeProgCVBit(const uint16_t cv, const uint8_t bit, const bool value)
{
  bool writeVerified = false;
  dcc::Packet pkt, verifyPkt;
  pkt.set_dcc_svc_write_bit(cv, bit, value);
  verifyPkt.set_dcc_svc_verify_bit(cv, bit, value);

  for(uint8_t attempt = 1;
      attempt <= PROG_TRACK_CV_ATTEMPTS && !writeVerified;
      attempt++) {
    LOG(INFO, "[PROG %d/%d] Attempting to write CV %d bit %d as %d", attempt
      , PROG_TRACK_CV_ATTEMPTS, cv, bit, value);
    if (executeProgTrackWriteRequest(pkt) &&
        executeProgTrackWriteRequest(verifyPkt))
    {
      // write byte and verify byte were successful
      writeVerified = true;
    }

    if (!writeVerified)
    {
      LOG(WARNING, "[PROG %d/%d] CV %d write bit %d could not be verified."
        , attempt, PROG_TRACK_CV_ATTEMPTS, cv, bit);
    }
    else
    {
      LOG(INFO, "[PROG %d/%d] CV %d write bit %d verified.", attempt
        , PROG_TRACK_CV_ATTEMPTS, cv, bit);
    }
  }
  return writeVerified;
}

void DCCProgrammer::writeOpsCVByte(const uint16_t locoAddress, const uint16_t cv
                  , const uint8_t cvValue)
{
  auto track = Singleton<BeagleCS::DuplexedTrackIf>::instance();
  dcc::PacketFlowInterface::message_type *pkt = nullptr;
  mainBufferPool->alloc(&pkt);
  if (pkt)
  {
    LOG(INFO, "[OPS] Updating CV %d to %d for loco %d", cv, cvValue
      , locoAddress);

    pkt->data()->start_dcc_packet();
    if(locoAddress > 127)
    {
      pkt->data()->add_dcc_address(dcc::DccLongAddress(locoAddress));
    }
    else
    {
      pkt->data()->add_dcc_address(dcc::DccShortAddress(locoAddress));
    }
    pkt->data()->add_dcc_pom_write1(cv - 1, cvValue);
    pkt->data()->packet_header.rept_count = 3;
    track->send(pkt);
  }
  else
  {
    LOG_ERROR("[OPS] Failed to retrieve DCC Packet for programming request");
  }
}

void DCCProgrammer::writeOpsCVBit(const uint16_t locoAddress, const uint16_t cv
                 , const uint8_t bit, const bool value)
{
  auto track = Singleton<BeagleCS::DuplexedTrackIf>::instance();
  dcc::PacketFlowInterface::message_type *pkt = nullptr;
  mainBufferPool->alloc(&pkt);
  if (pkt)
  {
    LOG(INFO, "[OPS] Updating CV %d bit %d to %d for loco %d", cv, bit, value
      , locoAddress);
    pkt->data()->start_dcc_packet();
    if(locoAddress > 127)
    {
      pkt->data()->add_dcc_address(dcc::DccLongAddress(locoAddress));
    }
    else
    {
      pkt->data()->add_dcc_address(dcc::DccShortAddress(locoAddress));
    }
    // TODO add_dcc_pom_write_bit(cv, bit, value)
    pkt->data()->add_dcc_prog_command(0xe8, cv - 1
                                    , (uint8_t)(0xF0 + bit + value * 8));
    pkt->data()->packet_header.rept_count = 3;
    track->send(pkt);
  }
  else
  {
    LOG_ERROR("[OPS] Failed to retrieve DCC Packet for programming request");
  }
}


}
