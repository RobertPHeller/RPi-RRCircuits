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
LIBS:tc442x
LIBS:QuadSMCSenseHat-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "11 mar 2019"
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
P 1450 1150
F 0 "J0" H 2200 1400 60  0000 C CNN
F 1 "RPi_GPIO" H 2200 1300 60  0000 C CNN
F 2 "RPi_Hat:Pin_Header_Straight_2x20" H 1450 1150 60  0001 C CNN
F 3 "" H 1450 1150 60  0000 C CNN
F 4 "855-M20-6102045" H 1450 1150 60  0001 C CNN "Mouser Part Number"
	1    1450 1150
	1    0    0    -1  
$EndComp
$Comp
L TC4428 U1
U 1 1 5C560185
P 4400 1600
F 0 "U1" H 4400 1300 60  0000 C CNN
F 1 "TC4428" H 4400 1900 60  0000 C CNN
F 2 "~" H 4400 1600 60  0000 C CNN
F 3 "~" H 4400 1600 60  0000 C CNN
F 4 "579-TC4428VPA" H 4400 1600 60  0001 C CNN "Mouser Part Number"
	1    4400 1600
	1    0    0    -1  
$EndComp
$Comp
L TC4428 U1
U 2 1 5C560194
P 4400 2400
F 0 "U1" H 4400 2100 60  0000 C CNN
F 1 "TC4428" H 4400 2700 60  0000 C CNN
F 2 "~" H 4400 2400 60  0000 C CNN
F 3 "~" H 4400 2400 60  0000 C CNN
	2    4400 2400
	1    0    0    -1  
$EndComp
$Comp
L TC4428 U2
U 1 1 5C5601A3
P 4400 3300
F 0 "U2" H 4400 3000 60  0000 C CNN
F 1 "TC4428" H 4400 3600 60  0000 C CNN
F 2 "~" H 4400 3300 60  0000 C CNN
F 3 "~" H 4400 3300 60  0000 C CNN
F 4 "579-TC4428VPA" H 4400 3300 60  0001 C CNN "Mouser Part Number"
	1    4400 3300
	1    0    0    -1  
$EndComp
$Comp
L TC4428 U2
U 2 1 5C5601B2
P 4400 4200
F 0 "U2" H 4400 3900 60  0000 C CNN
F 1 "TC4428" H 4400 4500 60  0000 C CNN
F 2 "~" H 4400 4200 60  0000 C CNN
F 3 "~" H 4400 4200 60  0000 C CNN
	2    4400 4200
	1    0    0    -1  
$EndComp
$Comp
L TC4428 U3
U 1 1 5C5601C1
P 6550 1600
F 0 "U3" H 6550 1300 60  0000 C CNN
F 1 "TC4428" H 6550 1900 60  0000 C CNN
F 2 "~" H 6550 1600 60  0000 C CNN
F 3 "~" H 6550 1600 60  0000 C CNN
F 4 "579-TC4428VPA" H 6550 1600 60  0001 C CNN "Mouser Part Number"
	1    6550 1600
	1    0    0    -1  
$EndComp
$Comp
L TC4428 U3
U 2 1 5C5601D0
P 6550 2400
F 0 "U3" H 6550 2100 60  0000 C CNN
F 1 "TC4428" H 6550 2700 60  0000 C CNN
F 2 "~" H 6550 2400 60  0000 C CNN
F 3 "~" H 6550 2400 60  0000 C CNN
	2    6550 2400
	1    0    0    -1  
$EndComp
$Comp
L TC4428 U4
U 1 1 5C5601DF
P 6550 3300
F 0 "U4" H 6550 3000 60  0000 C CNN
F 1 "TC4428" H 6550 3600 60  0000 C CNN
F 2 "~" H 6550 3300 60  0000 C CNN
F 3 "~" H 6550 3300 60  0000 C CNN
F 4 "579-TC4428VPA" H 6550 3300 60  0001 C CNN "Mouser Part Number"
	1    6550 3300
	1    0    0    -1  
$EndComp
$Comp
L TC4428 U4
U 2 1 5C5601EE
P 6550 4200
F 0 "U4" H 6550 3900 60  0000 C CNN
F 1 "TC4428" H 6550 4500 60  0000 C CNN
F 2 "~" H 6550 4200 60  0000 C CNN
F 3 "~" H 6550 4200 60  0000 C CNN
	2    6550 4200
	1    0    0    -1  
