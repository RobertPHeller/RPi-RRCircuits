// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Oct 20 20:21:25 2019
//  Last Modified : <191023.2121>
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

#ifndef __COMMANDSTATIONDCCMAINTRACK_HXX
#define __COMMANDSTATIONDCCMAINTRACK_HXX

#include "executor/Executor.hxx"
#include "executor/StateFlow.hxx"
#include "dcc/Packet.hxx"

#include <prussdrv.h>
#include <pruss_intc_mapping.h>

class CommandStationDCCMainTrack : public StateFlow<Buffer<dcc::Packet>, QList<1>>
{
public:
    CommandStationDCCMainTrack(Service *service, int pool_size);
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
    static constexpr int PRU_NUM = 0;
    static constexpr char const *PRU_Prog = "MainTrackDCC.bin";
    static constexpr int PRU_DATARAM = PRUSS0_PRU0_DATARAM;
    static constexpr int PRUEVENT = PRU_EVTOUT_0;
};


#endif // __COMMANDSTATIONDCCMAINTRACK_HXX

