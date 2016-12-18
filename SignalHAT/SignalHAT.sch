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
LIBS:SignalHAT-cache
EELAYER 27 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 1
Title "Signal HAT (MAX7221)"
Date "5 dec 2016"
Rev "0.0"
Comp "Deepwoods Software"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RPi_GPIO J0
U 1 1 5516AE26
P 1000 5300
F 0 "J0" H 1750 5550 60  0000 C CNN
F 1 "RPi_GPIO" H 1750 5450 60  0001 C CNN
F 2 "RPi_Hat:Pin_Header_Straight_2x20" H 1000 5300 60  0001 C CNN
F 3 "" H 1000 5300 60  0000 C CNN
F 4 "517-929975-01-20" H 1000 5300 60  0001 C CNN "Mouser Part Number"
F 5 "2223" H 1000 5300 60  0001 C CNN "AdaFruit part number (stackable)"
F 6 "2222" H 1000 5300 60  0001 C CNN "AdaFruit part number (non-stackable)"
F 7 "1979" H 1000 5300 60  0001 C CNN "AdaFruit part number (stackable, extra tall)"
	1    1000 5300
	1    0    0    -1  
$EndComp
$Comp
L MAX7221 U1
U 1 1 580EA749
P 5000 6150
F 0 "U1" H 4950 5850 60  0000 C CNN
F 1 "MAX7221" H 4950 5850 60  0001 C CNN
F 2 "DIP-24__300" H 4950 5850 60  0001 C CNN
F 3 "MAX7219-MAX7221.pdf" H 4950 5850 60  0001 C CNN
F 4 "700-MAX7221CNG" H 5000 6150 60  0001 C CNN "Mouser Part Number"
	1    5000 6150
	1    0    0    -1  
$EndComp
$Comp
L CONN_5X2 J1
U 1 1 580EA758
P 3100 2400
F 0 "J1" H 3100 2700 60  0000 C CNN
F 1 "DIL Header 2x5" H 3100 2400 50  0001 C CNN
F 2 "PIN_ARRAY_5x2" H 3100 2400 60  0001 C CNN
F 3 "~" H 3100 2400 60  0000 C CNN
F 4 "649-66506-066LF" H 3100 2400 60  0001 C CNN "Mouser Part Number"
	1    3100 2400
	1    0    0    -1  
$EndComp
$Comp
L CONN_5X2 J2
U 1 1 580EA767
P 5000 2400
F 0 "J2" H 5000 2700 60  0000 C CNN
F 1 "DIL Header 2x5" H 5000 2400 50  0001 C CNN
F 2 "PIN_ARRAY_5x2" H 5000 2400 60  0001 C CNN
F 3 "~" H 5000 2400 60  0001 C CNN
F 4 "649-66506-066LF" H 5000 2400 60  0001 C CNN "Mouser Part Number"
	1    5000 2400
	1    0    0    -1  
$EndComp
$Comp
L CONN_5X2 J3
U 1 1 580EA776
P 6800 2400
F 0 "J3" H 6800 2700 60  0000 C CNN
F 1 "DIL Header 2x5" H 6800 2400 50  0001 C CNN
F 2 "PIN_ARRAY_5x2" H 6800 2400 60  0001 C CNN
F 3 "~" H 6800 2400 60  0000 C CNN
F 4 "649-66506-066LF" H 6800 2400 60  0001 C CNN "Mouser Part Number"
	1    6800 2400
	1    0    0    -1  
$EndComp
$Comp
L CONN_5X2 J4
U 1 1 580EA785
P 6800 3850
F 0 "J4" H 6800 4150 60  0000 C CNN
F 1 "DIL Header 2x5" H 6800 3850 50  0001 C CNN
F 2 "PIN_ARRAY_5x2" H 6800 3850 60  0001 C CNN
F 3 "~" H 6800 3850 60  0000 C CNN
F 4 "649-66506-066LF" H 6800 3850 60  0001 C CNN "Mouser Part Number"
	1    6800 3850
	1    0    0    -1  
