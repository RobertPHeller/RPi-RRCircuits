
#include <Energia.h>
#include <EEPROM.h>
#define DCC_PWM PF_2
#define DCC_CurrentSense A0
#define DCC_Enable PC_5
#define DCC_Current() (analogRead(DCC_CurrentSense)*(3.0/4095))
#define LED GREEN_LED
#include <DCCPacket.h>
#include <DCCPacketQueue.h>
#include <DCCPacketScheduler.h>
#define DCCLIB
#include "DCCState.h"
DCCPacketScheduler dps;
#include "Parser.h"
Parser parser;
#include <LiquidCrystal_I2C.h>

unsigned long previousMillis = 0; // last time update
unsigned long interval = 2000; // interval at which to do refresh (milliseconds)

void setup() {
  // put your setup code here, to run once:
  pinMode(DCC_PWM,OUTPUT);
  pinMode(DCC_Enable,OUTPUT);
  pinMode(DCC_CurrentSense,INPUT_PULLUP);
  Serial.begin(115200);
  parser.setup();
  dps.setup(DCC_PWM);
  DCCState::setup(&dps);
  DCCState::loadState();
  DCCState::eStop();
  DCCState::updateDCC();
  Serial.println("Ready");
}

void loop() {
  // put your main code here, to run repeatedly: 
    //this section sends DCC updates every 2 seconds (interval)
    // not sure if it is necessary but the functions are slow to respond
    // at times - may be due to the DCC library setting priorities as the
    // speed controls always work
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > interval) {
        previousMillis = currentMillis;
        DCCState::updateDCC();
        digitalWrite(LED, !digitalRead(LED));
    }

    
    digitalWrite(DCC_Enable, LOW);// HIGH = disable DCC
    if (Serial.available() > 0) {
        char buffer[128];
        int len = Serial.readBytesUntil('\r',buffer,sizeof(buffer)-1);
        if (len > 0) {
            //buffer[len++] = '\n';
            buffer[len] = '\0';                                               
            // Parse the command and send a DCC command packet
            // to update the LCD and/or for local state capture.
            int result = parser.ProcessCommandLine (buffer);
            Serial.print("Command Processed: ");
            Serial.print(buffer);
            Serial.print(" - ");
            Serial.println(result);
            if (result == 0) DCCState::saveState();
        }
        Serial.println("Ready");
    }
    dps.update();
}
