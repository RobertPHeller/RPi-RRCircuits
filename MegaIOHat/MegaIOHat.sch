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
LIBS:MCP23xxx
LIBS:MegaIOHat-cache
EELAYER 27 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 3
Title "Mega I/O Hat"
Date "6 apr 2019"
Rev "1.0"
Comp "Deepwoods Software"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RPi_GPIO J0
U 1 1 5516AE26
P 1700 1400
F 0 "J0" H 2450 1650 60  0000 C CNN
F 1 "RPi_GPIO" H 2450 1550 60  0000 C CNN
F 2 "RPi_Hat:Pin_Header_Straight_2x20" H 1700 1400 60  0001 C CNN
F 3 "" H 1700 1400 60  0000 C CNN
	1    1700 1400
	1    0    0    -1  
$EndComp
$Comp
L 3V3 #PWR01
U 1 1 5CA8B30E
P 1300 1250
F 0 "#PWR01" H 1300 1350 40  0001 C CNN
F 1 "3V3" H 1300 1375 40  0000 C CNN
F 2 "" H 1300 1250 60  0000 C CNN
F 3 "" H 1300 1250 60  0000 C CNN
	1    1300 1250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 5CA8B327
P 2500 3900
F 0 "#PWR02" H 2500 3900 30  0001 C CNN
F 1 "GND" H 2500 3830 30  0001 C CNN
F 2 "" H 2500 3900 60  0000 C CNN
F 3 "" H 2500 3900 60  0000 C CNN
	1    2500 3900
	1    0    0    -1  
$EndComp
Text Label 1500 1500 2    60   ~ 0
SDA
Text Label 1500 1600 2    60   ~ 0
SCL
$Sheet
S 6200 1400 1500 1250
U 5CA8B421
F0 "MCP23017 1" 50
F1 "MCP23017-1.sch" 50
F2 "SCL" I L 6200 1550 60 
F3 "SDA" B L 6200 1700 60 
F4 "A1" I L 6200 1900 60 
F5 "A0" I L 6200 2050 60 
F6 "Reset" I L 6200 2250 60 
$EndSheet
$Sheet
S 6200 3100 1650 1300
U 5CA8B423
F0 "MCP23017 2" 50
F1 "MCP23017-2.sch" 50
F2 "SCL" I L 6200 3400 60 
F3 "SDA" B L 6200 3550 60 
F4 "A1" I L 6200 3750 60 
F5 "A0" I L 6200 3900 60 
F6 "Reset" I L 6200 4100 60 
$EndSheet
$Comp
L CONN_2X2 P101
U 1 1 5CA8BB3B
P 4500 2000
F 0 "P101" H 4500 2150 50  0000 C CNN
F 1 "0 1" H 4510 1870 40  0000 C CNN
F 2 "" H 4500 2000 60  0000 C CNN
F 3 "" H 4500 2000 60  0000 C CNN
	1    4500 2000
	1    0    0    -1  
$EndComp
$Comp
L R R101
U 1 1 5CA8C1FC
P 5050 1600
F 0 "R101" V 5130 1600 40  0000 C CNN
F 1 "10K Ohms" V 5057 1601 40  0000 C CNN
F 2 "~" V 4980 1600 30  0000 C CNN
F 3 "~" H 5050 1600 30  0000 C CNN
	1    5050 1600
	1    0    0    -1  
$EndComp
$Comp
L R R102
U 1 1 5CA8C251
P 5250 1600
F 0 "R102" V 5330 1600 40  0000 C CNN
F 1 "10K Ohms" V 5257 1601 40  0000 C CNN
F 2 "~" V 5180 1600 30  0000 C CNN
F 3 "~" H 5250 1600 30  0000 C CNN
	1    5250 1600
	1    0    0    -1  
$EndComp
$Comp
L R R103
U 1 1 5CA8C2BA
P 5450 1600
F 0 "R103" V 5530 1600 40  0000 C CNN
F 1 "10K Ohms" V 5457 1601 40  0000 C CNN
F 2 "~" V 5380 1600 30  0000 C CNN
F 3 "~" H 5450 1600 30  0000 C CNN
	1    5450 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 1250 1300 2200
Wire Wire Line
	1300 1400 1500 1400
Wire Wire Line
	1300 2200 1500 2200
Connection ~ 1300 1400
Wire Wire Line
	3550 3750 1350 3750
Wire Wire Line
	2500 3750 2500 3900
Wire Wire Line
	1500 2600 1350 2600
Wire Wire Line
	1350 3750 1350 1800
Wire Wire Line
	1350 3300 1500 3300
Wire Wire Line
	3400 3000 3550 3000
Wire Wire Line
	3550 1600 3550 3750
Connection ~ 2500 3750
Wire Wire Line
	3550 2800 3400 2800
Connection ~ 3550 3000
Connection ~ 1350 3300
Wire Wire Line
	3400 2300 3550 2300
Connection ~ 3550 2800
Wire Wire Line
	3550 2000 3400 2000
Connection ~ 3550 2300
Wire Wire Line
	1350 1800 1500 1800
Connection ~ 1350 2600
Wire Wire Line
	3400 1600 3550 1600
Connection ~ 3550 2000
Wire Wire Line
	4900 1900 6200 1900
Wire Wire Line
	4900 1900 4900 1950
Wire Wire Line
	6200 2050 4900 2050
Wire Wire Line
	5050 1850 5050 3750
Connection ~ 5050 1900
Wire Wire Line
	5250 1850 5250 3900
Connection ~ 5250 2050
Wire Wire Line
	5450 1850 5450 4100
Wire Wire Line
	5450 2250 6200 2250
Wire Wire Line
	4100 1950 4100 3250
Wire Wire Line
	4100 3250 3550 3250
Connection ~ 3550 3250
Connection ~ 4100 2050
Wire Wire Line
	4450 1350 5450 1350
Connection ~ 5250 1350
Wire Wire Line
	4450 1350 4450 850 
Wire Wire Line
	4450 850  1000 850 
Wire Wire Line
	1000 850  1000 1600
Wire Wire Line
	1000 1600 1300 1600
Connection ~ 1300 1600
Connection ~ 5050 1350
Wire Wire Line
	5450 4100 6200 4100
Connection ~ 5450 2250
Wire Wire Line
	5250 3900 6200 3900
Wire Wire Line
	5050 3750 6200 3750
Text Label 6200 1550 2    60   ~ 0
SCL
Text Label 6200 1700 2    60   ~ 0
SDA
Text Label 6200 3400 2    60   ~ 0
SCL
Text Label 6200 3550 2    60   ~ 0
SDA
$EndSCHEMATC
