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
Sheet 4 7
Title "Pocket Beagle DCC Command Station"
Date "3 mar 2019"
Rev "A"
Comp "Deepwoods Software"
Comment1 "OPS DCC Driver"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L LMD18200 U3
U 1 1 603F10F3
P 5400 2800
F 0 "U3" H 5400 2800 60  0000 C CNN
F 1 "LMD18200" H 5400 2900 60  0000 C CNN
F 2 "TO220_11Pin:TO220_11PIN_VERT" H 5400 2800 60  0001 C CNN
F 3 "" H 5400 2800 60  0000 C CNN
	1    5400 2800
	0    1    1    0   
$EndComp
Text HLabel 3200 1750 0    60   Input ~ 0
PowerIn
$Comp
L C C9
U 1 1 603F1229
P 3450 1750
F 0 "C9" H 3475 1850 50  0000 L CNN
F 1 "1 uf" H 3475 1650 50  0000 L CNN
F 2 "Capacitors_THT:C_Rect_L7.0mm_W2.0mm_P5.00mm" H 3488 1600 50  0001 C CNN
F 3 "" H 3450 1750 50  0001 C CNN
	1    3450 1750
	0    1    1    0   
$EndComp
$Comp
L GND #PWR027
U 1 1 603F1296
P 3700 1750
F 0 "#PWR027" H 3700 1500 50  0001 C CNN
F 1 "GND" H 3700 1600 50  0000 C CNN
F 2 "" H 3700 1750 50  0001 C CNN
F 3 "" H 3700 1750 50  0001 C CNN
	1    3700 1750
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR028
U 1 1 603F130C
P 4800 2850
F 0 "#PWR028" H 4800 2600 50  0001 C CNN
F 1 "GND" H 4800 2700 50  0000 C CNN
F 2 "" H 4800 2850 50  0001 C CNN
F 3 "" H 4800 2850 50  0001 C CNN
	1    4800 2850
	0    1    1    0   
$EndComp
Text HLabel 4700 2600 0    60   Output ~ 0
A
Text HLabel 4700 3000 0    60   Output ~ 0
B
$Comp
L C C12
U 1 1 603F13C3
P 4950 3150
F 0 "C12" H 4975 3250 50  0000 L CNN
F 1 "10 nf" H 4975 3050 50  0000 L CNN
F 2 "Capacitors_THT:C_Rect_L4.0mm_W2.5mm_P2.50mm" H 4988 3000 50  0001 C CNN
F 3 "" H 4950 3150 50  0001 C CNN
	1    4950 3150
	0    1    1    0   
$EndComp
$Comp
L C C11
U 1 1 603F1459
P 4950 2400
F 0 "C11" H 4975 2500 50  0000 L CNN
F 1 "10 nf" H 4975 2300 50  0000 L CNN
F 2 "Capacitors_THT:C_Rect_L4.0mm_W2.5mm_P2.50mm" H 4988 2250 50  0001 C CNN
F 3 "" H 4950 2400 50  0001 C CNN
	1    4950 2400
	0    1    1    0   
$EndComp
Text HLabel 4900 2950 0    60   Output ~ 0
THERM
Text HLabel 4400 3300 0    60   Output ~ 0
Current-Sense
$Comp
L R R13
U 1 1 603F16E6
P 4750 3450
F 0 "R13" V 4830 3450 50  0000 C CNN
F 1 "1.5K Ohms" V 4750 3450 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 4680 3450 50  0001 C CNN
F 3 "" H 4750 3450 50  0001 C CNN
	1    4750 3450
	0    1    1    0   
$EndComp
$Comp
L GND #PWR029
U 1 1 603EA966
P 5200 3550
F 0 "#PWR029" H 5200 3300 50  0001 C CNN
F 1 "GND" H 5200 3400 50  0000 C CNN
F 2 "" H 5200 3550 50  0001 C CNN
F 3 "" H 5200 3550 50  0001 C CNN
	1    5200 3550
	1    0    0    -1  
$EndComp
$Comp
L C C10
U 1 1 603EA99F
P 4750 3700
F 0 "C10" H 4775 3800 50  0000 L CNN
F 1 "100 nf" H 4775 3600 50  0000 L CNN
F 2 "Capacitors_THT:C_Rect_L4.0mm_W2.5mm_P2.50mm" H 4788 3550 50  0001 C CNN
F 3 "" H 4750 3700 50  0001 C CNN
	1    4750 3700
	0    1    1    0   
