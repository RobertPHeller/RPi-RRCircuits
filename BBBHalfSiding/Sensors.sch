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
LIBS:mcp25xxx
LIBS:74lv125AN
LIBS:cat24c256w
LIBS:BBBHalfSiding-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
Title ""
Date "19 oct 2018"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L TRS202E U7
U 1 1 5BBBFBEE
P 4550 3050
F 0 "U7" H 4550 3250 60  0000 C CNN
F 1 "TRS202E" H 4550 3100 30  0000 C CNN
F 2 "~" H 4550 3050 60  0000 C CNN
F 3 "~" H 4550 3050 60  0000 C CNN
	1    4550 3050
	1    0    0    -1  
$EndComp
$Comp
L SN75LBC179 U8
U 1 1 5BBBFBEF
P 4550 4700
F 0 "U8" H 4550 4850 60  0000 C CNN
F 1 "SN75LBC179" H 4550 4700 60  0000 C CNN
F 2 "~" H 4550 4700 60  0000 C CNN
F 3 "~" H 4550 4700 60  0000 C CNN
	1    4550 4700
	1    0    0    -1  
$EndComp
$Comp
L CONN_5 P4
U 1 1 5BBBFBF0
P 6050 4650
F 0 "P4" V 6000 4650 50  0000 C CNN
F 1 "ABS Slave Bus" V 6100 4650 50  0000 C CNN
F 2 "~" H 6050 4650 60  0000 C CNN
F 3 "~" H 6050 4650 60  0000 C CNN
	1    6050 4650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR027
U 1 1 5BBBFBF1
P 5650 5000
F 0 "#PWR027" H 5650 5000 30  0001 C CNN
F 1 "GND" H 5650 4930 30  0001 C CNN
F 2 "" H 5650 5000 60  0000 C CNN
F 3 "" H 5650 5000 60  0000 C CNN
	1    5650 5000
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR028
U 1 1 5BBBFBF2
P 4550 4000
F 0 "#PWR028" H 4550 4090 20  0001 C CNN
F 1 "+5V" H 4550 4090 30  0000 C CNN
F 2 "" H 4550 4000 60  0000 C CNN
F 3 "" H 4550 4000 60  0000 C CNN
	1    4550 4000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR029
U 1 1 5BBBFBF3
P 4550 5400
F 0 "#PWR029" H 4550 5400 30  0001 C CNN
F 1 "GND" H 4550 5330 30  0001 C CNN
F 2 "" H 4550 5400 60  0000 C CNN
F 3 "" H 4550 5400 60  0000 C CNN
	1    4550 5400
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 5BBBFBF4
P 5300 4300
F 0 "R7" V 5380 4300 40  0000 C CNN
F 1 "60 Ohms" V 5307 4301 40  0000 C CNN
F 2 "~" V 5230 4300 30  0000 C CNN
F 3 "~" H 5300 4300 30  0000 C CNN
	1    5300 4300
	0    -1   -1   0   
$EndComp
$Comp
L R R6
U 1 1 5BBBFBF5
P 5250 5150
F 0 "R6" V 5330 5150 40  0000 C CNN
F 1 "60 Ohms" V 5257 5151 40  0000 C CNN
F 2 "~" V 5180 5150 30  0000 C CNN
F 3 "~" H 5250 5150 30  0000 C CNN
	1    5250 5150
	0    -1   -1   0   
