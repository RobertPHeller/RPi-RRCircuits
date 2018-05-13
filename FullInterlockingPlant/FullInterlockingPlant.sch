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
LIBS:FullInterlockingPlant-cache
LIBS:FullInterlockingPlant-cache
LIBS:SMCSenseHAT-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
Title ""
Date "13 may 2018"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RPI_GPIO J?
U 1 1 5AF76E30
P 1550 1250
F 0 "J?" H 2300 1500 60  0000 C CNN
F 1 "RPI_GPIO" H 2300 1400 60  0000 C CNN
F 2 "~" H 1550 1250 60  0000 C CNN
F 3 "~" H 1550 1250 60  0000 C CNN
	1    1550 1250
	1    0    0    -1  
$EndComp
$Sheet
S 4600 1250 1050 1100
U 5AF77157
F0 "StallMotors" 50
F1 "StallMotors.sch" 50
F2 "M Select 2" I L 4600 1950 60 
F3 "MSelect 1" I L 4600 1400 60 
F4 "Point Sense 1" O L 4600 1600 60 
F5 "Point Sense 2" O L 4600 2150 60 
$EndSheet
$Comp
L +3.3V #PWR?
U 1 1 5AF7B7BF
P 1350 1250
F 0 "#PWR?" H 1350 1210 30  0001 C CNN
F 1 "+3.3V" H 1350 1360 30  0000 C CNN
F 2 "" H 1350 1250 60  0000 C CNN
F 3 "" H 1350 1250 60  0000 C CNN
	1    1350 1250
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 5AF7B7DA
P 1350 2050
F 0 "#PWR?" H 1350 2010 30  0001 C CNN
F 1 "+3.3V" H 1350 2160 30  0000 C CNN
F 2 "" H 1350 2050 60  0000 C CNN
F 3 "" H 1350 2050 60  0000 C CNN
	1    1350 2050
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5AF7B7F5
P 1350 1650
F 0 "#PWR?" H 1350 1650 30  0001 C CNN
F 1 "GND" H 1350 1580 30  0001 C CNN
F 2 "" H 1350 1650 60  0000 C CNN
F 3 "" H 1350 1650 60  0000 C CNN
	1    1350 1650
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5AF7B810
P 1350 2450
F 0 "#PWR?" H 1350 2450 30  0001 C CNN
F 1 "GND" H 1350 2380 30  0001 C CNN
F 2 "" H 1350 2450 60  0000 C CNN
F 3 "" H 1350 2450 60  0000 C CNN
	1    1350 2450
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5AF7B837
P 1350 3150
F 0 "#PWR?" H 1350 3150 30  0001 C CNN
F 1 "GND" H 1350 3080 30  0001 C CNN
F 2 "" H 1350 3150 60  0000 C CNN
F 3 "" H 1350 3150 60  0000 C CNN
	1    1350 3150
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5AF7C0AA
P 3250 2850
F 0 "#PWR?" H 3250 2850 30  0001 C CNN
F 1 "GND" H 3250 2780 30  0001 C CNN
F 2 "" H 3250 2850 60  0000 C CNN
F 3 "" H 3250 2850 60  0000 C CNN
	1    3250 2850
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5AF7C0C5
P 3250 2650
F 0 "#PWR?" H 3250 2650 30  0001 C CNN
F 1 "GND" H 3250 2580 30  0001 C CNN
F 2 "" H 3250 2650 60  0000 C CNN
F 3 "" H 3250 2650 60  0000 C CNN
	1    3250 2650
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5AF7C0E0
P 3250 2150
F 0 "#PWR?" H 3250 2150 30  0001 C CNN
F 1 "GND" H 3250 2080 30  0001 C CNN
F 2 "" H 3250 2150 60  0000 C CNN
F 3 "" H 3250 2150 60  0000 C CNN
	1    3250 2150
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5AF7C0FB
P 3250 1850
F 0 "#PWR?" H 3250 1850 30  0001 C CNN
F 1 "GND" H 3250 1780 30  0001 C CNN
F 2 "" H 3250 1850 60  0000 C CNN
F 3 "" H 3250 1850 60  0000 C CNN
	1    3250 1850
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5AF7C116
P 3250 1450
F 0 "#PWR?" H 3250 1450 30  0001 C CNN
F 1 "GND" H 3250 1380 30  0001 C CNN
F 2 "" H 3250 1450 60  0000 C CNN
F 3 "" H 3250 1450 60  0000 C CNN
	1    3250 1450
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR?
U 1 1 5AF7C1FD
P 3250 1250
F 0 "#PWR?" H 3250 1340 20  0001 C CNN
F 1 "+5V" H 3250 1340 30  0000 C CNN
F 2 "" H 3250 1250 60  0000 C CNN
F 3 "" H 3250 1250 60  0000 C CNN
	1    3250 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 1250 3250 1350
Wire Wire Line
	1350 1750 1100 1750
