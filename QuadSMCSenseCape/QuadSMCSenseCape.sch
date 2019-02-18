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
LIBS:tc442x
LIBS:mcp25xxx
LIBS:74lv125AN
LIBS:cat24c256w
LIBS:QuadSMCSenseCape-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
Title ""
Date "3 feb 2019"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L GNDD #PWR01
U 1 1 5589785A
P 2450 3650
F 0 "#PWR01" H 2450 3400 50  0001 C CNN
F 1 "GNDD" H 2450 3500 50  0000 C CNN
F 2 "" H 2450 3650 60  0000 C CNN
F 3 "" H 2450 3650 60  0000 C CNN
	1    2450 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 3250 2850 3250
Wire Wire Line
	2850 3250 2850 4050
Wire Wire Line
	2850 3550 2050 3550
Wire Wire Line
	2050 3550 2050 3250
Wire Wire Line
	2050 3250 2200 3250
Wire Wire Line
	2200 3350 2050 3350
Connection ~ 2050 3350
Wire Wire Line
	2700 3350 2850 3350
Connection ~ 2850 3350
Wire Wire Line
	2450 3650 2450 3550
Connection ~ 2450 3550
$Comp
L GNDD #PWR02
U 1 1 558978D3
P 3500 1150
F 0 "#PWR02" H 3500 900 50  0001 C CNN
F 1 "GNDD" H 3500 1000 50  0000 C CNN
F 2 "" H 3500 1150 60  0000 C CNN
F 3 "" H 3500 1150 60  0000 C CNN
	1    3500 1150
	0    -1   -1   0   
$EndComp
$Comp
L GNDD #PWR03
U 1 1 5589790A
P 1400 1150
F 0 "#PWR03" H 1400 900 50  0001 C CNN
F 1 "GNDD" H 1400 1000 50  0000 C CNN
F 2 "" H 1400 1150 60  0000 C CNN
F 3 "" H 1400 1150 60  0000 C CNN
	1    1400 1150
	0    1    1    0   
$EndComp
$Comp
L GNDD #PWR04
U 1 1 558979C1
P 4650 1150
F 0 "#PWR04" H 4650 900 50  0001 C CNN
F 1 "GNDD" H 4650 1000 50  0000 C CNN
F 2 "" H 4650 1150 60  0000 C CNN
F 3 "" H 4650 1150 60  0000 C CNN
	1    4650 1150
	0    1    1    0   
$EndComp
$Comp
L GNDD #PWR05
U 1 1 558979E8
P 5650 1150
F 0 "#PWR05" H 5650 900 50  0001 C CNN
F 1 "GNDD" H 5650 1000 50  0000 C CNN
F 2 "" H 5650 1150 60  0000 C CNN
F 3 "" H 5650 1150 60  0000 C CNN
	1    5650 1150
	0    -1   -1   0   
$EndComp
$Comp
L +3V3 #PWR06
U 1 1 55897A67
P 2850 950
F 0 "#PWR06" H 2850 800 50  0001 C CNN
F 1 "+3V3" H 2850 1090 50  0000 C CNN
F 2 "" H 2850 950 60  0000 C CNN
F 3 "" H 2850 950 60  0000 C CNN
	1    2850 950 
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR07
U 1 1 55897A7F
P 3050 950
F 0 "#PWR07" H 3050 800 50  0001 C CNN
F 1 "+5V" H 3050 1090 50  0000 C CNN
F 2 "" H 3050 950 60  0000 C CNN
F 3 "" H 3050 950 60  0000 C CNN
	1    3050 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 1250 2850 1250
Wire Wire Line
	2850 1250 2850 950 
Wire Wire Line
	2700 1350 3050 1350
Wire Wire Line
	3050 1350 3050 950 
Wire Wire Line
	3300 950  3300 1450
Wire Wire Line
	3300 1450 2700 1450