$EndComp
$Comp
L C C2
U 1 1 5BBBFBF6
P 3650 2600
F 0 "C2" H 3650 2700 40  0000 L CNN
F 1 ".1 uf" H 3656 2515 40  0000 L CNN
F 2 "~" H 3688 2450 30  0000 C CNN
F 3 "~" H 3650 2600 60  0000 C CNN
	1    3650 2600
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5BBBFBF7
P 3650 3100
F 0 "C3" H 3650 3200 40  0000 L CNN
F 1 ".1 uf" H 3656 3015 40  0000 L CNN
F 2 "~" H 3688 2950 30  0000 C CNN
F 3 "~" H 3650 3100 60  0000 C CNN
	1    3650 3100
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 5BBBFBF8
P 5350 2600
F 0 "C4" H 5350 2700 40  0000 L CNN
F 1 ".1 uf" H 5356 2515 40  0000 L CNN
F 2 "~" H 5388 2450 30  0000 C CNN
F 3 "~" H 5350 2600 60  0000 C CNN
	1    5350 2600
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 5BBBFBF9
P 5350 3100
F 0 "C5" H 5350 3200 40  0000 L CNN
F 1 ".1 uf" H 5356 3015 40  0000 L CNN
F 2 "~" H 5388 2950 30  0000 C CNN
F 3 "~" H 5350 3100 60  0000 C CNN
	1    5350 3100
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 5BBBFBFA
P 5650 2300
F 0 "C6" H 5650 2400 40  0000 L CNN
F 1 ".1 uf" H 5656 2215 40  0000 L CNN
F 2 "~" H 5688 2150 30  0000 C CNN
F 3 "~" H 5650 2300 60  0000 C CNN
	1    5650 2300
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR030
U 1 1 5BBBFBFB
P 4550 2400
F 0 "#PWR030" H 4550 2490 20  0001 C CNN
F 1 "+5V" H 4550 2490 30  0000 C CNN
F 2 "" H 4550 2400 60  0000 C CNN
F 3 "" H 4550 2400 60  0000 C CNN
	1    4550 2400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR031
U 1 1 5BBBFBFC
P 4550 3700
F 0 "#PWR031" H 4550 3700 30  0001 C CNN
F 1 "GND" H 4550 3630 30  0001 C CNN
F 2 "" H 4550 3700 60  0000 C CNN
F 3 "" H 4550 3700 60  0000 C CNN
	1    4550 3700
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P1
U 1 1 5BBBFBFE
P 1000 850
F 0 "P1" V 950 850 40  0000 C CNN
F 1 "In Track Out" V 1050 850 40  0000 C CNN
F 2 "~" H 1000 850 60  0000 C CNN
F 3 "~" H 1000 850 60  0000 C CNN
	1    1000 850 
	0    -1   -1   0   
$EndComp
$Comp
L R R3
U 1 1 5BBBFBFF
P 1900 900
F 0 "R3" V 1980 900 40  0000 C CNN
F 1 "10 Ohms" V 1907 901 40  0000 C CNN
F 2 "~" V 1830 900 30  0000 C CNN
F 3 "~" H 1900 900 30  0000 C CNN
	1    1900 900 
	0    -1   -1   0   
$EndComp
$Comp
L MCT6H IC1
U 1 1 5BBBFC00
P 3400 1200
F 0 "IC1" H 3190 1390 40  0000 C CNN
F 1 "MCT6H" H 3510 1010 40  0000 C CNN
F 2 "DIP-8__300" H 3240 1020 30  0001 C CIN
F 3 "~" H 3400 1200 60  0000 C CNN
	1    3400 1200
	1    0    0    -1  
$EndComp
$Comp
L MCT6H IC1
U 2 1 5BBBFC01
P 3400 1750
F 0 "IC1" H 3190 1940 40  0000 C CNN
F 1 "MCT6H" H 3510 1560 40  0000 C CNN
F 2 "DIP-8__300" H 3240 1570 30  0001 C CIN
F 3 "~" H 3400 1750 60  0000 C CNN
	2    3400 1750
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 5BBBFC02
P 4100 700
F 0 "R5" V 4180 700 40  0000 C CNN
F 1 "10K Ohms" V 4107 701 40  0000 C CNN
F 2 "~" V 4030 700 30  0000 C CNN
F 3 "~" H 4100 700 30  0000 C CNN
	1    4100 700 
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR032
U 1 1 5BBBFC03
P 3950 2100
F 0 "#PWR032" H 3950 2100 30  0001 C CNN
F 1 "GND" H 3950 2030 30  0001 C CNN
F 2 "" H 3950 2100 60  0000 C CNN
F 3 "" H 3950 2100 60  0000 C CNN
	1    3950 2100
	1    0    0    -1  
