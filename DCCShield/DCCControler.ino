/*
 d. bodnar  9-23-2014
 Uses 2 line LCD for display
 Uses IR remote control for throttle & functions (functions not working well yet)
 Uses CmdArduino library for DCC output
 
 UP = faster - 13 - hold to repeat
 DN = slower - 17 -hold to repeat
 * = STOP - 11
 # = Disable / enable DCC (pin 8) -12
 < = - 14
 > = -16
 OK = Menu choices -15
 */
// the following defines the codes for Keyes brand and Sony IR remote controls
#define KeyesUp 0xFF629D
#define SonyUp 0x90
#define KeyesLeft 0xFF22DD
#define SonyLeft 0xC90
#define KeyesOK 0xFF02FD
#define SonyMenu 0x70
#define KeyesRight 0xFFC23D
#define SonyRight 0x490
#define KeyesDown 0xFFA857
#define SonyDown 0x890
#define Keyes1 0xFF6897
#define Sony1 0x10
#define Keyes2 0xFF9867
#define Sony2 0x810
#define Keyes3 0xFFB04F
#define Sony3 0x410
#define Keyes4 0xFF30CF
#define Sony4 0xC10
#define Keyes5 0xFF18E7
#define Sony5 0x210
#define Keyes6 0xFF7A85
#define Sony6 0xA10
#define Keyes7 0xFF10EF
#define Sony7 0x610
#define Keyes8 0xFF38C7
#define Sony8 0xE10
#define Keyes9 0xFF5AA5
#define Sony9 0x110
#define Keyes0 0xFF4AB5
#define Sony0 0x910
#define KeyesStar 0xFF42BD
#define SonyExit 0xC70
#define KeyesPound 0xFF52AD
#define SonyPower 0xA90 
#include<EEPROM.h>
int irButton;
int LED = 13; // LED to blink when DCC packets are sent in loop
#include <IRremote.h>
int RECV_PIN = 11;
int Enable_PIN = 8; //low to enable DCC, high to stop
IRrecv irrecv(RECV_PIN);
decode_results results;
#include <DCCPacket.h>
#include <DCCPacketQueue.h>
#include <DCCPacketScheduler.h>
DCCPacketScheduler dps;
//unsigned int analog_value=0;
char speed_byte, old_speed = 0;
char temp;
byte Fx = 0;
byte DCCAddress = 3;
int irCode = 0;
int inMenu = 0;  // keeps track of being in the menu area 0=out, 1=in
int digits = 0;
int upFlag = 0;  // trying to get keys to repeat!
int dnFlag = 0;
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
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
LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

unsigned long previousMillis = 0; // last time update
long interval = 2000; // interval at which to do refresh (milliseconds)

void setup() {
  pinMode(LED, OUTPUT);
  DCCAddress = EEPROM.read(0);  
  if(DCCAddress >=100){  // set defalut as 3 if not in proper range (0-99)
    DCCAddress = 3;
  }
  pinMode(Enable_PIN, OUTPUT); 
  lcd.begin (16,2); //  LCD is 16 characters x 2 lines  
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);  // Switch on the backlight
  lcd.home (); // go home
  Serial.begin(115200);
  lcd.setCursor(0,0);  
  lcd.print("Version 2.7 ");
  lcd.setCursor(0,1);
  lcd.print("9-23-2014 ");
  delay(1500);
  lcd.clear();
  irrecv.enableIRIn(); // Start the receiver
  dps.setup();
  dps.setFunctions0to4(DCCAddress,DCC_SHORT_ADDRESS, B00000000); //clear functions
  dps.setFunctions5to8(DCCAddress,DCC_SHORT_ADDRESS, B00000000);    
}

