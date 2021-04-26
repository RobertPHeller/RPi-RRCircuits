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
//  Last Modified : <210426.1211>
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
#include <AllTrainNodes.hxx>
#include "BeagleTrainDatabase.hxx"
#include "CommandStationStack.hxx"
#include "CommandStationConsole.hxx"
#include "AnalogReadSysFS.h"
#include "Hardware.hxx"

CommandStationConsole::CommandStationConsole(commandstation::AllTrainNodes *trainnodes, openlcb::TrainService *tractionService, ExecutorBase *executor, uint16_t port)
      : Console(executor,port)
, traction_service_(tractionService)
, trainnodes_(trainnodes)
{
    add_command("define",define_command,this);
    add_command("undefine",undefine_command,this);
    add_command("list",list_command,this);
    add_command("describe",describe_command,this);
    add_command("status",status_command,this);
}


CommandStationConsole::CommandStationConsole(commandstation::AllTrainNodes *trainnodes, openlcb::TrainService *tractionService, ExecutorBase *executor, int fd_in, int fd_out, int port)
      : Console(executor,fd_in, fd_out, port)
, traction_service_(tractionService)
, trainnodes_(trainnodes)
{
    add_command("define",define_command,this);
    add_command("undefine",undefine_command,this);
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
        openlcb::NodeID id = trainnodes_->allocate_node(mode, address);
        if (id == 0) {
            fprintf(fp,"#define# false\n");
        } else {
            auto traindb = Singleton<BeagleCS::BeagleTrainDatabase>::instance();
            traindb->set_train_name(address,name);
            traindb->set_train_description(address,description);
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
        auto traindb = Singleton<BeagleCS::BeagleTrainDatabase>::instance();
        traindb->delete_entry(address);
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
        auto traindb = Singleton<BeagleCS::BeagleTrainDatabase>::instance();
        fprintf(fp,"%s\n",traindb->get_all_entries_as_list().c_str());
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
        auto traindb = Singleton<BeagleCS::BeagleTrainDatabase>::instance();
        commandstation::DccMode mode = traindb->get_train_mode(address);
        openlcb::TrainImpl* impl = trainnodes_->get_train_impl(mode,address);
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
        putTclBraceString(fp,traindb->get_train_name(address).c_str());
        fputc(' ',fp);
        putTclBraceString(fp,traindb->get_train_description(address).c_str());
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
        double CSenseMain = CurrentFromAIN(sysfs_adc_getvalue(CSenseMainAnalogChannel));
        double CSenseProg = CurrentFromAIN(sysfs_adc_getvalue(CSenseProgAnalogChannel));
        double TempCent   = TempFromAIN(sysfs_adc_getvalue(TempsensorChannel));
        fprintf(fp, "#status# %7.3f %7.3f %7.3f %1d %1d %1d %1d %1d %1d %1d %1d\n",
                CSenseMain,CSenseProg,TempCent,
                mains.EnabledP(), mains.ThermalFlagP(), mains.OverCurrentP(),
                progtrack.EnabledP(), progtrack.ThermalFlagP(), progtrack.OverCurrentP(),
                fan.FanOn(), fan.AlarmOn());
    }
    return COMMAND_OK;
}