$EndComp
Text Label 1250 1650 2    30   ~ 0
MSel1
Text Label 1250 1450 2    30   ~ 0
PSense4
Text Label 1250 1750 2    30   ~ 0
PSense1
Text Label 1250 1850 2    30   ~ 0
PSense2
Text Label 3150 1650 0    30   ~ 0
MSel2
Text Label 3150 1850 0    30   ~ 0
MSel3
Text Label 3150 1950 0    30   ~ 0
MSel4
Text Label 3150 2150 0    30   ~ 0
PSense3
Wire Wire Line
	4150 1600 4150 2400
Wire Wire Line
	4150 3300 4150 4200
Wire Wire Line
	6300 4200 6300 3300
Wire Wire Line
	6300 2400 6300 1600
Text Label 4150 1600 2    30   ~ 0
MSel1
Text Label 4150 3300 2    30   ~ 0
MSel2
Text Label 6300 1600 2    30   ~ 0
MSel3
Text Label 6300 3300 2    30   ~ 0
MSel4
Text Label 4800 1600 0    30   ~ 0
Motor1A
Text Label 4800 2400 0    30   ~ 0
Motor1B
$Comp
L GND #PWR01
U 1 1 5C560455
P 4350 1800
F 0 "#PWR01" H 4350 1800 30  0001 C CNN
F 1 "GND" H 4350 1730 30  0001 C CNN
F 2 "" H 4350 1800 60  0000 C CNN
F 3 "" H 4350 1800 60  0000 C CNN
	1    4350 1800
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR02
U 1 1 5C560495
P 4350 1450
F 0 "#PWR02" H 4350 1400 20  0001 C CNN
F 1 "+12V" H 4350 1550 30  0000 C CNN
F 2 "" H 4350 1450 60  0000 C CNN
F 3 "" H 4350 1450 60  0000 C CNN
	1    4350 1450
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR03
U 1 1 5C5604A4
P 6500 1450
F 0 "#PWR03" H 6500 1400 20  0001 C CNN
F 1 "+12V" H 6500 1550 30  0000 C CNN
F 2 "" H 6500 1450 60  0000 C CNN
F 3 "" H 6500 1450 60  0000 C CNN
	1    6500 1450
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR04
U 1 1 5C5604B3
P 4350 3150
F 0 "#PWR04" H 4350 3100 20  0001 C CNN
F 1 "+12V" H 4350 3250 30  0000 C CNN
F 2 "" H 4350 3150 60  0000 C CNN
F 3 "" H 4350 3150 60  0000 C CNN
	1    4350 3150
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR05
U 1 1 5C5604C2
P 6500 3150
F 0 "#PWR05" H 6500 3100 20  0001 C CNN
F 1 "+12V" H 6500 3250 30  0000 C CNN
F 2 "" H 6500 3150 60  0000 C CNN
F 3 "" H 6500 3150 60  0000 C CNN
	1    6500 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 5C5604D1
P 6500 1800
F 0 "#PWR06" H 6500 1800 30  0001 C CNN
F 1 "GND" H 6500 1730 30  0001 C CNN
F 2 "" H 6500 1800 60  0000 C CNN
F 3 "" H 6500 1800 60  0000 C CNN
	1    6500 1800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR07
U 1 1 5C5604E0
P 4350 3500
F 0 "#PWR07" H 4350 3500 30  0001 C CNN
F 1 "GND" H 4350 3430 30  0001 C CNN
F 2 "" H 4350 3500 60  0000 C CNN
F 3 "" H 4350 3500 60  0000 C CNN
	1    4350 3500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 5C5604EF
P 6500 3500
F 0 "#PWR08" H 6500 3500 30  0001 C CNN
F 1 "GND" H 6500 3430 30  0001 C CNN
F 2 "" H 6500 3500 60  0000 C CNN
F 3 "" H 6500 3500 60  0000 C CNN
	1    6500 3500
	1    0    0    -1  
$EndComp
Text Label 4800 3300 0    30   ~ 0
Motor2A
Text Label 4800 4200 0    30   ~ 0
Motor2B
Text Label 6950 1600 0    30   ~ 0
Motor3A
Text Label 6950 2400 0    30   ~ 0
Motor3B
Text Label 6950 3300 0    30   ~ 0
Motor4A
Text Label 6950 4200 0    30   ~ 0
Motor4B
$Comp
L 74HCT00 U5
U 1 1 5C560538
P 1900 4000
F 0 "U5" H 1900 4050 60  0000 C CNN
F 1 "74HCT00" H 1900 3900 60  0000 C CNN
F 2 "~" H 1900 4000 60  0000 C CNN
F 3 "~" H 1900 4000 60  0000 C CNN
F 4 "595-SN74AHC00N" H 1900 4000 60  0001 C CNN "Mouser Part Number"
	1    1900 4000
	1    0    0    -1  