void loop() {
  //this section sends DCC updates every 2 seconds (interval)
  // not sure if it is necessary but the functions are slow to respond
  // at times - may be due to the DCC library setting priorities as the
  // speed controls always work 
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;  
    dps.setSpeed128(DCCAddress,DCC_SHORT_ADDRESS,speed_byte);
    dps.setFunctions0to4(DCCAddress,DCC_SHORT_ADDRESS, fn0to4); //clear all functions
    dps.setFunctions5to8(DCCAddress,DCC_SHORT_ADDRESS, fn5to8); 
    digitalWrite(LED, !digitalRead(LED));
  }

  digitalWrite(Enable_PIN, LOW);// HIGH = disable DCC
  lcd.setCursor(0,0);  
  lcd.print("Speed=");
  lcd.print(speed_byte, DEC);
  lcd.print("  ");  
  lcd.setCursor(11,0);
  lcd.print("ad=");
  if(DCCAddress <=9){
    lcd.print("0");  // add leading zero for single digit addresses 
  }
  lcd.print(DCCAddress, DEC);
  lcd.setCursor (14,1);        // go to end of 2nd line
  //  lcd.print("IR code ");
  lcd.print(irButton, DEC);
  lcd.print("       ");
  lcd.setCursor(5,1);// start of 2nd line
  // pad with leading zeros  
  String temp = "0000" + String(fn0to4,BIN);
  int tlen= temp.length() - 5;
  lcd.print(temp.substring(tlen));
  temp = "000" + String(fn5to8,BIN);
  tlen= temp.length() - 4;
  lcd.setCursor(0,1);// start of 2nd line
  lcd.print(temp.substring(tlen));

  if (irrecv.decode(&results)) 
  {
    translateIR();
    Serial.println(irButton,DEC);
    irrecv.resume(); // Receive the next value
  }
  byte button_state = digitalRead(4); //high == not pushed; low == pushed
  if ((irButton >0 && irButton <13) | (irButton ==14 | irButton == 16)){
    upFlag=0;
    dnFlag=0; ////MAY NEED TO PUT THIS ON EACH KEY 
  }

  if(irButton >=1 && irButton <=10){  //Funtions done with numbers 1-9 - clear all with 10
    doFunction();
    irButton=0;
  }

  // MENU SECTION - stays here to get DCC address 
  if (irButton==15 )  //OK key for menu choices
  {
    Serial.println("found OK (menu)");
    inMenu=1;
    lcd.clear(); // blank screen
    lcd.setCursor(0,0);  
    lcd.print("MENU");
    lcd.setCursor(0,1);
    lcd.print("Ent 2 digit add");

    for (int i =0; i = 1; i++){  // do twice
      if (irrecv.decode(&results)) 
      {
        translateIR();
        irrecv.resume(); // Receive the next value
      }     
      if (irButton == 15 && inMenu == 0){
        Serial.print("BREAK OUT ");
        Serial.println(inMenu, DEC);
        inMenu=0;
        irButton=0;
        digits=0;
        lcd.clear();
        break;
      }
      if (irButton >=1 && irButton <=10)
      {       
        if (digits==1){
          DCCAddress = DCCAddress * 10;
          Serial.print("x10 address ");
          Serial.println(DCCAddress, DEC);
          if(irButton != 10){   // only add it not zero (zero button or remote returns a 10)
            DCCAddress = DCCAddress + irButton;  
          }
          lcd.setCursor(0,1);
          if(DCCAddress <=9){
            lcd.print("0");  // add leading zero for single digit addresses 
          }
          lcd.print(DCCAddress, DEC);
          lcd.setCursor(0,0);
          lcd.print("OK to Exit Menu:");
          digits = 3;
          inMenu=0;
          EEPROM.write(0,DCCAddress);
        }       
        if (digits ==0){
          DCCAddress = irButton;
          if (DCCAddress==10){
            DCCAddress = 0;   // "0" button returns 10 so make it zero
          }
          Serial.print("dig 1 address ");
          Serial.println(DCCAddress, DEC);
          lcd.clear(); // blank screen
          lcd.setCursor(0,0);  
          lcd.print("New Address");
          lcd.setCursor(0,1);
          lcd.print(DCCAddress, DEC);
          digits = 1;  
          delay(500);
          irButton=0;        
        } 
        Serial.print("new address ");
        Serial.println(DCCAddress, DEC);
      }
      irButton=0;   
    }
    irButton=0;
  }
  //END MENU SECTION  

  if (irButton==13 | (irButton==99 && upFlag==1)) // repeat key (99)
  {
    Serial.println("found UP");
    speed_byte++;  
    upFlag=1;
    dnFlag=0;
    irButton=0;
  }
  if (irButton==16)
  {
    Serial.println("found UP ");
    speed_byte++;  
    irButton=0;
  }
  if (irButton ==17 | (irButton==99 && dnFlag==1))
  {
    Serial.println("found DN");
    speed_byte--; 
    dnFlag=1;
    upFlag = 0;
    irButton=0;
  }
  if (irButton ==14)
  {
    Serial.println("found DN ");
    speed_byte--; 
    irButton=0;
  }
  if (irButton==11)  //* key  - does STOP
  {
    Serial.println("found ***");
    speed_byte=0; 
    irButton=0; 
  }
  //Functions will not work without this to limit speed command to only new speeds
  if(speed_byte != old_speed)
  {
    speed_byte = constrain(speed_byte, -127, 127);
    dps.setSpeed128(DCCAddress,DCC_SHORT_ADDRESS,speed_byte);
    old_speed = speed_byte;
  } 
  dps.update();
}  //END LOOP