$Comp
L +3V3 #PWR08
U 1 1 55897EE7
P 2050 950
F 0 "#PWR08" H 2050 800 50  0001 C CNN
F 1 "+3V3" H 2050 1090 50  0000 C CNN
F 2 "" H 2050 950 60  0000 C CNN
F 3 "" H 2050 950 60  0000 C CNN
	1    2050 950 
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR09
U 1 1 55897EF8
P 1850 950
F 0 "#PWR09" H 1850 800 50  0001 C CNN
F 1 "+5V" H 1850 1090 50  0000 C CNN
F 2 "" H 1850 950 60  0000 C CNN
F 3 "" H 1850 950 60  0000 C CNN
	1    1850 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 1250 2050 1250
Wire Wire Line
	2050 1250 2050 950 
Wire Wire Line
	2200 1350 1850 1350
Wire Wire Line
	1850 1350 1850 950 
Wire Wire Line
	2200 1450 1600 1450
Wire Wire Line
	1600 1450 1600 950 
Wire Wire Line
	2700 1150 3500 1150
Wire Wire Line
	2200 1150 1400 1150
Wire Wire Line
	4650 1150 4900 1150
Wire Wire Line
	5400 1150 5650 1150
$Comp
L SYS_5V #PWR010
U 1 1 55898497
P 3300 950
F 0 "#PWR010" H 3300 800 50  0001 C CNN
F 1 "SYS_5V" H 3300 1090 50  0000 C CNN
F 2 "" H 3300 950 60  0000 C CNN
F 3 "" H 3300 950 60  0000 C CNN
	1    3300 950 
	1    0    0    -1  
$EndComp
$Comp
L SYS_5V #PWR011
U 1 1 558984AF
P 1600 950
F 0 "#PWR011" H 1600 800 50  0001 C CNN
F 1 "SYS_5V" H 1600 1090 50  0000 C CNN
F 2 "" H 1600 950 60  0000 C CNN
F 3 "" H 1600 950 60  0000 C CNN
	1    1600 950 
	1    0    0    -1  
$EndComp
$Comp
L VDD_ADC #PWR012
U 1 1 558985D8
P 3050 2550
F 0 "#PWR012" H 3050 2400 50  0001 C CNN
F 1 "VDD_ADC" H 3050 2690 50  0000 C CNN
F 2 "" H 3050 2550 60  0000 C CNN
F 3 "" H 3050 2550 60  0000 C CNN
	1    3050 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 2650 3050 2650
Wire Wire Line
	3050 2650 3050 2550
$Comp
L GNDA_ADC #PWR013
U 1 1 5589865D
P 3050 2750
F 0 "#PWR013" H 3050 2500 50  0001 C CNN
F 1 "GNDA_ADC" H 3050 2600 50  0000 C CNN
F 2 "" H 3050 2750 60  0000 C CNN
F 3 "" H 3050 2750 60  0000 C CNN
	1    3050 2750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2700 2750 3050 2750
Text GLabel 2800 1550 2    50   Input ~ 0
SYS_RESETN
Wire Wire Line
	2700 1550 2800 1550
Text GLabel 2100 1550 0    50   Input ~ 0
PWR_BUT
Wire Wire Line
	2200 1550 2100 1550
$Comp
L BeagleBone_Black_Header P9
U 1 1 55DF7DBA
P 2450 2250
F 0 "P9" H 2450 3450 50  0000 C CNN
F 1 "BeagleBone_Black_Header" V 2450 2250 50  0000 C CNN
F 2 "Socket_BeagleBone_Black:Socket_BeagleBone_Black" H 2450 1400 60  0001 C CNN
F 3 "" H 2450 1400 60  0000 C CNN
	1    2450 2250
	1    0    0    -1  
$EndComp
$Comp
L BeagleBone_Black_Header P8
U 1 1 55DF7DE1
P 5150 2250
F 0 "P8" H 5150 3450 50  0000 C CNN
F 1 "BeagleBone_Black_Header" V 5150 2250 50  0000 C CNN
F 2 "Socket_BeagleBone_Black:Socket_BeagleBone_Black" H 5150 1400 60  0001 C CNN
F 3 "" H 5150 1400 60  0000 C CNN
	1    5150 2250
	1    0    0    -1  
$EndComp
$Comp
L TC4428 U1
U 1 1 5C571349
P 7850 1300
F 0 "U1" H 7850 1000 60  0000 C CNN
F 1 "TC4428" H 7850 1600 60  0000 C CNN
F 2 "~" H 7850 1300 60  0000 C CNN
F 3 "~" H 7850 1300 60  0000 C CNN
	1    7850 1300
	1    0    0    -1  