$EndComp
$Comp
L TRS202E U9
U 1 1 5BBBFC04
P 6850 3050
F 0 "U9" H 6850 3250 60  0000 C CNN
F 1 "TRS202E" H 6850 3100 30  0000 C CNN
F 2 "~" H 6850 3050 60  0000 C CNN
F 3 "~" H 6850 3050 60  0000 C CNN
	1    6850 3050
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 5BBBFC05
P 5950 2600
F 0 "C7" H 5950 2700 40  0000 L CNN
F 1 ".1 uf" H 5956 2515 40  0000 L CNN
F 2 "~" H 5988 2450 30  0000 C CNN
F 3 "~" H 5950 2600 60  0000 C CNN
	1    5950 2600
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 5BBBFC06
P 5950 3100
F 0 "C8" H 5950 3200 40  0000 L CNN
F 1 ".1 uf" H 5956 3015 40  0000 L CNN
F 2 "~" H 5988 2950 30  0000 C CNN
F 3 "~" H 5950 3100 60  0000 C CNN
	1    5950 3100
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 5BBBFC07
P 7650 2600
F 0 "C9" H 7650 2700 40  0000 L CNN
F 1 ".1 uf" H 7656 2515 40  0000 L CNN
F 2 "~" H 7688 2450 30  0000 C CNN
F 3 "~" H 7650 2600 60  0000 C CNN
	1    7650 2600
	1    0    0    -1  
$EndComp
$Comp
L C C10
U 1 1 5BBBFC08
P 7650 3100
F 0 "C10" H 7650 3200 40  0000 L CNN
F 1 ".1 uf" H 7656 3015 40  0000 L CNN
F 2 "~" H 7688 2950 30  0000 C CNN
F 3 "~" H 7650 3100 60  0000 C CNN
	1    7650 3100
	1    0    0    -1  
$EndComp
$Comp
L C C11
U 1 1 5BBBFC09
P 7950 2300
F 0 "C11" H 7950 2400 40  0000 L CNN
F 1 ".1 uf" H 7956 2215 40  0000 L CNN
F 2 "~" H 7988 2150 30  0000 C CNN
F 3 "~" H 7950 2300 60  0000 C CNN
	1    7950 2300
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR033
U 1 1 5BBBFC0A
P 6850 2400
F 0 "#PWR033" H 6850 2490 20  0001 C CNN
F 1 "+5V" H 6850 2490 30  0000 C CNN
F 2 "" H 6850 2400 60  0000 C CNN
F 3 "" H 6850 2400 60  0000 C CNN
	1    6850 2400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR034
U 1 1 5BBBFC0B
P 6850 3700
F 0 "#PWR034" H 6850 3700 30  0001 C CNN
F 1 "GND" H 6850 3630 30  0001 C CNN
F 2 "" H 6850 3700 60  0000 C CNN
F 3 "" H 6850 3700 60  0000 C CNN
	1    6850 3700
	1    0    0    -1  
$EndComp
$Comp
L CONN_4 P2
U 1 1 5BBBFC0C
P 5200 1000
F 0 "P2" V 5150 1000 50  0000 C CNN
F 1 "East (Points)" V 5250 1000 50  0000 C CNN
F 2 "~" H 5200 1000 60  0000 C CNN
F 3 "~" H 5200 1000 60  0000 C CNN
	1    5200 1000
	0    -1   -1   0   
$EndComp
$Comp
L CONN_4 P3
U 1 1 5BBBFC0D
P 5950 1000
F 0 "P3" V 5900 1000 50  0000 C CNN
F 1 "West Main (Frog, Normal)" V 6000 1000 50  0000 C CNN
F 2 "~" H 5950 1000 60  0000 C CNN
F 3 "~" H 5950 1000 60  0000 C CNN
	1    5950 1000
	0    -1   -1   0   
$EndComp
$Comp
L CONN_4 P5
U 1 1 5BBBFC0E
P 6800 1000
F 0 "P5" V 6750 1000 50  0000 C CNN
F 1 "West Div. (Frog, Revesed)" V 6850 1000 50  0000 C CNN
F 2 "~" H 6800 1000 60  0000 C CNN
F 3 "~" H 6800 1000 60  0000 C CNN
	1    6800 1000
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR035
U 1 1 5BBBFC0F
P 5000 1650
F 0 "#PWR035" H 5000 1650 30  0001 C CNN
F 1 "GND" H 5000 1580 30  0001 C CNN
F 2 "" H 5000 1650 60  0000 C CNN
F 3 "" H 5000 1650 60  0000 C CNN
	1    5000 1650
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR036
U 1 1 5BBBFC10
P 4650 650
F 0 "#PWR036" H 4650 610 30  0001 C CNN
F 1 "+3.3V" H 4650 760 30  0000 C CNN
F 2 "" H 4650 650 60  0000 C CNN
F 3 "" H 4650 650 60  0000 C CNN
	1    4650 650 
	1    0    0    -1  
