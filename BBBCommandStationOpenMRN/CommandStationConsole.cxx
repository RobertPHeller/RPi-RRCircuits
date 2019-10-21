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
//  Last Modified : <191021.0938>
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
        fprintf(fp,"defining locomotive with DCC address %d, steps %d, with name '%s', and desciption '%s'\n",
                address,steps,name.c_str(),description.c_str());
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
            fprintf(stderr,"*** CommandStationConsole::define_command(): created train implementation\n");
            n.node.reset(
                         new openlcb::TrainNodeForProxy(traction_service_,n.impl.get()));
            fprintf(stderr,"*** -: created train node\n");
            n.is_train_event.reset(
               new openlcb::FixedEventProducer<
                                   openlcb::TractionDefs::IS_TRAIN_EVENT>(n.node.get()));
            fprintf(stderr,"*** -: created FixedEventProducer\n");
            n.pip_handler.reset(
               new openlcb::ProtocolIdentificationHandler(
                     n.node.get(),
                     openlcb::Defs::EVENT_EXCHANGE | openlcb::Defs::TRACTION_CONTROL | openlcb::Defs::SIMPLE_NODE_INFORMATION));
            fprintf(stderr,"*** -: created pip_handler\n");
            n.snip_handler.reset(
               new TrainSNIPHandler(n.node.get()->iface(),
                                    n.node.get(),
                                    info_flow_,
                                    name,description));
            fprintf(stderr,"*** -: created snip_handler\n");
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
            //delete n.snip_handler.get();
            //fprintf(stderr,"*** CommandStationConsole::undefine_command(): deleted snip_handler\n");
            //n.snip_handler.reset(nullptr);
            //fprintf(stderr,"*** -: reset snip_handler pointer\n");
            //delete n.pip_handler.get();
            //fprintf(stderr,"*** -: deleted pip_handler\n");
            //n.pip_handler.reset(nullptr);
            //fprintf(stderr,"*** -: reset pip_handler pointer\n");
            //delete n.is_train_event.get();
            //fprintf(stderr,"*** -: deleted is_train_event\n");
            //n.is_train_event.reset(nullptr);
            //fprintf(stderr,"*** -: reset is_train_event pointer\n");
            //delete n.node.get();
            //fprintf(stderr,"*** -: deleted node\n");
            //n.node.reset(nullptr);
            //fprintf(stderr,"*** -: reset node pointer\n");
            //delete n.impl.get();
            //fprintf(stderr,"*** -: deleted impl\n");
            //n.impl.reset(nullptr);
            //fprintf(stderr,"*** -: reset impl pointer\n");
            trains_.erase(address);
        }
    }
    return Console::COMMAND_OK;
}

