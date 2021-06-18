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
LIBS:esp32_devboards
LIBS:sn65hvd233-ht
LIBS:lm2574n-5
LIBS:MCP23xxx
LIBS:tbd62x83a
LIBS:tc442x
LIBS:mct6h
LIBS:bridge
LIBS:pca9685
LIBS:4814p-2
LIBS:mechanical
LIBS:graphic_symbols
LIBS:references
LIBS:ESP32-MultiFunction-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 7
Title "ESP32 Multifunction board"
Date ""
Rev ""
Comp "Deepwoods Software"
Comment1 "Main Page: MCU and section interconnect"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ESP32_DEVKIT_C U0
U 1 1 60CCB7EE
P 3350 2450
F 0 "U0" H 3350 2450 60  0000 C CNN
F 1 "ESP32_DEVKIT_C" H 3350 3000 60  0000 C CNN
F 2 "ESP32-Boards:ESP32-Combo" H 3350 2450 60  0001 C CNN
F 3 "" H 3350 2450 60  0000 C CNN
	1    3350 2450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 60CCB9BA
P 3900 2300
F 0 "#PWR?" H 3900 2050 50  0001 C CNN
F 1 "GND" H 3900 2150 50  0000 C CNN
F 2 "" H 3900 2300 50  0001 C CNN
F 3 "" H 3900 2300 50  0001 C CNN
	1    3900 2300
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 60CCB9E8
P 3900 2000
F 0 "#PWR?" H 3900 1750 50  0001 C CNN
F 1 "GND" H 3900 1850 50  0000 C CNN
F 2 "" H 3900 2000 50  0001 C CNN
F 3 "" H 3900 2000 50  0001 C CNN
	1    3900 2000
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 60CCBA16
P 2800 2650
F 0 "#PWR?" H 2800 2400 50  0001 C CNN
F 1 "GND" H 2800 2500 50  0000 C CNN
F 2 "" H 2800 2650 50  0001 C CNN
F 3 "" H 2800 2650 50  0001 C CNN
	1    2800 2650
	0    1    1    0   
$EndComp
$Comp
L +3V3 #PWR?
U 1 1 60CCBA34
P 2800 2000
F 0 "#PWR?" H 2800 1850 50  0001 C CNN
F 1 "+3V3" H 2800 2140 50  0000 C CNN
F 2 "" H 2800 2000 50  0001 C CNN
F 3 "" H 2800 2000 50  0001 C CNN
	1    2800 2000
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 60CCBAA7
P 2550 2900
F 0 "#PWR?" H 2550 2750 50  0001 C CNN
F 1 "+5V" H 2550 3040 50  0000 C CNN
F 2 "" H 2550 2900 50  0001 C CNN
F 3 "" H 2550 2900 50  0001 C CNN
	1    2550 2900
	1    0    0    -1  
$EndComp
Text Label 3800 2450 0    30   ~ 0
CAN_TX
Text Label 3800 2600 0    30   ~ 0
CAN_RX
Text Label 3800 2100 0    30   ~ 0
SCL
Text Label 3800 2250 0    30   ~ 0
SDA
NoConn ~ 3800 2900
Text Label 2900 2100 2    30   ~ 0
GPIO36_IN
Text Label 2900 2150 2    30   ~ 0
GPIO39_IN
Text Label 2900 2200 2    30   ~ 0
GPIO34_IN
Text Label 2900 2250 2    30   ~ 0
GPIO35_IN
Text Label 2900 2300 2    30   ~ 0
GPIO32
Text Label 2900 2350 2    30   ~ 0
GPIO33
Text Label 2900 2400 2    30   ~ 0
GPIO25
Text Label 2900 2450 2    30   ~ 0
GPIO26
Text Label 2900 2500 2    30   ~ 0
GPIO27
Text Label 2900 2550 2    30   ~ 0
GPIO14
Text Label 2900 2600 2    30   ~ 0
GPIO12
Text Label 2900 2700 2    30   ~ 0
GPIO13
Text Label 3800 2650 0    30   ~ 0
GPIO0
Text Label 3800 2700 0    30   ~ 0
GPIO2
Text Label 3800 2750 0    30   ~ 0
GPIO15
Text Label 3800 2550 0    30   ~ 0
GPIO16
Text Label 3800 2500 0    30   ~ 0
GPIO17
Text Label 3800 2400 0    30   ~ 0
GPIO18
Text Label 3800 2350 0    30   ~ 0
GPIO19
Text Label 3800 2050 0    30   ~ 0
GPIO23
NoConn ~ 3800 2150
NoConn ~ 3800 2200
NoConn ~ 3800 2800
NoConn ~ 3800 2850
NoConn ~ 2900 2050
NoConn ~ 2900 2750
NoConn ~ 2900 2800
NoConn ~ 2900 2850
Text Notes 3000 1600 0    59   ~ 0
OD:      34,35,36,39\nMotor:   25,26,32,33\nPoints:  12,13,14,27\nButtons:  0, 2,15,16\nLEDs:    17,18,19,23\nSignals: 21,22 (I2C)
$Sheet
S 4650 2250 1250 900 
U 60CCDA7F
F0 "CAN Transceiver" 60
F1 "CAN_Transeiver.sch" 60
F2 "CAN_TX" I L 4650 2450 60 
F3 "CAN_RX" I L 4650 2600 60 
$EndSheet
Wire Wire Line
	2900 2900 2550 2900