$EndComp
Text Label 3600 4050 0    60   ~ 0
RX_
Text Label 3950 3050 3    28   ~ 0
East (Points) Occ_
Text Label 5000 3050 3    28   ~ 0
West Main (Frog Normal) Occ_
Text Label 6250 3050 3    28   ~ 0
West Diverg (Frog Reversed) Occ_
Wire Wire Line
	5050 4450 5500 4450
Wire Wire Line
	5050 4550 5300 4550
Wire Wire Line
	5050 4850 5450 4850
Wire Wire Line
	5050 4950 5600 4950
Wire Wire Line
	5650 4850 5650 5000
Wire Wire Line
	4550 4000 4550 4150
Wire Wire Line
	4550 5250 4550 5400
Wire Wire Line
	5000 5150 5000 5050
Wire Wire Line
	5000 5050 5100 5050
Wire Wire Line
	5100 5050 5100 4850
Connection ~ 5100 4850
Wire Wire Line
	5050 4300 5050 4350
Wire Wire Line
	5050 4350 5100 4350
Wire Wire Line
	5100 4350 5100 4450
Connection ~ 5100 4450
Wire Wire Line
	5550 4300 5550 4400
Wire Wire Line
	5550 4400 5250 4400
Wire Wire Line
	5250 4400 5250 4550
Connection ~ 5250 4550
Wire Wire Line
	3650 2800 4250 2800
Wire Wire Line
	3650 2900 4250 2900
Wire Wire Line
	5350 2800 4850 2800
Wire Wire Line
	5350 2900 4850 2900
Wire Wire Line
	3650 2400 4000 2400
Wire Wire Line
	4000 2400 4000 2700
Wire Wire Line
	4000 2700 4250 2700
Wire Wire Line
	5350 2400 5150 2400
Wire Wire Line
	5150 2400 5150 2700
Wire Wire Line
	5150 2700 4850 2700
Wire Wire Line
	4550 2400 4550 2550
Wire Wire Line
	4550 3550 4550 3700
Wire Wire Line
	3650 3300 3650 3600
Wire Wire Line
	3650 3600 5650 3600
Connection ~ 4550 3600
Wire Wire Line
	5350 3600 5350 3300
Wire Wire Line
	4750 2100 4750 2450
Wire Wire Line
	4750 2450 4550 2450
Connection ~ 4550 2450
Connection ~ 5350 3600
Wire Wire Line
	1950 1350 1950 3050
Wire Wire Line
	900  1200 900  2200
Wire Wire Line
	900  2200 1100 2200
Wire Wire Line
	2800 1200 2800 2200
Wire Wire Line
	900  1400 1550 1400
Wire Wire Line
	1550 1400 1550 900 
Wire Wire Line
	1550 900  1650 900 
Connection ~ 900  1400
Wire Wire Line
	2150 900  3050 900 
Wire Wire Line
	3050 900  3050 1100
Wire Wire Line
	3050 1300 3050 1650
Wire Wire Line
	1100 1200 2800 1200
Wire Wire Line
	3050 1100 2900 1100
Wire Wire Line
	2900 1100 2900 1850
Wire Wire Line
	2900 1850 3050 1850
Wire Wire Line
	3050 1450 2800 1450
Connection ~ 2800 1450
Connection ~ 3050 1450
Wire Wire Line
	3850 700  3850 1650
Wire Wire Line
	3850 1100 3750 1100
Wire Wire Line
	3850 1650 3750 1650
Connection ~ 3850 1100
Wire Wire Line
	3750 1300 3950 1300
Wire Wire Line
	3950 1300 3950 2100
Wire Wire Line
	3950 1850 3750 1850
Connection ~ 3950 1850
Wire Wire Line
	4350 700  4650 700 
Wire Wire Line
	4650 700  4650 650 