$EndComp
$Comp
L 74HCT00 U5
U 2 1 5C560547
P 1900 4800
F 0 "U5" H 1900 4850 60  0000 C CNN
F 1 "74HCT00" H 1900 4700 60  0000 C CNN
F 2 "~" H 1900 4800 60  0000 C CNN
F 3 "~" H 1900 4800 60  0000 C CNN
	2    1900 4800
	1    0    0    -1  
$EndComp
$Comp
L 74HCT00 U5
U 3 1 5C560556
P 1900 5700
F 0 "U5" H 1900 5750 60  0000 C CNN
F 1 "74HCT00" H 1900 5600 60  0000 C CNN
F 2 "~" H 1900 5700 60  0000 C CNN
F 3 "~" H 1900 5700 60  0000 C CNN
	3    1900 5700
	1    0    0    -1  
$EndComp
$Comp
L 74HCT00 U5
U 4 1 5C560565
P 1900 6600
F 0 "U5" H 1900 6650 60  0000 C CNN
F 1 "74HCT00" H 1900 6500 60  0000 C CNN
F 2 "~" H 1900 6600 60  0000 C CNN
F 3 "~" H 1900 6600 60  0000 C CNN
	4    1900 6600
	1    0    0    -1  
$EndComp
$Comp
L 74HCT00 U6
U 1 1 5C560574
P 8500 1600
F 0 "U6" H 8500 1650 60  0000 C CNN
F 1 "74HCT00" H 8500 1500 60  0000 C CNN
F 2 "~" H 8500 1600 60  0000 C CNN
F 3 "~" H 8500 1600 60  0000 C CNN
F 4 "595-SN74AHC00N" H 8500 1600 60  0001 C CNN "Mouser Part Number"
	1    8500 1600
	1    0    0    -1  
$EndComp
$Comp
L 74HCT00 U6
U 2 1 5C560583
P 8500 2500
F 0 "U6" H 8500 2550 60  0000 C CNN
F 1 "74HCT00" H 8500 2400 60  0000 C CNN
F 2 "~" H 8500 2500 60  0000 C CNN
F 3 "~" H 8500 2500 60  0000 C CNN
	2    8500 2500
	1    0    0    -1  
$EndComp
$Comp
L 74HCT00 U6
U 3 1 5C560592
P 8500 3400
F 0 "U6" H 8500 3450 60  0000 C CNN
F 1 "74HCT00" H 8500 3300 60  0000 C CNN
F 2 "~" H 8500 3400 60  0000 C CNN
F 3 "~" H 8500 3400 60  0000 C CNN
	3    8500 3400
	1    0    0    -1  
$EndComp
$Comp
L 74HCT00 U6
U 4 1 5C5605A1
P 8500 4300
F 0 "U6" H 8500 4350 60  0000 C CNN
F 1 "74HCT00" H 8500 4200 60  0000 C CNN
F 2 "~" H 8500 4300 60  0000 C CNN
F 3 "~" H 8500 4300 60  0000 C CNN
	4    8500 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 4000 2500 4450
Wire Wire Line
	2500 4450 1300 4450
Wire Wire Line
	1300 4450 1300 4700
Wire Wire Line
	1300 4100 1300 4350
Wire Wire Line
	1300 4350 2600 4350
Wire Wire Line
	2600 4350 2600 4800
Wire Wire Line
	2600 4800 2500 4800
Wire Wire Line
	2500 5700 2500 6100
Wire Wire Line
	2500 6100 1300 6100
Wire Wire Line
	1300 6100 1300 6500
Wire Wire Line
	1300 5800 1300 6000
Wire Wire Line
	1300 6000 2650 6000
Wire Wire Line
	2650 6000 2650 6600
Wire Wire Line
	2650 6600 2500 6600
Wire Wire Line
	9100 1600 9100 1950
Wire Wire Line
	9100 1950 7900 1950
Wire Wire Line
	7900 1950 7900 2400
Wire Wire Line
	7900 1700 7900 1900
Wire Wire Line
	7900 1900 9250 1900
Wire Wire Line
	9250 1900 9250 2500
Wire Wire Line
	9250 2500 9100 2500
Wire Wire Line
	9100 3400 9100 3900
Wire Wire Line
	9100 3900 7900 3900
