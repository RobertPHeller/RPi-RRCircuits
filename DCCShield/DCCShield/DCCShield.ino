#include <DCCHardware.h>
#include <DCCPacketQueue.h>
#include <DCCPacketScheduler.h>
#include <DCCPacket.h>
#define DCCLIB
#include <EEPROM.h>
int LED = 13; // LED to blink when DCC packets are sent in loop
int Enable_PIN = 8; //low to enable DCC, high to stop
#define DCC_CurrentSense A0 
#define DCC_Current() (analogRead(DCC_CurrentSense)*(3.0/4095))
#include <Wire.h>
#include "DCCState.h"
DCCPacketScheduler dps;
#include <LiquidCrystal_I2C.h>
#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
LiquidCrystal_I2C	lcd(I2C_ADDR,16,2);//  LCD is 16 characters x 2 lines  


unsigned long previousMillis = 0; // last time update
unsigned long interval = 2000; // interval at which to do refresh (milliseconds)

int ProcessCommandLine (char *line);                                            

void setup() {  
    pinMode(LED, OUTPUT);    
    pinMode(Enable_PIN, OUTPUT);
    pinMode(CurrentSense, INPUT);
    analogReference(EXTERNAL);    
    lcd.begin (); 
    lcd.setBacklight(HIGH);  // Switch on the backlight
    lcd.home (); // go home
    Serial.begin(115200);
    lcd.setCursor(0,0);  
    lcd.print("Version 1.0 ");
    lcd.setCursor(0,1);
    lcd.print("2-1-2018 ");
    delay(1500);
    lcd.clear();
    dps.setup();
    DCCState::setup(&dps);
    //DCCState::setup(NULL);
    DCCState::loadState();
    DCCState::eStop();
    DCCState::updateDCC();
    Serial.println("Ready");
    lcd.home ();
    lcd.setCursor(0,1);
    lcd.print("Ready");
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
        digitalWrite(LED, !digitalRead(LED));
    }

    
    digitalWrite(Enable_PIN, LOW);// HIGH = disable DCC
    if (Serial.available() > 0) {
        char buffer[32];
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Working");
        int len = Serial.readBytesUntil('\r',buffer,sizeof(buffer)-1);
        if (len > 0) {
            //buffer[len++] = '\n';
            buffer[len] = '\0';                                               
            // Parse the command and send a DCC command packet
            // to update the LCD and/or for local state capture.
            int result = ProcessCommandLine (buffer);
            Serial.print("Command Processed: ");
            Serial.print(buffer);
            Serial.print(" - ");
            Serial.println(result);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Cmd Status: ");
            lcd.print(result);
            if (result == 0) DCCState::saveState();
        }
        Serial.println("Ready");
        lcd.setCursor(0,1);
        lcd.print("Ready");
        double currentSensed = DCC_Current();
        lcd.print(" ");
        lcd.print(currentSensed);
        lcd.print(" Amps");
    }
    dps.update();
}  //END LOOP

void printHelp(void);

