EESchema Schematic File Version 2
LIBS:LCCCANCape-rescue
LIBS:valves
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
LIBS:74lv125AN
LIBS:mcp25xxx
LIBS:cat24c256w
LIBS:sn65hvd233-ht
LIBS:LCCCANCape-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title ""
Date "3 mar 2019"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CAT24C256W U2
U 1 1 5C573FFA
P 5200 3650
F 0 "U2" H 5200 3650 60  0000 C CNN
F 1 "CAT24C256W" H 5200 3800 30  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 5200 3650 60  0001 C CNN
F 3 "~" H 5200 3650 60  0000 C CNN
F 4 "698-CAT24C256WI-GT3" H 5200 3650 60  0001 C CNN "Mouser Part Number"
	1    5200 3650
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-LCCCANCape R8
U 1 1 5C574009
P 4650 3000
F 0 "R8" V 4730 3000 40  0000 C CNN
F 1 "4.75K Ohms" V 4657 3001 40  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4580 3000 30  0001 C CNN
F 3 "~" H 4650 3000 30  0000 C CNN
F 4 "603-AC0603FR-104K75L" V 4650 3000 60  0001 C CNN "Mouser Part Number"
	1    4650 3000
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-LCCCANCape R7
U 1 1 5C574018
P 4450 3000
F 0 "R7" V 4530 3000 40  0000 C CNN
F 1 "4.75K Ohms" V 4457 3001 40  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4380 3000 30  0001 C CNN
F 3 "~" H 4450 3000 30  0000 C CNN
F 4 "603-AC0603FR-104K75L" V 4450 3000 60  0001 C CNN "Mouser Part Number"
	1    4450 3000
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-LCCCANCape R6
U 1 1 5C574027
P 4200 3000
F 0 "R6" V 4280 3000 40  0000 C CNN
F 1 "4.75K Ohms" V 4207 3001 40  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4130 3000 30  0001 C CNN
F 3 "~" H 4200 3000 30  0000 C CNN
F 4 "603-AC0603FR-104K75L" V 4200 3000 60  0001 C CNN "Mouser Part Number"
	1    4200 3000
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-LCCCANCape R5
U 1 1 5C574036
P 3850 3000
F 0 "R5" V 3930 3000 40  0000 C CNN
F 1 "5.6K Ohms" V 3857 3001 40  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3780 3000 30  0001 C CNN
F 3 "~" H 3850 3000 30  0000 C CNN
F 4 "603-RC0603FR-075K6P" V 3850 3000 60  0001 C CNN "Mouser Part Number"
	1    3850 3000
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-LCCCANCape R4
U 1 1 5C574045
P 3650 3000
F 0 "R4" V 3730 3000 40  0000 C CNN
F 1 "5.6K Ohms" V 3657 3001 40  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3580 3000 30  0001 C CNN
F 3 "~" H 3650 3000 30  0000 C CNN
F 4 "603-RC0603FR-075K6P" V 3650 3000 60  0001 C CNN "Mouser Part Number"
	1    3650 3000
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-LCCCANCape R9
U 1 1 5C574063
P 6400 3600
F 0 "R9" V 6480 3600 40  0000 C CNN
F 1 "10K Ohms" V 6407 3601 40  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6330 3600 30  0001 C CNN
F 3 "~" H 6400 3600 30  0000 C CNN
F 4 "603-AC0603JR-0710KL" V 6400 3600 60  0001 C CNN "Mouser Part Number"
	1    6400 3600
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-LCCCANCape C4
U 1 1 5C57407C
P 6100 3550
AR Path="/5C57407C" Ref="C4"  Part="1" 
AR Path="/5C719AD8/5C57407C" Ref="C4"  Part="1" 
F 0 "C4" H 6100 3650 40  0000 L CNN
F 1 ".1 uf" H 6106 3465 40  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6138 3400 30  0001 C CNN
F 3 "~" H 6100 3550 60  0000 C CNN
F 4 "710-885012206095" H 6100 3550 60  0001 C CNN "Mouser Part Number"
	1    6100 3550
	1    0    0    -1  
