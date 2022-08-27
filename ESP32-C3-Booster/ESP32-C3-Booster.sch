EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:ESP32-C3FN4
LIBS:USBLC6-2SC6
LIBS:sn65hvd233-ht
LIBS:lm2574n-5
LIBS:DRV8873HPWPR
LIBS:mcp9701at
LIBS:MCP9700T-E_LT
LIBS:ESP32-C3-Booster-cache
EELAYER 25 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 6
Title "ESP32-C3 Booster"
Date ""
Rev "1.0"
Comp "Deepwoods Software"
Comment1 "Main Sheet"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 5100 1300 2150 2000
U 630A2B31
F0 "ESP32-C3 MCU" 60
F1 "ESP32-C3-MCU.sch" 60
$EndSheet
$Sheet
S 1800 1400 1700 1700
U 630A2B38
F0 "CAN Transeiver" 60
F1 "CAN_Transceiver.sch" 60
F2 "CAN_TX" I R 3500 1700 60 
F3 "CAN_RX" O R 3500 1900 60 
F4 "ALT_L" B R 3500 2200 60 
F5 "ALT_H" B R 3500 2400 60 
$EndSheet
$Sheet
S 1700 4000 1800 1600
U 630A2B3D
F0 "Power Supply" 60
F1 "PowerSupply.sch" 60
F2 "3.3V Out" O R 3500 4150 60 
F3 "Power Input" I R 3500 4350 60 
F4 "12V" O R 3500 4600 60 
$EndSheet
$Sheet
S 5000 3900 2150 1750
U 630A2B40
F0 "OPS Driver" 60
F1 "OpsDriver.sch" 60
F2 "IN_B" I L 5000 4100 60 
F3 "IN_A" I L 5000 4000 60 
F4 "BRAKE" I L 5000 4350 60 
F5 "PowerIn" I L 5000 4500 60 
F6 "A" O L 5000 4650 60 
F7 "B" O L 5000 4800 60 
F8 "Sense_Pos" O L 5000 5050 60 
F9 "Sense_Neg" O L 5000 5200 60 
$EndSheet
$Sheet
S 7950 1550 2250 2000
U 630A2B43
F0 "Fan Controls" 60
F1 "FanControls.sch" 60
F2 "FAN_POWER" I L 7950 1800 60 
F3 "FAN_CONTROL" I L 7950 2000 60 
F4 "TEMP_SENSE" O L 7950 2200 60 
$EndSheet
$EndSCHEMATC
