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
//  Last Modified : <210503.0934>
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
#include <AllTrainNodes.hxx>
#include "BeagleTrainDatabase.hxx"
#include "CommandStationStack.hxx"
#include "HBridgeControl.hxx"
#include "FanControl.hxx"
#include "CommandStationDCCMainTrack.hxx"
#include "CommandStationDCCProgTrack.hxx"
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
#include "Hardware.hxx"

class CommandStationConsole : public Console {
public:
    CommandStationConsole(openlcb::SimpleStackBase *stack, openlcb::TrainService *tractionService, ExecutorBase *executor, uint16_t port);
    CommandStationConsole(openlcb::SimpleStackBase *stack, openlcb::TrainService *tractionService, ExecutorBase *executor, int fd_in, int fd_out, int port = -1);
    static void Begin(openlcb::SimpleStackBase *stack, 
                      openlcb::TrainService *tractionService,
                      const HBridgeControlConfig &maincfg,
                      const HBridgeControlConfig &progcfg,
                      const FanControlConfig &fancfg);
private:
    static std::unique_ptr<HBridgeControl> mains;
    static std::unique_ptr<HBridgeControl> progtrack;
    static std::unique_ptr<FanControl> fan;
    static std::unique_ptr<openlcb::RefreshLoop> cs_poller;
    static std::unique_ptr<dcc::RailcomHubFlow> railcom_hub; 
    static std::unique_ptr<dcc::RailcomPrintfFlow> railcom_dumper;
    static std::unique_ptr<BeagleCS::BeagleTrainDatabase> trainDb;
    static std::unique_ptr<commandstation::AllTrainNodes> trainNodes;
    static std::unique_ptr<CommandStationDCCMainTrack> mainDCC;
    static std::unique_ptr<CommandStationDCCProgTrack> progDCC;
    static std::unique_ptr<BeagleCS::DuplexedTrackIf> track;
    static std::unique_ptr<dcc::SimpleUpdateLoop> dccUpdateLoop;
    static std::unique_ptr<ProgrammingTrackBackend> prog_track_backend;
    static void initiate_estop();
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
        if (!ProgEN_Pin::get())
        {
            ProgEN_Pin::set(true);
        }
    }
    static void disable_prog_track_output()
    {
        ProgEN_Pin::set(false);
    }    
    static CommandStatus define_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->define_command(fp,argc,argv);
    }
    CommandStatus define_command(FILE *fp, int argc, const char *argv[]);
    static CommandStatus undefine_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->undefine_command(fp,argc,argv);
    }
    CommandStatus undefine_command(FILE *fp, int argc, const char *argv[]);
    static CommandStatus list_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->list_command(fp,argc,argv);
    }
    CommandStatus list_command(FILE *fp, int argc, const char *argv[]);
    static CommandStatus describe_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->describe_command(fp,argc,argv);
    }
    CommandStatus describe_command(FILE *fp, int argc, const char *argv[]);
    CommandStatus status_command(FILE *fp, int argc, const char *argv[]);
    static CommandStatus status_command(FILE *fp, int argc, const char *argv[], void *context)
    {
        return static_cast<CommandStationConsole*>(context)->status_command(fp,argc,argv);
    }
    static CommandStatus power_command(FILE *fp, int argc, const char *argv[], void *context);
    void putTclBraceString(FILE *fp, const char *s) const;
    openlcb::SimpleStackBase *stack_;
    openlcb::TrainService *traction_service_;
    
};

#endif // __COMMANDSTATIONCONSOLE_HXX

