#include "ButtonLed.h"

//#include <Arduino.h>

void ButtonLed::init(uint8_t s) {
  sense = s;
  pinMode(pin,OUTPUT);                    // default to driving LED..
  digitalWrite(pin,sense);                // ..and turn it on
  lastState=false;                        // start with led off
  duration=lastDuration=0;                // init durations
  state=false;                            // start with button off
}


ButtonLed::ButtonLed(const uint8_t p,uint8_t s) : sense(s), pin(p) {      // define Button with pin and sense, 
//                                        // Arduino pin the button/LED is attached
//                                        // sense: HIGH=active high, LOW=active low. 
  init(s);
}

ButtonLed::ButtonLed(uint8_t p) : pin(p) {            // default to sense=HIGH
  init(HIGH);
}

ButtonLed::ButtonLed() {
}

void ButtonLed::setPinSense(uint8_t p, uint8_t s) {
  pin = p;
  init(s);
}  

void ButtonLed::on(long mask) {
      pattern = mask;
}

void ButtonLed::blink(uint8_t mask) {
  once |= mask;
  // wait for next time step to display
}

bool ButtonLed::unique() {
	process();
	if(state==true && lastUState==false) {
		lastUState = true;
		return true;
	} 
	lastUState=state;
	return false;	
}
//     state: F T T F
// lastState: F T T F
//   returns: F T F F 

void ButtonLed::process() {
  long now = millis();
  int period = now & 0xFFE0;                    // each period is 32 ms
  if(period != lastButtonPeriod) {              // If we are in a new period
    lastButtonPeriod = period;                  // .. remember it
    pinMode(pin, INPUT);                        // .. change the pin to input..
    digitalWrite(pin, HIGH);                    // .. and activate pull up
    newState = (sense == digitalRead(pin));     // .. and read it, is the button up or down
    pinMode(pin, OUTPUT);                       // .. return pin to output mode
    digitalWrite(pin, ledState);                // .. and make sure its showing its state
    if(newState != lastState) {                 // ..if button changed then..
      lastState = newState;                     // ....remember the button state
    } else {                                    // ..else its position is stable and so debounced
      if(state != newState) {                   // ....But is it a new state?..
        state = newState;                       // .....yes, so update
        lastDuration = duration + 32;           //       and remember the duration of the last state
        timeOfLastChange = now;                 //       so we can calc duration
        duration = 0;                           //       start timing new state
      } else {                                  // .....no, the same state is continuing, so
        duration = millis() - timeOfLastChange; // ......calculate its duration
      } 
    }
  }
  // process LED
  period = now & 0xFFC0;                          // Each period is 64 ms 
  if( period != lastLEDPeriod ) {               // if we are in a new period
    lastLEDPeriod = period;                     // .. remember it
    if ((pattern & 0x1) !=0) {                  // ..if low bit 1 then ..
      ledState = sense;                         // .... update LED and 
      pattern = 0x80000000 | (pattern>>1);      //      mimic roll with 1 in
    } else {                                    // ..else low bit is 0, so ..
      ledState = !sense;                        // ....update LED and
      pattern = 0x7FFFFFFF & (pattern>>1);      //     mimic a roll with a 0 in
    }
    if ((once & 0x1) != 0) ledState = LOW;      // handle once-through pattern
    once = once>>1;                             // ..once shifts out
    digitalWrite(pin,ledState);                 // ..set the pin
  } 
  return;
}

/* Test sketch     

ButtonLed b[64];

void setup() {
  DDRG |= 0x03;
  for(int i=0; i<50; i++) {
    b[i].setPinSense(i, LOW);
    b[i].pattern = 0x0F0F0F0FL;
  }
}
int p;
void loop() {
  if(p>=50) p=0;
  b[p++].process();
}
*/