Wire Wire Line
	4050 1200 4050 3800
Wire Wire Line
	4050 1200 3850 1200
Connection ~ 3850 1200
Wire Wire Line
	4050 3800 4950 3800
Wire Wire Line
	4950 3800 4950 3200
Wire Wire Line
	4950 3200 4850 3200
Wire Wire Line
	4250 3200 4050 3200
Connection ~ 4050 3200
Connection ~ 5650 2100
Wire Wire Line
	4750 2100 5650 2100
Wire Wire Line
	5650 3600 5650 2500
Connection ~ 5650 3600
Wire Wire Line
	5950 2800 6550 2800
Wire Wire Line
	5950 2900 6550 2900
Wire Wire Line
	7650 2800 7150 2800
Wire Wire Line
	7650 2900 7150 2900
Wire Wire Line
	5950 2400 6300 2400
Wire Wire Line
	6300 2400 6300 2700
Wire Wire Line
	6300 2700 6550 2700
Wire Wire Line
	7650 2400 7450 2400
Wire Wire Line
	7450 2400 7450 2700
Wire Wire Line
	7450 2700 7150 2700
Wire Wire Line
	6850 2400 6850 2550
Wire Wire Line
	6850 3550 6850 3700
Wire Wire Line
	5950 3300 5950 3600
Wire Wire Line
	5950 3600 7950 3600
Connection ~ 6850 3600
Wire Wire Line
	7650 3600 7650 3300
Wire Wire Line
	7050 2100 7050 2450
Wire Wire Line
	7050 2450 6850 2450
Connection ~ 6850 2450
Connection ~ 7650 3600
Wire Wire Line
	6350 1850 6350 3800
Wire Wire Line
	6350 3800 7250 3800
Wire Wire Line
	7250 3800 7250 3200
Wire Wire Line
	7250 3200 7150 3200
Wire Wire Line
	6550 3200 6350 3200
Connection ~ 6350 3200
Wire Wire Line
	7950 3600 7950 2500
Connection ~ 7950 3600
Connection ~ 7050 2100
Wire Wire Line
	7950 2100 7050 2100
Wire Wire Line
	6350 1850 4050 1850
Connection ~ 4050 1850
Wire Wire Line
	5050 1350 5050 1600
Wire Wire Line
	5000 1600 6950 1600
Wire Wire Line
	5000 1600 5000 1650
Wire Wire Line
	5350 1350 5350 1600
Connection ~ 5050 1600
Wire Wire Line
	5800 1600 5800 1350
Connection ~ 5350 1600
Wire Wire Line
	6100 1600 6100 1350
Connection ~ 5800 1600
Wire Wire Line
	6650 1600 6650 1350
Connection ~ 6100 1600
Wire Wire Line
	6950 1600 6950 1350
Connection ~ 6650 1600
Wire Wire Line
	5150 1350 5150 1450
Wire Wire Line
	5150 1450 4100 1450
Wire Wire Line
	4100 1450 4100 3100
Wire Wire Line
	5050 3100 5050 1950
Wire Wire Line
	5050 1950 6000 1950
Wire Wire Line
	6000 1950 6000 1350
Wire Wire Line
	6850 1350 6850 1850
Wire Wire Line
	6850 1850 6450 1850
Wire Wire Line
	6450 1850 6450 3100
Wire Wire Line
	4250 3150 4150 3150
Wire Wire Line
	4150 3150 4150 1500
Wire Wire Line
	4150 1500 5250 1500
Wire Wire Line
	5250 1500 5250 1350
Wire Wire Line
	5900 1350 5900 2050
Wire Wire Line
	5900 2050 5100 2050
Wire Wire Line
	5100 2050 5100 3150
Wire Wire Line
	5100 3150 4850 3150
Wire Wire Line
	6550 3150 6500 3150
Wire Wire Line
	6500 3150 6500 1700
Wire Wire Line
	6500 1700 6750 1700
Wire Wire Line
	6750 1700 6750 1350
Wire Wire Line
	5500 4450 5500 4750
Wire Wire Line
	5500 4750 5650 4750
Wire Wire Line
	5300 4550 5300 4650
Wire Wire Line
	5300 4650 5650 4650
Wire Wire Line
	5450 4850 5450 4550