$EndComp
$Comp
L CONN_5X2 J5
U 1 1 580EA794
P 8900 2400
F 0 "J5" H 8900 2700 60  0000 C CNN
F 1 "DIL Header 2x5" H 8900 2400 50  0001 C CNN
F 2 "PIN_ARRAY_5x2" H 8900 2400 60  0001 C CNN
F 3 "~" H 8900 2400 60  0000 C CNN
F 4 "649-66506-066LF" H 8900 2400 60  0001 C CNN "Mouser Part Number"
	1    8900 2400
	1    0    0    -1  
$EndComp
$Comp
L CONN_5X2 J6
U 1 1 580EA7A3
P 8900 3800
F 0 "J6" H 8900 4100 60  0000 C CNN
F 1 "DIL Header 2x5" H 8900 3800 50  0001 C CNN
F 2 "PIN_ARRAY_5x2" H 8900 3800 60  0001 C CNN
F 3 "~" H 8900 3800 60  0000 C CNN
F 4 "649-66506-066LF" H 8900 3800 60  0001 C CNN "Mouser Part Number"
	1    8900 3800
	1    0    0    -1  
$EndComp
$Comp
L CONN_5X2 J7
U 1 1 580EA7F8
P 8900 5150
F 0 "J7" H 8900 5450 60  0000 C CNN
F 1 "DIL Header 2x5" H 8900 5150 50  0001 C CNN
F 2 "PIN_ARRAY_5x2" H 8900 5150 60  0001 C CNN
F 3 "~" H 8900 5150 60  0000 C CNN
F 4 "649-66506-066LF" H 8900 5150 60  0001 C CNN "Mouser Part Number"
	1    8900 5150
	1    0    0    -1  
$EndComp
$Comp
L CONN_5X2 J8
U 1 1 580EA807
P 8900 6450
F 0 "J8" H 8900 6750 60  0000 C CNN
F 1 "DIL Header 2x5" H 8900 6450 50  0001 C CNN
F 2 "PIN_ARRAY_5x2" H 8900 6450 60  0001 C CNN
F 3 "~" H 8900 6450 60  0000 C CNN
F 4 "649-66506-066LF" H 8900 6450 60  0001 C CNN "Mouser Part Number"
	1    8900 6450
	1    0    0    -1  
$EndComp
$Comp
L CONN_3 J9
U 1 1 580EA953
P 3450 6400
F 0 "J9" V 3400 6400 50  0000 C CNN
F 1 "SIL 3 header" V 3500 6400 40  0001 C CNN
F 2 "PIN_ARRAY_3X1" H 3450 6400 60  0001 C CNN
F 3 "~" H 3450 6400 60  0000 C CNN
F 4 "538-22-28-8034 " H 3450 6400 60  0001 C CNN "Mouser Part Number"
	1    3450 6400
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 580EAA59
P 5050 4650
F 0 "R1" V 5130 4650 40  0000 C CNN
F 1 "33K Ohms" V 5057 4651 40  0000 C CNN
F 2 "R1" H 4980 4650 30  0001 C CNN
F 3 "~" H 5050 4650 30  0000 C CNN
F 4 "660-MF1/4LCT52R333G" H 5050 4650 60  0001 C CNN "Mouser Part Number"
	1    5050 4650
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 580EAA6C
P 4000 4600
F 0 "C2" H 4000 4700 40  0000 L CNN
F 1 "100pf 50V" H 4006 4515 40  0000 L CNN
F 2 "C1" H 4038 4450 30  0001 C CNN
F 3 "~" H 4000 4600 60  0000 C CNN
F 4 "TAP104M035DCS  " H 4000 4600 60  0001 C CNN "Mouser Part Number"
	1    4000 4600
	1    0    0    -1  