Wire Wire Line
	1100 1750 1100 750 
Wire Wire Line
	1100 750  4500 750 
Wire Wire Line
	4500 750  4500 1400
Wire Wire Line
	4500 1400 4600 1400
Wire Wire Line
	1350 1850 1000 1850
Wire Wire Line
	1000 1850 1000 650 
Wire Wire Line
	1000 650  4400 650 
Wire Wire Line
	4400 650  4400 1600
Wire Wire Line
	4400 1600 4600 1600
Wire Wire Line
	3250 1750 4450 1750
Wire Wire Line
	4450 1750 4450 1950
Wire Wire Line
	4450 1950 4600 1950
Wire Wire Line
	1350 1950 900  1950
Wire Wire Line
	900  1950 900  550 
Wire Wire Line
	900  550  4300 550 
Wire Wire Line
	4300 550  4300 2150
Wire Wire Line
	4300 2150 4600 2150
Text GLabel 1350 1350 0    39   Input ~ 0
SDA
Text GLabel 1350 1450 0    39   Input ~ 0
SCL
Text GLabel 1350 1550 0    39   Input ~ 0
#4
Text GLabel 3250 1950 2    39   Input ~ 0
#23
Text GLabel 3250 2050 2    39   Input ~ 0
#24
Text GLabel 3250 2250 2    39   Input ~ 0
#25
Text GLabel 1350 2650 0    39   Input ~ 0
#5
Text GLabel 1350 2750 0    39   Input ~ 0
#6
Text GLabel 3250 2750 2    39   Input ~ 0
#12
Text GLabel 1350 2850 0    39   Input ~ 0
#13
Text GLabel 3250 2950 2    39   Input ~ 0
#16
Text GLabel 1350 2950 0    39   Input ~ 0
#19
Text GLabel 3250 3050 2    39   Input ~ 0
#20
Text GLabel 3250 3150 2    39   Input ~ 0
#21
Text GLabel 1350 3050 0    39   Input ~ 0
#26
$Comp
L CONN_5 P?
U 1 1 5AF819CD
P 1150 4300
F 0 "P?" V 1100 4300 50  0000 C CNN
F 1 "CONN_5" V 1200 4300 50  0000 C CNN
F 2 "~" H 1150 4300 60  0000 C CNN
F 3 "~" H 1150 4300 60  0000 C CNN
	1    1150 4300
	0    -1   1    0   
$EndComp
$Comp
L CONN_5 P?
U 1 1 5AF819F4
P 1750 4300
F 0 "P?" V 1700 4300 50  0000 C CNN
F 1 "CONN_5" V 1800 4300 50  0000 C CNN
F 2 "~" H 1750 4300 60  0000 C CNN
F 3 "~" H 1750 4300 60  0000 C CNN
	1    1750 4300
	0    -1   1    0   
$EndComp
$Comp
L CONN_5 P?
U 1 1 5AF81A0F
P 2350 4300
F 0 "P?" V 2300 4300 50  0000 C CNN
F 1 "CONN_5" V 2400 4300 50  0000 C CNN
F 2 "~" H 2350 4300 60  0000 C CNN
F 3 "~" H 2350 4300 60  0000 C CNN
	1    2350 4300
	0    -1   1    0   
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 5AF81A2A
P 950 3900
F 0 "#PWR?" H 950 3860 30  0001 C CNN
F 1 "+3.3V" H 950 4010 30  0000 C CNN
F 2 "" H 950 3900 60  0000 C CNN
F 3 "" H 950 3900 60  0000 C CNN
	1    950  3900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5AF81B59
P 2550 3900
F 0 "#PWR?" H 2550 3900 30  0001 C CNN
F 1 "GND" H 2550 3830 30  0001 C CNN
F 2 "" H 2550 3900 60  0000 C CNN
F 3 "" H 2550 3900 60  0000 C CNN
	1    2550 3900
	-1   0    0    1   
$EndComp
Text GLabel 1050 3900 1    39   Input ~ 0
#4
Text GLabel 1150 3900 1    39   Input ~ 0
#23
Text GLabel 1250 3900 1    39   Input ~ 0
#24
Text GLabel 1550 3900 1    39   Input ~ 0
#5
Text GLabel 1650 3900 1    39   Input ~ 0
#6
Text GLabel 1750 3900 1    39   Input ~ 0
#12
Text GLabel 1850 3900 1    39   Input ~ 0
#13
Text GLabel 1950 3900 1    39   Input ~ 0
#16
Text GLabel 2150 3900 1    39   Input ~ 0
#19
Text GLabel 2250 3900 1    39   Input ~ 0
#20
Text GLabel 2350 3900 1    39   Input ~ 0
#21
Text GLabel 1350 3900 1    39   Input ~ 0
#25
Text GLabel 2450 3900 1    39   Input ~ 0
#26
$EndSCHEMATC
