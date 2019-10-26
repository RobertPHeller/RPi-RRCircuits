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
//  Last Modified : <191026.1341>
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
#include "CommandStationStack.hxx"
#include "TrainSNIP.hxx"
#include "CommandStationConsole.hxx"


CommandStationConsole::CommandStationConsole(openlcb::SimpleInfoFlow *infoFlow, openlcb::TrainService *tractionService, ExecutorBase *executor, uint16_t port)
      : Console(executor,port)
, traction_service_(tractionService)
, info_flow_(infoFlow)
{
    add_command("define",define_command,this);
    add_command("undefine",undefine_command,this);
    add_command("list",list_command,this);
    add_command("describe",describe_command,this);
}


CommandStationConsole::CommandStationConsole(openlcb::SimpleInfoFlow *infoFlow, openlcb::TrainService *tractionService, ExecutorBase *executor, int fd_in, int fd_out, int port)
      : Console(executor,fd_in, fd_out, port)
, traction_service_(tractionService)
, info_flow_(infoFlow)
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
        TrainNodeImpl &n = trains_[address];
        if (!n.node)
        {
#if 0
            if (steps == 28) {
                if (address < 128) {
                    n.impl.reset(new dcc::Dcc28Train(dcc::DccShortAddress(address)));
                } else {
                    n.impl.reset(new dcc::Dcc28Train(dcc::DccLongAddress(address)));
                }
            } else {
                if (address < 128) {
                    n.impl.reset(new dcc::Dcc128Train(dcc::DccShortAddress(address)));
                } else {
                    n.impl.reset(new dcc::Dcc128Train(dcc::DccLongAddress(address)));
                }
            }
#else
            n.impl.reset(new openlcb::LoggingTrain(address));
#endif
            //fprintf(stderr,"*** CommandStationConsole::define_command(): created train implementation\n");
            n.node.reset(
                         new openlcb::TrainNodeForProxy(traction_service_,n.impl.get()));
            //fprintf(stderr,"*** -: created train node\n");
            n.is_train_event.reset(
               new openlcb::FixedEventProducer<
                                   openlcb::TractionDefs::IS_TRAIN_EVENT>(n.node.get()));
            //fprintf(stderr,"*** -: created FixedEventProducer\n");
            n.pip_handler.reset(
               new openlcb::ProtocolIdentificationHandler(
                     n.node.get(),
                     openlcb::Defs::EVENT_EXCHANGE | openlcb::Defs::TRACTION_CONTROL | openlcb::Defs::SIMPLE_NODE_INFORMATION));
            //fprintf(stderr,"*** -: created pip_handler\n");
            n.snip_handler.reset(
               new TrainSNIPHandler(n.node.get()->iface(),
                                    n.node.get(),
                                    info_flow_,
                                    name,description));
            //fprintf(stderr,"*** -: created snip_handler\n");
            fprintf(fp,"#define# true\n");
        } else {
            fprintf(fp,"#define# false\n");
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
        TrainNodeImpl &n = trains_[address];
        if (n.node)
        {
            n.node.get()->iface()->delete_local_node(n.node.get());
            trains_.erase(address);
            fprintf(fp,"#undefine# true\n");
        } else {
            fprintf(fp,"#undefine# false\n");
        }
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
        bool needsp = false;
        fprintf(fp,"#list# ");
        for (TrainMap::const_iterator itrain = trains_.begin();
             itrain != trains_.end();
             itrain++) {
            if (needsp) fputc(' ',fp);
            fprintf(fp,"%d",itrain->first);
            needsp = true;
        }
        fputc('\n',fp);
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
        TrainNodeImpl &n = trains_[address];
        if (n.node)
        {
            fprintf(fp,"#describe# %d ",address);
            TrainSNIPHandler *snip_handler = (TrainSNIPHandler *)n.snip_handler.get();
            putTclBraceString(fp,snip_handler->UserName());
            fputc(' ',fp);
            putTclBraceString(fp,snip_handler->UserDescription());
            fputc(' ',fp);            
            openlcb::SpeedType speed = n.impl.get()->get_speed();
            fprintf(fp,"%c %.0f ",(speed.direction() == speed.FORWARD)?'F':'R',speed.mph());
            char sp = '{';
            for (uint32_t f=0; f <= 28; f++) {
                fprintf(fp,"%c%s",sp,n.impl.get()->get_fn(f)?"true":"false");
                sp = ' ';
            }
            fputc('}',fp);
            fputc(' ',fp);
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
            fprintf(fp,"\n");
        } else {
            fprintf(fp,"#describe# %d false.\n",address);
            return COMMAND_OK;
        }
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