$EndComp
$Comp
L TC4428 U1
U 2 1 5C571358
P 7850 2000
F 0 "U1" H 7850 1700 60  0000 C CNN
F 1 "TC4428" H 7850 2300 60  0000 C CNN
F 2 "~" H 7850 2000 60  0000 C CNN
F 3 "~" H 7850 2000 60  0000 C CNN
	2    7850 2000
	1    0    0    -1  
$EndComp
$Comp
L TC4428 U2
U 1 1 5C571367
P 7850 2850
F 0 "U2" H 7850 2550 60  0000 C CNN
F 1 "TC4428" H 7850 3150 60  0000 C CNN
F 2 "~" H 7850 2850 60  0000 C CNN
F 3 "~" H 7850 2850 60  0000 C CNN
	1    7850 2850
	1    0    0    -1  
$EndComp
$Comp
L TC4428 U2
U 2 1 5C571376
P 7850 3600
F 0 "U2" H 7850 3300 60  0000 C CNN
F 1 "TC4428" H 7850 3900 60  0000 C CNN
F 2 "~" H 7850 3600 60  0000 C CNN
F 3 "~" H 7850 3600 60  0000 C CNN
	2    7850 3600
	1    0    0    -1  
$EndComp
$Comp
L TC4428 U3
U 1 1 5C571383
P 7850 4300
F 0 "U3" H 7850 4000 60  0000 C CNN
F 1 "TC4428" H 7850 4600 60  0000 C CNN
F 2 "~" H 7850 4300 60  0000 C CNN
F 3 "~" H 7850 4300 60  0000 C CNN
	1    7850 4300
	1    0    0    -1  
$EndComp
$Comp
L TC4428 U3
U 2 1 5C571389
P 7850 5050
F 0 "U3" H 7850 4750 60  0000 C CNN
F 1 "TC4428" H 7850 5350 60  0000 C CNN
F 2 "~" H 7850 5050 60  0000 C CNN
F 3 "~" H 7850 5050 60  0000 C CNN
	2    7850 5050
	1    0    0    -1  
$EndComp
$Comp
L TC4428 U4
U 1 1 5C57138F
P 7850 5850
F 0 "U4" H 7850 5550 60  0000 C CNN
F 1 "TC4428" H 7850 6150 60  0000 C CNN
F 2 "~" H 7850 5850 60  0000 C CNN
F 3 "~" H 7850 5850 60  0000 C CNN
	1    7850 5850
	1    0    0    -1  
$EndComp
$Comp
L TC4428 U4
U 2 1 5C571395
P 7850 6650
F 0 "U4" H 7850 6350 60  0000 C CNN
F 1 "TC4428" H 7850 6950 60  0000 C CNN
F 2 "~" H 7850 6650 60  0000 C CNN
F 3 "~" H 7850 6650 60  0000 C CNN
	2    7850 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 1300 7600 2000
Wire Wire Line
	7600 2850 7600 3600
Wire Wire Line
	7600 4300 7600 5050
Wire Wire Line
	7600 5850 7600 6650
Text Label 7600 1300 2    60   ~ 0
MSel1
Text Label 7600 2850 2    60   ~ 0
MSel2
Text Label 7600 4300 2    60   ~ 0
MSel3
Text Label 7600 5850 2    60   ~ 0
MSel4
Text Label 8250 1300 0    60   ~ 0
Motor1A
Text Label 8250 2000 0    60   ~ 0
Motor1B
Text Label 8250 2850 0    60   ~ 0
Motor2A
Text Label 8250 3600 0    60   ~ 0
Motor2B
Text Label 8250 4300 0    60   ~ 0
Motor3A
Text Label 8250 5050 0    60   ~ 0
Motor3B
Text Label 8250 5850 0    60   ~ 0
Motor4A
Text Label 8250 6650 0    60   ~ 0
Motor4B
$Comp
L +12V #PWR014
U 1 1 5C571477
P 7800 900
F 0 "#PWR014" H 7800 850 20  0001 C CNN
F 1 "+12V" H 7800 1000 30  0000 C CNN
F 2 "" H 7800 900 60  0000 C CNN
F 3 "" H 7800 900 60  0000 C CNN
	1    7800 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 900  7800 1150
