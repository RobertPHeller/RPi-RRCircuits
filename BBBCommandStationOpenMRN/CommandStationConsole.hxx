// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Oct 20 09:45:53 2019
//  Last Modified : <221117.1623>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//! @copyright
//!    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
//!			51 Locke Hill Road
//!			Wendell, MA 01379-9728
//!
//!    This program is free software; you can redistribute it and/or modify
//!    it under the terms of the GNU General Public License as published by
//!    the Free Software Foundation; either version 2 of the License, or
//!    (at your option) any later version.
//!
//!    This program is distributed in the hope that it will be useful,
//!    but WITHOUT ANY WARRANTY; without even the implied warranty of
//!    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//!    GNU General Public License for more details.
//!
//!    You should have received a copy of the GNU General Public License
//!    along with this program; if not, write to the Free Software
//!    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//!
//!
//! @file CommandStationConsole.hxx
//!   Implements a Comand Console for the Command Station, generally
//!   connecting a "front end" (eg GUI or Web Based) via a Tcp/Ip 
//!   socket.
//! 
//////////////////////////////////////////////////////////////////////////////

#ifndef __COMMANDSTATIONCONSOLE_HXX
#define __COMMANDSTATIONCONSOLE_HXX

#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>

#include "console/Console.hxx"
#include "nmranet_config.h"
#include "openlcb/DefaultNode.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/EventService.hxx"
#include "openlcb/TractionTrain.hxx"
#include "openlcb/TrainInterface.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "openlcb/EventService.hxx"
#include "executor/PoolToQueueFlow.hxx"
#include "utils/Singleton.hxx"
#include <AllTrainNodes.hxx>
#include "BeagleTrainDatabase.hxx"
#include "CommandStationStack.hxx"
#include "HBridgeControl.hxx"
#include "FanControl.hxx"
#include "CommandStationDCCPRUTrack.hxx"
#include <dcc/ProgrammingTrackBackend.hxx>
#include "DuplexedTrackIf.hxx" 
#include <dcc/SimpleUpdateLoop.hxx>
#include "openlcb/SimpleStack.hxx"
#include "os/LinuxGpio.hxx"
#include "freertos_drivers/common/DummyGPIO.hxx"
#include "freertos_drivers/common/LoggingGPIO.hxx"
#include "BBRailComDriver.hxx"
#include "BeagleTrainDatabase.hxx"
#include <AllTrainNodes.hxx>
#include <dcc/RailcomHub.hxx>
#include <dcc/RailcomPortDebug.hxx>
#include "DCCProgrammer.hxx"
#include "EStopHandler.hxx"
#include "Hardware.hxx"


