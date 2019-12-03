/* 
 * ------------------------------------------------------------------
 * RaildriverIO.cc - Raildriver I/O implementation
 * Created by Robert Heller on Tue Mar 27 14:33:58 2007
 * ------------------------------------------------------------------
 * Modification History: $Log$
 * Modification History: Revision 1.1  2007/04/19 17:23:21  heller
 * Modification History: April 19 Lock Down
 * Modification History:
 * Modification History: Revision 1.1  2002/07/28 14:03:50  heller
 * Modification History: Add it copyright notice headers
 * Modification History:
 * ------------------------------------------------------------------
 * Contents:
 * ------------------------------------------------------------------
 *  
 *     Model RR System, Version 2
 *     Copyright (C) 1994,1995,2002-2005  Robert Heller D/B/A Deepwoods Software
 * 			51 Locke Hill Road
 * 			Wendell, MA 01379-9728
 * 
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 *  
 */

//static char Id[] = "$Id: RaildriverIO.cc 2220 2015-12-18 15:27:16Z heller $";

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "RailDriverIO.hxx"

// Magic constants:
const unsigned short int RaildriverIO::PIEngineering = 0x05f3;		// PIEngineering's Vender code.
const unsigned short int RaildriverIO::RailDriverModernDesktop = 0x00D2;	// Rail Driver product code.
const int RaildriverIO::LEDCommand = 134;			// Command code to set the LEDs.
const int RaildriverIO::SpeakerCommand = 133;		// Command code to set the speaker state.


bool RaildriverIO::begin(const char *path)
{
        //int status;		/* Status result codes. */
    //static char buffer[1024];	/* for error messages */
    //int err = 0;
    
    if (path == NULL || *path == '\0') {
        rdriverdev = hid_open ( PIEngineering, RailDriverModernDesktop, NULL );
    } else {
        rdriverdev = hid_open_path ( path );
    }
    
    if (rdriverdev == NULL) {
        return false;
    }
    /* Set the  speedometer LED to 000 -- let operator know we've
     * got the device.
     */
    SetLEDS("000");
    return true;
}

// Destructor -- clean up allocated resources.

RaildriverIO::~RaildriverIO()
{
    //int status;		/* Status codes. */
    
    if (rdriverdev != NULL) hid_close(rdriverdev);
    hid_exit();
    ConfigUpdateService::instance()->unregister_update_listener(this);
    if (registered_) unregister_handler();
}

void RaildriverIO::factory_reset(int fd)
{
    CDI_FACTORY_RESET(cfg_.entropy);
    CDI_FACTORY_RESET(cfg_.acceleration);
    CDI_FACTORY_RESET(cfg_.brake);
    CDI_FACTORY_RESET(cfg_.maximumspeed);
}

ConfigUpdateListener::UpdateAction RaildriverIO::apply_configuration(int fd, bool initial_load, BarrierNotifiable* done)
{
    AutoNotify n(done);
    entropyFactor_ = cfg_.entropy().read(fd);
    accelerationFactor_ = cfg_.acceleration().read(fd);
    brakeFactor_ = cfg_.brake().read(fd);
    maximumSpeed_ = cfg_.maximumspeed().read(fd);
    bool needReinit = false;
    for (int i = 0; i < BUTTONCOUNT; i++) {
        openlcb::EventId cfg_buttonPushed_event = cfg_.buttons().entry(i).eventpushed().read(fd);
        openlcb::EventId cfg_buttonReleased_event = cfg_.buttons().entry(i).eventreleased().read(fd);
        if (buttonPushedEvents_[i] != cfg_buttonPushed_event ||
            buttonReleasedEvents_[i] != cfg_buttonReleased_event) {
            if (registered_) unregister_handler();
            buttonPushedEvents_[i]   = cfg_buttonPushed_event;
            buttonReleasedEvents_[i] = cfg_buttonReleased_event;
            needReinit = true;
        }
    }
    if (needReinit) {
        register_handler();
        return REINIT_NEEDED;
    }
    return UPDATED;
}


