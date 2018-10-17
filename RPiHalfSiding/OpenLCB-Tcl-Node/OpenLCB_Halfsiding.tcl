#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Mon Oct 8 20:20:19 2018
#  Last Modified : <181017.1331>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2018  Robert Heller D/B/A Deepwoods Software
#			51 Locke Hill Road
#			Wendell, MA 01379-9728
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# 
#
#*****************************************************************************


set argv0 [file join  [file dirname [info nameofexecutable]] OpenLCB_Halfsiding]

package require snit;#     require the SNIT OO framework
package require LCC;#      require the OpenLCB code
package require OpenLCB_Common;# Common code
package require ParseXML;# require the XML parsing code (for the conf file)
package require gettext;#  require the localized message handler
package require log;#      require the logging package.

package require HWInit
package require Config
package require Masts
package require ABSSlaveBus
package require ConfiguredProducer
package require ConfiguredNoProducer
package require StallMotor



snit::type OpenLCB_Halfsiding {
    pragma -hastypeinfo no -hastypedestroy no -hasinstances no
    
    typecomponent transport;#         Transport layer
    typevariable nodename
    typevariable nodedescriptor
    typecomponent configuration;#     Parsed configuration
    typecomponent configurationTool;# Configuration tools
    typecomponent xmlconfig;#         Psuedo-CDI
    typevariable  consumers {};#      Object that consume events (outputs)
    typevariable  eventsconsumed {};# Events consumed.
    typevariable  producers {};#      Objects that produce events (inputs)
    typevariable  eventsproduced {};# Events produced.
    

    delegate typemethod * to configurationTool
    
    typeconstructor {
        #** @brief Global static initialization.
        #
        # Process command line.  Runs the GUI configuration tool or connects to
        # the OpenLCB network and manages the Halfsiding board
        # events.
        
        set configurationTool configuration::Configuration
        
        global argv
        global argc
        global argv0
        
        set debugnotvis 1
        set debugIdx [lsearch -exact $argv -debug]
        if {$debugIdx >= 0} {
            set debugnotvis 0
            set argv [lreplace $argv $debugIdx $debugIdx]
        }
        set configureator no
        set configureIdx [lsearch -exact $argv -configure]
        if {$configureIdx >= 0} {
            set configureator yes
            set argv [lreplace $argv $configureIdx $configureIdx]
        }
        set sampleconfiguration no
        set sampleconfigureIdx [lsearch -exact $argv -sampleconfiguration]
        if {$sampleconfigureIdx >= 0} {
            set sampleconfiguration yes
            set argv [lreplace $argv $sampleconfigureIdx $sampleconfigureIdx]
        }
        set conffile [from argv -configuration "halfsidingconf.xml"]
        #puts stderr "*** $type typeconstructor: configureator = $configureator, debugnotvis = $debugnotvis, conffile = $conffile"
        if {$configureator} {
            $type ConfiguratorGUI $conffile
            return
        }
        if {$sampleconfiguration} {
            $type SampleConfiguration $conffile
            return
        }
        set deflogfilename [format {%s.log} [file tail $argv0]]
        set logfilename [from argv -log $deflogfilename]
        if {[file extension $logfilename] ne ".log"} {append logfilename ".log"}
        close stdin
        close stdout
        close stderr
        set null /dev/null
        if {$::tcl_platform(platform) eq "windows"} {
            set null nul
        }
        open $null r
        open $null w
        set logchan [open $logfilename w]
        fconfigure $logchan  -buffering none
        
        ::log::lvChannelForall $logchan
        ::log::lvSuppress info 0
        ::log::lvSuppress notice 0
        ::log::lvSuppress debug $debugnotvis
        ::log::lvCmdForall [mytypemethod LogPuts]
        
        ::log::logMsg [_ "%s starting" $type]
        
        ::log::log debug "*** $type typeconstructor: argv = $argv"
        
        if {[catch {open $conffile r} conffp]} {
            ::log::logError [_ "Could not open %s because: %s" $conffile $conffp]
            exit 99
        }
        set confXML [read $conffp]
        close $conffp
        set configuration [$type ParseConfiguration $confXML]
        
        $type GetTransport transportConstructor transportOpts
        
        set nodename ""
        set nodedescriptor ""
        $type GetIdentification nodename nodedescriptor
        if {[catch {eval [list lcc::OpenLCBNode %AUTO% \
                          -transport $transportConstructor \
                          -eventhandler [mytypemethod _eventHandler] \
                          -generalmessagehandler [mytypemethod _messageHandler] \
                          -softwaremodel "OpenLCB Halfsiding" \
                          -softwareversion "1.0" \
                          -nodename $nodename \
                          -nodedescription $nodedescriptor \
                          -additionalprotocols {EventExchange} \
                          ] \
                          $transportOpts} transport]} {
            ::log::logError [_ "Could not open OpenLCBNode: %s" $transport]
            exit 95
        }
        $transport SendVerifyNodeID
        lappend consumers [stallmotor::StallMotor create %AUTO% \
                           -configuration [$type GetConfigurationElement turnout 0] \
                           -pin ::M0_Control]
        lappend consumers [stallmotor::StallMotor create %AUTO% \
                           -configuration [$type GetConfigurationElement turnout 1] \
                           -pin ::M1_Control]
        
        lappend producers [configuredproducer::ConfiguredProducerNoDebouncer \
                           create %AUTO% \
                           -configuration [$type GetConfigurationElement points 0] \
                           -pin ::M0_Sense \
                           -eventsendcallback [mytypemethod EventSendCallback]]
        lappend producers [configuredproducer::ConfiguredProducerNoDebouncer \
                           create %AUTO% \
                           -configuration [$type GetConfigurationElement points 1] \
                           -pin ::M1_Sense \
                           -eventsendcallback [mytypemethod EventSendCallback]]

        lappend producers [configuredproducer::ConfiguredProducer \
                           create %AUTO% \
                           -configuration [$type GetConfigurationElement occupancy] \
                           -pin ::Occupancy \
                           -eventsendcallback [mytypemethod EventSendCallback]]

        configurednoproducer::ConfiguredNoProducer \
             create %AUTO% \
             -configuration [$type GetConfigurationElement eastocc] \
             -pin ::EastOcc
        configurednoproducer::ConfiguredNoProducer \
             create %AUTO% \
             -configuration [$type GetConfigurationElement westmainocc] \
             -pin ::WestMain
        configurednoproducer::ConfiguredNoProducer \
             create %AUTO% \
             -configuration [$type GetConfigurationElement westdivocc] \
             -pin ::WestDiverg
        
        set mastsconfig [$type GetConfigurationElement masts]
        
        lappend producers [masts::MastPoints \
                           create %AUTO% \
                           -configuration [lindex [$mastsconfig getElementsByTagName points] 0] \
                           -eventsendcallback [mytypemethod EventSendCallback] \
                           -os ::Occupancy \
                           -points ::M0_Sense \
                           -point_state false \
                           -nextocc ::EastOcc \
                           -highgreen ::PointsHighGreen \
                           -highyellow ::PointsHighYellow \
                           -highred ::PointsHighRed \
                           -lowyellow ::PointsLowYellow \
                           -lowred ::PointsLowRed]
        
        lappend producers [masts::MastFrog \
                           create %AUTO% \
                           -configuration [lindex [$mastsconfig getElementsByTagName frogmain] 0] \
                           -eventsendcallback [mytypemethod EventSendCallback] \
                           -os ::Occupancy \
                           -points ::M0_Sense \
                           -point_state false \
                           -nextocc ::WestMain \
                           -green ::FrogMainGreen \
                           -yellow ::FrogMainYellow \
                           -red ::FrogMainRed]
    
        lappend producers [masts::MastFrog \
                           create %AUTO% \
                           -configuration [lindex [$mastsconfig getElementsByTagName frogdiv] 0] \
                           -eventsendcallback [mytypemethod EventSendCallback] \
                           -os ::Occupancy \
                           -points ::M0_Sense \
                           -point_state true \
                           -nextocc ::WestDiverg \
                           -green ::FrogDivGreen \
                           -yellow ::FrogDivYellow \
                           -red ::FrogDivRed]
        
        lappend producers [absslaves::ABSSlave Initialize \
                           -configuration [$type GetConfigurationElement absslave *] \
                           -eventsendcallback [mytypemethod EventSendCallback] \
                           -serialpath "/dev/ttyAMA0"]
        foreach c $consumers {
            catch {uplevel #0 $c identify-consumer}
        }
        foreach p $producers {
            catch {uplevel #0 $p identify-producer}
        }
    }
    typemethod EventSendCallback {function event args} {
        ::log::log debug "*** $type EventSendCallback $function $event $args"
        switch $function {
            producer-invalid {
                $transport ProducerIdentified $event invalid
            }
            producer-valid {
                $transport ProducerIdentified $event valid
            }
            consumer-invalid {
                $transport ConsumerIdentified $event invalid
            }
            consumer-valid {
                $transport ConsumerIdentified $event valid
            }
            send-event-report {
                $transport ProduceEvent $event
            }
        }
    }
    typemethod _eventHandler {command eventid {validity {}}} {
        #* Event Exchange handler.  Handle Event Exchange messages.
        #
        # @param command The type of event operation.
        # @param eventid The eventid.
        # @param validity The validity of the event.
        
        switch $command {
            consumerrangeidentified {
            }
            consumeridentified {
            }
            producerrangeidentified {
            }
            produceridentified {
                if {$validity eq "valid"} {
                    foreach c $consumers {
                        ::log::log debug "*** $type _eventHandler: pin is [$c cget -pinnumber]"
                        ::log::log debug "*** $type _eventHandler: event is [$eventid cget -eventidstring]"
                        $c consumeEvent $eventid
                    }
                }
            }
            learnevents {
            }
            identifyconsumer {
                foreach c $consumers {
                    $c handle_identify_consumer $eventid
                }
            }
            identifyproducer {
                foreach p $producers {
                    $p handle_identify_producer $eventid
                }
            }
            identifyevents {
                foreach c $consumers {
                    $c handle_identify_global
                }
                foreach p $producers {
                    $p handle_identify_global
                }
            }
            report {
                foreach c $consumers {
                    ::log::log debug "*** $type _eventHandler: pin is [$c cget -pinnumber]"
                    ::log::log debug "*** $type _eventHandler: event is [$eventid cget -eventidstring]"
                    $c consumeEvent $eventid
                    
                }
            }
        }
    }
    typemethod _messageHandler {message} {
        #** General message handler.
        #
        # @param message The OpenLCB message
        
        switch [format {0x%04X} [$message cget -mti]] {
            0x0490 -
            0x0488 {
                #* Verify Node ID
                $transport SendMyNodeVerifcation
            }
            0x0828 {
                #* Protocol Support Inquiry
                $transport SendMySupportedProtocols [$message cget -sourcenid]
            }
            0x0DE8 {
                #* Simple Node Information Request
                $transport SendMySimpleNodeInfo [$message cget -sourcenid]
            }
            default {
            }
        }
    }
    
    typemethod LogPuts {level message} {
        #** Log output function.
        #
        # @param level Level of log message.
        # @param message The message text.
        
        puts [::log::lv2channel $level] "[clock format [clock seconds] -format {%b %d %T}] \[[pid]\] $level $message"
    }
    
}

vwait forever
