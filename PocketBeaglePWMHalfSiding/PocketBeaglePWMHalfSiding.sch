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
LIBS:pca9685
LIBS:tbd62x83a
LIBS:tc442x
LIBS:mct6h
LIBS:bridge
LIBS:PocketBeaglePWMHalfSiding-cache
EELAYER 27 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 7
Title "Main Sheet"
Date "3 apr 2019"
Rev ""
Comp "Deepwoods Software"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L POCKETBEAGLE-P1 P1
U 1 1 5C7C1FE0
P 2850 2550
F 0 "P1" H 2850 2550 60  0000 C CNN
F 1 "POCKETBEAGLE-P1" H 2850 3700 60  0000 C CNN
F 2 "" H 2850 2550 60  0000 C CNN
F 3 "" H 2850 2550 60  0000 C CNN
	1    2850 2550
	1    0    0    -1  
$EndComp
$Comp
L POCKETBEAGLE-P2 P2
U 1 1 5C7C1FEF
P 5150 2550
F 0 "P2" H 5150 2550 60  0000 C CNN
F 1 "POCKETBEAGLE-P2" H 5150 3700 60  0000 C CNN
F 2 "" H 5150 2550 60  0000 C CNN
F 3 "" H 5150 2550 60  0000 C CNN
	1    5150 2550
	1    0    0    -1  
$EndComp
$Sheet
S 1500 4600 1300 1500
U 5CA3E60E
F0 "CAN Transeiver" 50
F1 "CAN_Transeiver.sch" 50
F2 "CAN_TX" I L 1500 4800 60 
F3 "CAN_RX" I L 1500 5000 60 
$EndSheet
$Sheet
S 3650 4600 1500 1600
U 5CA3E635
F0 "Power Supply" 50
F1 "PowerSupply.sch" 50
$EndSheet
$Sheet
S 6000 4650 1550 1650
U 5CA3E65E
F0 "PCA9685 Driver" 50
F1 "PCA9685Driver.sch" 50
F2 "SCL" I L 6000 4900 60 
F3 "SDA" B L 6000 5150 60 
F4 "OE" I L 6000 5450 60 
$EndSheet
$Sheet
S 6700 1500 1550 1900
U 5CA3E684
F0 "Occupancy Detector 1" 50
F1 "OD1.sch" 50
F2 "OD1" I L 6700 1750 60 
$EndSheet
$Sheet
S 8900 1700 1400 1700
U 5CA3E6A2
F0 "Occupany Detector 2" 50
F1 "OD2.sch" 50
F2 "OD2" I L 8900 1900 60 
$EndSheet
$Sheet
S 8450 4550 1600 1800
U 5CA3E6BB
F0 "Turnouts And Points" 50
F1 "TurnoutsAndPoints.sch" 50
F2 "Motor 1" I L 8450 4750 60 
F3 "Motor 2" I L 8450 4900 60 
F4 "Sense 1" O L 8450 5050 60 
F5 "Sense 2" O L 8450 5200 60 
$EndSheet
$Comp
L +5V #PWR01
U 1 1 5CA42753
P 2250 1450
F 0 "#PWR01" H 2250 1540 20  0001 C CNN
F 1 "+5V" H 2250 1540 30  0000 C CNN
F 2 "" H 2250 1450 60  0000 C CNN
F 3 "" H 2250 1450 60  0000 C CNN
	1    2250 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 1450 2250 1650
Wire Wire Line
	2250 1650 2400 1650
$Comp
L 3V3 #PWR02
U 1 1 5CA427A5
P 3550 2250
F 0 "#PWR02" H 3550 2350 40  0001 C CNN
F 1 "3V3" H 3550 2375 40  0000 C CNN
F 2 "" H 3550 2250 60  0000 C CNN
F 3 "" H 3550 2250 60  0000 C CNN
	1    3550 2250
	0    1    1    0   
$EndComp
Wire Wire Line
	3300 2250 3550 2250