void RaildriverIO::poll_33hz(openlcb::WriteHelper* writehelper, 
                             Notifiable* done)
{
    unsigned char reportbuffer[14];	// Buffer.
    size_t i, xfered;			// Index, status.
    bool controls_updated = false;
#ifdef DEBUG
    fprintf(stderr,"*** RaildriverIO::ReadInputs()\n");
#endif
    //Read the device.
    
    xfered = hid_read_timeout(rdriverdev,(unsigned char *)reportbuffer,sizeof(reportbuffer),100);
#ifdef DEBUG
    fprintf(stderr,"*** RaildriverIO::ReadInputs: after xfered: xfered = %d\n",xfered);
#endif
    // If the read was successful, procede to update the mask and data
    // buffer.
#ifdef DEBUG
    fprintf(stderr,"*** RaildriverIO::ReadInputs: before test, xfered == sizeof(reportbuffer) is %d\n",xfered == sizeof(reportbuffer));
#endif
    if (xfered == sizeof(reportbuffer)) {
        // For all buffer elements and all mask bits...
        for (i = 0;
             i < xfered;
             i++) {
            // If byte has changed, copy it and set its mask bit.
#ifdef DEBUG
            fprintf(stderr,"*** RaildriverIO::ReadInputs (before test): reportbuffer[%d] = 0x%02x,temp = 0x%08x\n",i,reportbuffer[i],temp);
            fprintf(stderr,"*** RaildriverIO::ReadInputs (before test): RDInput.ReportBuffer[%d] = 0x%02x\n",i,RDInput.ReportBuffer[i]);
#endif
            if (reportbuffer[i] != RDInput.ReportBuffer[i]) {
                /* handle bits or whatever... */
                switch (1<<i) {
                case REVERSER_M:
                    if (reportbuffer[i] < 64) reverserPosition_ = Forward;
                    else if (reportbuffer[i] > 220) reverserPosition_ = Reverse;
                    else if (reportbuffer[i] < 128 && reportbuffer[i] > 100) reverserPosition_ = Neutral;
                    controls_updated = true;
                    break;
                case THROTTLE_M:
                    throttle_ = reportbuffer[i];
                    controls_updated = true;
                    break;
                case AUTOBRAKE_M:
                    autobrake_ = reportbuffer[i];
                    controls_updated = true;
                    break;
                case INDEPENDBRK_M:
                    indenpendbrk_ = reportbuffer[i];
                    controls_updated = true;
                    break;
                case BAILOFF_M:
                    bailoff_ = reportbuffer[i];
                    controls_updated = true;
                    break;
                case WIPER_M:
                    //processWiper(reportbuffer[i]);
                    break;
                case HEADLIGHT_M:
                    //processHeadlight(reportbuffer[i]);
                    break;
                case DIGITAL1_M:
                    process8Buttons(0,reportbuffer[i],RDInput.ReportBuffer[i],done);
                    break;
                case DIGITAL2_M:
                    process8Buttons(8,reportbuffer[i],RDInput.ReportBuffer[i],done);
                    break;
                case DIGITAL3_M:
                    process8Buttons(16,reportbuffer[i],RDInput.ReportBuffer[i],done);
                    break;
                case DIGITAL4_M:
                    process4Buttons(24,reportbuffer[i],RDInput.ReportBuffer[i],done);
                    //processZoomUp(0x10,reportbuffer[i],RDInput.ReportBuffer[i]);
                    //processZoomDown(0x20,reportbuffer[i],RDInput.ReportBuffer[i]);
                    //processPanUp(0x40,reportbuffer[i],RDInput.ReportBuffer[i]);
                    //processPanRight(0x080,reportbuffer[i],RDInput.ReportBuffer[i]);
                    break;
                case DIGITAL5_M:
                    //processPanDown(0x01,reportbuffer[i],RDInput.ReportBuffer[i]);
                    //processPanLeft(0x02,reportbuffer[i],RDInput.ReportBuffer[i]);
                    //processRangeUp(0x04,reportbuffer[i],RDInput.ReportBuffer[i]);
                    //processRangeDown(0x08,reportbuffer[i],RDInput.ReportBuffer[i]);
                    //processEBrakeUp(0x10,reportbuffer[i],RDInput.ReportBuffer[i]);
                    //processEBrakeDown(0x20,reportbuffer[i],RDInput.ReportBuffer[i]);
                    //processAlert(0x40,reportbuffer[i],RDInput.ReportBuffer[i]);
                    //processSand(0x080,reportbuffer[i],RDInput.ReportBuffer[i]);
                    break;
                case DIGITAL6_M:
                    //processPantograph(0x01,reportbuffer[i],RDInput.ReportBuffer[i]);
                    //processBell(0x02,reportbuffer[i],RDInput.ReportBuffer[i]);
                    //processWhistleUp(0x04,reportbuffer[i],RDInput.ReportBuffer[i]);
                    //processWhistleDown(0x08,reportbuffer[i],RDInput.ReportBuffer[i]);
                    break;
                default:
                    break;
                }
                RDInput.ReportBuffer[i] = reportbuffer[i];
#ifdef DEBUG
                fprintf(stderr,"*** RaildriverIO::ReadInputs: reportbuffer[%d] = 0x%02x,temp = 0x%08x\n",i,reportbuffer[i],temp);
                fprintf(stderr,"*** RaildriverIO::ReadInputs: newMask = 0x%08x\n",newMask);
#endif
            }
        }
    }
    if (controls_updated) 
    {
        // compute and send new traction commands
    }
    //status = hid_get_manufacturer_string(rdriverdev,tempstring,sizeof(tempstring)-1);
#ifdef DEBUG
    fprintf(stderr,"*** RaildriverIO::ReadInputs: after log\n");
#endif
    
}

