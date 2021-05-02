// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Oct 20 13:40:14 2019
//  Last Modified : <210502.1601>
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

static const char rcsid[] = "@(#) : $Id$";

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
#include "executor/CallableFlow.hxx"
#include "dcc/Loco.hxx"
#include "dcc/DccOutput.hxx"
#include <AllTrainNodes.hxx>
#include "BeagleTrainDatabase.hxx"
#include "CommandStationStack.hxx"
#include "CommandStationConsole.hxx"
#include "AnalogReadSysFS.h"
#include <AllTrainNodes.hxx>
#include "BeagleTrainDatabase.hxx"
#include "CommandStationStack.hxx"
#include "HBridgeControl.hxx"
#include "FanControl.hxx"
#include "DuplexUpdateLoop.hxx" 
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#define RAILCOM_BAUD B230400
struct RailComHW
{
    using HB_BRAKE = MainBRAKE_Pin;
    using HB_ENABLE = ProgEN_Pin;
    using RC_ENABLE = RailcomEN_Pin;
    static void hw_init()
    {
    }
    static int openport()
    {
        struct termios railcomtermios;
        int fd = open(RAILCOM_DATA_PORT,O_RDWR);
        if (fd < 0) {
            LOG(FATAL,"RailComHW: Could not open %s (%d)", RAILCOM_DATA_PORT, errno);
            exit(errno);
        }
        tcgetattr(fd,&railcomtermios);
        cfmakeraw(&railcomtermios);
        cfsetspeed(&railcomtermios,RAILCOM_BAUD);
        // 1 stop bit, 8 data bits
        railcomtermios.c_cflag &= ~CSTOPB;
        railcomtermios.c_cflag &= ~CSIZE;
        railcomtermios.c_cflag |= CS8; 
        tcsetattr(fd,TCSANOW,&railcomtermios);
        return fd;
    }
    static size_t data_avail(int fd)
    {
        int bytes = 0;
        ioctl(fd,TIOCINQ,&bytes);
        return bytes;
    }
    static uint8_t readbyte(int fd)
    {
        uint8_t buff;
        /*size_t s =*/ read(fd,&buff,1);
        return buff;
    }
    static size_t readbuff(int fd,uint8_t *buf, size_t max_len)
    {
        return read(fd,buf,max_len);
    }
    static int flush(int fd)
    {
        return tcflush(fd,TCIFLUSH);
    }
  /// Number of microseconds to wait after the final packet bit completes
  /// before disabling the ENABLE pin on the h-bridge.
  static constexpr uint32_t RAILCOM_TRIGGER_DELAY_USEC = 1;

  /// Number of microseconds to wait for railcom data on channel 1.
  static constexpr uint32_t RAILCOM_MAX_READ_DELAY_CH_1 =
    177 - RAILCOM_TRIGGER_DELAY_USEC;

  /// Number of microseconds to wait for railcom data on channel 2.
  static constexpr uint32_t RAILCOM_MAX_READ_DELAY_CH_2 =
    454 - RAILCOM_MAX_READ_DELAY_CH_1;
};

BBRailComDriver<RailComHW> opsRailComDriver(RAILCOM_FEEDBACK_QUEUE);


std::unique_ptr<HBridgeControl> CommandStationConsole::mains;
std::unique_ptr<HBridgeControl> CommandStationConsole::progtrack;
std::unique_ptr<FanControl> CommandStationConsole::fan;
std::unique_ptr<openlcb::RefreshLoop> CommandStationConsole::cs_poller;
std::unique_ptr<dcc::RailcomHubFlow> CommandStationConsole::railcom_hub; 
std::unique_ptr<dcc::RailcomPrintfFlow> CommandStationConsole::railcom_dumper;
std::unique_ptr<BeagleCS::BeagleTrainDatabase> CommandStationConsole::trainDb;
std::unique_ptr<commandstation::AllTrainNodes> CommandStationConsole::trainNodes;
std::unique_ptr<CommandStationDCCMainTrack> CommandStationConsole::mainDCC;
std::unique_ptr<CommandStationDCCProgTrack> CommandStationConsole::progDCC;
std::unique_ptr<DuplexUpdateLoop> CommandStationConsole::DccPacketLoop;
std::unique_ptr<ProgrammingTrackBackend> CommandStationConsole::prog_track_backend;