$EndComp
$Comp
L CP1 C1
U 1 1 580EAA81
P 3500 4600
F 0 "C1" H 3550 4700 50  0000 L CNN
F 1 "10uf 25V" H 3550 4500 50  0000 L CNN
F 2 "C1V5" H 3500 4600 60  0001 C CNN
F 3 "~" H 3500 4600 60  0000 C CNN
F 4 "598-SK100M025ST" H 3500 4600 60  0001 C CNN "Mouser Part Number"
	1    3500 4600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 580EAAC5
P 4700 7450
F 0 "#PWR01" H 4700 7450 30  0001 C CNN
F 1 "GND" H 4700 7380 30  0001 C CNN
F 2 "" H 4700 7450 60  0000 C CNN
F 3 "" H 4700 7450 60  0000 C CNN
	1    4700 7450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 580EAAD5
P 4850 7450
F 0 "#PWR02" H 4850 7450 30  0001 C CNN
F 1 "GND" H 4850 7380 30  0001 C CNN
F 2 "" H 4850 7450 60  0000 C CNN
F 3 "" H 4850 7450 60  0000 C CNN
	1    4850 7450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 580EAB2D
P 550 5750
F 0 "#PWR03" H 550 5750 30  0001 C CNN
F 1 "GND" H 550 5680 30  0001 C CNN
F 2 "" H 550 5750 60  0000 C CNN
F 3 "" H 550 5750 60  0000 C CNN
	1    550  5750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 580EAB43
P 550 6550
F 0 "#PWR04" H 550 6550 30  0001 C CNN
F 1 "GND" H 550 6480 30  0001 C CNN
F 2 "" H 550 6550 60  0000 C CNN
F 3 "" H 550 6550 60  0000 C CNN
	1    550  6550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 580EAB52
P 550 7250
F 0 "#PWR05" H 550 7250 30  0001 C CNN
F 1 "GND" H 550 7180 30  0001 C CNN
F 2 "" H 550 7250 60  0000 C CNN
F 3 "" H 550 7250 60  0000 C CNN
	1    550  7250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 580EAB69
P 2900 5550
F 0 "#PWR06" H 2900 5550 30  0001 C CNN
F 1 "GND" H 2900 5480 30  0001 C CNN
F 2 "" H 2900 5550 60  0000 C CNN
F 3 "" H 2900 5550 60  0000 C CNN
	1    2900 5550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR07
U 1 1 580EAB78
P 2900 5950
F 0 "#PWR07" H 2900 5950 30  0001 C CNN
F 1 "GND" H 2900 5880 30  0001 C CNN
F 2 "" H 2900 5950 60  0000 C CNN
F 3 "" H 2900 5950 60  0000 C CNN
	1    2900 5950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 580EAB87
P 2900 6250
F 0 "#PWR08" H 2900 6250 30  0001 C CNN
F 1 "GND" H 2900 6180 30  0001 C CNN
F 2 "" H 2900 6250 60  0000 C CNN
F 3 "" H 2900 6250 60  0000 C CNN
	1    2900 6250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 580EAB96
P 2900 6750
F 0 "#PWR09" H 2900 6750 30  0001 C CNN
F 1 "GND" H 2900 6680 30  0001 C CNN
F 2 "" H 2900 6750 60  0000 C CNN
F 3 "" H 2900 6750 60  0000 C CNN
	1    2900 6750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 580EABA5