class CommandStationConsole : public Console, public BeagleCS::DCCProgrammer, public Singleton<CommandStationConsole> {
public:
    CommandStationConsole(openlcb::SimpleStackBase *stack, openlcb::TrainService *tractionService, ExecutorBase *executor, uint16_t port);
    CommandStationConsole(openlcb::SimpleStackBase *stack, openlcb::TrainService *tractionService, ExecutorBase *executor, int fd_in, int fd_out, int port = -1);
    static void Begin(openlcb::SimpleStackBase *stack, 
                      openlcb::TrainService *tractionService,
                      const HBridgeControlConfig &maincfg,
                      const HBridgeControlConfig &progcfg,
                      const FanControlConfig &fancfg,
                      const char *mainPRUfirmware = "MainTrackDCC.out",
                      const char *progPRUfirmware = "ProgTrackDCC.out");
    static void initiate_estop()
    {
        estop_handler->set_state(true);
    }
private:
    // The various (mostly Singletons) unique quasy global objects
    // that implement the Command Station
    //
    // Montitors the OPS track current
    static std::unique_ptr<HBridgeControl> mains;
    // Montitors the Prog track current
    static std::unique_ptr<HBridgeControl> progtrack;
    // Montitors the temperature and fan
    static std::unique_ptr<FanControl> fan;
    // Refresh loop for the polled producers (the three above)
    static std::unique_ptr<openlcb::RefreshLoop> cs_poller;
    // The Railcom HUB
    static std::unique_ptr<dcc::RailcomHubFlow> railcom_hub; 
    // The Railcom debug dumper
    static std::unique_ptr<dcc::RailcomPrintfFlow> railcom_dumper;
    // The lowwer level train database
    static std::unique_ptr<BeagleCS::BeagleTrainDatabase> trainDb;
    // The upper level train database
    static std::unique_ptr<commandstation::AllTrainNodes> trainNodes;
    // The DCC output interface for the OPS track
    static std::unique_ptr<CommandStationDCCPRUTrack<0>> mainDCC;
    // The DCC output interface for the Pog track
    static std::unique_ptr<CommandStationDCCPRUTrack<1>> progDCC;
    // The DCC output interface for both tracks (duplexer)
    static std::unique_ptr<BeagleCS::DuplexedTrackIf> track;
    // The DCC output update loop
    static std::unique_ptr<dcc::SimpleUpdateLoop> dccUpdateLoop;
    // The DCC pool to queue flow
    static std::unique_ptr<PoolToQueueFlow<Buffer<dcc::Packet>>> pool_translator;
    // The DCC programming track backend
    static std::unique_ptr<ProgrammingTrackBackend> prog_track_backend;
    // The emergency stop handler
    static std::unique_ptr<BeagleCS::EStopHandler> estop_handler;
    //
    // Quasy globals to access track enable pins
    static bool is_ops_track_output_enabled()
    {
        return MainEN_Pin::get();
    }
    static void enable_ops_track_output()
    {
        if (!is_ops_track_output_enabled())
        {
            MainEN_Pin::set(true);
        }
    }
    static void disable_ops_track_output()
    {
        MainEN_Pin::set(false);
    }
    static void disable_track_outputs()
    {
        disable_ops_track_output();
        disable_prog_track_output();
    }
    static void enable_prog_track_output()
    {
        //LOG(INFO,"[enable_prog_track_output] ProgEN_Pin::get() is %d",ProgEN_Pin::get());
        if (!ProgEN_Pin::get())
        {
            //LOG(INFO,"[enable_prog_track_output] Setting ProgEN_Pin.");
            ProgEN_Pin::set(true);
        }
        //LOG(INFO,"[enable_prog_track_output] ProgEN_Pin::get() is now %d",ProgEN_Pin::get());
    }
    static void disable_prog_track_output()
    {
        //LOG(INFO,"[disable_prog_track_output] ProgEN_Pin::get() is %d",ProgEN_Pin::get());
        ProgEN_Pin::set(false);
        //LOG(INFO,"[disable_prog_track_output] ProgEN_Pin::get() is now %d",ProgEN_Pin::get());
    }
    // Defined Commands
    // Define [a locomotive in the train DB]
    static CommandStatus define_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->define_command(fp,argc,argv);
    }
    CommandStatus define_command(FILE *fp, int argc, const char *argv[]);
    // Undefine [a locomotive in the train DB]
    static CommandStatus undefine_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->undefine_command(fp,argc,argv);
    }
    CommandStatus undefine_command(FILE *fp, int argc, const char *argv[]);
    // List [locomotives in the train DB]
    static CommandStatus list_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->list_command(fp,argc,argv);
    }
    CommandStatus list_command(FILE *fp, int argc, const char *argv[]);
    // Describe [a locomotive in the train DB]
    static CommandStatus describe_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->describe_command(fp,argc,argv);
    }
    CommandStatus describe_command(FILE *fp, int argc, const char *argv[]);
    // [get the] Status [of the command station]
    CommandStatus status_command(FILE *fp, int argc, const char *argv[]);
    static CommandStatus status_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->status_command(fp,argc,argv);
    }
    // Power [on/off the tracks]
    static CommandStatus power_command(FILE *fp, int argc, const char *argv[], void *context);
    // [Send an] EStop
    static CommandStatus estop_command(FILE *fp, int argc, const char *argv[], void *context);
    // Shutdown [the command station]
    static CommandStatus shutdown_command(FILE *fp, int argc, const char *argv[], void *context);
    // Readcv [byte]
    CommandStatus readcv_command(FILE *fp, int argc, const char *argv[]);
    static CommandStatus readcv_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->readcv_command(fp,argc,argv);
    }
    // Readcvwork
    CommandStatus readcvword_command(FILE *fp, int argc, const char *argv[]);
    static CommandStatus readcvword_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->readcvword_command(fp,argc,argv);
    }
    // WriteCVProgByte
    CommandStatus writeprogcvbyte_command(FILE *fp, int argc, const char *argv[]);
    static CommandStatus writeprogcvbyte_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->writeprogcvbyte_command(fp, argc, argv);
    }
    // WriteCVProgWord
    CommandStatus writeprogcvword_command(FILE *fp, int argc, const char *argv[]);
    static CommandStatus writeprogcvword_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->writeprogcvword_command(fp, argc, argv);
    }
    // WriteCVProgbit
    CommandStatus writeprogcvbit_command(FILE *fp, int argc, const char *argv[]);
    static CommandStatus writeprogcvbit_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->writeprogcvbit_command(fp, argc, argv);
    }
    // WriteCVOpsByte
    CommandStatus writeopscvbyte_command(FILE *fp, int argc, const char *argv[]);
    static CommandStatus writeopscvbyte_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->writeopscvbyte_command(fp, argc, argv);
    }
    // WriteCVOpsBit
    CommandStatus writeopscvbit_command(FILE *fp, int argc, const char *argv[]);
    static CommandStatus writeopscvbit_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->writeopscvbit_command(fp, argc, argv);
    }
    // Add braces to a string for Tcl
    void putTclBraceString(FILE *fp, const char *s) const;
    openlcb::SimpleStackBase *stack_;
    openlcb::TrainService *traction_service_;
    
};

#endif // __COMMANDSTATIONCONSOLE_HXX

