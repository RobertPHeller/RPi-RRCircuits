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
//  Last Modified : <191023.2328>
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

#include "CommandStationStack.hxx"
#include "TrainSNIP.hxx"

struct TrainNodeImpl
{
    std::unique_ptr<openlcb::TrainImpl> impl;
    std::unique_ptr<openlcb::TrainNode> node;
    std::unique_ptr<openlcb::EventHandler> is_train_event;
    std::unique_ptr<openlcb::IncomingMessageStateFlow> pip_handler;
    std::unique_ptr<openlcb::IncomingMessageStateFlow> snip_handler;
};

class CommandStationConsole : public Console {
public:
    CommandStationConsole(openlcb::SimpleInfoFlow *infoFlow, openlcb::TrainService *tractionService, ExecutorBase *executor, uint16_t port);
    CommandStationConsole(openlcb::SimpleInfoFlow *infoFlow, openlcb::TrainService *tractionService, ExecutorBase *executor, int fd_in, int fd_out, int port = -1);
private:
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
    
    openlcb::TrainService *traction_service_;
    openlcb::SimpleInfoFlow *info_flow_;
    typedef map<uint16_t, TrainNodeImpl> TrainMap;
    TrainMap trains_;
};

#endif // __COMMANDSTATIONCONSOLE_HXX