Wire Wire Line
	7800 1050 8600 1050
Wire Wire Line
	8600 1050 8600 5600
Wire Wire Line
	8600 2250 7800 2250
Wire Wire Line
	7800 2250 7800 2700
Connection ~ 7800 1050
Wire Wire Line
	8600 4050 7800 4050
Wire Wire Line
	7800 4050 7800 4150
Connection ~ 8600 2250
Wire Wire Line
	8600 5600 7800 5600
Wire Wire Line
	7800 5600 7800 5700
Connection ~ 8600 4050
$Comp
L GND #PWR015
U 1 1 5C57152E
P 7800 7050
F 0 "#PWR015" H 7800 7050 30  0001 C CNN
F 1 "GND" H 7800 6980 30  0001 C CNN
F 2 "" H 7800 7050 60  0000 C CNN
F 3 "" H 7800 7050 60  0000 C CNN
	1    7800 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 7050 7800 6850
Wire Wire Line
	7800 6850 8700 6850
Wire Wire Line
	8700 6850 8700 2550
Wire Wire Line
	8700 5400 7800 5400
Wire Wire Line
	7800 5400 7800 5250
Wire Wire Line
	8700 3950 7800 3950
Wire Wire Line
	7800 3950 7800 3800
Connection ~ 8700 5400
Wire Wire Line
	8700 2550 7950 2550
Wire Wire Line
	7950 2550 7950 2200
Wire Wire Line
	7950 2200 7800 2200
Connection ~ 8700 3950
$Comp
L CONN_5 T1
U 1 1 5C571614
P 10750 1600
F 0 "T1" V 10700 1600 50  0000 C CNN
F 1 "Motor 1" V 10800 1600 50  0000 C CNN
F 2 "" H 10750 1600 60  0000 C CNN
F 3 "" H 10750 1600 60  0000 C CNN
	1    10750 1600
	1    0    0    -1  
$EndComp
$Comp
L CONN_5 T2
U 1 1 5C571623
P 10750 2850
F 0 "T2" V 10700 2850 50  0000 C CNN
F 1 "Motor 2" V 10800 2850 50  0000 C CNN
F 2 "" H 10750 2850 60  0000 C CNN
F 3 "" H 10750 2850 60  0000 C CNN
	1    10750 2850
	1    0    0    -1  
$EndComp
$Comp
L CONN_5 T3
U 1 1 5C571632
P 10750 4050
F 0 "T3" V 10700 4050 50  0000 C CNN
F 1 "Motor 3" V 10800 4050 50  0000 C CNN
F 2 "" H 10750 4050 60  0000 C CNN
F 3 "" H 10750 4050 60  0000 C CNN
	1    10750 4050
	1    0    0    -1  
$EndComp
$Comp
L CONN_5 T4
U 1 1 5C571641
P 10750 5150
F 0 "T4" V 10700 5150 50  0000 C CNN
F 1 "Motor 4" V 10800 5150 50  0000 C CNN
F 2 "" H 10750 5150 60  0000 C CNN
F 3 "" H 10750 5150 60  0000 C CNN
	1    10750 5150
	1    0    0    -1  
$EndComp
Text Label 10350 1400 2    60   ~ 0
Motor1A
Text Label 10350 1500 2    60   ~ 0
Motor1B
Text Label 10350 1600 2    60   ~ 0
Points1A
Text Label 10350 1800 2    60   ~ 0
Points1B
$Comp
L GND #PWR016
U 1 1 5C57167E
P 9550 1800
F 0 "#PWR016" H 9550 1800 30  0001 C CNN
F 1 "GND" H 9550 1730 30  0001 C CNN
F 2 "" H 9550 1800 60  0000 C CNN
F 3 "" H 9550 1800 60  0000 C CNN
	1    9550 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	10350 1700 9550 1700
Wire Wire Line
	9550 1700 9550 1800
Wire Wire Line
	10350 2950 9650 2950