P 2900 6950
F 0 "#PWR010" H 2900 6950 30  0001 C CNN
F 1 "GND" H 2900 6880 30  0001 C CNN
F 2 "" H 2900 6950 60  0000 C CNN
F 3 "" H 2900 6950 60  0000 C CNN
	1    2900 6950
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR011
U 1 1 580EAC3B
P 2850 5250
F 0 "#PWR011" H 2850 5350 30  0001 C CNN
F 1 "VCC" H 2850 5350 30  0000 C CNN
F 2 "" H 2850 5250 60  0000 C CNN
F 3 "" H 2850 5250 60  0000 C CNN
	1    2850 5250
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR012
U 1 1 580EAC4D
P 4800 4900
F 0 "#PWR012" H 4800 5000 30  0001 C CNN
F 1 "VCC" H 4800 5000 30  0000 C CNN
F 2 "" H 4800 4900 60  0000 C CNN
F 3 "" H 4800 4900 60  0000 C CNN
	1    4800 4900
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR013
U 1 1 580EAC5C
P 5050 4250
F 0 "#PWR013" H 5050 4350 30  0001 C CNN
F 1 "VCC" H 5050 4350 30  0000 C CNN
F 2 "" H 5050 4250 60  0000 C CNN
F 3 "" H 5050 4250 60  0000 C CNN
	1    5050 4250
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR014
U 1 1 580EAC6B
P 3750 4300
F 0 "#PWR014" H 3750 4400 30  0001 C CNN
F 1 "VCC" H 3750 4400 30  0000 C CNN
F 2 "" H 3750 4300 60  0000 C CNN
F 3 "" H 3750 4300 60  0000 C CNN
	1    3750 4300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 580EAC7A
P 3750 4900
F 0 "#PWR015" H 3750 4900 30  0001 C CNN
F 1 "GND" H 3750 4830 30  0001 C CNN
F 2 "" H 3750 4900 60  0000 C CNN
F 3 "" H 3750 4900 60  0000 C CNN
	1    3750 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 7350 4850 7450
Wire Wire Line
	4700 7350 4700 7450
Wire Wire Line
	800  5700 550  5700
Wire Wire Line
	550  5700 550  5750
Wire Wire Line
	800  6500 550  6500
Wire Wire Line
	550  6500 550  6550
Wire Wire Line
	800  7200 550  7200
Wire Wire Line
	550  7200 550  7250
Wire Wire Line
	2700 5500 2900 5500
Wire Wire Line
	2900 5500 2900 5550
Wire Wire Line
	2700 5900 2900 5900
Wire Wire Line
	2900 5900 2900 5950
Wire Wire Line
	2700 6200 2900 6200
Wire Wire Line
	2900 6200 2900 6250
Wire Wire Line
	2700 6700 2900 6700
Wire Wire Line
	2900 6700 2900 6750
Wire Wire Line
	2700 6900 2900 6900
Wire Wire Line
	2900 6900 2900 6950
Wire Wire Line
	5050 4900 5050 5050
Wire Wire Line
	4800 4900 4800 5050
Wire Wire Line
	5050 4250 5050 4400
Wire Wire Line
	3500 4400 4000 4400
Wire Wire Line
	3750 4400 3750 4300
Connection ~ 3750 4400
Wire Wire Line
	3500 4800 4000 4800
Wire Wire Line
	3750 4800 3750 4900
Connection ~ 3750 4800
Wire Wire Line
	2700 5400 2700 5300
Wire Wire Line
	2700 5300 2850 5300
Wire Wire Line
	2850 5300 2850 5250
Wire Wire Line
	5650 6950 7850 6950
Wire Wire Line
	7850 6950 7850 6250
Wire Wire Line
	7850 6250 8500 6250
Wire Wire Line
	5650 6850 7750 6850
Wire Wire Line
	7750 6850 7750 4950
Wire Wire Line
	7750 4950 8500 4950
Wire Wire Line
	5650 6750 7600 6750
Wire Wire Line
	7600 6750 7600 4650
Wire Wire Line
	7600 4650 8100 4650
Wire Wire Line
	8100 4650 8100 3600
Wire Wire Line
	8100 3600 8500 3600
Wire Wire Line
	5650 6650 7500 6650
Wire Wire Line
	7500 6650 7500 4500
Wire Wire Line
	7500 4500 7900 4500
Wire Wire Line
	7900 4500 7900 2200
