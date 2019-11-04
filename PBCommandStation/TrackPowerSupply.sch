EESchema Schematic File Version 2
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
LIBS:sn65hvd233-ht
LIBS:lmd18200
LIBS:sn75als174
LIBS:tmp36
LIBS:cl2
LIBS:dpdt
LIBS:gbu4a
LIBS:PBCommandStation-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
Title ""
Date "3 nov 2019"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CL2 T301
U 1 1 5DBDEBCC
P 6000 2350
F 0 "T301" H 6000 2850 70  0000 C CNN
F 1 "CL2" H 6000 1850 70  0000 C CNN
F 2 "~" H 6000 2350 60  0000 C CNN
F 3 "~" H 6000 2350 60  0000 C CNN
F 4 "530-CL2-40-12" H 6000 2350 60  0001 C CNN "Mouser Part Number"
	1    6000 2350
	-1   0    0    1   
$EndComp
$Comp
L DPDT SW301
U 1 1 5DBDEBCD
P 4300 2300
F 0 "SW301" H 4300 2400 70  0000 C CNN
F 1 "DPDT" H 4300 2200 70  0000 C CNN
F 2 "~" H 4300 2300 60  0000 C CNN
F 3 "~" H 4300 2300 60  0000 C CNN
	1    4300 2300
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4800 2100 4800 2250
Wire Wire Line
	4800 2250 5600 2250
Wire Wire Line
	4800 2500 4800 2400
Wire Wire Line
	4800 2400 5600 2400
Wire Wire Line
	5600 2400 5600 2450
Wire Wire Line
	3800 1950 3550 1950
Wire Wire Line
	3550 1950 3550 2350
Wire Wire Line
	3550 2350 3800 2350
Wire Wire Line
	3800 2100 3650 2100
Wire Wire Line
	3650 2100 3650 2750
Wire Wire Line
	2950 2750 5600 2750
Wire Wire Line
	5600 1950 5150 1950
Wire Wire Line
	5150 1950 5150 1750
Wire Wire Line
	5150 1750 2950 1750
Wire Wire Line
	3350 1750 3350 2500
Wire Wire Line
	3350 2500 3800 2500
$Comp
L CONN_3 K301
U 1 1 5DBDEBCE
P 2600 2200
F 0 "K301" V 2550 2200 50  0000 C CNN
F 1 "AC Power" V 2650 2200 40  0000 C CNN
F 2 "~" H 2600 2200 60  0000 C CNN
F 3 "~" H 2600 2200 60  0000 C CNN
	1    2600 2200
	-1   0    0    -1  
$EndComp
$Comp
L GNDPWR #PWR020
U 1 1 5DBDEBCF
P 3100 2200
F 0 "#PWR020" H 3100 2250 40  0001 C CNN
F 1 "GNDPWR" H 3100 2120 40  0000 C CNN
F 2 "" H 3100 2200 60  0000 C CNN
F 3 "" H 3100 2200 60  0000 C CNN
	1    3100 2200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3100 2200 2950 2200
Wire Wire Line
	2950 1750 2950 2100
Connection ~ 3350 1750
Wire Wire Line
	2950 2300 2950 2750
Connection ~ 3650 2750
$Comp
L GBU4A D301
U 1 1 5DBDEBD0
P 7850 2350
F 0 "D301" H 7850 2400 70  0000 C CNN
F 1 "GBU4A" H 7850 2300 70  0000 C CNN
F 2 "~" H 7850 2350 60  0000 C CNN
F 3 "~" H 7850 2350 60  0000 C CNN
	1    7850 2350
	0    1    -1   0   
$EndComp
Wire Wire Line
	6400 2550 6800 2550
Wire Wire Line
	6800 2550 6800 2350
Wire Wire Line
	6800 2350 7000 2350
Wire Wire Line
	6400 2150 6400 1200
Wire Wire Line
	6400 1200 8700 1200
Wire Wire Line
	8700 1200 8700 2350
$Comp
L CP1 C301
U 1 1 5DBDEBD1
P 9700 2250
F 0 "C301" H 9750 2350 50  0000 L CNN
F 1 "CP1" H 9750 2150 50  0000 L CNN
F 2 "~" H 9700 2250 60  0000 C CNN
F 3 "~" H 9700 2250 60  0000 C CNN
	1    9700 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 1500 9150 1500
Wire Wire Line
	9150 1500 9150 2050
Wire Wire Line
	9150 2050 10350 2050
Wire Wire Line
	7850 3200 8850 3200
Wire Wire Line
	8850 3200 8850 2450
Wire Wire Line
	8850 2450 10350 2450
$Comp
L CONN_2 TERM301
U 1 1 5DBDEBD2
P 10700 2200
F 0 "TERM301" V 10650 2200 40  0000 C CNN
F 1 "Track Power" V 10750 2200 40  0000 C CNN
F 2 "~" H 10700 2200 60  0000 C CNN
F 3 "~" H 10700 2200 60  0000 C CNN
	1    10700 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	10350 2050 10350 2100
Connection ~ 9700 2050
Wire Wire Line
	10350 2450 10350 2300
Connection ~ 9700 2450
$EndSCHEMATC