Wire Wire Line
	9650 1700 9650 5250
Connection ~ 9650 1700
Wire Wire Line
	9650 4150 10350 4150
Connection ~ 9650 2950
Wire Wire Line
	9650 5250 10350 5250
Connection ~ 9650 4150
Text Label 10350 2650 2    60   ~ 0
Motor2A
Text Label 10350 2750 2    60   ~ 0
Motor2B
Text Label 10350 2850 2    60   ~ 0
Points2A
Text Label 10350 3050 2    60   ~ 0
Points2B
Text Label 10350 3850 2    60   ~ 0
Motor3A
Text Label 10350 3950 2    60   ~ 0
Motor3B
Text Label 10350 4050 2    60   ~ 0
Points3A
Text Label 10350 4250 2    60   ~ 0
Points3B
Text Label 10350 4950 2    60   ~ 0
Motor4A
Text Label 10350 5050 2    60   ~ 0
Motor4B
Text Label 10350 5150 2    60   ~ 0
Points4A
Text Label 10350 5350 2    60   ~ 0
Points4B
$Comp
L +12V #PWR017
U 1 1 5C5717AA
P 10400 5750
F 0 "#PWR017" H 10400 5700 20  0001 C CNN
F 1 "+12V" H 10400 5850 30  0000 C CNN
F 2 "" H 10400 5750 60  0000 C CNN
F 3 "" H 10400 5750 60  0000 C CNN
	1    10400 5750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR018
U 1 1 5C5717B9
P 10400 6450
F 0 "#PWR018" H 10400 6450 30  0001 C CNN
F 1 "GND" H 10400 6380 30  0001 C CNN
F 2 "" H 10400 6450 60  0000 C CNN
F 3 "" H 10400 6450 60  0000 C CNN
	1    10400 6450
	1    0    0    -1  
$EndComp
$Comp
L CP1 C1
U 1 1 5C571855
P 9500 6150
F 0 "C1" H 9550 6250 50  0000 L CNN
F 1 "10 uf 35V" H 9550 6050 50  0000 L CNN
F 2 "~" H 9500 6150 60  0000 C CNN
F 3 "~" H 9500 6150 60  0000 C CNN
	1    9500 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 5950 10400 5950
Wire Wire Line
	9500 6350 10400 6350
$Comp
L 74HCT00 U5
U 1 1 5C5718FB
P 1800 4600
F 0 "U5" H 1800 4650 60  0000 C CNN
F 1 "74HCT00" H 1800 4500 60  0000 C CNN
F 2 "~" H 1800 4600 60  0000 C CNN
F 3 "~" H 1800 4600 60  0000 C CNN
	1    1800 4600
	1    0    0    -1  
$EndComp
$Comp
L 74HCT00 U5
U 2 1 5C57190A
P 1800 5400
F 0 "U5" H 1800 5450 60  0000 C CNN
F 1 "74HCT00" H 1800 5300 60  0000 C CNN
F 2 "~" H 1800 5400 60  0000 C CNN
F 3 "~" H 1800 5400 60  0000 C CNN
	2    1800 5400
	1    0    0    -1  
$EndComp
$Comp
L 74HCT00 U5
U 3 1 5C571919
P 1800 6200
F 0 "U5" H 1800 6250 60  0000 C CNN
F 1 "74HCT00" H 1800 6100 60  0000 C CNN
F 2 "~" H 1800 6200 60  0000 C CNN
F 3 "~" H 1800 6200 60  0000 C CNN
	3    1800 6200
	1    0    0    -1  
$EndComp
$Comp
L 74HCT00 U5
U 4 1 5C571928
P 1800 7000
F 0 "U5" H 1800 7050 60  0000 C CNN
F 1 "74HCT00" H 1800 6900 60  0000 C CNN
F 2 "~" H 1800 7000 60  0000 C CNN
F 3 "~" H 1800 7000 60  0000 C CNN
	4    1800 7000
	1    0    0    -1  
