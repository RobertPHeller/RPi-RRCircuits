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
LIBS:ButtonDaughterBoard-cache
LIBS:ButtonDaughterBoard-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "17 may 2018"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L I2CIN J1
U 1 1 5AFDC931
P 10450 1200
F 0 "J1" H 10450 1050 25  0000 C CNN
F 1 "I2CIN" H 10450 1350 25  0000 C CNN
F 2 "~" H 10450 1200 60  0000 C CNN
F 3 "~" H 10450 1200 60  0000 C CNN
	1    10450 1200
	1    0    0    -1  
$EndComp
$Comp
L I2COUT J2
U 1 1 5AFDC983
P 10450 2450
F 0 "J2" H 10450 2300 25  0000 C CNN
F 1 "I2COUT" H 10450 2600 25  0000 C CNN
F 2 "~" H 10450 2450 60  0000 C CNN
F 3 "~" H 10450 2450 60  0000 C CNN
	1    10450 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 1000 9450 1000
Wire Wire Line
	9450 1000 9450 2250
Wire Wire Line
	9450 2250 10050 2250
$Comp
L +5V #PWR01
U 1 1 5AFDC999
P 9450 1550
F 0 "#PWR01" H 9450 1640 20  0001 C CNN
F 1 "+5V" H 9450 1640 30  0000 C CNN
F 2 "" H 9450 1550 60  0000 C CNN
F 3 "" H 9450 1550 60  0000 C CNN
	1    9450 1550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10050 1100 9550 1100
Wire Wire Line
	9550 1100 9550 2350
Wire Wire Line
	9550 2350 10050 2350
$Comp
L +3.3V #PWR02
U 1 1 5AFDC9B0
P 9550 1700
F 0 "#PWR02" H 9550 1660 30  0001 C CNN
F 1 "+3.3V" H 9550 1810 30  0000 C CNN
F 2 "" H 9550 1700 60  0000 C CNN
F 3 "" H 9550 1700 60  0000 C CNN
	1    9550 1700
	0    1    1    0   
$EndComp
Wire Wire Line
	10050 1200 9700 1200
Wire Wire Line
	9700 1200 9700 2450
Wire Wire Line
	9700 2450 10050 2450
Wire Wire Line
	10050 1300 9850 1300
Wire Wire Line
	9850 1300 9850 2550
Wire Wire Line
	9850 2550 10050 2550
$Comp
L GND #PWR03
U 1 1 5AFDCA17
P 10050 1400
F 0 "#PWR03" H 10050 1400 30  0001 C CNN
F 1 "GND" H 10050 1330 30  0001 C CNN
F 2 "" H 10050 1400 60  0000 C CNN
F 3 "" H 10050 1400 60  0000 C CNN
	1    10050 1400
	0    1    1    0   
$EndComp
$Comp
L GND #PWR04
U 1 1 5AFDCA26
P 10050 2650
F 0 "#PWR04" H 10050 2650 30  0001 C CNN
F 1 "GND" H 10050 2580 30  0001 C CNN
F 2 "" H 10050 2650 60  0000 C CNN
F 3 "" H 10050 2650 60  0000 C CNN
	1    10050 2650
	0    1    1    0   
$EndComp
Text GLabel 9350 1400 0    60   Input ~ 0
SDA
Text GLabel 9350 1700 0    60   Input ~ 0
SCL
Wire Wire Line
	9350 1700 9850 1700
Connection ~ 9850 1700
Wire Wire Line
	9350 1400 9700 1400
Connection ~ 9700 1400
$EndSCHEMATC