Wire Wire Line
	7900 2200 8500 2200
Wire Wire Line
	5650 6550 6300 6550
Wire Wire Line
	6300 6550 6300 3650
Wire Wire Line
	6300 3650 6400 3650
Wire Wire Line
	5650 6450 6200 6450
Wire Wire Line
	6200 6450 6200 2200
Wire Wire Line
	6200 2200 6400 2200
Wire Wire Line
	5650 6250 6050 6250
Wire Wire Line
	6050 6250 6050 3650
Wire Wire Line
	6050 3650 2050 3650
Wire Wire Line
	2050 3650 2050 2200
Wire Wire Line
	2050 2200 2700 2200
Wire Wire Line
	5650 6150 7850 6150
Wire Wire Line
	7850 6150 7850 5950
Wire Wire Line
	7850 5950 9400 5950
Wire Wire Line
	9400 1850 9400 6250
Wire Wire Line
	9400 6250 9300 6250
Wire Wire Line
	8300 5450 5650 5450
Wire Wire Line
	8300 2300 8300 6350
Wire Wire Line
	8300 6350 8500 6350
Wire Wire Line
	9400 4950 9300 4950
Connection ~ 9400 5950
Wire Wire Line
	9400 3600 9300 3600
Connection ~ 9400 4950
Wire Wire Line
	9400 2200 9300 2200
Connection ~ 9400 3600
Wire Wire Line
	5400 1850 9400 1850
Wire Wire Line
	7200 1850 7200 2200
Connection ~ 9400 2200
Wire Wire Line
	5400 1850 5400 2200
Connection ~ 7200 1850
Wire Wire Line
	5400 1900 3500 1900
Wire Wire Line
	3500 1900 3500 2200
Connection ~ 5400 1900
Wire Wire Line
	7200 3650 7350 3650
Wire Wire Line
	7350 3650 7350 1850
Connection ~ 7350 1850
Wire Wire Line
	5950 5050 8500 5050
Connection ~ 8300 5450
Wire Wire Line
	8300 3700 8500 3700
Connection ~ 8300 5050
Wire Wire Line
	8300 2300 8500 2300
Connection ~ 8300 3700
Wire Wire Line
	5950 3750 6400 3750
Wire Wire Line
	5950 3750 5950 5050
Wire Wire Line
	6400 2300 6000 2300
Wire Wire Line
	6000 2300 6000 3750
Connection ~ 6000 3750
Wire Wire Line
	4600 2300 4250 2300
Wire Wire Line
	4250 2300 4250 3150
Wire Wire Line
	2150 3150 6000 3150
Connection ~ 6000 3150
Wire Wire Line
	2700 2300 2150 2300
Wire Wire Line
	2150 2300 2150 3150
Connection ~ 4250 3150
Wire Wire Line
	10200 5550 5650 5550
Wire Wire Line
	10200 2300 10200 6350
Wire Wire Line
	10200 5050 9300 5050
Wire Wire Line
	10200 6350 9300 6350
Connection ~ 10200 5550
Wire Wire Line
	10200 3700 9300 3700
Connection ~ 10200 5050
Wire Wire Line
	10200 2300 9300 2300
Connection ~ 10200 3700
Wire Wire Line
	7700 3750 7200 3750
Wire Wire Line
	7700 2300 7700 4750
Wire Wire Line
	7700 4750 10200 4750
Connection ~ 10200 4750
Wire Wire Line
	7200 2300 7700 2300
Connection ~ 7700 3750
Wire Wire Line
	4050 3000 7700 3000
Wire Wire Line
	5850 3000 5850 2300
Wire Wire Line
	5850 2300 5400 2300
Connection ~ 7700 3000
Wire Wire Line
	4050 3000 4050 2300
Wire Wire Line
	4050 2300 3500 2300
Connection ~ 5850 3000
Wire Wire Line
	8250 5650 5650 5650