Wire Wire Line
	2900 2650 2800 2650
Wire Wire Line
	3900 2000 3800 2000
Wire Wire Line
	3900 2300 3800 2300
Wire Wire Line
	2800 2000 2900 2000
Wire Wire Line
	3800 2600 4650 2600
Wire Wire Line
	3800 2450 4650 2450
$Sheet
S 6600 2300 1600 900 
U 60CCFE36
F0 "Power Supply" 60
F1 "PowerSupply.sch" 60
$EndSheet
Text Label 4550 4000 2    60   ~ 0
SCL
Text Label 4550 4200 2    60   ~ 0
SDA
$Sheet
S 4550 3800 1850 1400
U 60CD053F
F0 "PWM Driver" 60
F1 "PCA9685Driver.sch" 60
F2 "SCL" I L 4550 4000 60 
F3 "SDA" B L 4550 4200 60 
$EndSheet
$Sheet
S 6800 3800 2250 1350
U 60CD421F
F0 "Occupancy Detectors" 60
F1 "OccDetect4.sch" 60
$EndSheet
$Sheet
S 1150 3800 2350 1300
U 60CD422F
F0 "Stall Motors" 60
F1 "StallMotors4.sch" 60
$EndSheet
$Sheet
S 1100 5750 2500 1400
U 60CD423F
F0 "Buttons" 60
F1 "Buttons4.sch" 60
$EndSheet
$Sheet
S 4250 5750 2000 1400
U 60CD424F
F0 "LEDS" 60
F1 "LEDS4.sch" 60
$EndSheet
$Comp
L Mounting_Hole MK?
U 1 1 60CD4484
P 10450 1000
F 0 "MK?" H 10450 1200 50  0000 C CNN
F 1 "Mounting_Hole" H 10450 1125 50  0000 C CNN
F 2 "" H 10450 1000 50  0001 C CNN
F 3 "" H 10450 1000 50  0001 C CNN
	1    10450 1000
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole MK?
U 1 1 60CD44E3
P 10450 1550
F 0 "MK?" H 10450 1750 50  0000 C CNN
F 1 "Mounting_Hole" H 10450 1675 50  0000 C CNN
F 2 "" H 10450 1550 50  0001 C CNN
F 3 "" H 10450 1550 50  0001 C CNN
	1    10450 1550
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole MK?
U 1 1 60CD4516
P 10500 2150
F 0 "MK?" H 10500 2350 50  0000 C CNN
F 1 "Mounting_Hole" H 10500 2275 50  0000 C CNN
F 2 "" H 10500 2150 50  0001 C CNN
F 3 "" H 10500 2150 50  0001 C CNN
	1    10500 2150
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole_PAD MK?
U 1 1 60CD454B
P 10450 2750
F 0 "MK?" H 10450 3000 50  0000 C CNN
F 1 "Mounting_Hole_PAD" H 10450 2925 50  0000 C CNN
F 2 "" H 10450 2750 50  0001 C CNN
F 3 "" H 10450 2750 50  0001 C CNN
	1    10450 2750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 60CD4616
P 10450 3100
F 0 "#PWR?" H 10450 2850 50  0001 C CNN
F 1 "GND" H 10450 2950 50  0000 C CNN
F 2 "" H 10450 3100 50  0001 C CNN
F 3 "" H 10450 3100 50  0001 C CNN
	1    10450 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	10450 2850 10450 3100
$Comp
L Mounting_Hole FID?
U 1 1 60CD48B9
P 9450 1700
F 0 "FID?" H 9450 1900 50  0000 C CNN
F 1 "FidMark" H 9450 1825 50  0000 C CNN
F 2 "" H 9450 1700 50  0001 C CNN
F 3 "" H 9450 1700 50  0001 C CNN
	1    9450 1700
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole FID?
U 1 1 60CD49DA
P 9450 2050
F 0 "FID?" H 9450 2250 50  0000 C CNN
F 1 "FidMark" H 9450 2175 50  0000 C CNN
F 2 "" H 9450 2050 50  0001 C CNN
F 3 "" H 9450 2050 50  0001 C CNN
	1    9450 2050
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole FID?
U 1 1 60CD4A15
P 9450 2350
F 0 "FID?" H 9450 2550 50  0000 C CNN
F 1 "FidMark" H 9450 2475 50  0000 C CNN
F 2 "" H 9450 2350 50  0001 C CNN
F 3 "" H 9450 2350 50  0001 C CNN
	1    9450 2350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