$EndComp
$Comp
L GND-RESCUE-LCCCANCape #PWR020
U 1 1 5C57408B
P 6100 4250
F 0 "#PWR020" H 6100 4250 30  0001 C CNN
F 1 "GND" H 6100 4180 30  0001 C CNN
F 2 "" H 6100 4250 60  0000 C CNN
F 3 "" H 6100 4250 60  0000 C CNN
	1    6100 4250
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR021
U 1 1 5C57409A
P 5900 2550
F 0 "#PWR021" H 5900 2400 50  0001 C CNN
F 1 "+3V3" H 5900 2690 50  0000 C CNN
F 2 "~" H 5900 2550 60  0000 C CNN
F 3 "~" H 5900 2550 60  0000 C CNN
	1    5900 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2750 6100 2750
Connection ~ 3850 2750
Connection ~ 4200 2750
Connection ~ 4450 2750
Wire Wire Line
	6100 2750 6100 3350
Connection ~ 4650 2750
Wire Wire Line
	5900 2550 5900 2750
Connection ~ 5900 2750
Wire Wire Line
	5650 3350 6400 3350
Wire Wire Line
	5700 3350 5700 2750
Connection ~ 5700 2750
Connection ~ 5700 3350
Connection ~ 6100 3350
Wire Wire Line
	5650 3550 5850 3550
Wire Wire Line
	5850 3550 5850 3750
Wire Wire Line
	5850 3750 6100 3750
Wire Wire Line
	6400 3850 5650 3850
Wire Wire Line
	6100 3750 6100 4250
$Comp
L GND-RESCUE-LCCCANCape #PWR022
U 1 1 5C574113
P 3150 4200
F 0 "#PWR022" H 3150 4200 30  0001 C CNN
F 1 "GND" H 3150 4130 30  0001 C CNN
F 2 "" H 3150 4200 60  0000 C CNN
F 3 "" H 3150 4200 60  0000 C CNN
	1    3150 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 3700 3150 4200
Connection ~ 3150 3800
Connection ~ 3150 3900
Wire Wire Line
	3600 3700 4750 3700
Wire Wire Line
	3600 3800 4750 3800
Wire Wire Line
	4650 3250 4650 3900
Wire Wire Line
	4650 3900 4750 3900
Wire Wire Line
	4450 3250 4450 3800
Connection ~ 4450 3800
Wire Wire Line
	4200 3250 4200 3700
Connection ~ 4200 3700
Wire Wire Line
	4750 3350 3350 3350
Wire Wire Line
	4750 3450 3350 3450
Wire Wire Line
	3850 3250 3850 3350
Connection ~ 3850 3350
Wire Wire Line
	3650 3250 3650 3450
Connection ~ 3650 3450
Wire Wire Line
	5750 3850 5750 4100
Wire Wire Line
	5750 4100 3950 4100
Wire Wire Line
	3950 4100 3950 3900
Connection ~ 5750 3850
Text HLabel 3350 3350 0    60   Input ~ 0
SCL
Text HLabel 3350 3450 0    60   BiDi ~ 0
SDA
$Comp
L Jumper_NO_Small JP1
U 1 1 63093DB8
P 3500 3700
F 0 "JP1" H 3500 3780 50  0000 C CNN
F 1 "A0" H 3510 3640 50  0000 C CNN
F 2 "Jumpers_SMD:JP_0603_Open" H 3500 3700 50  0001 C CNN
F 3 "" H 3500 3700 50  0001 C CNN
	1    3500 3700
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NO_Small JP2
U 1 1 63093E7B
P 3500 3800
F 0 "JP2" H 3500 3880 50  0000 C CNN
F 1 "A1" H 3510 3740 50  0000 C CNN
F 2 "Jumpers_SMD:JP_0603_Open" H 3500 3800 50  0001 C CNN
F 3 "" H 3500 3800 50  0001 C CNN
	1    3500 3800
	1    0    0    -1  
$EndComp
$Comp
L Jumper_NC_Small JP3
U 1 1 63093EA1
P 3500 3900
F 0 "JP3" H 3500 3980 50  0000 C CNN
F 1 "WP" H 3510 3840 50  0000 C CNN
F 2 "Jumpers_SMD:JP_0603_Closed" H 3500 3900 50  0001 C CNN
F 3 "" H 3500 3900 50  0001 C CNN
	1    3500 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 3700 3150 3700
Wire Wire Line
	3150 3800 3400 3800
Wire Wire Line
	3150 3900 3400 3900
Wire Wire Line
	3950 3900 3600 3900
$EndSCHEMATC
