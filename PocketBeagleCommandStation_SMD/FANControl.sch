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
LIBS:DRV8873HPWPR
LIBS:DRV8801PWP
LIBS:74ahc1g08
LIBS:mcp9701at
LIBS:PocketBeagleCommandStation_SMD-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 7
Title "Pocket Beagle DCC Command Station"
Date "3 mar 2019"
Rev "A"
Comp "Deepwoods Software"
Comment1 "Fan Control"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Screw_Terminal_01x02 J6
U 1 1 6070C31D
P 5800 2600
F 0 "J6" H 5800 2700 50  0000 C CNN
F 1 "Fan" H 5800 2400 50  0000 C CNN
F 2 "Connectors_Terminal_Blocks:TerminalBlock_Pheonix_MPT-2.54mm_2pol" H 5800 2600 50  0001 C CNN
F 3 "" H 5800 2600 50  0001 C CNN
	1    5800 2600
	1    0    0    -1  
$EndComp
$Comp
L R R31
U 1 1 6070C321
P 4600 3300
F 0 "R31" V 4680 3300 50  0000 C CNN
F 1 "1K Ohms" V 4600 3300 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 4530 3300 50  0001 C CNN
F 3 "" H 4600 3300 50  0001 C CNN
	1    4600 3300
	0    1    1    0   
$EndComp
$Comp
L +3.3V #PWR043
U 1 1 6070C322
P 3450 3850
F 0 "#PWR043" H 3450 3700 50  0001 C CNN
F 1 "+3.3V" H 3450 3990 50  0000 C CNN
F 2 "" H 3450 3850 50  0001 C CNN
F 3 "" H 3450 3850 50  0001 C CNN
	1    3450 3850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR044
U 1 1 6070C323
P 5550 3750
F 0 "#PWR044" H 5550 3500 50  0001 C CNN
F 1 "GND" H 5550 3600 50  0000 C CNN
F 2 "" H 5550 3750 50  0001 C CNN
F 3 "" H 5550 3750 50  0001 C CNN
	1    5550 3750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR045
U 1 1 6070C324
P 3450 4900
F 0 "#PWR045" H 3450 4650 50  0001 C CNN
F 1 "GND" H 3450 4750 50  0000 C CNN
F 2 "" H 3450 4900 50  0001 C CNN
F 3 "" H 3450 4900 50  0001 C CNN
	1    3450 4900
	1    0    0    -1  
$EndComp
Text HLabel 5350 1900 0    60   Input ~ 0
FAN_POWER
Text HLabel 4250 3300 0    60   Input ~ 0
FAN_CONTROL
Text HLabel 3950 4400 2    60   Output ~ 0
TEMP_SENSE
$Comp
L MCP9701AT U9
U 1 1 60715777
P 3450 4400
F 0 "U9" H 3450 4400 60  0000 C CNN
F 1 "MCP9701AT" H 2800 4400 60  0000 C CNN
F 2 "" H 3450 4400 60  0001 C CNN
F 3 "" H 3450 4400 60  0001 C CNN
	1    3450 4400
	1    0    0    -1  
$EndComp
$Comp
L Q_NPN_Darlington_BCEC Q3
U 1 1 60715B68
P 5450 3300
F 0 "Q3" H 5670 3350 50  0000 L CNN
F 1 "BSP52T3G " H 5670 3250 50  0000 L CNN
F 2 "" H 5650 3400 50  0001 C CNN
F 3 "" H 5450 3300 50  0001 C CNN
	1    5450 3300
	1    0    0    -1  
$EndComp
$Comp
L D D9
U 1 1 60715FC4
P 6100 3300
F 0 "D9" H 6100 3400 50  0000 C CNN
F 1 "D" H 6100 3200 50  0000 C CNN
F 2 "" H 6100 3300 50  0001 C CNN
F 3 "" H 6100 3300 50  0001 C CNN
	1    6100 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	5550 3500 5550 3750
Wire Wire Line
	5250 3300 4750 3300
Wire Wire Line
	5600 2700 5550 2700
Wire Wire Line
	5550 2700 5550 3100
Wire Wire Line
	5550 1900 5550 2600
Wire Wire Line
	5550 2600 5600 2600
Wire Wire Line
	4250 3300 4450 3300
Wire Wire Line
	5350 1900 5550 1900
Wire Wire Line
	3950 4400 3850 4400
Wire Wire Line
	3450 3850 3450 4100
Wire Wire Line
	3450 4900 3450 4750
Wire Wire Line
	5550 3100 6100 3100
Wire Wire Line
	6100 3100 6100 3150
Connection ~ 5650 3100
Wire Wire Line
	5550 3500 6100 3500
Wire Wire Line
	6100 3500 6100 3450
$EndSCHEMATC