$Comp
L GND #PWR03
U 1 1 5CA4286B
P 3600 2350
F 0 "#PWR03" H 3600 2350 30  0001 C CNN
F 1 "GND" H 3600 2280 30  0001 C CNN
F 2 "" H 3600 2350 60  0000 C CNN
F 3 "" H 3600 2350 60  0000 C CNN
	1    3600 2350
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR04
U 1 1 5CA428C1
P 2150 2350
F 0 "#PWR04" H 2150 2350 30  0001 C CNN
F 1 "GND" H 2150 2280 30  0001 C CNN
F 2 "" H 2150 2350 60  0000 C CNN
F 3 "" H 2150 2350 60  0000 C CNN
	1    2150 2350
	0    1    1    0   
$EndComp
Wire Wire Line
	3600 2350 3300 2350
Wire Wire Line
	2400 2350 2150 2350
$Comp
L GND #PWR05
U 1 1 5CA43E81
P 4400 2350
F 0 "#PWR05" H 4400 2350 30  0001 C CNN
F 1 "GND" H 4400 2280 30  0001 C CNN
F 2 "" H 4400 2350 60  0000 C CNN
F 3 "" H 4400 2350 60  0000 C CNN
	1    4400 2350
	0    1    1    0   
$EndComp
Wire Wire Line
	4400 2350 4700 2350
Text Label 3300 2850 0    60   ~ 0
SDA
Text Label 3300 2950 0    60   ~ 0
SCL
Text Label 4700 2850 2    60   ~ 0
CAN1_RX
Text Label 4700 2950 2    60   ~ 0
CAN1_TX
Text Label 1500 4800 2    60   ~ 0
CAN1_TX
Text Label 1500 5000 2    60   ~ 0
CAN1_RX
Text Label 6000 4900 2    60   ~ 0
SCL
Text Label 6000 5150 2    60   ~ 0
SDA
Text Label 6000 5450 2    60   ~ 0
GPIO89
Text Label 3300 1750 0    60   ~ 0
GPIO89
Text Label 3300 2550 0    60   ~ 0
GPIO20
Text Label 3300 1650 0    60   ~ 0
GPIO87
Text Label 3300 3250 0    60   ~ 0
GPIO26
Text Label 4700 1750 2    60   ~ 0
GPIO23
Text Label 4700 2450 2    60   ~ 0
GPIO65
Text Label 4700 2550 2    60   ~ 0
GPIO27
Wire Wire Line
	4700 2650 4250 2650
Wire Wire Line
	4250 2650 4250 2200
Wire Wire Line
	4250 2200 4500 2200
Wire Wire Line
	4500 2200 4500 2350
Connection ~ 4500 2350
Wire Wire Line
	4700 2750 4150 2750
Wire Wire Line
	4150 2750 4150 2100
Wire Wire Line
	4150 2100 3500 2100
Wire Wire Line
	3500 2100 3500 2250
Text Label 4700 3250 2    60   ~ 0
GPIO45
Text Label 5600 1650 0    60   ~ 0
GPIO59
Text Label 5600 1750 0    60   ~ 0
GPIO58
Text Label 5600 1850 0    60   ~ 0
GPIO57
Text Label 5600 1950 0    60   ~ 0
GPIO60
Text Label 5600 2050 0    60   ~ 0
GPIO52
Text Label 5600 2450 0    60   ~ 0
GPIO47
Text Label 5600 2550 0    60   ~ 0
GPIO64
Text Label 5600 2650 0    60   ~ 0
GPIO46
Text Label 5600 2750 0    60   ~ 0
GPIO48
Text Label 8450 4750 2    60   ~ 0
GPIO20
Text Label 8450 4900 2    60   ~ 0
GPIO23
Text Label 8450 5050 2    60   ~ 0
GPIO45
Text Label 8450 5200 2    60   ~ 0
GPIO46
Text Label 6700 1750 2    60   ~ 0
GPIO52
Text Label 8900 1900 2    60   ~ 0
GPIO57
Connection ~ 3500 2250
$EndSCHEMATC