$EndComp
$Comp
L 74HCT00 U6
U 1 1 5C571937
P 4500 4600
F 0 "U6" H 4500 4650 60  0000 C CNN
F 1 "74HCT00" H 4500 4500 60  0000 C CNN
F 2 "~" H 4500 4600 60  0000 C CNN
F 3 "~" H 4500 4600 60  0000 C CNN
	1    4500 4600
	1    0    0    -1  
$EndComp
$Comp
L 74HCT00 U6
U 2 1 5C571946
P 4500 5400
F 0 "U6" H 4500 5450 60  0000 C CNN
F 1 "74HCT00" H 4500 5300 60  0000 C CNN
F 2 "~" H 4500 5400 60  0000 C CNN
F 3 "~" H 4500 5400 60  0000 C CNN
	2    4500 5400
	1    0    0    -1  
$EndComp
$Comp
L 74HCT00 U6
U 3 1 5C571955
P 4500 6200
F 0 "U6" H 4500 6250 60  0000 C CNN
F 1 "74HCT00" H 4500 6100 60  0000 C CNN
F 2 "~" H 4500 6200 60  0000 C CNN
F 3 "~" H 4500 6200 60  0000 C CNN
	3    4500 6200
	1    0    0    -1  
$EndComp
$Comp
L 74HCT00 U6
U 4 1 5C571964
P 4500 7000
F 0 "U6" H 4500 7050 60  0000 C CNN
F 1 "74HCT00" H 4500 6900 60  0000 C CNN
F 2 "~" H 4500 7000 60  0000 C CNN
F 3 "~" H 4500 7000 60  0000 C CNN
	4    4500 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 4700 1200 5100
Wire Wire Line
	1200 5100 2400 5100
Wire Wire Line
	2400 5100 2400 5400
Wire Wire Line
	2400 4600 2400 4850
Wire Wire Line
	2400 4850 1050 4850
Wire Wire Line
	1050 4850 1050 5300
Wire Wire Line
	1050 5300 1200 5300
Wire Wire Line
	2400 6200 2400 6700
Wire Wire Line
	2400 6700 1200 6700
Wire Wire Line
	1200 6700 1200 6900
Wire Wire Line
	1200 6300 1200 6600
Wire Wire Line
	1200 6600 2500 6600
Wire Wire Line
	2500 6600 2500 7000
Wire Wire Line
	2500 7000 2400 7000
Wire Wire Line
	5100 6200 5100 6650
Wire Wire Line
	5100 6650 3900 6650
Wire Wire Line
	3900 6650 3900 6900
Wire Wire Line
	5100 7000 5250 7000
Wire Wire Line
	5250 7000 5250 6550
Wire Wire Line
	5250 6550 3900 6550
Wire Wire Line
	3900 6550 3900 6300
Wire Wire Line
	5100 4600 5100 4950
Wire Wire Line
	5100 4950 3900 4950
Wire Wire Line
	3900 4950 3900 5300
Wire Wire Line
	5100 5400 5100 5050
Wire Wire Line
	5100 5050 3800 5050
Wire Wire Line
	3800 5050 3800 4700
Wire Wire Line
	3800 4700 3900 4700
$Comp
L +3V3 #PWR019
U 1 1 5C571D03
P 1600 4200
F 0 "#PWR019" H 1600 4050 50  0001 C CNN
F 1 "+3V3" H 1600 4340 50  0000 C CNN
F 2 "" H 1600 4200 60  0000 C CNN
F 3 "" H 1600 4200 60  0000 C CNN
	1    1600 4200
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR020
U 1 1 5C571D12
P 4300 4150
F 0 "#PWR020" H 4300 4000 50  0001 C CNN
F 1 "+3V3" H 4300 4290 50  0000 C CNN
F 2 "" H 4300 4150 60  0000 C CNN
F 3 "" H 4300 4150 60  0000 C CNN
	1    4300 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 4150 4300 4400
Wire Wire Line
	1600 4200 1600 4400
$Comp
L C C2
U 1 1 5C571DF2
P 2750 4150
F 0 "C2" H 2750 4250 40  0000 L CNN
F 1 ".1 uf" H 2756 4065 40  0000 L CNN
F 2 "~" H 2788 4000 30  0000 C CNN
F 3 "~" H 2750 4150 60  0000 C CNN
	1    2750 4150
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5C571E01
P 3300 4150
F 0 "C3" H 3300 4250 40  0000 L CNN
F 1 ".1 uf" H 3306 4065 40  0000 L CNN
F 2 "~" H 3338 4000 30  0000 C CNN
F 3 "~" H 3300 4150 60  0000 C CNN
	1    3300 4150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR021