int ProcessCommandLine (char *line) {
    char *p;
    while (*line != '\0' && isspace(*line)) line++;
    //Serial.print("*** line is: '");
    //Serial.print(line);
    //Serial.println("'");
    switch (toupper(*line)) {
    case 'E':
        //Serial.println("*** Saw E");
        if (strncasecmp(line,"EMERGENCY",9) == 0) {
            line += 9;
            //Serial.println("*** Saw EMERGENCY");
            while (*line != '\0' && isspace(*line)) line++;
            if (strncasecmp(line,"STOP",4) == 0) {
                line += 4;
                //Serial.println("*** Saw STOP");
                while (*line != '\0' && isspace(*line)) line++;
                if (strncasecmp(line,"ALL",3) == 0) {
                    line += 3;
                    //Serial.println("*** Saw ALL");
                    while (*line != '\0' && isspace(*line)) line++;
                    if (*line != '\0') return 1;
                    //Serial.println("*** Saw eol");
                    if (!DCCState::eStop()) return -1;
                    return 0;
                } else if (isdigit(*line)) {
                    uint16_t address = strtoul(line,&p,10);
                    if (p == line)  return 1;
                    line = p;
                    //Serial.print("*** Saw ");
                    //Serial.println(address);
                    while (*line != '\0' && isspace(*line)) line++;
                    if (*line != '\0')  return 1;
                    //Serial.println("*** Saw eol");
                    if (!DCCState::eStop(address)) return -1;   
                    return 0;
                } else {
                    //Serial.println("Parse error");
                    return 1;
                }
            } else {
                //Serial.println("Parse error");
                return 1;
            }          
        } else {
            //Serial.println("Parse error");
            return 1;
        }
        break;
    case 'S':
        if (strncasecmp(line,"SET",3) == 0) {
            line += 3;
            while (*line != '\0' && isspace(*line)) line++;
            if (strncasecmp(line,"SPEED",5) == 0) {
                line += 5;
                while (*line != '\0' && isspace(*line)) line++;
                uint16_t address = strtoul(line,&p,10);
                if (p == line)  return 1;
                line = p;
                int8_t speed = strtol(line,&p,10);
                if (p == line)  return 1;
                line = p;
                uint8_t steps = strtoul(line,&p,10);
                if (p == line)  return 1;
                line = p;
                while (*line != '\0' && isspace(*line)) line++;
                if (*line != '\0')  return 1;
                if (!DCCState::setSpeed(address,speed,steps)) return -1;
                return 0;
            } else if (strncasecmp(line,"FUNCTIONS1",10) == 0) {
                line += 10;
                while (*line != '\0' && isspace(*line)) line++;
                uint16_t address = strtoul(line,&p,10);
                if (p == line)  return 1;
                line = p;
                uint16_t functions = strtoul(line,&p,16);
                if (p == line)  return 1;
                line = p;
                while (*line != '\0' && isspace(*line)) line++;
                if (*line != '\0')  return 1;
                if (!DCCState::setFunctions0to4(address,functions)) return -1;
                return 0;
            } else if (strncasecmp(line,"FUNCTIONS2",10) == 0) {
                line += 10;
                while (*line != '\0' && isspace(*line)) line++;
                uint16_t address = strtoul(line,&p,10);
                if (p == line)  return 1;
                line = p;
                uint16_t functions = strtoul(line,&p,16);
                if (p == line)  return 1;
                line = p;
                while (*line != '\0' && isspace(*line)) line++;
                if (*line != '\0')  return 1;
                if (!DCCState::setFunctions5to8(address,functions)) return -1;
                return 0;
            } else if (strncasecmp(line,"FUNCTIONS3",10) == 0) {
                line += 10;
                while (*line != '\0' && isspace(*line)) line++;
                uint16_t address = strtoul(line,&p,10);
                if (p == line)  return 1;
                line = p;
                uint16_t functions = strtoul(line,&p,16);
                if (p == line)  return 1;
                line = p;
                while (*line != '\0' && isspace(*line)) line++;
                if (*line != '\0')  return 1;
                if (!DCCState::setFunctions9to12(address,functions)) return -1;
                return 0;
            } else if (strncasecmp(line,"ACCESSORY",9) == 0) {
                line += 9;
                while (*line != '\0' && isspace(*line)) line++;
                uint16_t address = strtoul(line,&p,10);
                if (p == line)  return 1;
                line = p;
                uint8_t function = strtoul(line,&p,16);
                if (p == line)  return 1;
                line = p;
                while (*line != '\0' && isspace(*line)) line++;
                if (*line != '\0')  return 1;
                if (!DCCState::setBasicAccessory(address,function)) return -1;
                return 0;
            } else {
                //Serial.println("Parse error");
                return 1;
            }
        } else if (strncasecmp(line,"SAVE",4) == 0) {
            line += 4;
            while (*line != '\0' && isspace(*line)) line++;
            if (strncasecmp(line,"STATE",5) == 0) {
                line += 5;
                while (*line != '\0' && isspace(*line)) line++;
                if (*line != '\0')  return 1;
                if (!DCCState::saveState()) return -1;
                return 0;
            } else {
                //Serial.println("Parse error");
                return 1;
            }
        } else {
            return 1;
        }
        break;
    case 'U':
        if (strncasecmp(line,"UNSET",5) == 0) {
            line += 5;
            while (*line != '\0' && isspace(*line)) line++;
            if (strncasecmp(line,"ACCESSORY",9) == 0) {
                line += 9;
                while (*line != '\0' && isspace(*line)) line++;
                uint16_t address = strtoul(line,&p,10);
                if (p == line)  return 1;
                line = p;
                uint8_t function = strtoul(line,&p,16);
                if (p == line)  return 1;
                line = p;
                while (*line != '\0' && isspace(*line)) line++;
                if (*line != '\0')  return 1;
                if (!DCCState::unsetBasicAccessory(address,function)) return -1;
                return 0;
            } else {
                return 1;
            }
        } else {
            return 1;
        }
        break;
    case 'L':
        if (strncasecmp(line,"LOAD",4) == 0) {
            line += 4;
            while (*line != '\0' && isspace(*line)) line++;
            if (strncasecmp(line,"STATE",5) == 0) {
                line += 5;
                while (*line != '\0' && isspace(*line)) line++;
                if (*line != '\0')  return 1;
                if (!DCCState::loadState()) return -1;
                return 0;
            } else {
                //Serial.println("Parse error");
                return 1;
            }
        } else {
            return 1;
        }
        break;
    case 'D':
        if (strncasecmp(line,"DUMP",4) == 0) {
            line += 4;
            while (*line != '\0' && isspace(*line)) line++;
            if (strncasecmp(line,"STATE",5) == 0) {
                line += 5;
                while (*line != '\0' && isspace(*line)) line++;
                if (*line != '\0')  return 1;
                if (!DCCState::dumpState()) return -1;
                return 0;
            } else {
                //Serial.println("Parse error");
                return 1;
            }
        } else {
            return 1;
        }
        break;
    case 'I':
        if (strncasecmp(line,"INIT",4) == 0) {
            line += 4;
            while (*line != '\0' && isspace(*line)) line++;
            if (strncasecmp(line,"STATE",5) == 0) {
                line += 5;
                while (*line != '\0' && isspace(*line)) line++;
                if (*line != '\0')  return 1;
                if (!DCCState::initState()) return -1;
                return 0;
            } else {
                //Serial.println("Parse error");
                return 1;
            }
        } else {
            return 1;
        }
        break;
    case 'H':
        if (strncasecmp(line,"HELP",4) == 0) {
            line += 4;
            while (*line != '\0' && isspace(*line)) line++;
            if (*line != '\0') return 1;
            printHelp();
            return 0;
        } else {
            return 1;
        }
        break;
    case 'G':
        if (strncasecmp(line,"GET",3) == 0) {
            line += 3;
            while (*line != '\0' && isspace(*line)) line++;
            if (strncasecmp(line,"INFORMATION",11) == 0) {
                line += 11;
                while (*line != '\0' && isspace(*line)) line++;
                uint16_t address = strtoul(line,&p,10);
                if (p == line)  return 1;
                line = p;
                while (*line != '\0' && isspace(*line)) line++;
                if (*line != '\0')  return 1;
                if (!DCCState::getInformation(address)) return -1;
                return 0;
            } else {
                return 1;
            }
        } else {
            return 1;
        }
        break;
    } 
    //Serial.println("Parse error");
    return 1;
}

const char helpmessage[] = 
"Commands:\nEMERGENCY STOP ALL\nEMERGENCY STOP address\nSET SPEED address speed steps\nSET FUNCTIONS address functions\nSET ACCESSORY address function\nUNSET ACCESSORY address function\nSAVE STATE\nLOAD STATE\nINIT STATE\nDUMP STATE\nHELP\n";

void printHelp(void)
{
    Serial.print(helpmessage);
}