Wire Wire Line
	5450 4550 5650 4550
Wire Wire Line
	5600 4950 5600 4450
Wire Wire Line
	5600 4450 5650 4450
Wire Wire Line
	5500 5150 5500 4950
Connection ~ 5500 4950
Wire Wire Line
	4050 4450 3800 4450
Wire Wire Line
	3800 4450 3800 4050
Wire Wire Line
	3800 4050 3600 4050
Wire Wire Line
	4100 3100 4250 3100
Wire Wire Line
	4250 3250 4100 3250
Wire Wire Line
	4100 3250 4100 3150
Wire Wire Line
	4100 3150 4000 3150
Wire Wire Line
	4000 3150 4000 3050
Wire Wire Line
	4000 3050 3950 3050
Wire Wire Line
	4850 3100 5050 3100
Wire Wire Line
	4850 3250 4900 3250
Wire Wire Line
	4900 3250 4900 3050
Wire Wire Line
	4900 3050 5000 3050
Wire Wire Line
	6450 3100 6550 3100
Wire Wire Line
	6550 3250 6450 3250
Wire Wire Line
	6450 3250 6450 3150
Wire Wire Line
	6450 3150 6400 3150
Wire Wire Line
	6400 3150 6400 3050
Wire Wire Line
	6400 3050 6250 3050
$Comp
L R R4
U 1 1 5BBBFC12
P 3750 4650
F 0 "R4" V 3830 4650 40  0000 C CNN
F 1 "10K Ohms" V 3757 4651 40  0000 C CNN
F 2 "~" V 3680 4650 30  0000 C CNN
F 3 "~" H 3750 4650 30  0000 C CNN
	1    3750 4650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3500 4650 3500 4550
Wire Wire Line
	4000 4650 4000 4850
Wire Wire Line
	4000 4850 4050 4850
$Comp
L 74LV125AN U10
U 1 1 5BBC0003
P 7450 4200
F 0 "U10" H 7450 4300 50  0000 L BNN
F 1 "74LV125AN" H 7500 4050 40  0000 L TNN
F 2 "~" H 7450 4200 60  0000 C CNN
F 3 "~" H 7450 4200 60  0000 C CNN
	1    7450 4200
	1    0    0    -1  
$EndComp
$Comp
L 74LV125AN U10
U 2 1 5BBC0012
P 7450 4900
F 0 "U10" H 7450 5000 50  0000 L BNN
F 1 "74LV125AN" H 7500 4750 40  0000 L TNN
F 2 "~" H 7450 4900 60  0000 C CNN
F 3 "~" H 7450 4900 60  0000 C CNN
	2    7450 4900
	1    0    0    -1  
$EndComp
$Comp
L 74LV125AN U10
U 3 1 5BBC0021
P 7500 5700
F 0 "U10" H 7500 5800 50  0000 L BNN
F 1 "74LV125AN" H 7550 5550 40  0000 L TNN
F 2 "~" H 7500 5700 60  0000 C CNN
F 3 "~" H 7500 5700 60  0000 C CNN
	3    7500 5700
	1    0    0    -1  
$EndComp
$Comp
L 74LV125AN U10
U 4 1 5BBC0030
P 7500 6500
F 0 "U10" H 7500 6600 50  0000 L BNN
F 1 "74LV125AN" H 7550 6350 40  0000 L TNN
F 2 "~" H 7500 6500 60  0000 C CNN
F 3 "~" H 7500 6500 60  0000 C CNN
	4    7500 6500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR037
U 1 1 5BBC0055
P 7450 4550
F 0 "#PWR037" H 7450 4550 30  0001 C CNN
F 1 "GND" H 7450 4480 30  0001 C CNN
F 2 "" H 7450 4550 60  0000 C CNN
F 3 "" H 7450 4550 60  0000 C CNN
	1    7450 4550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR038
U 1 1 5BBC0064
P 7450 5250
F 0 "#PWR038" H 7450 5250 30  0001 C CNN
F 1 "GND" H 7450 5180 30  0001 C CNN
F 2 "" H 7450 5250 60  0000 C CNN
F 3 "" H 7450 5250 60  0000 C CNN
	1    7450 5250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR039