U 1 1 5C571E21
P 1600 7550
F 0 "#PWR021" H 1600 7550 30  0001 C CNN
F 1 "GND" H 1600 7480 30  0001 C CNN
F 2 "" H 1600 7550 60  0000 C CNN
F 3 "" H 1600 7550 60  0000 C CNN
	1    1600 7550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR022
U 1 1 5C571E30
P 4300 7400
F 0 "#PWR022" H 4300 7400 30  0001 C CNN
F 1 "GND" H 4300 7330 30  0001 C CNN
F 2 "" H 4300 7400 60  0000 C CNN
F 3 "" H 4300 7400 60  0000 C CNN
	1    4300 7400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 7200 4300 7400
Wire Wire Line
	1600 7200 1600 7550
Wire Wire Line
	2100 3950 3300 3950
Wire Wire Line
	2100 3950 2100 4300
Wire Wire Line
	2100 4300 1600 4300
Connection ~ 1600 4300
Connection ~ 2750 3950
Wire Wire Line
	2850 4050 3050 4050
Wire Wire Line
	3050 4050 3050 4500
Wire Wire Line
	2750 4350 3300 4350
Connection ~ 2850 3550
Connection ~ 3050 4350
Text Label 2400 4600 0    60   ~ 0
PSense1
Text Label 2400 6200 0    60   ~ 0
PSense2
Text Label 5100 4600 0    60   ~ 0
PSense3
Text Label 5100 6200 0    60   ~ 0
PSense4
Text Label 1200 4500 2    60   ~ 0
Points1A
Text Label 1200 5500 2    60   ~ 0
Points1B
Text Label 1200 6100 2    60   ~ 0
Points2A
Text Label 1200 7100 2    60   ~ 0
Points2B
Text Label 3900 4500 2    60   ~ 0
Points3A
Text Label 3900 5500 2    60   ~ 0
Points3B
Text Label 3900 6100 2    60   ~ 0
Points4A
Text Label 3900 7100 2    60   ~ 0
Points4B
$Comp
L RR8 RR1
U 1 1 5C572142
P 6500 6300
F 0 "RR1" H 6550 6850 70  0000 C CNN
F 1 "10K Ohms" V 6530 6300 70  0000 C CNN
F 2 "~" H 6500 6300 60  0000 C CNN
F 3 "~" H 6500 6300 60  0000 C CNN
	1    6500 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 4250 6150 4250
Wire Wire Line
	6150 4250 6150 5850
Connection ~ 4300 4250
Text Label 6150 5950 2    60   ~ 0
Points1A
Text Label 6150 6050 2    60   ~ 0
Points1B
Text Label 6150 6150 2    60   ~ 0
Points2A
Text Label 6150 6250 2    60   ~ 0
Points2B
Text Label 6150 6350 2    60   ~ 0
Points3A
Text Label 6150 6450 2    60   ~ 0
Points3B
Text Label 6150 6550 2    60   ~ 0
Points4A
Text Label 6150 6650 2    60   ~ 0
Points4B
$Sheet
S 3450 1900 550  650 
U 5C572C55
F0 "CAN Bus Driver" 50
F1 "CANBUSDriver.sch" 50
F2 "CANTX" I L 3450 2150 60 
F3 "CANRX" O L 3450 2000 60 
$EndSheet
Wire Wire Line
	10400 5950 10400 5750
Wire Wire Line
	10400 6350 10400 6450
Text Label 2700 3150 0    60   ~ 0
MSel1
Text Label 4900 1250 2    60   ~ 0
MSel2
Text Label 2700 1650 0    60   ~ 0
PSense1
Text Label 5400 1250 0    60   ~ 0
PSense2
Wire Wire Line
	3450 2000 2900 2000
Wire Wire Line
	2900 2000 2900 2250
Wire Wire Line
	2900 2250 2700 2250