Wire Wire Line
	7900 3900 7900 4200
Wire Wire Line
	7900 3500 7900 3800
Wire Wire Line
	7900 3800 9200 3800
Wire Wire Line
	9200 3800 9200 4300
Wire Wire Line
	9200 4300 9100 4300
$Comp
L RR8 RR1
U 1 1 5C560645
P 4400 5400
F 0 "RR1" H 4450 5950 70  0000 C CNN
F 1 "10K Ohms" V 4430 5400 70  0000 C CNN
F 2 "~" H 4400 5400 60  0000 C CNN
F 3 "~" H 4400 5400 60  0000 C CNN
F 4 "652-4609X-1LF-10K" H 4400 5400 60  0001 C CNN "Mouser Part Number"
	1    4400 5400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 5C560654
P 1700 5100
F 0 "#PWR09" H 1700 5100 30  0001 C CNN
F 1 "GND" H 1700 5030 30  0001 C CNN
F 2 "" H 1700 5100 60  0000 C CNN
F 3 "" H 1700 5100 60  0000 C CNN
	1    1700 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 5C560663
P 8300 4650
F 0 "#PWR010" H 8300 4650 30  0001 C CNN
F 1 "GND" H 8300 4580 30  0001 C CNN
F 2 "" H 8300 4650 60  0000 C CNN
F 3 "" H 8300 4650 60  0000 C CNN
	1    8300 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 4500 8300 4650
Wire Wire Line
	1700 5000 1700 5100
$Comp
L +3.3V #PWR011
U 1 1 5C5606D8
P 1700 3600
F 0 "#PWR011" H 1700 3560 30  0001 C CNN
F 1 "+3.3V" H 1700 3710 30  0000 C CNN
F 2 "" H 1700 3600 60  0000 C CNN
F 3 "" H 1700 3600 60  0000 C CNN
	1    1700 3600
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR012
U 1 1 5C5606E7
P 4050 4750
F 0 "#PWR012" H 4050 4710 30  0001 C CNN
F 1 "+3.3V" H 4050 4860 30  0000 C CNN
F 2 "" H 4050 4750 60  0000 C CNN
F 3 "" H 4050 4750 60  0000 C CNN
	1    4050 4750
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR013
U 1 1 5C5606F6
P 8300 1200
F 0 "#PWR013" H 8300 1160 30  0001 C CNN
F 1 "+3.3V" H 8300 1310 30  0000 C CNN
F 2 "" H 8300 1200 60  0000 C CNN
F 3 "" H 8300 1200 60  0000 C CNN
	1    8300 1200
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR014
U 1 1 5C560705
P 1200 1000
F 0 "#PWR014" H 1200 960 30  0001 C CNN
F 1 "+3.3V" H 1200 1110 30  0000 C CNN
F 2 "" H 1200 1000 60  0000 C CNN
F 3 "" H 1200 1000 60  0000 C CNN
	1    1200 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 1000 1200 1950
Wire Wire Line
	1200 1150 1250 1150
Wire Wire Line
	1200 1950 1250 1950
Connection ~ 1200 1150
$Comp
L GND #PWR015
U 1 1 5C56075D
P 1000 1550
F 0 "#PWR015" H 1000 1550 30  0001 C CNN
F 1 "GND" H 1000 1480 30  0001 C CNN
F 2 "" H 1000 1550 60  0000 C CNN
F 3 "" H 1000 1550 60  0000 C CNN
	1    1000 1550
	0    1    1    0   
$EndComp
Wire Wire Line
	1250 1550 1000 1550
Wire Wire Line
	1250 2350 1100 2350
Wire Wire Line
	1100 1550 1100 3250
Connection ~ 1100 1550
Wire Wire Line
	1100 3050 1250 3050
Connection ~ 1100 2350
Wire Wire Line
	3150 2750 3250 2750
Wire Wire Line
	3250 1350 3250 3250
Wire Wire Line
	3250 3250 1100 3250
Connection ~ 1100 3050
Wire Wire Line
	3150 2550 3250 2550
Connection ~ 3250 2750
Wire Wire Line
	3150 2050 3250 2050
Connection ~ 3250 2550
Wire Wire Line
	3250 1750 3150 1750
Connection ~ 3250 2050
Wire Wire Line
	3250 1350 3150 1350
Connection ~ 3250 1750
Wire Wire Line
	1700 3600 1700 3800
Wire Wire Line
	8300 1400 8300 1200