$EndComp
Text HLabel 3750 2050 0    60   Input ~ 0
SIGNAL
Text HLabel 3750 2200 0    60   Input ~ 0
BRAKE
Text HLabel 3750 2350 0    60   Input ~ 0
ENABLE
Wire Wire Line
	3250 2800 5100 2800
Wire Wire Line
	3200 1750 3300 1750
Connection ~ 3250 1750
Wire Wire Line
	3700 1750 3600 1750
Wire Wire Line
	5100 2850 4800 2850
Wire Wire Line
	4700 2600 5100 2600
Wire Wire Line
	4700 3000 5100 3000
Wire Wire Line
	5100 2400 5100 2550
Wire Wire Line
	4800 2150 4800 2600
Connection ~ 4800 2600
Wire Wire Line
	4800 3150 4800 3000
Connection ~ 4800 3000
Wire Wire Line
	5100 3150 5100 3050
Wire Wire Line
	5100 2950 4900 2950
Wire Wire Line
	5100 2900 4500 2900
Wire Wire Line
	4500 2900 4500 3450
Wire Wire Line
	4500 3300 4400 3300
Wire Wire Line
	4500 3450 4600 3450
Connection ~ 4500 3300
Wire Wire Line
	4900 3450 5200 3450
Wire Wire Line
	5200 3450 5200 3550
Wire Wire Line
	4900 3700 4900 3450
Wire Wire Line
	4600 3450 4600 3700
Wire Wire Line
	3250 2800 3250 1750
Wire Wire Line
	5100 2650 4300 2650
Wire Wire Line
	4300 2650 4300 2050
Wire Wire Line
	4300 2050 3750 2050
Wire Wire Line
	4200 2700 5100 2700
Wire Wire Line
	4200 2200 4200 2700
Wire Wire Line
	3750 2200 4200 2200
Wire Wire Line
	4100 2750 5100 2750
Wire Wire Line
	4100 2350 4100 2750
Wire Wire Line
	3750 2350 4100 2350
$Comp
L R R10
U 1 1 603EB304
P 3800 2550
F 0 "R10" V 3880 2550 50  0000 C CNN
F 1 "20K Ohms" V 3800 2550 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 3730 2550 50  0001 C CNN
F 3 "" H 3800 2550 50  0001 C CNN
	1    3800 2550
	1    0    0    -1  
$EndComp
$Comp
L R R11
U 1 1 603EB358
P 3950 2550
F 0 "R11" V 4030 2550 50  0000 C CNN
F 1 "20K Ohms" V 3950 2550 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 3880 2550 50  0001 C CNN
F 3 "" H 3950 2550 50  0001 C CNN
	1    3950 2550
	1    0    0    -1  
$EndComp
$Comp
L R R12
U 1 1 603EB37F
P 4450 1950
F 0 "R12" V 4530 1950 50  0000 C CNN
F 1 "20K Ohms" V 4450 1950 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 4380 1950 50  0001 C CNN
F 3 "" H 4450 1950 50  0001 C CNN
	1    4450 1950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR030
U 1 1 603EB3A8
P 3800 2950
F 0 "#PWR030" H 3800 2700 50  0001 C CNN
F 1 "GND" H 3800 2800 50  0000 C CNN
F 2 "" H 3800 2950 50  0001 C CNN
F 3 "" H 3800 2950 50  0001 C CNN
	1    3800 2950
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR031
U 1 1 603EB3FB
P 4450 1600
F 0 "#PWR031" H 4450 1450 50  0001 C CNN
F 1 "+3.3V" H 4450 1740 50  0000 C CNN
F 2 "" H 4450 1600 50  0001 C CNN
F 3 "" H 4450 1600 50  0001 C CNN
	1    4450 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 1600 4450 1800
Wire Wire Line
	4450 2100 4450 2700
Connection ~ 4450 2700
Wire Wire Line
	3800 2700 3800 2950
Wire Wire Line
	3950 2700 3800 2700
Wire Wire Line
	3800 2400 3800 2350
