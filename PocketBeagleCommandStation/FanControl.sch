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
LIBS:lm2574n-5
LIBS:lmd18200
LIBS:tmp36
LIBS:sn65hvd233-ht
LIBS:L298N
LIBS:PocketBeagleCommandStation-cache
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
L Screw_Terminal_01x02 J5
U 1 1 603F86FA
P 5800 2600
F 0 "J5" H 5800 2700 50  0000 C CNN
F 1 "Fan" H 5800 2400 50  0000 C CNN
F 2 "Connectors_Terminal_Blocks:TerminalBlock_Pheonix_MPT-2.54mm_2pol" H 5800 2600 50  0001 C CNN
F 3 "" H 5800 2600 50  0001 C CNN
	1    5800 2600
	1    0    0    -1  
$EndComp
$Comp
L TIP120 Q13
U 1 1 603F872B
P 5450 3300
F 0 "Q13" H 5700 3375 50  0000 L CNN
F 1 "TIP120" H 5700 3300 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Vertical" H 5700 3225 50  0001 L CIN
F 3 "" H 5450 3300 50  0001 L CNN
	1    5450 3300
	1    0    0    -1  
$EndComp
$Comp
L TMP36 U7
U 1 1 603F8754
P 3450 4400
F 0 "U7" H 3550 4250 60  0000 C CNN
F 1 "TMP36" H 3650 4650 60  0000 C CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 3450 4400 60  0001 C CNN
F 3 "" H 3450 4400 60  0000 C CNN
	1    3450 4400
	1    0    0    -1  
$EndComp
$Comp
L R R31
U 1 1 603F8791
P 5550 2150
F 0 "R31" V 5630 2150 50  0000 C CNN
F 1 "56 Ohms 1/2 watt" V 5550 2150 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Vertical" V 5480 2150 50  0001 C CNN
F 3 "" H 5550 2150 50  0001 C CNN
	1    5550 2150
	1    0    0    -1  
$EndComp
$Comp
L R R30
U 1 1 603F8836
P 4600 3300
F 0 "R30" V 4680 3300 50  0000 C CNN
F 1 "1K Ohms" V 4600 3300 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 4530 3300 50  0001 C CNN
F 3 "" H 4600 3300 50  0001 C CNN
	1    4600 3300
	0    1    1    0   
$EndComp
$Comp
L +3.3V #PWR057
U 1 1 603F893A
P 3450 3850
F 0 "#PWR057" H 3450 3700 50  0001 C CNN
F 1 "+3.3V" H 3450 3990 50  0000 C CNN
F 2 "" H 3450 3850 50  0001 C CNN
F 3 "" H 3450 3850 50  0001 C CNN
	1    3450 3850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR058
U 1 1 603F8971
P 5550 3750
F 0 "#PWR058" H 5550 3500 50  0001 C CNN
F 1 "GND" H 5550 3600 50  0000 C CNN
F 2 "" H 5550 3750 50  0001 C CNN
F 3 "" H 5550 3750 50  0001 C CNN
	1    5550 3750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR059
U 1 1 603F898F
P 3450 4900
F 0 "#PWR059" H 3450 4650 50  0001 C CNN
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
Wire Wire Line
	5550 3500 5550 3750
Wire Wire Line
	5250 3300 4750 3300
Wire Wire Line
	3450 4900 3450 4700
Wire Wire Line
	3450 3850 3450 4100
Wire Wire Line
	5600 2700 5550 2700
Wire Wire Line
	5550 2700 5550 3100
Wire Wire Line
	5550 2300 5550 2600
Wire Wire Line
	5550 2600 5600 2600
Wire Wire Line
	4250 3300 4450 3300
Wire Wire Line
	3700 4400 3950 4400
Wire Wire Line
	5350 1900 5550 1900
Wire Wire Line
	5550 1900 5550 2000
$EndSCHEMATC
