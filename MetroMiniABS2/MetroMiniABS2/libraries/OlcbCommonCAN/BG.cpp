#include <string.h>

#include "BG.h"

#include "PCE.h"
#include "NodeMemory.h"
#include "OpenLcbCanBuffer.h"

#define UNREADY_BLINK 0xFF00FFL
#define READY_BLINK   0x1L

BG::BG(PCE* pc, ButtonLed** bC, long* pt, uint8_t n, ButtonLed* bptr, ButtonLed* gptr, OpenLcbCanBuffer* b) {
      pce = pc;
      buttons = bC;
      patterns = pt;
      nEvents = n;
      blue = bptr;
      gold = gptr;
      buffer = b;

      started = false;
      index = -1;

      // all buttons off (might be redundant, as buttons can appear twice)
      for (int i = 0; i<nEvents; i++)
          buttons[i]->on(0);

      // initial blue/gold setting
      blue->on(0); // turn off 
      blue->process();
      lastBlue = blue->state;
      gold->on(UNREADY_BLINK); // unready blink until intialized
      gold->process();
      lastGold = gold->state;
}
  
void BG::check() {
    if (!started) {
        started = true;
        gold->on(READY_BLINK); // turn off waiting to init flash, start heartbeat ready blink
    }

    // check if blue button pressed
    blue->process();
    if (lastBlue != blue->state) {
        lastBlue = blue->state;
        if (lastBlue) { // act on button down
            // check gold button state
				if (gold->state) {
                // if gold button down, send ident
                sendIdent();
            } else {
                // gold button not down, increment channel
                // turn off current channel
                if (index>=0 && index<nEvents) 
                    buttons[index]->on(0L);
                // turn on next
                index++;
                if (index >= nEvents) {  // off end, turn off blue
                    blue->on(0L);
                    index = -1;
                } else if (index>=0 && index<nEvents) {  // blink next light and turn on blue
                    buttons[index]->on(patterns[index]);
                    blue->on(~0L);
                }
            }
        }
    }

    // clear all learn settings if blue down more than 3 seconds, 
    // gold not down.
	if ( (blue->state) && (!gold->state) && (blue->duration > 3000) ) {
        // turn blue off
        blue->on(0L);
        for (int i = 0; i < nEvents; i++)
            pce->markToLearn(i, false);
    }

    // check if gold button pressed
    gold->process();
    if (lastGold != gold->state) {
        lastGold = gold->state;
			if (lastGold) { // act on down
            // check blue button state
		    if (blue->state) {
                // if blue down, send ident
                sendIdent();
            } else {
                // blue button not down
                // if gold LED lit, send message
                if (gold->pattern == ~0L) { // check for full lit
                    if (index == -1) {
                        // no selection, send new ID
                        // needs to be better
                        
                    } else if (index>=0 && index<nEvents) {
                        buttons[index]->on(0x0); // off if lit
                        pce->sendTeach(index);
                    } // otherwise, nothing to do?
                    gold->on(READY_BLINK);  // turn off (back to ready blink)
                    blue->on(0);  // turn off
                    index = -1;
                // if blue lit without gold, mark channel
                } else if (blue->pattern == ~0L) {
                    if (index>=0 && index<nEvents) {
                        buttons[index]->on(0x0); // off if lit
                        pce->markToLearn(index, true);
                    } // otherwise, nothing to do?
                    blue->on(0);  // turn off
                    index = -1;
                } else {
                    // neither, light gold to start sequence
                    gold->on(~0L);
                }
            }
        }
    }
    
    // check for factory reset, defined as both down for
    // more than 5 seconds.
	  if ( (blue->state) && (gold->state) && (blue->duration > 5000) && (gold->duration > 5000) ) {
        factoryReset();
    }
    
    // process buttons to flash LEDs
    for (int i = 0; i<nEvents; i++) {
        buttons[i]->process();
    }
}

/**
 * Send an event in response to the "ident" button pushes
 */
// ToDo: make this smarter so don't have to do wait send
Event ident(0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00);

void BG::sendIdent() {
    buffer->setProducerIdentified(&ident);
    OpenLcb_can_queue_xmt_wait(buffer);  // wait until buffer queued, need to improve this 
}

/**
 * Fire factory reset
 * ToDo: better name!  Not really a true "factory reset"
 */
void BG::factoryReset() {
    // write to keep teh node ID, but reload everything else
    NodeMemory::forceInitEvents();
    // cast a 0 to a function pointer, then dereference it. Ugly!
    (*  ((void (*)())0)  )();
}
