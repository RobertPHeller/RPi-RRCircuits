/*
 * d. bodnar  9-23-2014
 * hacked by R. Heller 1-31-2018 
 * Uses 2 line LCD for display
 * Uses CmdArduino library for DCC output
 */
#include <EEPROM.h>
int LED = 13; // LED to blink when DCC packets are sent in loop
int Enable_PIN = 8; //low to enable DCC, high to stop
#include <DCCPacket.h>
#include <DCCPacketQueue.h>
#include <DCCPacketScheduler.h>
DCCPacketScheduler dps;
#include "DCCState.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
LiquidCrystal_I2C	lcd(I2C_ADDR,16,2);//  LCD is 16 characters x 2 lines  

unsigned long previousMillis = 0; // last time update
long interval = 2000; // interval at which to do refresh (milliseconds)

int ProcessCommandLine (char *line);

void setup() {
    pinMode(LED, OUTPUT);
    pinMode(Enable_PIN, OUTPUT); 
    lcd.begin (); 
    lcd.setBacklight(HIGH);  // Switch on the backlight
    lcd.home (); // go home
    Serial.begin(115200);
    lcd.setCursor(0,0);  
    lcd.print(F("Version 1.0 "));
    lcd.setCursor(0,1);
    lcd.print(F("2-1-2018 "));
    delay(1500);
    lcd.clear();
    dps.setup();
    DCCState::setup(&dps);
    DCCState::loadState();
    DCCState::eStop();
    DCCState::updateDCC();
    Serial.println(F("Ready"));  
    lcd.home ();
    lcd.setCursor(0,1);
    lcd.print(F("Ready"));
}

void loop() {
  //this section sends DCC updates every 2 seconds (interval)
  // not sure if it is necessary but the functions are slow to respond
  // at times - may be due to the DCC library setting priorities as the
  // speed controls always work 
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;  
      DCCState::updateDCC();
      DCCState::saveState();
      digitalWrite(LED, !digitalRead(LED));
  }

  digitalWrite(Enable_PIN, LOW);// HIGH = disable DCC
  if (Serial.available() > 0) {
      char buffer[32];
      int len;
      if (Serial.available() > 0) {
          len = Serial.readBytesUntil('\r',buffer,sizeof(buffer)-2);
          if (len > 0) {
              buffer[len++] = '\n';
              buffer[len] = '\0';
              // Parse the command and send a DCC command packet -- need to revist
              // to update the LCD and/or for local state capture.
              int result = ProcessCommandLine (buffer);
              Serial.print(F("Command Processed: "));
              Serial.println(result);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print(F("Cmd Status: "));
              lcd.print(result);
              lcd.setCursor(0,1);
              Serial.println(F("Ready"));
          }
      }
  }
  dps.update();
}  //END LOOP


 