CommandStationConsole::CommandStationConsole(openlcb::SimpleStackBase *stack, openlcb::TrainService *tractionService, ExecutorBase *executor, uint16_t port)
      : Console(executor,port)
, stack_(stack)
, traction_service_(tractionService)
{
    add_command("define",define_command,this);
    add_command("undefine",undefine_command,this);
    add_command("list",list_command,this);
    add_command("describe",describe_command,this);
    add_command("status",status_command,this);
    add_command("power",power_command,this);
}


CommandStationConsole::CommandStationConsole(openlcb::SimpleStackBase *stack, openlcb::TrainService *tractionService, ExecutorBase *executor, int fd_in, int fd_out, int port)
      : Console(executor,fd_in, fd_out, port)
, stack_(stack)
, traction_service_(tractionService)
{
    add_command("define",define_command,this);
    add_command("undefine",undefine_command,this);
}

void CommandStationConsole::Begin(openlcb::SimpleStackBase *stack, 
                                  openlcb::TrainService *tractionService,
                                  const HBridgeControlConfig &maincfg,
                                  const HBridgeControlConfig &progcfg,
                                  const FanControlConfig &fancfg)
{
#ifdef NO_THERMFAULT
    mains.reset(new HBridgeControl(stack->node(), 
                                   maincfg, 
                                   CSenseMainAnalogChannel, 
                                   MAIN_MAX_MILLIAMPS, 
                                   MAIN_LIMIT_MILLIAMPS, 
                                   MainEN_Pin::instance()));
#else
    mains.reset(new HBridgeControl(stack->node(),
                                   maincfg, 
                                   CSenseMainAnalogChannel, 
                                   MAIN_MAX_MILLIAMPS, 
                                   MAIN_LIMIT_MILLIAMPS, 
                                   MainEN_Pin::instance(), 
                                   MainTF_Pin::instance()));
#endif
    LOG(INFO, "[CommandStationConsole] Main track HBridgeControl setup...");
    progtrack.reset(new HBridgeControl(stack->node(),
                                       progcfg, 
                                       CSenseProgAnalogChannel, 
                                       PROG_MAX_MILLIAMPS, 
                                       ProgEN_Pin::instance()));
    LOG(INFO, "[CommandStationConsole] Prog track HBridgeControl setup...");
    fan.reset(new FanControl(stack->node(),fancfg, 
                             TempsensorChannel,
                             FanControl_Pin::instance()));
    LOG(INFO, "[CommandStationConsole] Fan control setup...");
    trainDb.reset(new BeagleCS::BeagleTrainDatabase(stack));
    LOG(INFO, "[CommandStationConsole] train DB setup...");
    trainNodes.reset(new commandstation::AllTrainNodes(trainDb.get(),
                                                       tractionService,
                                                       stack->info_flow(),
                                                       stack->memory_config_handler(),
                                                       trainDb->get_train_cdi(),
                                                       trainDb->get_temp_train_cdi()));
    LOG(INFO, "[CommandStationConsole] train nodes setup...");
    mainDCC.reset(new CommandStationDCCMainTrack(stack->service(),2));
    LOG(INFO, "[CommandStationConsole] Ops track setup...");
    progDCC.reset(new CommandStationDCCProgTrack(stack->service(),2));
    LOG(INFO, "[CommandStationConsole] Prog track setup...");
    DccPacketLoop.reset(new DuplexUpdateLoop(stack->service(),
                                             mainDCC.get(),
                                             progDCC.get()));
    LOG(INFO, "[CommandStationConsole] DCC UpdateLoop setup...");
    prog_track_backend.reset(new ProgrammingTrackBackend(stack->service(),
                                                         &CommandStationConsole::enable_prog_track_output,
                                                         &CommandStationConsole::disable_prog_track_output));
    LOG(INFO, "[CommandStationConsole] Programming Track Backend setup...");
    cs_poller.reset(new openlcb::RefreshLoop(stack->node(),
                                         {mains->polling(),
                                             progtrack->polling(),
                                             fan->polling()}));
    LOG(INFO, "[CommandStationConsole] HBridge and Fan polling setup...");
    
    mainDCC->StartPRU();
    progDCC->StartPRU();
    LOG(INFO, "[CommandStationConsole] PRUs started...");
    railcom_hub.reset(new dcc::RailcomHubFlow(stack->service()));
    LOG(INFO, "[CommandStationConsole] RailcomHub started...");
    opsRailComDriver.hw_init(railcom_hub.get());
    LOG(INFO, "[CommandStationConsole] RailComDriver initialized...");
    railcom_dumper.reset(new dcc::RailcomPrintfFlow(railcom_hub.get()));
    LOG(INFO, "[CommandStationConsole] railcom_dumper started...");
}    
    