Wire Wire Line
	8250 2400 8250 6450
Wire Wire Line
	8250 6450 8500 6450
Wire Wire Line
	8500 5150 8250 5150
Connection ~ 8250 5650
Wire Wire Line
	8500 3800 8250 3800
Connection ~ 8250 5150
Wire Wire Line
	8250 2400 8500 2400
Connection ~ 8250 3800
Wire Wire Line
	2200 3050 8250 3050
Connection ~ 8250 3050
Wire Wire Line
	4600 2400 4300 2400
Wire Wire Line
	4300 2400 4300 3050
Wire Wire Line
	2700 2400 2200 2400
Wire Wire Line
	2200 2400 2200 3050
Connection ~ 4300 3050
Wire Wire Line
	10150 5750 5650 5750
Wire Wire Line
	10150 2400 10150 6450
Wire Wire Line
	10150 6450 9300 6450
Wire Wire Line
	9300 5150 10150 5150
Connection ~ 10150 5750
Wire Wire Line
	10150 3800 9300 3800
Connection ~ 10150 5150
Wire Wire Line
	10150 2400 9300 2400
Connection ~ 10150 3800
Wire Wire Line
	7200 2400 7650 2400
Wire Wire Line
	7650 2400 7650 3850
Wire Wire Line
	7650 3850 7200 3850
Wire Wire Line
	7650 3150 10150 3150
Connection ~ 10150 3150
Connection ~ 7650 3150
Wire Wire Line
	5400 2400 5750 2400
Wire Wire Line
	5750 2400 5750 3200
Wire Wire Line
	3950 3200 7650 3200
Connection ~ 7650 3200
Wire Wire Line
	3950 3200 3950 2400
Wire Wire Line
	3950 2400 3500 2400
Connection ~ 5750 3200
Wire Wire Line
	8200 5850 5650 5850
Wire Wire Line
	8200 2500 8200 6550
Wire Wire Line
	8200 6550 8500 6550
Wire Wire Line
	8200 5250 8500 5250
Connection ~ 8200 5850
Wire Wire Line
	8200 3900 8500 3900
Connection ~ 8200 5250
Wire Wire Line
	8500 2500 8200 2500
Connection ~ 8200 3900
Wire Wire Line
	6400 2500 6250 2500
Wire Wire Line
	6250 2500 6250 3950
Wire Wire Line
	6250 3950 6400 3950
Wire Wire Line
	2250 2950 8200 2950
Connection ~ 8200 2950
Connection ~ 6250 2950
Wire Wire Line
	4350 2950 4350 2500
Wire Wire Line
	4350 2500 4600 2500
Wire Wire Line
	2250 2950 2250 2500
Wire Wire Line
	2250 2500 2700 2500
Connection ~ 4350 2950
Wire Wire Line
	5650 5950 7800 5950
Wire Wire Line
	7800 5950 7800 6000
Wire Wire Line
	7800 6000 10050 6000
Wire Wire Line
	10050 2500 10050 6550
Wire Wire Line
	10050 6550 9300 6550
Wire Wire Line
	9300 5250 10050 5250
Connection ~ 10050 6000
Wire Wire Line
	9300 3900 10050 3900
Connection ~ 10050 5250
Wire Wire Line
	7200 3950 7650 3950
Wire Wire Line
	7650 3950 7650 4700
Wire Wire Line
	7650 4700 10050 4700
Connection ~ 10050 4700
Wire Wire Line
	7200 2500 7600 2500
Wire Wire Line
	7600 2500 7600 3950
Connection ~ 7600 3950
Wire Wire Line
	5400 2500 5700 2500
Wire Wire Line
	5700 2500 5700 3300
Wire Wire Line
	3850 3300 7600 3300
Connection ~ 7600 3300
Wire Wire Line
	3500 2500 3850 2500
Wire Wire Line
	3850 2500 3850 3300
Connection ~ 5700 3300
Wire Wire Line
	9300 2500 10050 2500
