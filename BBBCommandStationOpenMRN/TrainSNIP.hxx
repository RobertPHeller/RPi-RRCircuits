// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Oct 20 09:07:10 2019
//  Last Modified : <191023.2336>
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

#ifndef __TRAINSNIP_HXX
#define __TRAINSNIP_HXX

#include "executor/StateFlow.hxx"
#include "openlcb/If.hxx"
#include "openlcb/SimpleInfoProtocol.hxx"
#include "openlcb/SimpleNodeInfo.hxx"
#include "openlcb/Defs.hxx"
#include "openlcb/If.hxx"

class TrainSNIPHandler : public openlcb::IncomingMessageStateFlow
{
public:
    TrainSNIPHandler(openlcb::If *iface, openlcb::Node* node, 
                     openlcb::SimpleInfoFlow *response_flow,
                     string train_name, string train_description)
        : IncomingMessageStateFlow(iface)
        , node_(node)
        , responseFlow_(response_flow)
    {
        iface->dispatcher()->register_handler(
                                              this, openlcb::Defs::MTI_IDENT_INFO_REQUEST, openlcb::Defs::MTI_EXACT);
        SNIP_RESPONSE[0].cmd = openlcb::SimpleInfoDescriptor::LITERAL_BYTE;
        SNIP_RESPONSE[0].arg = 4;
        SNIP_RESPONSE[0].arg2 = 0;
        SNIP_RESPONSE[0].data = nullptr;
        
        SNIP_RESPONSE[1].cmd = openlcb::SimpleInfoDescriptor::C_STRING;
        SNIP_RESPONSE[1].arg = 0;
        SNIP_RESPONSE[1].arg2 = 0;
        SNIP_RESPONSE[1].data = "Deepwoods Software";
        
        SNIP_RESPONSE[2].cmd = openlcb::SimpleInfoDescriptor::C_STRING;
        SNIP_RESPONSE[2].arg = 0;
        SNIP_RESPONSE[2].arg2 = 0;
        SNIP_RESPONSE[2].data = "BBB Command Station Locomotive";
        
        SNIP_RESPONSE[3].cmd = openlcb::SimpleInfoDescriptor::C_STRING;
        SNIP_RESPONSE[3].arg = 0;
        SNIP_RESPONSE[3].arg2 = 0;
        SNIP_RESPONSE[3].data = "linux.armv7a";
        
        SNIP_RESPONSE[4].cmd = openlcb::SimpleInfoDescriptor::C_STRING;
        SNIP_RESPONSE[4].arg = 0;
        SNIP_RESPONSE[4].arg2 = 0;
        SNIP_RESPONSE[4].data = "1.0";
        
        SNIP_RESPONSE[5].cmd = openlcb::SimpleInfoDescriptor::LITERAL_BYTE;
        SNIP_RESPONSE[5].arg = 2;
        SNIP_RESPONSE[5].arg2 = 0;
        SNIP_RESPONSE[5].data = nullptr;
        
        SNIP_RESPONSE[6].cmd = openlcb::SimpleInfoDescriptor::C_STRING;
        SNIP_RESPONSE[6].arg = 0;
        SNIP_RESPONSE[6].arg2 = 0;
        user_name_ = new char[train_name.length()+1];
        strcpy(user_name_,train_name.c_str());
        SNIP_RESPONSE[6].data = user_name_;
        
        SNIP_RESPONSE[7].cmd = openlcb::SimpleInfoDescriptor::C_STRING;
        SNIP_RESPONSE[7].arg = 0;
        SNIP_RESPONSE[7].arg2 = 0;
        user_description_ = new char[train_description.length()+1];
        strcpy(user_description_,train_description.c_str());
        SNIP_RESPONSE[7].data = user_description_;
        
        SNIP_RESPONSE[8].cmd = openlcb::SimpleInfoDescriptor::END_OF_DATA;
        SNIP_RESPONSE[8].arg = 0;
        SNIP_RESPONSE[8].arg2 = 0;
        SNIP_RESPONSE[8].data = nullptr;
        
    }

    ~TrainSNIPHandler()
    {
        node_->iface()->dispatcher()->unregister_handler(
            this, openlcb::Defs::MTI_IDENT_INFO_REQUEST, openlcb::Defs::MTI_EXACT);
    }

    Action entry() OVERRIDE
    {
        if (!nmsg()->dstNode)
            return release_and_exit();
        if (node_ && nmsg()->dstNode != node_)
            return release_and_exit();
        return allocate_and_call(responseFlow_, STATE(send_response_request));
    }

    Action send_response_request()
    {
        auto *b = get_allocation_result(responseFlow_);
        b->data()->reset(nmsg(), SNIP_RESPONSE, openlcb::Defs::MTI_IDENT_INFO_REPLY);
        responseFlow_->send(b);
        return release_and_exit();
    }
    const char *UserName() const {return user_name_;}
    const char *UserDescription() const {return user_description_;}
private:
    /** Defines the SNIP response fields. */
    openlcb::SimpleInfoDescriptor SNIP_RESPONSE[9];
    char *user_name_;
    char *user_description_;
    openlcb::Node* node_;
    openlcb::SimpleInfoFlow *responseFlow_;
};

    


#endif // __TRAINSNIP_HXX