Console::CommandStatus CommandStationConsole::define_command(FILE *fp, int argc, const char *argv[])
{
    string name, description;
    uint16_t address;
    int steps;
    if (argc == 0) {
        fprintf(fp, "Define a new locomotive\n");
        return COMMAND_OK;
    } else {
        if (argc < 7) return COMMAND_ERROR;
        if (strcmp(argv[1],"locomotive") != 0) {
            return Console::COMMAND_ERROR;
        }
        address = atoi(argv[2]);
        if (strcmp(argv[3],"steps") != 0) {
            return Console::COMMAND_ERROR;
        }
        steps = atoi(argv[4]);
        if (steps != 28 && steps != 128) return Console::COMMAND_ERROR;
        name = argv[5];
        description = argv[6];
        fprintf(stderr,"*** CommandStationConsole::define_command(): Defining loco address %d, steps %d, name %s, description %s\n",address,steps,name.c_str(),description.c_str());
        commandstation::DccMode mode;
        if (steps == 28) {
            if (address < 128) {
                mode = commandstation::DccMode::DCC_28;
            } else {
                mode = commandstation::DccMode::DCC_28_LONG_ADDRESS;
            }
        } else {
            if (address < 128) {
                mode = commandstation::DccMode::DCC_128;
            } else {
                mode = commandstation::DccMode::DCC_128_LONG_ADDRESS;
            }
        }
        openlcb::NodeID id = trainNodes->allocate_node(mode, address);
        if (id == 0) {
            fprintf(fp,"#define# false\n");
        } else {
            trainDb->set_train_name(address,name);
            trainDb->set_train_description(address,description);
            fprintf(fp,"#define# true\n");
        }
    
    } 
    return Console::COMMAND_OK;
}

Console::CommandStatus CommandStationConsole::undefine_command(FILE *fp, int argc, const char *argv[])
{
    if (argc == 0) {
        fprintf(fp, "Undefine a new locomotive\n");
        return COMMAND_OK;
    } else {
        if (argc < 3) return COMMAND_ERROR; 
        if (strcmp(argv[1],"locomotive") != 0) {
            return Console::COMMAND_ERROR;
        }
        uint16_t address = atoi(argv[2]);
        trainDb->delete_entry(address);
        fprintf(fp,"#undefine# true\n");
    }
    return Console::COMMAND_OK;
}

Console::CommandStatus CommandStationConsole::list_command(FILE *fp, int argc, const char *argv[])
{
    if (argc == 0) {
        fprintf(fp, "List locomotives\n");
    } else {
        if (argc < 2) return COMMAND_ERROR;
        if (strcmp(argv[1],"locomotives") != 0) {
            return Console::COMMAND_ERROR;
        }
        /*bool needsp = false;*/
        fprintf(fp,"#list# ");
        fprintf(fp,"%s\n",trainDb->get_all_entries_as_list().c_str());
    }
    return COMMAND_OK;
}