void RaildriverIO::process8Buttons(int boff,uint8_t readbyte, uint8_t storedbyte, Notifiable *done)
{
    uint8_t mask;
    int i, button;
    for (i = 0; i < 8; i++) {
        button = boff+i;
        mask   = 1 << i;
        if ((readbyte&mask) != (storedbyte&mask)) {
            if ((readbyte&mask) != 0) {
                SendEventReport(button,buttonPushedEvents_[button],done);
            } else {
                SendEventReport(button,buttonReleasedEvents_[button],done);
            }
        }
    }
}

void RaildriverIO::process4Buttons(int boff,uint8_t readbyte, uint8_t storedbyte, Notifiable *done)
{
    uint8_t mask;
    int i, button;
    for (i = 0; i < 4; i++) {
        button = boff+i;
        mask   = 1 << i;
        if ((readbyte&mask) != (storedbyte&mask)) {
            if ((readbyte&mask) != 0) {
                SendEventReport(button,buttonPushedEvents_[button],done);
            } else {
                SendEventReport(button,buttonReleasedEvents_[button],done);
            }
        }
    }
}


// Seven segment lookup table.
static const unsigned char SevenSegment[] = {
	0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
#define BLANKSEGMENT 0x00
#define DASHSEGMENT  0x40
#define DPSEGMENT    0x80
// Set the speedometer LEDS.
void RaildriverIO::SetLEDS(const char *ledstring)
{
    unsigned char buff[8];	// Segment buffer.
    int id;
    size_t status/*,xfered*/;	// Indexes and status.
    const char *digit;	// Current digit.
    //static char buffer[1024];

    memset(buff,0,sizeof(buff));	// Clear buffer.
    buff[0] = LEDCommand;		// Set up LED Command code.
    // If there is a LED string...
    if (ledstring != NULL) {
        id = 3;			// Start with the leftmost digit.
        digit = ledstring;	// First digit.
        // While there are both digits and digit positions.
        while (*digit != '\0' && id > 0) {
            // Skip non digits.
            while (*digit != '\0' && !isdigit(*digit) &&
                   *digit != '_' && *digit != '-') {digit++;}
            // Out of digits? Break out of the loop/
            if (*digit == '\0') break;
            // Get seven segment code for digit.
            if (isdigit(*digit)) buff[id] = SevenSegment[(*digit) - '0'];
            else if (*digit == '_') buff[id] = BLANKSEGMENT;
            else if (*digit == '-') buff[id] = DASHSEGMENT;
            // Next character.
            digit++;
            // Is it a decimal point?  If so, OR in the decimal
            // point segment.
            if (*digit == '.') {
                buff[id] |= DPSEGMENT;
                digit++;
            }
            // Next digit position.
            id--;
        }
    }
    // Write out to Rail Driver.
    
    status = hid_write(rdriverdev,(unsigned char *)buff,sizeof(buff));
#ifdef DEBUG
    fprintf(stderr,"*** RaildriverIO::SetLEDS(): LEDs set, status = %d, xfered = %d\n",status,xfered);
#endif
    if (status < sizeof(buff)) {
    }
}

// Turn speaker on.
void RaildriverIO::SpeakerOn()
{
	unsigned char buff[8];
	size_t status/*,xfered*/;
	//static char buffer[1024];

	memset(buff,0,sizeof(buff));	// Clear buffer.
	buff[0] = SpeakerCommand;	// Speaker command.
	buff[6] = 1;			// On.
	// Write out to Rail Driver.
        status = hid_write(rdriverdev,(unsigned char *)buff,sizeof(buff));
	if (status < sizeof(buff)) {
	}
}

// Turn speaker off.
void RaildriverIO::SpeakerOff()
{
	unsigned char buff[8];
	size_t status/*,xfered*/;
	//static char buffer[1024];

	memset(buff,0,sizeof(buff));	// Clear buffer.
	buff[0] = SpeakerCommand;	// Speaker command
	buff[6] = 0;			// Off.
	// Write out to Rail Driver.
	status = hid_write(rdriverdev,(unsigned char *)buff,sizeof(buff));
	if (status < sizeof(buff)) {
	}
}

void RaildriverIO::handle_identify_global(openlcb::EventRegistryEntry const&registry_entry, openlcb::EventReport* event, BarrierNotifiable* done)
{
}

void RaildriverIO::handle_identify_producer(const EventRegistryEntry &registry_entry,
                                                 openlcb::EventReport *event, 
                                                 BarrierNotifiable *done)
{
}


void RaildriverIO::register_handler()
{
}

void RaildriverIO::unregister_handler()
{
}

void RaildriverIO::SendEventReport(int helperIndex, openlcb::EventId event, Notifiable *done)
{
}

