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
LIBS:mct6h
LIBS:bridge
LIBS:QuadOCDectHat-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 5
Title ""
Date "24 feb 2019"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RPi_GPIO J0
U 1 1 5516AE26
P 1650 1250
F 0 "J0" H 2400 1500 60  0000 C CNN
F 1 "RPi_GPIO" H 2400 1400 60  0000 C CNN
F 2 "RPi_Hat:Pin_Header_Straight_2x20" H 1650 1250 60  0001 C CNN
F 3 "" H 1650 1250 60  0000 C CNN
	1    1650 1250
	1    0    0    -1  
$EndComp
Text Label 1450 2650 2    60   ~ 0
OC1
Text Label 1450 2750 2    60   ~ 0
OC2
Text Label 1450 2850 2    60   ~ 0
OC3
Text Label 1450 2950 2    60   ~ 0
OC4
$Sheet
S 4500 1550 1750 1700
U 5C70A8FB
F0 "Occupancy Detector 1" 50
F1 "OD1.sch" 50
F2 "OD1" O L 4500 1750 60 
$EndSheet
$Sheet
S 7200 1550 1750 1700
U 5C70A8FB
F0 "Occupancy Detector 2" 50
F1 "OD2.sch" 50
F2 "OD2" O L 7200 1850 60 
$EndSheet
$Sheet
S 4450 4000 1850 1850
U 5C70A8FB
F0 "Occupancy Detector 3" 50
F1 "OD3.sch" 50
F2 "OD3" O L 4450 4250 60 
$EndSheet
$Sheet
S 7250 4150 1600 1800
U 5C70A8FB
F0 "Occupancy Detector 4" 50
F1 "OD4.sch" 50
F2 "OD4" O L 7250 4350 60 
$EndSheet
Text Label 4200 1750 2    60   ~ 0
OC1
Wire Wire Line
	4500 1750 4200 1750
Text Label 6950 1850 2    60   ~ 0
OC2
Wire Wire Line
	7200 1850 6950 1850
Text Label 4150 4250 2    60   ~ 0
OC3
Text Label 6900 4350 2    60   ~ 0
OC4
Wire Wire Line
	6900 4350 7250 4350
Wire Wire Line
	4450 4250 4150 4250
$Comp
L +3.3V #PWR01
U 1 1 5C70B840
P 1350 950
F 0 "#PWR01" H 1350 910 30  0001 C CNN
F 1 "+3.3V" H 1350 1060 30  0000 C CNN
F 2 "" H 1350 950 60  0000 C CNN
F 3 "" H 1350 950 60  0000 C CNN
	1    1350 950 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 5C70B85B
P 1300 3450
F 0 "#PWR02" H 1300 3450 30  0001 C CNN
F 1 "GND" H 1300 3380 30  0001 C CNN
F 2 "" H 1300 3450 60  0000 C CNN
F 3 "" H 1300 3450 60  0000 C CNN
	1    1300 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 1250 1350 1250
Wire Wire Line
	1350 950  1350 2050
Wire Wire Line
	1350 2050 1450 2050
Connection ~ 1350 1250
Wire Wire Line
	1450 1650 1250 1650
Wire Wire Line
	1250 1650 1250 3150
Wire Wire Line
	1250 2450 1450 2450
Wire Wire Line
	1250 3150 1450 3150
Connection ~ 1250 2450
Wire Wire Line
	1300 3450 1300 3150
Connection ~ 1300 3150
Wire Wire Line
	3550 2850 3350 2850
Wire Wire Line
	3550 1450 3550 3350
Wire Wire Line
	3550 2650 3350 2650
Wire Wire Line
	3550 2150 3350 2150
Connection ~ 3550 2650
Wire Wire Line
	3350 1850 3550 1850
Connection ~ 3550 2150
Wire Wire Line
	3350 1450 3550 1450
Connection ~ 3550 1850
Wire Wire Line
	3550 3350 1300 3350
Connection ~ 1300 3350
Connection ~ 3550 2850
$EndSCHEMATC