Console::CommandStatus CommandStationConsole::describe_command(FILE *fp, int argc, const char *argv[])
{
    if (argc == 0) {
        fprintf(fp, "Describe locomotive\n");
    } else {
        if (argc < 3) return COMMAND_ERROR;
        if (strcmp(argv[1],"locomotive") != 0) {
            return Console::COMMAND_ERROR;
        }
        uint16_t address = atoi(argv[2]);
        commandstation::DccMode mode = trainDb->get_train_mode(address);
        openlcb::TrainImpl* impl = trainNodes->get_train_impl(mode,address);
        if (impl == nullptr) {
            fprintf(fp,"#describe# false\n");
            return COMMAND_OK;
        }
        int steps;
        if ((mode & commandstation::DccMode::DCC_28) == commandstation::DccMode::DCC_28)
        {
            steps = 28;
        }
        else
        {
            steps = 128;
        }
        fprintf(fp,"#describe# %d %d ",address,steps);
        putTclBraceString(fp,trainDb->get_train_name(address).c_str());
        fputc(' ',fp);
        putTclBraceString(fp,trainDb->get_train_description(address).c_str());
        fputc(' ',fp);
        openlcb::SpeedType speed = impl->get_speed();
        fprintf(fp,"%c %.0f ",(speed.direction() == speed.FORWARD)?'F':'R',speed.mph());
        char sp = '{';
        for (uint32_t f=0; f <= 28; f++) {
            fprintf(fp,"%c%s",sp,impl->get_fn(f)?"true":"false");
            sp = ' ';
        }
        fputc('}',fp);
        fputc(' ',fp);
#if 1
        fprintf(fp,"%llu {}",0ULL); /* fake controlled and consist */
#else
        openlcb::NodeHandle controller = n.node.get()->get_controller();
        if (controller.id == 0)
        {
            if (controller.alias != 0)
            {
                openlcb::NodeIdLookupFlow nodeIdLookup((openlcb::IfCan*)(n.node.get()->iface()));
                auto result = invoke_flow(&nodeIdLookup,n.node.get(),controller);
                if (result->data()->resultCode == 0) {
                    controller.id = result->data()->handle.id;
                }
            }
        }
        fprintf(fp,"%llu ",controller.id);
        fputc('{',fp);
        bool needsp = false;
        for (int i=0; i < n.node.get()->query_consist_length(); i++)
        {
            if (needsp) fputc(' ',fp);
            openlcb::NodeID cn = n.node.get()->query_consist(i,NULL);
            fprintf(fp,"%llu", cn);
            needsp = true;
        }
        fputc('}',fp);
#endif
        fprintf(fp,"\n");
    }
    return COMMAND_OK;
}

void CommandStationConsole::putTclBraceString(FILE *fp, const char *s) const
{
    const char *p = s;
    fputc('{',fp);
    while (*p) {
        if (*p == '{' || *p == '}') fputc('\\',fp);
        fputc(*p++,fp);
    }
    fputc('}',fp);
}

Console::CommandStatus CommandStationConsole::status_command(FILE *fp, int argc, const char *argv[])
{
    if (argc == 0) {
        fprintf(fp, "Status\n");
    } else {
        uint32_t CSenseMain = mains->getLastReading();
        uint32_t CSenseProg = progtrack->getLastReading();
        double TempCent   = TempFromAIN(sysfs_adc_getvalue(TempsensorChannel));
        fprintf(fp, "#status# %7d %7d %7.3f %1d %1d %1d %1d %1d %1d %1d %1d\n",
                CSenseMain,CSenseProg,TempCent,
                mains->EnabledP(), mains->ThermalFlagP(), mains->OverCurrentP(),
                progtrack->EnabledP(), progtrack->ThermalFlagP(), progtrack->OverCurrentP(),
                fan->FanOn(), fan->AlarmOn());
    }
    return COMMAND_OK;
}

Console::CommandStatus CommandStationConsole::power_command(FILE *fp, int argc, const char *argv[], void *context)
{
    if (argc == 0) {
        fprintf(fp, "power on/off\n");
    } else {
        if (strncasecmp(argv[1],"on",2) == 0) {
            CommandStationConsole::enable_ops_track_output();
        } else {
            CommandStationConsole::disable_track_outputs();
        }
        fprintf(fp, "#power# %1d\n",CommandStationConsole::is_ops_track_output_enabled());
    }
    return COMMAND_OK;
}

DccOutput *get_dcc_output(DccOutput::Type type)
{
#ifdef DccOutputDefined
    switch (type)
    {
    case DccOutput::Type::TRACK:
        return DccOutputImpl<BeagleCS::DccHardware::OPSDccOutput>::instance();
    case DccOutput::Type::PGM:
        return DccOutputImpl<BeagleCS::DccHardware::PROGDccOutput>::instance();
    case DccOutput::Type::LCC:
        return DccOutputImpl<BeagleCS::DccHardware::LCCDccOutput>::instance();
    }
#endif
    return nullptr;
}