int translateIR() // takes action based on IR code received
// describing KEYES Remote IR codes (first) and Sony IR codes (second)
{
  Serial.println(results.value, HEX);
  switch(results.value)
  {
  case KeyesUp:   //Keyes remote code for UP
  case SonyUp: //Sony remote code for UP
    Serial.println(" UP"); 
    irButton = 13; 
    break;
  case KeyesLeft:
  case SonyLeft:
    Serial.println(" LEFT"); 
    irButton = 14;   
    break;
  case KeyesOK:
  case SonyMenu: 
    Serial.println(" -MENU-");
    irButton = 15;    
    break;
  case KeyesRight:
  case SonyRight: 
    Serial.println(" RIGHT");
    irButton = 16;   
    break;
  case KeyesDown:
  case SonyDown:
    Serial.println(" DOWN");
    irButton = 17; 
    break;
  case Keyes1:
  case Sony1: 
    Serial.println(" 1"); 
    irButton = 1;   
    break;
  case Keyes2:
  case Sony2: 
    Serial.println(" 2"); 
    irButton = 2;   
    break;
  case Keyes3:
  case Sony3: 
    Serial.println(" 3"); 
    irButton = 3;   
    break;
  case Keyes4:
  case Sony4: 
    Serial.println(" 4"); 
    irButton = 4;   
    break;
  case Keyes5:
  case Sony5: 
    Serial.println(" 5"); 
    irButton = 5;   
    break;
  case Keyes6:
  case Sony6: 
    Serial.println(" 6"); 
    irButton = 6;   
    break;
  case Keyes7:
  case Sony7: 
    Serial.println(" 7"); 
    irButton = 7;   
    break;
  case Keyes8:
  case Sony8: 
    Serial.println(" 8"); 
    irButton = 8;   
    break;
  case Keyes9:
  case Sony9: 
    Serial.println(" 9"); 
    irButton = 9;   
    break;
  case Keyes0:
  case Sony0: 
    Serial.println(" 0"); 
    irButton = 10;   
    break;
  case KeyesStar:
    Serial.println(" *"); 
    irButton = 11;   
    break;
  case SonyExit:  //EXIT
    Serial.println(" *"); 
    irButton = 11;   
    break;
  case KeyesPound: 
    Serial.println(" #"); 
    irButton = 12;   
    break;
  case SonyPower:  //POWER
    Serial.println(" #"); 
    irButton = 12;   
    break;
  case 0xFFFFFFFF: 
    Serial.println(" REPEAT");
    irButton = 99;
    break;  
  default: 
    Serial.println(" other button   ");
    irButton = 99;
  }// End Case
  delay(100); // Do not get immediate repeat
} //END translateIR

//START DO FUNCTION BUTTONS
int doFunction(){
  int irTemp= irButton-1;
  lcd.setCursor (14,1);        // go to end of 2nd line
  ///  lcd.print("FN code ");
  lcd.print(irButton, DEC);
  Serial.print("got a keypad button ");
  Serial.println(irButton,DEC);
  if (irTemp<=4){
    if(bitRead(fn0to4,irTemp)== 0 ){
      bitWrite(fn0to4,irTemp,1); 
    } 
    else {
      if(bitRead(fn0to4,irTemp)== 1 ){
        bitWrite(fn0to4,irTemp,0);       
      }
    } 
    dps.setFunctions0to4(DCCAddress,DCC_SHORT_ADDRESS,fn0to4);
    Serial.print(fn0to4,BIN);
    Serial.println(" fn0to4");
  }

  if (irTemp>=5 && irTemp<=8){
    irTemp=irTemp-5;
    if(bitRead(fn5to8,irTemp)== 0 ){
      bitWrite(fn5to8,irTemp,1); 
    } 
    else {
      if(bitRead(fn5to8,irTemp)== 1 ){
        bitWrite(fn5to8,irTemp,0);       
      }
    } 
    dps.setFunctions5to8(DCCAddress,DCC_SHORT_ADDRESS,fn5to8);
    Serial.print(fn5to8,BIN);
    Serial.println(" fn5to8");
  }

  if (irButton == 10)
  {
    lcd.setCursor (14,1);        // go to end of 2nd line
    ///    lcd.print("FN code ");
    lcd.print(irButton, DEC);
    Serial.println("got a keypad button 0 (reads as 10)");
    dps.setFunctions0to4(DCCAddress,DCC_SHORT_ADDRESS, B00000000); 
    dps.setFunctions5to8(DCCAddress,DCC_SHORT_ADDRESS, B00000000);    
    irButton = 0;
    fn0to4 = B00000000; //clear variables for which functions are set
    fn5to8 = B00000000;
    delay(500);
    irButton=0;
  }
  irButton = 0;
  delay(500);
}
//END DO FUNCTION BUTTONS

 