Connection ~ 3800 2350
Wire Wire Line
	3950 2400 3950 2050
Connection ~ 3950 2050
$Comp
L LED D8
U 1 1 60633461
P 5950 2500
F 0 "D8" H 5950 2600 50  0000 C CNN
F 1 "DCC Activity" H 5950 2400 50  0000 C CNN
F 2 "LEDs:LED_D3.0mm" H 5950 2500 50  0001 C CNN
F 3 "" H 5950 2500 50  0001 C CNN
	1    5950 2500
	0    -1   -1   0   
$EndComp
$Comp
L R R41
U 1 1 606334FD
P 5950 2950
F 0 "R41" V 6030 2950 50  0000 C CNN
F 1 "3K Ohms" V 5950 2950 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P1.90mm_Vertical" V 5880 2950 50  0001 C CNN
F 3 "" H 5950 2950 50  0001 C CNN
	1    5950 2950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR032
U 1 1 60633528
P 5950 3300
F 0 "#PWR032" H 5950 3050 50  0001 C CNN
F 1 "GND" H 5950 3150 50  0000 C CNN
F 2 "" H 5950 3300 50  0001 C CNN
F 3 "" H 5950 3300 50  0001 C CNN
	1    5950 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 3300 5950 3100
Wire Wire Line
	5950 2800 5950 2650
Wire Wire Line
	5950 2350 5950 2150
Wire Wire Line
	5950 2150 4800 2150
Connection ~ 4800 2400
$Comp
L LED D9
U 1 1 60633BC4
P 6450 4050
F 0 "D9" H 6450 4150 50  0000 C CNN
F 1 "DCC Activity" H 6450 3950 50  0000 C CNN
F 2 "LEDs:LED_D3.0mm" H 6450 4050 50  0001 C CNN
F 3 "" H 6450 4050 50  0001 C CNN
	1    6450 4050
	0    -1   -1   0   
$EndComp
$Comp
L R R42
U 1 1 60633BCA
P 6450 4500
F 0 "R42" V 6530 4500 50  0000 C CNN
F 1 "3K Ohms" V 6450 4500 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P1.90mm_Vertical" V 6380 4500 50  0001 C CNN
F 3 "" H 6450 4500 50  0001 C CNN
	1    6450 4500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR033
U 1 1 60633BD0
P 6450 4850
F 0 "#PWR033" H 6450 4600 50  0001 C CNN
F 1 "GND" H 6450 4700 50  0000 C CNN
F 2 "" H 6450 4850 50  0001 C CNN
F 3 "" H 6450 4850 50  0001 C CNN
	1    6450 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 4850 6450 4650
Wire Wire Line
	6450 4350 6450 4200
Wire Wire Line
	6450 3650 6450 3900
Text HLabel 6250 3650 0    60   Output ~ 0
B
Wire Wire Line
	6250 3650 6450 3650
$Comp
L TEST_1P J8
U 1 1 606DC398
P 4200 1250
F 0 "J8" H 4200 1520 50  0000 C CNN
F 1 "OpsDigTP" H 4200 1450 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Round-TH_Small" H 4400 1250 50  0001 C CNN
F 3 "" H 4400 1250 50  0001 C CNN
	1    4200 1250
	1    0    0    -1  
$EndComp
$Comp
L TEST_1P J7
U 1 1 606DC483
P 4100 1500
F 0 "J7" H 4100 1770 50  0000 C CNN
F 1 "BrakeTP" H 4100 1700 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Round-TH_Small" H 4300 1500 50  0001 C CNN
F 3 "" H 4300 1500 50  0001 C CNN
	1    4100 1500
	1    0    0    -1  
$EndComp
$Comp
L TEST_1P J6
U 1 1 606DC4BA
P 4000 1750
F 0 "J6" H 4000 2020 50  0000 C CNN
F 1 "OpsEnableTP" H 4000 1950 50  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Round-TH_Small" H 4200 1750 50  0001 C CNN
F 3 "" H 4200 1750 50  0001 C CNN
	1    4000 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 1750 4000 2350
Connection ~ 4000 2350
Wire Wire Line
	4100 1500 4100 2200
Connection ~ 4100 2200
Wire Wire Line
	4200 1250 4200 2050
Connection ~ 4200 2050
$EndSCHEMATC
