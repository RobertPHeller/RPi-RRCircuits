/*
 * d. bodnar  9-23-2014
 * hacked by R. Heller 1-31-2018 
 * Uses 2 line LCD for display
 * Uses CmdArduino library for DCC output
 */ 
#include<EEPROM.h>
int LED = 13; // LED to blink when DCC packets are sent in loop
int Enable_PIN = 8; //low to enable DCC, high to stop
#include <DCCPacket.h>
#include <DCCPacketQueue.h>
#include <DCCPacketScheduler.h>
DCCPacketScheduler dps;
//unsigned int analog_value=0;
char speed_byte, old_speed = 0;
char temp;
byte Fx = 0;
byte DCCAddress = 3;
int inMenu = 0;  // keeps track of being in the menu area 0=out, 1=in
int digits = 0;
int upFlag = 0;  // trying to get keys to repeat!
int dnFlag = 0;
#include <Wire.h>
//#include <LCD.h>
//#include <LiquidCrystal_I2C.h>
#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
byte fn0to4 = 0;  // DCC function variables
byte fn5to8 = 0;
byte fn9to12 = 0;
//LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

unsigned long previousMillis = 0; // last time update
long interval = 2000; // interval at which to do refresh (milliseconds)

int ProcessCommandLine (char *line,DCCPacketScheduler *dps);

void setup() {
  pinMode(LED, OUTPUT);
  DCCAddress = EEPROM.read(0);  
  if(DCCAddress >=100){  // set defalut as 3 if not in proper range (0-99)
    DCCAddress = 3;
  }
  pinMode(Enable_PIN, OUTPUT); 
  //lcd.begin (16,2); //  LCD is 16 characters x 2 lines  
  //lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  //lcd.setBacklight(HIGH);  // Switch on the backlight
  //lcd.home (); // go home
  Serial.begin(115200);
  //lcd.setCursor(0,0);  
  //lcd.print("Version 2.7 ");
  //lcd.setCursor(0,1);
  //lcd.print("9-23-2014 ");
  delay(1500);
  //lcd.clear();
  dps.setup();
  dps.setFunctions0to4(DCCAddress,DCC_SHORT_ADDRESS, B00000000); //clear functions
  dps.setFunctions5to8(DCCAddress,DCC_SHORT_ADDRESS, B00000000);    
}

void loop() {
  //this section sends DCC updates every 2 seconds (interval)
  // not sure if it is necessary but the functions are slow to respond
  // at times - may be due to the DCC library setting priorities as the
  // speed controls always work 
  //unsigned long currentMillis = millis();
  //if(currentMillis - previousMillis > interval) {
  //  previousMillis = currentMillis;  
  //  dps.setSpeed128(DCCAddress,DCC_SHORT_ADDRESS,speed_byte);
  //  dps.setFunctions0to4(DCCAddress,DCC_SHORT_ADDRESS, fn0to4); //clear all functions
  //  dps.setFunctions5to8(DCCAddress,DCC_SHORT_ADDRESS, fn5to8); 
  //  digitalWrite(LED, !digitalRead(LED));
  //}

  digitalWrite(Enable_PIN, LOW);// HIGH = disable DCC
  if (Serial.available() > 0) {
      char buffer[4096];
      int len;
      if (Serial.available() > 0) {
          len = Serial.readBytesUntil('\r',buffer,sizeof(buffer)-2);
          if (len == 0) return;
          buffer[len++] = '\n';
          buffer[len] = '\0';
          // Parse the command and send a DCC command packet -- need to revist
          // to update the LCD and/or for local state capture.
          ProcessCommandLine (buffer,&dps); 
      }
  }
  //lcd.setCursor(0,0);  
  //lcd.print("Speed=");
  //lcd.print(speed_byte, DEC);
  //lcd.print("  ");  
  //lcd.setCursor(11,0);
  //lcd.print("ad=");
  //if(DCCAddress <=9){
  //  lcd.print("0");  // add leading zero for single digit addresses 
  //}
  //lcd.print(DCCAddress, DEC);
  //lcd.setCursor (14,1);        // go to end of 2nd line
  //lcd.print("       ");
  //lcd.setCursor(5,1);// start of 2nd line
  // pad with leading zeros  
  //String temp = "0000" + String(fn0to4,BIN);
  //int tlen= temp.length() - 5;
  //lcd.print(temp.substring(tlen));
  //temp = "000" + String(fn5to8,BIN);
  //tlen= temp.length() - 4;
  //lcd.setCursor(0,1);// start of 2nd line
  //lcd.print(temp.substring(tlen));

  //byte button_state = digitalRead(4); //high == not pushed; low == pushed
  //Functions will not work without this to limit speed command to only new speeds
  //if(speed_byte != old_speed)
  //{
  //  speed_byte = constrain(speed_byte, -127, 127);
  //  dps.setSpeed128(DCCAddress,DCC_SHORT_ADDRESS,speed_byte);
  //  old_speed = speed_byte;
  //} 
  dps.update();
}  //END LOOP


 