Wire Wire Line
	3450 2150 3000 2150
Wire Wire Line
	3000 2150 3000 2350
Wire Wire Line
	3000 2350 2700 2350
$Sheet
S 800  2100 700  750 
U 5C573FEC
F0 "Cape EEPROM" 50
F1 "CapeEEPROM.sch" 50
F2 "SCL" I R 1500 2250 60 
F3 "SDA" B R 1500 2500 60 
$EndSheet
Wire Wire Line
	2200 2050 1700 2050
Wire Wire Line
	1700 2050 1700 2250
Wire Wire Line
	1700 2250 1500 2250
Wire Wire Line
	2700 2100 1850 2100
Wire Wire Line
	1850 2100 1850 2500
Wire Wire Line
	1850 2500 1500 2500
Wire Wire Line
	2700 2050 2700 2100
Text Label 2200 1850 2    60   ~ 0
MSel3
Text Label 2200 2250 2    60   ~ 0
MSel4
Text Label 2200 2350 2    60   ~ 0
PSense3
Text Label 2200 2450 2    60   ~ 0
PSense4
NoConn ~ 2200 1650
NoConn ~ 2200 1750
NoConn ~ 2200 1950
NoConn ~ 2200 2150
NoConn ~ 2200 2550
NoConn ~ 2200 2650
NoConn ~ 2200 2750
NoConn ~ 2200 3150
NoConn ~ 2200 3050
NoConn ~ 2200 2950
NoConn ~ 2200 2850
NoConn ~ 2700 3050
NoConn ~ 2700 2950
NoConn ~ 2700 2850
NoConn ~ 2700 2550
NoConn ~ 2700 2450
NoConn ~ 2700 2150
NoConn ~ 2700 1750
NoConn ~ 2700 1850
NoConn ~ 2700 1950
NoConn ~ 4900 1350
NoConn ~ 4900 1450
NoConn ~ 4900 1550
NoConn ~ 4900 1650
NoConn ~ 4900 1750
NoConn ~ 4900 1850
NoConn ~ 4900 1950
NoConn ~ 4900 2050
NoConn ~ 4900 2150
NoConn ~ 4900 2250
NoConn ~ 4900 2350
NoConn ~ 4900 2450
NoConn ~ 4900 2550
NoConn ~ 4900 2650
NoConn ~ 4900 2750
NoConn ~ 4900 2850
NoConn ~ 4900 2950
NoConn ~ 4900 3050
NoConn ~ 5400 3050
NoConn ~ 5400 2950
NoConn ~ 5400 2850
NoConn ~ 5400 2750
NoConn ~ 5400 2650
NoConn ~ 5400 2550
NoConn ~ 5400 2450
NoConn ~ 5400 2350
NoConn ~ 5400 2250
NoConn ~ 5400 2150
NoConn ~ 5400 2050
NoConn ~ 5400 1950
NoConn ~ 5400 1850
NoConn ~ 5400 1750
NoConn ~ 5400 1650
NoConn ~ 5400 1550
NoConn ~ 5400 1450
NoConn ~ 5400 1350
NoConn ~ 5400 3150
NoConn ~ 5400 3250
NoConn ~ 5400 3350
NoConn ~ 4900 3350
NoConn ~ 4900 3250
NoConn ~ 4900 3150
$Comp
L +3.3V #PWR023
U 1 1 5C574DC7
P 5000 4100
F 0 "#PWR023" H 5000 4060 30  0001 C CNN
F 1 "+3.3V" H 5000 4210 30  0000 C CNN
F 2 "" H 5000 4100 60  0000 C CNN
F 3 "" H 5000 4100 60  0000 C CNN
	1    5000 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 4100 5000 4250
Connection ~ 5000 4250
$Comp
L GND #PWR024
U 1 1 5C575F92
P 3050 4500
F 0 "#PWR024" H 3050 4500 30  0001 C CNN
F 1 "GND" H 3050 4430 30  0001 C CNN
F 2 "" H 3050 4500 60  0000 C CNN
F 3 "" H 3050 4500 60  0000 C CNN
	1    3050 4500
	1    0    0    -1  
$EndComp
$EndSCHEMATC