#include <Energia.h>
#include "DCCState.h"
#include <EEPROM.h>
#define DCC_PWM PF_2
#define DCC_CurrentSense A0
#define DCC_Enable PC_5
#define DCC_Current() (analogRead(DCC_CurrentSense)*(3.0/4095))

void setup() {
  // put your setup code here, to run once:
  pinMode(DCC_PWM,OUTPUT);
  pinMode(DCC_Enable,OUTPUT);
  pinMode(DCC_CurrentSense,INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly: 
  double current = DCC_Current();
}
