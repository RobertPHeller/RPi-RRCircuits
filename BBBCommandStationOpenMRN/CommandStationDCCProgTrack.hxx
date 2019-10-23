// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Oct 20 20:21:40 2019
//  Last Modified : <191023.1921>
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

#ifndef __COMMANDSTATIONDCCPROGTRACK_HXX
#define __COMMANDSTATIONDCCPROGTRACK_HXX

#include "executor/Executor.hxx"
#include "executor/StateFlow.hxx"
#include "dcc/Packet.hxx"

#include <prussdrv.h>
#include <pruss_intc_mapping.h>

class CommandStationDCCProgTrack : public StateFlow<Buffer<dcc::Packet>, QList<1>>
{
public:
    CommandStationDCCProgTrack(Service *service, int pool_size);
    FixedPool *pool() OVERRIDE
    {
        return &pool_;
    }
    void StartPRU();
protected:
    Action entry() OVERRIDE;
    
    /// @return next action.
    Action finish()
    {
        return release_and_exit();
    }
    /// Packet pool from which to allocate packets.
    FixedPool pool_;
private:
    static const int PRU_NUM = 1;
    static const char *PRU_Prog = "ProgTrackDCC.bin";
    static const int PRU_DATARAM = PRUSS0_PRU1_DATARAM;
    static const int PRUEVENT = PRU_EVOUT_1;
    void *pruDataMem_;
    dcc::Packet *pruDataMem_packet_;
};



#endif // __COMMANDSTATIONDCCPROGTRACK_HXX