Connection ~ 10050 3900
Wire Wire Line
	2700 2600 2350 2600
Wire Wire Line
	2350 2600 2350 2900
Wire Wire Line
	2350 2900 8500 2900
Wire Wire Line
	4600 2900 4600 2600
Wire Wire Line
	6400 2900 6400 2600
Connection ~ 4600 2900
Wire Wire Line
	8500 2900 8500 2600
Connection ~ 6400 2900
Wire Wire Line
	6400 4050 6350 4050
Wire Wire Line
	6350 2900 6350 4250
Connection ~ 6350 2900
Wire Wire Line
	6350 4250 8500 4250
Wire Wire Line
	8500 4250 8500 4000
Connection ~ 6350 4050
Wire Wire Line
	8500 5350 8400 5350
Wire Wire Line
	8400 4250 8400 6650
Connection ~ 8400 4250
Wire Wire Line
	8400 6650 8500 6650
Connection ~ 8400 5350
Wire Wire Line
	5650 6050 8400 6050
Connection ~ 8400 6050
Text Label 5800 5450 0    60   ~ 0
Segment A
Text Label 5800 5550 0    60   ~ 0
Segment B
Text Label 5800 5650 0    60   ~ 0
Segment C
Text Label 5800 5750 0    60   ~ 0
Segment D
Text Label 5800 5850 0    60   ~ 0
Segment E
Text Label 5800 5950 0    60   ~ 0
Segment F
Text Label 5800 6050 0    60   ~ 0
Segment G
Text Label 5800 6150 0    60   ~ 0
Segment DP
Text Label 5700 6250 0    60   ~ 0
Digit 0
Text Label 5700 6350 0    60   ~ 0
Digit 1
Text Label 5700 6450 0    60   ~ 0
Digit 2
Text Label 5700 6550 0    60   ~ 0
Digit 3
Text Label 5700 6650 0    60   ~ 0
Digit 4
Text Label 5700 6750 0    60   ~ 0
Digit 5
Text Label 5700 6850 0    60   ~ 0
Digit 6
Text Label 5700 6950 0    60   ~ 0
Digit 7
Text Label 5050 5000 0    60   ~ 0
ISet
Text Label 4800 5000 0    60   ~ 0
Vcc
Text Label 2750 6500 0    60   ~ 0
CE1
Text Label 2750 6700 0    60   ~ 0
GND
Wire Wire Line
	6400 2400 6150 2400
Wire Wire Line
	6150 2400 6150 3850
Wire Wire Line
	6150 3850 6400 3850
Connection ~ 6150 3050
Wire Wire Line
	5650 6350 6100 6350
Wire Wire Line
	6100 6350 6100 2800
Wire Wire Line
	6100 2800 4450 2800
Wire Wire Line
	4450 2800 4450 2200
Wire Wire Line
	4450 2200 4600 2200
NoConn ~ 600  4250
Wire Wire Line
	2700 6500 3100 6500
Wire Wire Line
	4100 5200 4250 5200
Wire Wire Line
	4250 5200 4250 5450
$Comp
L 74AHCT125 U2
U 1 1 5814F97E
P 3650 5200
F 0 "U2" H 3650 5300 50  0000 L BNN
F 1 "74AHCT125" H 3700 5050 40  0000 L TNN
F 2 "DIP-14__300" H 3650 5200 60  0001 C CNN
F 3 "~" H 3650 5200 60  0000 C CNN
F 4 "595-SN74AHCT125N" H 3650 5200 60  0001 C CNN "Mouser Part Number"
	1    3650 5200
	1    0    0    -1  
$EndComp
$Comp
L 74AHCT125 U2
U 2 1 5814F992
P 3650 5750
F 0 "U2" H 3650 5850 50  0000 L BNN
F 1 "74AHCT125" H 3700 5600 40  0000 L TNN
F 2 "~" H 3650 5750 60  0000 C CNN
F 3 "~" H 3650 5750 60  0000 C CNN
	2    3650 5750
	1    0    0    -1  