U 1 1 5BBC0073
P 7500 6100
F 0 "#PWR039" H 7500 6100 30  0001 C CNN
F 1 "GND" H 7500 6030 30  0001 C CNN
F 2 "" H 7500 6100 60  0000 C CNN
F 3 "" H 7500 6100 60  0000 C CNN
	1    7500 6100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR040
U 1 1 5BBC0082
P 7500 6900
F 0 "#PWR040" H 7500 6900 30  0001 C CNN
F 1 "GND" H 7500 6830 30  0001 C CNN
F 2 "" H 7500 6900 60  0000 C CNN
F 3 "" H 7500 6900 60  0000 C CNN
	1    7500 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 6650 7350 6800
Wire Wire Line
	7350 6800 7500 6800
Wire Wire Line
	7500 6800 7500 6900
Wire Wire Line
	7500 6000 7500 6100
Wire Wire Line
	7450 5200 7450 5250
Wire Wire Line
	7450 4500 7450 4550
$Comp
L +3.3V #PWR041
U 1 1 5BBC0419
P 7300 3950
F 0 "#PWR041" H 7300 3910 30  0001 C CNN
F 1 "+3.3V" H 7300 4060 30  0000 C CNN
F 2 "" H 7300 3950 60  0000 C CNN
F 3 "" H 7300 3950 60  0000 C CNN
	1    7300 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 3950 7300 4050
$Comp
L +3.3V #PWR042
U 1 1 5BBC04E0
P 3500 4550
F 0 "#PWR042" H 3500 4510 30  0001 C CNN
F 1 "+3.3V" H 3500 4660 30  0000 C CNN
F 2 "" H 3500 4550 60  0000 C CNN
F 3 "" H 3500 4550 60  0000 C CNN
	1    3500 4550
	1    0    0    -1  
$EndComp
Text HLabel 4050 1200 2    60   Output ~ 0
Occupancy
Text HLabel 4050 4850 0    60   Input ~ 0
TX
Text Label 7000 4200 2    28   ~ 0
RX_
Text HLabel 7900 4200 2    28   Output ~ 0
RX
Text Label 7000 4900 2    28   ~ 0
East (Points) Occ_
Text HLabel 7900 4900 2    28   Output ~ 0
East (Points) Occ
Text Label 7050 5700 2    28   ~ 0
West Main (Frog Normal) Occ_
Text Label 7050 6500 2    28   ~ 0
West Diverg (Frog Reversed) Occ_
Text HLabel 7950 5700 2    28   Output ~ 0
West Main (Frog Normal) Occ
Text HLabel 7950 6500 2    28   Output ~ 0
West Diverg (Frog Reversed) Occ
$Comp
L C C12
U 1 1 5BBC04FA
P 8600 4650
F 0 "C12" H 8600 4750 40  0000 L CNN
F 1 ".1 uf" H 8606 4565 40  0000 L CNN
F 2 "~" H 8638 4500 30  0000 C CNN
F 3 "~" H 8600 4650 60  0000 C CNN
	1    8600 4650
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR043
U 1 1 5BBC051C
P 8600 4350
F 0 "#PWR043" H 8600 4310 30  0001 C CNN
F 1 "+3.3V" H 8600 4460 30  0000 C CNN
F 2 "" H 8600 4350 60  0000 C CNN
F 3 "" H 8600 4350 60  0000 C CNN
	1    8600 4350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR044
U 1 1 5BBC052B
P 8600 4950
F 0 "#PWR044" H 8600 4950 30  0001 C CNN
F 1 "GND" H 8600 4880 30  0001 C CNN
F 2 "" H 8600 4950 60  0000 C CNN
F 3 "" H 8600 4950 60  0000 C CNN
	1    8600 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 4850 8600 4950
Wire Wire Line
	8600 4350 8600 4450
$Comp
L BRIDGEX D1
U 1 1 5BBBB126
P 1950 2200
F 0 "D1" H 1950 2250 70  0000 C CNN
F 1 "BRIDGEX" H 1950 2150 70  0000 C CNN
F 2 "~" H 1950 2200 60  0000 C CNN
F 3 "~" H 1950 2200 60  0000 C CNN
	1    1950 2200
	0    1    1    0   
$EndComp
$EndSCHEMATC