Text Label 1300 3900 2    30   ~ 0
Points1A
Text Label 1300 4900 2    30   ~ 0
Points1B
Text Label 2500 4000 0    30   ~ 0
PSense1
Text Label 2500 5700 0    30   ~ 0
PSense2
Text Label 1300 5600 2    30   ~ 0
Points2A
Text Label 1300 6700 2    30   ~ 0
Points2B
Text Label 7900 1500 2    30   ~ 0
Points3A
Text Label 7900 2600 2    30   ~ 0
Points3B
Text Label 7900 3300 2    30   ~ 0
Points4A
Text Label 7900 4400 2    30   ~ 0
Points4B
Text Label 9100 1600 0    30   ~ 0
PSense3
Text Label 9100 3400 0    30   ~ 0
PSense4
Wire Wire Line
	4050 4750 4050 4950
Text Label 4050 5050 2    30   ~ 0
Points1A
Text Label 4050 5150 2    30   ~ 0
Points1B
Text Label 4050 5250 2    30   ~ 0
Points2A
Text Label 4050 5350 2    30   ~ 0
Points2B
Text Label 4050 5450 2    30   ~ 0
Points3A
Text Label 4050 5550 2    30   ~ 0
Points3B
Text Label 4050 5650 2    30   ~ 0
Points4A
Text Label 4050 5750 2    30   ~ 0
Points4B
$Comp
L CONN_5 T1
U 1 1 5C560A1F
P 10750 2000
F 0 "T1" V 10700 2000 50  0000 C CNN
F 1 "Motor 1" V 10800 2000 50  0000 C CNN
F 2 "~" H 10750 2000 60  0000 C CNN
F 3 "~" H 10750 2000 60  0000 C CNN
F 4 "651-1725685" H 10750 2000 60  0001 C CNN "Mouser Part Number"
	1    10750 2000
	1    0    0    -1  
$EndComp
$Comp
L CONN_5 T2
U 1 1 5C560A2E
P 10750 2850
F 0 "T2" V 10700 2850 50  0000 C CNN
F 1 "Motor 2" V 10800 2850 50  0000 C CNN
F 2 "~" H 10750 2850 60  0000 C CNN
F 3 "~" H 10750 2850 60  0000 C CNN
F 4 "651-1725685 " H 10750 2850 60  0001 C CNN "Mouser Part Number"
	1    10750 2850
	1    0    0    -1  
$EndComp
$Comp
L CONN_5 T3
U 1 1 5C560A3D
P 10750 3700
F 0 "T3" V 10700 3700 50  0000 C CNN
F 1 "Motor 3" V 10800 3700 50  0000 C CNN
F 2 "~" H 10750 3700 60  0000 C CNN
F 3 "~" H 10750 3700 60  0000 C CNN
F 4 "651-1725685" H 10750 3700 60  0001 C CNN "Mouser Part Number"
	1    10750 3700
	1    0    0    -1  
$EndComp
$Comp
L CONN_5 T4
U 1 1 5C560A4C
P 10750 4650
F 0 "T4" V 10700 4650 50  0000 C CNN
F 1 "Motor 4" V 10800 4650 50  0000 C CNN
F 2 "~" H 10750 4650 60  0000 C CNN
F 3 "~" H 10750 4650 60  0000 C CNN
F 4 "651-1725685 " H 10750 4650 60  0001 C CNN "Mouser Part Number"
	1    10750 4650
	1    0    0    -1  
$EndComp
Text Label 10350 1800 2    30   ~ 0
Motor1A
Text Label 10350 1900 2    30   ~ 0
Motor1B
Text Label 10350 2000 2    30   ~ 0
Points1A
Text Label 10350 2200 2    30   ~ 0
Points1B
Text Label 10350 2650 2    30   ~ 0
Motor2A
Text Label 10350 2750 2    30   ~ 0
Motor2B
Text Label 10350 2850 2    30   ~ 0
Points2A
Text Label 10350 3050 2    30   ~ 0
Points2B
Text Label 10350 3500 2    30   ~ 0
Motor3A
Text Label 10350 3600 2    30   ~ 0
Motor3B
Text Label 10350 3700 2    30   ~ 0
Points3A
Text Label 10350 3900 2    30   ~ 0
Points3B
Text Label 10350 4450 2    30   ~ 0
Motor4A
Text Label 10350 4550 2    30   ~ 0
Motor4B
Text Label 10350 4650 2    30   ~ 0
Points4A
Text Label 10350 4850 2    30   ~ 0
Points4B
$Comp
L GND #PWR016
U 1 1 5C560AAC
P 10200 5000
F 0 "#PWR016" H 10200 5000 30  0001 C CNN
F 1 "GND" H 10200 4930 30  0001 C CNN
F 2 "" H 10200 5000 60  0000 C CNN
F 3 "" H 10200 5000 60  0000 C CNN
	1    10200 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 2100 10200 5000