$EndComp
$Comp
L 74AHCT125 U2
U 3 1 5814F9AB
P 3650 6850
F 0 "U2" H 3650 6950 50  0000 L BNN
F 1 "74AHCT125" H 3700 6700 40  0000 L TNN
F 2 "~" H 3650 6850 60  0000 C CNN
F 3 "~" H 3650 6850 60  0000 C CNN
	3    3650 6850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 5814F9C4
P 3650 5500
F 0 "#PWR016" H 3650 5500 30  0001 C CNN
F 1 "GND" H 3650 5430 30  0001 C CNN
F 2 "" H 3650 5500 60  0000 C CNN
F 3 "" H 3650 5500 60  0000 C CNN
	1    3650 5500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR017
U 1 1 5814F9D3
P 3650 6050
F 0 "#PWR017" H 3650 6050 30  0001 C CNN
F 1 "GND" H 3650 5980 30  0001 C CNN
F 2 "" H 3650 6050 60  0000 C CNN
F 3 "" H 3650 6050 60  0000 C CNN
	1    3650 6050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR018
U 1 1 5814F9E2
P 3650 7150
F 0 "#PWR018" H 3650 7150 30  0001 C CNN
F 1 "GND" H 3650 7080 30  0001 C CNN
F 2 "" H 3650 7150 60  0000 C CNN
F 3 "" H 3650 7150 60  0000 C CNN
	1    3650 7150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR019
U 1 1 5814F9F1
P 3500 5350
F 0 "#PWR019" H 3500 5350 30  0001 C CNN
F 1 "GND" H 3500 5280 30  0001 C CNN
F 2 "" H 3500 5350 60  0000 C CNN
F 3 "" H 3500 5350 60  0000 C CNN
	1    3500 5350
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR020
U 1 1 5814FA16
P 3500 5050
F 0 "#PWR020" H 3500 5150 30  0001 C CNN
F 1 "VCC" H 3500 5150 30  0000 C CNN
F 2 "" H 3500 5050 60  0000 C CNN
F 3 "" H 3500 5050 60  0000 C CNN
	1    3500 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 6850 3050 6850
Wire Wire Line
	3050 6850 3050 6400
Wire Wire Line
	3050 6400 3100 6400
Wire Wire Line
	3200 5750 3200 6150
Wire Wire Line
	3200 6150 4350 6150
Wire Wire Line
	4350 7450 650  7450
Wire Wire Line
	650  7450 650  6400
Wire Wire Line
	650  6400 800  6400
Wire Wire Line
	3200 5200 3200 4950
Wire Wire Line
	3200 4950 650  4950
Wire Wire Line
	650  4950 650  6200
Wire Wire Line
	650  6200 800  6200
Text Label 2400 4950 0    60   ~ 0
MOSI
Text Label 4250 5200 0    60   ~ 0
DIN
Wire Wire Line
	4350 6150 4350 7450
Wire Wire Line
	4100 5750 4250 5750
Wire Wire Line
	4250 5650 4150 5650
Wire Wire Line
	4150 5650 4150 6850
Wire Wire Line
	4150 6850 4100 6850
Text Label 3200 6000 0    60   ~ 0
SCLK
Text Label 4200 5750 0    60   ~ 0
CLK
Wire Wire Line
	2700 6400 2900 6400
Wire Wire Line
	2900 6400 2900 6350
Wire Wire Line
	2900 6350 3000 6350
Wire Wire Line
	3000 6350 3000 6300
Wire Wire Line
	3000 6300 3100 6300
Text Label 2750 6400 0    60   ~ 0
CE0
Text Label 3050 6650 0    60   ~ 0
CE
Text Label 4150 5950 0    60   ~ 0
LOAD
$EndSCHEMATC
