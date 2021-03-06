EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
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
LIBS:pocketbeagle
LIBS:sn65hvd233-ht
LIBS:lm2574n-5
LIBS:MCP23xxx
LIBS:PocketBeagleMegaIOBaseBoard-cache
EELAYER 27 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 3 4
Title "Mega I/O Hat"
Date "8 jul 2019"
Rev "1.0"
Comp "Deepwoods Software"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCP23017 U201
U 1 1 5CA8B48F
P 3650 2900
F 0 "U201" H 3500 3050 60  0000 C CNN
F 1 "MCP23017" H 3650 2900 60  0000 C CNN
F 2 "~" H 3650 2900 60  0000 C CNN
F 3 "~" H 3650 2900 60  0000 C CNN
	1    3650 2900
	1    0    0    -1  
$EndComp
$Comp
L CONN_5X2 P201
U 1 1 5CA8B4BC
P 5850 2400
F 0 "P201" H 5850 2700 60  0000 C CNN
F 1 "GPIO 3" V 5850 2400 50  0000 C CNN
F 2 "" H 5850 2400 60  0000 C CNN
F 3 "" H 5850 2400 60  0000 C CNN
	1    5850 2400
	1    0    0    -1  
$EndComp
$Comp
L CONN_5X2 P202
U 1 1 5CA8D4AA
P 5850 3500
F 0 "P202" H 5850 3800 60  0000 C CNN
F 1 "GPIO 4" V 5850 3500 50  0000 C CNN
F 2 "" H 5850 3500 60  0000 C CNN
F 3 "" H 5850 3500 60  0000 C CNN
	1    5850 3500
	1    0    0    -1  
$EndComp
$Comp
L C C201
U 1 1 5CA8D4AB
P 5100 1450
F 0 "C201" H 5100 1550 40  0000 L CNN
F 1 ".1 uf" H 5106 1365 40  0000 L CNN
F 2 "~" H 5138 1300 30  0000 C CNN
F 3 "~" H 5100 1450 60  0000 C CNN
	1    5100 1450
	1    0    0    -1  
$EndComp
Text HLabel 2950 2100 0    60   Input ~ 0
SCL
Text HLabel 2950 2200 0    60   BiDi ~ 0
SDA
Text HLabel 2950 2350 0    60   Input ~ 0
A1
Text HLabel 2950 2450 0    60   Input ~ 0
A0
Text HLabel 2950 3350 0    60   Input ~ 0
Reset
$Comp
L +3.3V #PWR020
U 1 1 5CA8D4AC
P 3650 1100
F 0 "#PWR020" H 3650 1200 40  0001 C CNN
F 1 "+3.3V" H 3650 1225 40  0000 C CNN
F 2 "" H 3650 1100 60  0000 C CNN
F 3 "" H 3650 1100 60  0000 C CNN
	1    3650 1100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR021
U 1 1 5CA8D4AD
P 3550 4600
F 0 "#PWR021" H 3550 4600 30  0001 C CNN
F 1 "GND" H 3550 4530 30  0001 C CNN
F 2 "" H 3550 4600 60  0000 C CNN
F 3 "" H 3550 4600 60  0000 C CNN
	1    3550 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 2550 2350 2550
Wire Wire Line
	2350 2550 2350 4400
Wire Wire Line
	2350 4400 5100 4400
Wire Wire Line
	3550 4150 3550 4600
Connection ~ 3550 4400
Wire Wire Line
	3650 1100 3650 1700
Wire Wire Line
	3650 1250 6550 1250
Connection ~ 3650 1250
Wire Wire Line
	5100 4400 5100 1650
Wire Wire Line
	5450 2200 5450 2050
Wire Wire Line
	5450 2050 4300 2050
Wire Wire Line
	6250 2200 6250 1950
Wire Wire Line
	6250 1950 4450 1950
Wire Wire Line
	4450 1950 4450 2150
Wire Wire Line
	4450 2150 4300 2150
Wire Wire Line
	5450 2300 4400 2300
Wire Wire Line
	4400 2300 4400 2250
Wire Wire Line
	4400 2250 4300 2250
Wire Wire Line
	6250 2300 6450 2300
Wire Wire Line
	6450 2300 6450 1900
Wire Wire Line
	6450 1900 4550 1900
Wire Wire Line
	4550 1900 4550 2350
Wire Wire Line
	4550 2350 4300 2350
Wire Wire Line
	6250 2600 6250 2750
Wire Wire Line
	6250 2750 4300 2750
Wire Wire Line
	5450 2600 5450 2650
Wire Wire Line
	5450 2650 4300 2650
Wire Wire Line
	6250 2500 6400 2500
Wire Wire Line
	6400 2500 6400 2900
Wire Wire Line
	6400 2900 4800 2900
Wire Wire Line
	4800 2900 4800 2550
Wire Wire Line
	4800 2550 4300 2550
Wire Wire Line
	5450 2500 5150 2500
Wire Wire Line
	5150 2500 5150 2450
Wire Wire Line
	5150 2450 4300 2450
Wire Wire Line
	6250 2400 6550 2400
Wire Wire Line
	6550 1250 6550 3500
Connection ~ 5100 1250
Wire Wire Line
	5450 2400 5100 2400
Connection ~ 5100 2400
Wire Wire Line
	5450 3500 5100 3500
Connection ~ 5100 3500
Wire Wire Line
	6550 3500 6250 3500
Connection ~ 6550 2400
Wire Wire Line
	5450 3300 5450 3100
Wire Wire Line
	5450 3100 4300 3100
Wire Wire Line
	6250 3300 6250 3200
Wire Wire Line
	6250 3200 4300 3200
Wire Wire Line
	6250 3700 6250 3800
Wire Wire Line
	6250 3800 4300 3800
Wire Wire Line
	5450 3700 4300 3700
Wire Wire Line
	6250 3600 6350 3600
Wire Wire Line
	6350 3600 6350 3900
Wire Wire Line
	6350 3900 4650 3900
Wire Wire Line
	4650 3900 4650 3600
Wire Wire Line
	4650 3600 4300 3600
Wire Wire Line
	4300 3500 4750 3500
Wire Wire Line
	4750 3500 4750 3600
Wire Wire Line
	4750 3600 5450 3600
Wire Wire Line
	5450 3400 4950 3400
Wire Wire Line
	4950 3400 4950 3300
Wire Wire Line
	4950 3300 4300 3300
Wire Wire Line
	4300 3400 4650 3400
Wire Wire Line
	4650 3400 4650 2950
Wire Wire Line
	4650 2950 6400 2950
Wire Wire Line
	6400 2950 6400 3400
Wire Wire Line
	6400 3400 6250 3400
$EndSCHEMATC