Wire Wire Line
	10200 4750 10350 4750
Wire Wire Line
	10200 3800 10350 3800
Connection ~ 10200 4750
Wire Wire Line
	10200 2950 10350 2950
Connection ~ 10200 3800
Wire Wire Line
	10200 2100 10350 2100
Connection ~ 10200 2950
$Comp
L CONN_2 T5
U 1 1 5C560C0A
P 10750 5550
F 0 "T5" V 10700 5550 40  0000 C CNN
F 1 "+ 12V -" V 10800 5550 40  0000 C CNN
F 2 "~" H 10750 5550 60  0000 C CNN
F 3 "~" H 10750 5550 60  0000 C CNN
F 4 "651-1725656" H 10750 5550 60  0001 C CNN "Mouser Part Number"
	1    10750 5550
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR017
U 1 1 5C560C3F
P 10400 5350
F 0 "#PWR017" H 10400 5300 20  0001 C CNN
F 1 "+12V" H 10400 5450 30  0000 C CNN
F 2 "" H 10400 5350 60  0000 C CNN
F 3 "" H 10400 5350 60  0000 C CNN
	1    10400 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	10400 5350 10400 5450
Wire Wire Line
	10200 4900 10300 4900
Wire Wire Line
	10300 4900 10300 5650
Wire Wire Line
	10300 5650 10400 5650
Connection ~ 10200 4900
$Comp
L C C1
U 1 1 5C57081B
P 6300 5300
F 0 "C1" H 6300 5400 40  0000 L CNN
F 1 ".1 uf" H 6306 5215 40  0000 L CNN
F 2 "~" H 6338 5150 30  0000 C CNN
F 3 "~" H 6300 5300 60  0000 C CNN
F 4 "581-SR201C104KARTR1" H 6300 5300 60  0001 C CNN "Mouser Part Number"
	1    6300 5300
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 5C57082A
P 6700 5300
F 0 "C2" H 6700 5400 40  0000 L CNN
F 1 ".1 uf" H 6706 5215 40  0000 L CNN
F 2 "~" H 6738 5150 30  0000 C CNN
F 3 "~" H 6700 5300 60  0000 C CNN
F 4 "581-SR201C104KARTR1" H 6700 5300 60  0001 C CNN "Mouser Part Number"
	1    6700 5300
	1    0    0    -1  
$EndComp
$Comp
L CP1 C3
U 1 1 5C570839
P 7400 5300
F 0 "C3" H 7450 5400 50  0000 L CNN
F 1 "10 uf 35V" H 7450 5200 50  0000 L CNN
F 2 "~" H 7400 5300 60  0000 C CNN
F 3 "~" H 7400 5300 60  0000 C CNN
F 4 "667-ECA-1HM100I" H 7400 5300 60  0001 C CNN "Mouser Part Number"
	1    7400 5300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR018
U 1 1 5C570848
P 6300 4950
F 0 "#PWR018" H 6300 4910 30  0001 C CNN
F 1 "+3.3V" H 6300 5060 30  0000 C CNN
F 2 "" H 6300 4950 60  0000 C CNN
F 3 "" H 6300 4950 60  0000 C CNN
	1    6300 4950
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR019
U 1 1 5C570857
P 7400 4950
F 0 "#PWR019" H 7400 4900 20  0001 C CNN
F 1 "+12V" H 7400 5050 30  0000 C CNN
F 2 "" H 7400 4950 60  0000 C CNN
F 3 "" H 7400 4950 60  0000 C CNN
	1    7400 4950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR020
U 1 1 5C57087C
P 6300 5700
F 0 "#PWR020" H 6300 5700 30  0001 C CNN
F 1 "GND" H 6300 5630 30  0001 C CNN
F 2 "" H 6300 5700 60  0000 C CNN
F 3 "" H 6300 5700 60  0000 C CNN
	1    6300 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 4950 6300 5100
Wire Wire Line
	6300 5100 6700 5100
Wire Wire Line
	6300 5500 6300 5700
Wire Wire Line
	6300 5500 7400 5500
Connection ~ 6700 5500
Wire Wire Line
	7400 5100 7400 4950
$EndSCHEMATC
