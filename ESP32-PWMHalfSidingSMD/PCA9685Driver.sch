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
LIBS:esp32_devboards
LIBS:smalljumper
LIBS:sn65hvd233-ht
LIBS:lm2574n-5
LIBS:MCP23xxx
LIBS:tbd62x83a
LIBS:tc442x
LIBS:mct6h
LIBS:bridge
LIBS:pca9685
LIBS:4814p-2
LIBS:ESP32-PWMHalfSidingSMD-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 6
Title ""
Date "18 feb 2020"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L PCA9685 U601
U 1 1 5E35C9FE
P 2800 2400
F 0 "U601" H 2800 2200 60  0000 C CNN
F 1 "PCA9685" V 2800 2500 60  0000 C CNN
F 2 "~" H 2800 2400 60  0000 C CNN
F 3 "~" H 2800 2400 60  0000 C CNN
F 4 " 771-PCA9685PW,118" H 2800 2400 60  0001 C CNN "Mouser Part Number"
	1    2800 2400
	1    0    0    -1  
$EndComp
Text HLabel 2450 2000 0    25   Input ~ 0
SCL
Text HLabel 2450 2050 0    25   BiDi ~ 0
SDA
Text HLabel 1300 3350 0    25   Input ~ 0
OE
$Comp
L 3V3 #PWR021
U 1 1 5E35C9FF
P 2800 1750
F 0 "#PWR021" H 2800 1710 30  0001 C CNN
F 1 "3V3" H 2800 1860 30  0000 C CNN
F 2 "" H 2800 1750 60  0000 C CNN
F 3 "" H 2800 1750 60  0000 C CNN
	1    2800 1750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR022
U 1 1 5E35CA00
P 2800 3050
F 0 "#PWR022" H 2800 3050 30  0001 C CNN
F 1 "GND" H 2800 2980 30  0001 C CNN
F 2 "" H 2800 3050 60  0000 C CNN
F 3 "" H 2800 3050 60  0000 C CNN
	1    2800 3050
	1    0    0    -1  
$EndComp
$Comp
L C C601
U 1 1 5E35CA01
P 3550 1600
F 0 "C601" H 3550 1700 40  0000 L CNN
F 1 ".1 uf" H 3556 1515 40  0000 L CNN
F 2 "~" H 3588 1450 30  0000 C CNN
F 3 "~" H 3550 1600 60  0000 C CNN
F 4 "710-885012206095" H 3550 1600 60  0001 C CNN "Mouser Part Number"
	1    3550 1600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR023
U 1 1 5E35CA02
P 3550 1850
F 0 "#PWR023" H 3550 1850 30  0001 C CNN
F 1 "GND" H 3550 1780 30  0001 C CNN
F 2 "" H 3550 1850 60  0000 C CNN
F 3 "" H 3550 1850 60  0000 C CNN
	1    3550 1850
	1    0    0    -1  
$EndComp
$Comp
L TBD62X83A U602
U 1 1 5E35CA03
P 4800 1100
F 0 "U602" H 4900 950 60  0000 C CNN
F 1 "TBD62X83A" H 5050 1200 60  0000 C CNN
F 2 "~" H 4800 1100 60  0000 C CNN
F 3 "~" H 4800 1100 60  0000 C CNN
F 4 " 757-TBD62083AFG or 757-TBD62783AFG" H 4800 1100 60  0001 C CNN "Mouser Part Number"
	1    4800 1100
	1    0    0    -1  
$EndComp
$Comp
L TBD62X83A U602
U 2 1 5E35CA04
P 4800 1600
F 0 "U602" H 4900 1450 60  0000 C CNN
F 1 "TBD62X83A" H 5050 1700 60  0000 C CNN
F 2 "~" H 4800 1600 60  0000 C CNN
F 3 "~" H 4800 1600 60  0000 C CNN
	2    4800 1600
	1    0    0    -1  
$EndComp
$Comp
L TBD62X83A U602
U 3 1 5E35CA05
P 4800 2100
F 0 "U602" H 4900 1950 60  0000 C CNN
F 1 "TBD62X83A" H 5050 2200 60  0000 C CNN
F 2 "~" H 4800 2100 60  0000 C CNN
F 3 "~" H 4800 2100 60  0000 C CNN
	3    4800 2100
	1    0    0    -1  
$EndComp
$Comp
L TBD62X83A U602
U 4 1 5E35CA06
P 4800 2600
F 0 "U602" H 4900 2450 60  0000 C CNN
F 1 "TBD62X83A" H 5050 2700 60  0000 C CNN
F 2 "~" H 4800 2600 60  0000 C CNN
F 3 "~" H 4800 2600 60  0000 C CNN
	4    4800 2600
	1    0    0    -1  
$EndComp
$Comp
L TBD62X83A U602
U 5 1 5E35CA07
P 4800 3100
F 0 "U602" H 4900 2950 60  0000 C CNN
F 1 "TBD62X83A" H 5050 3200 60  0000 C CNN
F 2 "~" H 4800 3100 60  0000 C CNN
F 3 "~" H 4800 3100 60  0000 C CNN
	5    4800 3100
	1    0    0    -1  
$EndComp
$Comp
L TBD62X83A U602
U 6 1 5E35CA08
P 4800 3600
F 0 "U602" H 4900 3450 60  0000 C CNN
F 1 "TBD62X83A" H 5050 3700 60  0000 C CNN
F 2 "~" H 4800 3600 60  0000 C CNN
F 3 "~" H 4800 3600 60  0000 C CNN
	6    4800 3600
	1    0    0    -1  
$EndComp
$Comp
L TBD62X83A U602
U 7 1 5E35CA09
P 4800 4100
F 0 "U602" H 4900 3950 60  0000 C CNN
F 1 "TBD62X83A" H 5050 4200 60  0000 C CNN
F 2 "~" H 4800 4100 60  0000 C CNN
F 3 "~" H 4800 4100 60  0000 C CNN
	7    4800 4100
	1    0    0    -1  
$EndComp
$Comp
L TBD62X83A U602
U 8 1 5E35CA0A
P 4800 4600
F 0 "U602" H 4900 4450 60  0000 C CNN
F 1 "TBD62X83A" H 5050 4700 60  0000 C CNN
F 2 "~" H 4800 4600 60  0000 C CNN
F 3 "~" H 4800 4600 60  0000 C CNN
	8    4800 4600
	1    0    0    -1  
$EndComp
$Comp
L TBD62X83A U603
U 1 1 5E35CA0B
P 7300 1050
F 0 "U603" H 7400 900 60  0000 C CNN
F 1 "TBD62X83A" H 7550 1150 60  0000 C CNN
F 2 "~" H 7300 1050 60  0000 C CNN
F 3 "~" H 7300 1050 60  0000 C CNN
F 4 "757-TBD62083AFG or 757-TBD62783AFG" H 7300 1050 60  0001 C CNN "Mouser Part Number"
	1    7300 1050
	1    0    0    -1  
$EndComp
$Comp
L TBD62X83A U603
U 2 1 5E35CA0C
P 7300 1550
F 0 "U603" H 7400 1400 60  0000 C CNN
F 1 "TBD62X83A" H 7550 1650 60  0000 C CNN
F 2 "~" H 7300 1550 60  0000 C CNN
F 3 "~" H 7300 1550 60  0000 C CNN
	2    7300 1550
	1    0    0    -1  
$EndComp
$Comp
L TBD62X83A U603
U 3 1 5E35CA0D
P 7300 2050
F 0 "U603" H 7400 1900 60  0000 C CNN
F 1 "TBD62X83A" H 7550 2150 60  0000 C CNN
F 2 "~" H 7300 2050 60  0000 C CNN
F 3 "~" H 7300 2050 60  0000 C CNN
	3    7300 2050
	1    0    0    -1  
$EndComp
$Comp
L TBD62X83A U603
U 4 1 5E35CA0E
P 7300 2550
F 0 "U603" H 7400 2400 60  0000 C CNN
F 1 "TBD62X83A" H 7550 2650 60  0000 C CNN
F 2 "~" H 7300 2550 60  0000 C CNN
F 3 "~" H 7300 2550 60  0000 C CNN
	4    7300 2550
	1    0    0    -1  
$EndComp
$Comp
L TBD62X83A U603
U 5 1 5E35CA0F
P 7300 3050
F 0 "U603" H 7400 2900 60  0000 C CNN
F 1 "TBD62X83A" H 7550 3150 60  0000 C CNN
F 2 "~" H 7300 3050 60  0000 C CNN
F 3 "~" H 7300 3050 60  0000 C CNN
	5    7300 3050
	1    0    0    -1  
$EndComp
$Comp
L TBD62X83A U603
U 6 1 5E35CA10
P 7300 3550
F 0 "U603" H 7400 3400 60  0000 C CNN
F 1 "TBD62X83A" H 7550 3650 60  0000 C CNN
F 2 "~" H 7300 3550 60  0000 C CNN
F 3 "~" H 7300 3550 60  0000 C CNN
	6    7300 3550
	1    0    0    -1  
$EndComp
$Comp
L TBD62X83A U603
U 7 1 5E35CA11
P 7300 4050
F 0 "U603" H 7400 3900 60  0000 C CNN
F 1 "TBD62X83A" H 7550 4150 60  0000 C CNN
F 2 "~" H 7300 4050 60  0000 C CNN
F 3 "~" H 7300 4050 60  0000 C CNN
	7    7300 4050
	1    0    0    -1  
$EndComp
$Comp
L TBD62X83A U603
U 8 1 5E35CA12
P 7300 4550
F 0 "U603" H 7400 4400 60  0000 C CNN
F 1 "TBD62X83A" H 7550 4650 60  0000 C CNN
F 2 "~" H 7300 4550 60  0000 C CNN
F 3 "~" H 7300 4550 60  0000 C CNN
	8    7300 4550
	1    0    0    -1  
$EndComp
$Comp
L R_PACK8 RP601
U 1 1 5E35CA13
P 9500 1400
F 0 "RP601" H 9500 1850 40  0000 C CNN
F 1 "1.5K Ohms" H 9500 950 40  0000 C CNN
F 2 "~" H 9500 1400 60  0000 C CNN
F 3 "~" H 9500 1400 60  0000 C CNN
F 4 "652-4816P-T1LF-1.5K" H 9500 1400 60  0001 C CNN "Mouser Part Number"
	1    9500 1400
	1    0    0    -1  
$EndComp
$Comp
L R_PACK8 RP602
U 1 1 5E35CA14
P 9550 3850
F 0 "RP602" H 9550 4300 40  0000 C CNN
F 1 "1.5K Ohms" H 9550 3400 40  0000 C CNN
F 2 "~" H 9550 3850 60  0000 C CNN
F 3 "~" H 9550 3850 60  0000 C CNN
F 4 "652-4816P-T1LF-1.5K" H 9550 3850 60  0001 C CNN "Mouser Part Number"
	1    9550 3850
	1    0    0    -1  
$EndComp
$Comp
L JUMPER3 JP605
U 1 1 5E35CA15
P 4800 5150
F 0 "JP605" H 4850 5050 40  0000 L CNN
F 1 "CK A CA" H 4800 5250 40  0000 C CNN
F 2 "~" H 4800 5150 60  0000 C CNN
F 3 "~" H 4800 5150 60  0000 C CNN
	1    4800 5150
	1    0    0    1   
$EndComp
$Comp
L JUMPER3 JP607
U 1 1 5E35CA16
P 7300 5100
F 0 "JP607" H 7350 5000 40  0000 L CNN
F 1 "CK B CA" H 7300 5200 40  0000 C CNN
F 2 "~" H 7300 5100 60  0000 C CNN
F 3 "~" H 7300 5100 60  0000 C CNN
	1    7300 5100
	1    0    0    1   
$EndComp
$Comp
L JUMPER3 JP604
U 1 1 5E35CA17
P 4800 700
F 0 "JP604" H 4850 600 40  0000 L CNN
F 1 "CK A CA" H 4800 800 40  0000 C CNN
F 2 "~" H 4800 700 60  0000 C CNN
F 3 "~" H 4800 700 60  0000 C CNN
	1    4800 700 
	1    0    0    -1  
$EndComp
$Comp
L JUMPER3 JP606
U 1 1 5E35CA18
P 7300 700
F 0 "JP606" H 7350 600 40  0000 L CNN
F 1 "CK B CA" H 7300 800 40  0000 C CNN
F 2 "~" H 7300 700 60  0000 C CNN
F 3 "~" H 7300 700 60  0000 C CNN
	1    7300 700 
	1    0    0    -1  
$EndComp
$Comp
L CONN_9 P601
U 1 1 5E35CA19
P 10600 1450
F 0 "P601" V 10550 1450 60  0000 C CNN
F 1 "C A0 A1 A2 A3 A4 A5 A6 A7" V 10650 1450 60  0000 C CNN
F 2 "~" H 10600 1450 60  0000 C CNN
F 3 "~" H 10600 1450 60  0000 C CNN
F 4 "651-1725724" H 10600 1450 60  0001 C CNN "Mouser Part Number"
	1    10600 1450
	1    0    0    -1  
$EndComp
$Comp
L CONN_9 P602
U 1 1 5E35CA1A
P 10600 3900
F 0 "P602" V 10550 3900 60  0000 C CNN
F 1 "C B0 B1 B2 B3 B4 B5 B6 B7" V 10650 3900 60  0000 C CNN
F 2 "~" H 10600 3900 60  0000 C CNN
F 3 "~" H 10600 3900 60  0000 C CNN
F 4 "651-1725724" H 10600 3900 60  0001 C CNN "Mouser Part Number"
	1    10600 3900
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR024
U 1 1 5E35CA1B
P 4550 600
F 0 "#PWR024" H 4550 690 20  0001 C CNN
F 1 "+12V" H 4550 690 30  0000 C CNN
F 2 "" H 4550 600 60  0000 C CNN
F 3 "" H 4550 600 60  0000 C CNN
	1    4550 600 
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR025
U 1 1 5E35CA1C
P 6900 650
F 0 "#PWR025" H 6900 740 20  0001 C CNN
F 1 "+12V" H 6900 740 30  0000 C CNN
F 2 "" H 6900 650 60  0000 C CNN
F 3 "" H 6900 650 60  0000 C CNN
	1    6900 650 
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR026
U 1 1 5E35CA1D
P 7550 4900
F 0 "#PWR026" H 7550 4990 20  0001 C CNN
F 1 "+12V" H 7550 4990 30  0000 C CNN
F 2 "" H 7550 4900 60  0000 C CNN
F 3 "" H 7550 4900 60  0000 C CNN
	1    7550 4900
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR027
U 1 1 5E35CA1E
P 5000 4900
F 0 "#PWR027" H 5000 4990 20  0001 C CNN
F 1 "+12V" H 5000 4990 30  0000 C CNN
F 2 "" H 5000 4900 60  0000 C CNN
F 3 "" H 5000 4900 60  0000 C CNN
	1    5000 4900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR028
U 1 1 5E35CA1F
P 4550 5350
F 0 "#PWR028" H 4550 5350 30  0001 C CNN
F 1 "GND" H 4550 5280 30  0001 C CNN
F 2 "" H 4550 5350 60  0000 C CNN
F 3 "" H 4550 5350 60  0000 C CNN
	1    4550 5350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR029
U 1 1 5E35CA20
P 7050 5300
F 0 "#PWR029" H 7050 5300 30  0001 C CNN
F 1 "GND" H 7050 5230 30  0001 C CNN
F 2 "" H 7050 5300 60  0000 C CNN
F 3 "" H 7050 5300 60  0000 C CNN
	1    7050 5300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR030
U 1 1 5E35CA21
P 7550 750
F 0 "#PWR030" H 7550 750 30  0001 C CNN
F 1 "GND" H 7550 680 30  0001 C CNN
F 2 "" H 7550 750 60  0000 C CNN
F 3 "" H 7550 750 60  0000 C CNN
	1    7550 750 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR031
U 1 1 5E35CA22
P 5050 750
F 0 "#PWR031" H 5050 750 30  0001 C CNN
F 1 "GND" H 5050 680 30  0001 C CNN
F 2 "" H 5050 750 60  0000 C CNN
F 3 "" H 5050 750 60  0000 C CNN
	1    5050 750 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR032
U 1 1 5E35CA24
P 1200 3100
F 0 "#PWR032" H 1200 3100 30  0001 C CNN
F 1 "GND" H 1200 3030 30  0001 C CNN
F 2 "" H 1200 3100 60  0000 C CNN
F 3 "" H 1200 3100 60  0000 C CNN
	1    1200 3100
	1    0    0    -1  
$EndComp
$Comp
L CLOSED JP601
U 1 1 5E35CA26
P 1400 3250
F 0 "JP601" H 1400 3250 60  0000 C CNN
F 1 "OE GPIO" H 1400 3000 60  0000 C CNN
F 2 "~" H 1400 3250 60  0000 C CNN
F 3 "~" H 1400 3250 60  0000 C CNN
	1    1400 3250
	1    0    0    -1  
$EndComp
$Comp
L OPEN JP603
U 1 1 5E35CA27
P 2550 2950
F 0 "JP603" H 2550 2950 60  0000 C CNN
F 1 "OE Bypass" H 2550 2700 60  0000 C CNN
F 2 "~" H 2550 2950 60  0000 C CNN
F 3 "~" H 2550 2950 60  0000 C CNN
	1    2550 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 2950 2800 3050
Wire Wire Line
	2800 1750 2800 1850
Wire Wire Line
	3200 1800 3200 1400
Wire Wire Line
	2800 1800 3200 1800
Connection ~ 2800 1800
Wire Wire Line
	3200 1400 3550 1400
Wire Wire Line
	3550 1800 3550 1850
Wire Wire Line
	3150 2350 4500 2350
Wire Wire Line
	4500 2350 4500 1100
Wire Wire Line
	4500 1100 4550 1100
Wire Wire Line
	4550 1600 4450 1600
Wire Wire Line
	4450 1600 4450 2300
Wire Wire Line
	4450 2300 3150 2300
Wire Wire Line
	3150 2250 4400 2250
Wire Wire Line
	4400 2250 4400 2100
Wire Wire Line
	4400 2100 4550 2100
Wire Wire Line
	4550 2200 4550 2600
Wire Wire Line
	3150 2200 4550 2200
Wire Wire Line
	3150 2150 4350 2150
Wire Wire Line
	4350 2150 4350 3100
Wire Wire Line
	4350 3100 4550 3100
Wire Wire Line
	4550 3600 4300 3600
Wire Wire Line
	4300 3600 4300 2100
Wire Wire Line
	4300 2100 3150 2100
Wire Wire Line
	4200 4600 4550 4600
Wire Wire Line
	3150 2000 4200 2000
Wire Wire Line
	4200 2000 4200 4600
Wire Wire Line
	3150 2800 6950 2800
Wire Wire Line
	6950 2800 6950 1050
Wire Wire Line
	6950 1050 7050 1050
Wire Wire Line
	7050 1550 6900 1550
Wire Wire Line
	6900 1550 6900 2750
Wire Wire Line
	6900 2750 3150 2750
Wire Wire Line
	3150 2700 4400 2700
Wire Wire Line
	4400 2700 4400 3300
Wire Wire Line
	4400 3300 6850 3300
Wire Wire Line
	6850 3300 6850 2050
Wire Wire Line
	6850 2050 7050 2050
Wire Wire Line
	7050 2550 6800 2550
Wire Wire Line
	6800 2550 6800 3350
Wire Wire Line
	6800 3350 4450 3350
Wire Wire Line
	4450 3350 4450 2650
Wire Wire Line
	4450 2650 3150 2650
Wire Wire Line
	3150 2600 4500 2600
Wire Wire Line
	4500 2600 4500 3400
Wire Wire Line
	4500 3400 6750 3400
Wire Wire Line
	6750 3400 6750 3050
Wire Wire Line
	6750 3050 7050 3050
Wire Wire Line
	7050 3550 6850 3550
Wire Wire Line
	6850 3550 6850 3800
Wire Wire Line
	6850 3800 4150 3800
Wire Wire Line
	4150 3800 4150 2550
Wire Wire Line
	4150 2550 3150 2550
Wire Wire Line
	3150 2500 4100 2500
Wire Wire Line
	4100 2500 4100 3850
Wire Wire Line
	4250 4100 4550 4100
Wire Wire Line
	3150 2050 4250 2050
Wire Wire Line
	4250 2050 4250 4100
Wire Wire Line
	4100 3850 7050 3850
Wire Wire Line
	7050 3850 7050 4050
Wire Wire Line
	7050 4550 7000 4550
Wire Wire Line
	7000 4550 7000 3900
Wire Wire Line
	7000 3900 4050 3900
Wire Wire Line
	4050 3900 4050 2450
Wire Wire Line
	4050 2450 3150 2450
Wire Wire Line
	4800 800  4800 1000
Wire Wire Line
	7300 800  7300 950 
Wire Wire Line
	7300 5000 7300 4650
Wire Wire Line
	4800 5050 4800 4700
Wire Wire Line
	4800 850  6200 850 
Wire Wire Line
	6200 850  6200 550 
Wire Wire Line
	6200 550  9900 550 
Wire Wire Line
	9900 550  9900 1850
Wire Wire Line
	9900 1850 10250 1850
Connection ~ 4800 850 
Wire Wire Line
	7300 850  9200 850 
Wire Wire Line
	9200 850  9200 3300
Wire Wire Line
	9200 3300 10150 3300
Wire Wire Line
	10150 3300 10150 4300
Wire Wire Line
	10150 4300 10250 4300
Connection ~ 7300 850 
Wire Wire Line
	9700 1050 10250 1050
Wire Wire Line
	10250 1150 9700 1150
Wire Wire Line
	9700 1250 10250 1250
Wire Wire Line
	10250 1350 9700 1350
Wire Wire Line
	9700 1450 10250 1450
Wire Wire Line
	10250 1550 9700 1550
Wire Wire Line
	9700 1650 10250 1650
Wire Wire Line
	10250 1750 9700 1750
Wire Wire Line
	10250 3500 9750 3500
Wire Wire Line
	9750 3600 10250 3600
Wire Wire Line
	10250 3700 9750 3700
Wire Wire Line
	9750 3800 10250 3800
Wire Wire Line
	10250 3900 9750 3900
Wire Wire Line
	9750 4000 10250 4000
Wire Wire Line
	10250 4100 9750 4100
Wire Wire Line
	9750 4200 10250 4200
Wire Wire Line
	5100 1100 6900 1100
Wire Wire Line
	6900 1100 6900 1250
Wire Wire Line
	6900 1250 9150 1250
Wire Wire Line
	9150 1250 9150 1050
Wire Wire Line
	9150 1050 9300 1050
Wire Wire Line
	9300 1150 9100 1150
Wire Wire Line
	9100 1150 9100 1300
Wire Wire Line
	9100 1300 5100 1300
Wire Wire Line
	5100 1300 5100 1600
Wire Wire Line
	5100 2100 5150 2100
Wire Wire Line
	5150 2100 5150 1350
Wire Wire Line
	5150 1350 9050 1350
Wire Wire Line
	9050 1350 9050 1200
Wire Wire Line
	9050 1200 9250 1200
Wire Wire Line
	9250 1200 9250 1250
Wire Wire Line
	9250 1250 9300 1250
Wire Wire Line
	9300 1350 9100 1350
Wire Wire Line
	9100 1350 9100 1750
Wire Wire Line
	9100 1750 5200 1750
Wire Wire Line
	5200 1750 5200 2600
Wire Wire Line
	5200 2600 5100 2600
Wire Wire Line
	5100 3100 5250 3100
Wire Wire Line
	5250 3100 5250 1800
Wire Wire Line
	5250 1800 9150 1800
Wire Wire Line
	9150 1800 9150 1450
Wire Wire Line
	9150 1450 9300 1450
Wire Wire Line
	5100 3600 5300 3600
Wire Wire Line
	5300 3600 5300 1850
Wire Wire Line
	5300 1850 9250 1850
Wire Wire Line
	9250 1850 9250 1550
Wire Wire Line
	9250 1550 9300 1550
Wire Wire Line
	9300 1650 9050 1650
Wire Wire Line
	9050 1650 9050 4250
Wire Wire Line
	9050 4250 5100 4250
Wire Wire Line
	5100 4250 5100 4100
Wire Wire Line
	5100 4600 6950 4600
Wire Wire Line
	6950 4600 6950 4300
Wire Wire Line
	6950 4300 9000 4300
Wire Wire Line
	9000 4300 9000 1900
Wire Wire Line
	9000 1900 9300 1900
Wire Wire Line
	9300 1900 9300 1750
Wire Wire Line
	9350 3500 8950 3500
Wire Wire Line
	8950 3500 8950 1050
Wire Wire Line
	8950 1050 7600 1050
Wire Wire Line
	7600 1550 8900 1550
Wire Wire Line
	8900 1550 8900 3600
Wire Wire Line
	8900 3600 9350 3600
Wire Wire Line
	9350 3700 8850 3700
Wire Wire Line
	8850 3700 8850 2050
Wire Wire Line
	8850 2050 7600 2050
Wire Wire Line
	7600 2550 8800 2550
Wire Wire Line
	8800 2550 8800 3800
Wire Wire Line
	8800 3800 9350 3800
Wire Wire Line
	9350 3900 8750 3900
Wire Wire Line
	8750 3900 8750 3050
Wire Wire Line
	8750 3050 7600 3050
Wire Wire Line
	7600 3550 8700 3550
Wire Wire Line
	8700 3550 8700 4000
Wire Wire Line
	8700 4000 9350 4000
Wire Wire Line
	9350 4100 7850 4100
Wire Wire Line
	7850 4100 7850 4050
Wire Wire Line
	7850 4050 7600 4050
Wire Wire Line
	9350 4200 9350 4550
Wire Wire Line
	9350 4550 7600 4550
Wire Wire Line
	4550 600  4550 700 
Wire Wire Line
	5050 700  5050 750 
Wire Wire Line
	6900 650  7050 650 
Wire Wire Line
	7050 650  7050 700 
Wire Wire Line
	7550 700  7550 750 
Wire Wire Line
	7550 4900 7550 5100
Wire Wire Line
	7050 5100 7050 5300
Wire Wire Line
	4550 5150 4550 5350
Wire Wire Line
	5050 5150 5050 4900
Wire Wire Line
	5050 4900 5000 4900
Wire Wire Line
	1200 2450 2450 2450
Wire Wire Line
	2450 2450 2450 2550
Wire Wire Line
	1200 2950 2450 2950
Wire Wire Line
	2450 2950 2450 2800
Wire Wire Line
	2450 2600 2400 2600
Wire Wire Line
	2400 2600 2400 2550
Wire Wire Line
	2400 2550 1200 2550
Wire Wire Line
	2450 2750 2400 2750
Wire Wire Line
	1200 2850 2400 2850
Wire Wire Line
	2250 2700 2450 2700
Wire Wire Line
	1200 2650 2450 2650
Wire Wire Line
	1200 2250 1200 3100
Connection ~ 1200 2550
Connection ~ 1200 2650
Connection ~ 1200 2750
Connection ~ 1200 2850
Connection ~ 1200 2950
Wire Wire Line
	2350 2150 2450 2150
Wire Wire Line
	1200 2250 2450 2250
Wire Wire Line
	1500 3350 1800 3350
Wire Wire Line
	1800 3350 1800 3050
Wire Wire Line
	1800 3050 2450 3050
Wire Wire Line
	2650 3050 2700 3050
Wire Wire Line
	2700 3050 2700 3000
Wire Wire Line
	2700 3000 2800 3000
Connection ~ 2800 3000
Wire Wire Line
	2400 2850 2400 2750
Wire Wire Line
	2250 2750 2250 2700
Wire Wire Line
	1200 2750 2250 2750
Connection ~ 2350 3050
Wire Wire Line
	2350 2150 2350 3050
Connection ~ 1200 2450
$Comp
L DIODESCH D601
U 1 1 5E35CA28
P 3500 5050
F 0 "D601" H 3500 5150 40  0000 C CNN
F 1 "DFLS240-7" H 3500 4950 40  0000 C CNN
F 2 "~" H 3500 5050 60  0000 C CNN
F 3 "~" H 3500 5050 60  0000 C CNN
F 4 "621-DFLS240-7" H 3500 5050 60  0001 C CNN "Mouser Part Number"
	1    3500 5050
	-1   0    0    1   
$EndComp
$Comp
L +12V #PWR033
U 1 1 5E35CA29
P 3300 4850
F 0 "#PWR033" H 3300 4800 20  0001 C CNN
F 1 "+12V" H 3300 4950 30  0000 C CNN
F 2 "" H 3300 4850 60  0000 C CNN
F 3 "" H 3300 4850 60  0000 C CNN
	1    3300 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 5050 3300 4850
$Comp
L CONN_2 T601
U 1 1 5E35CA2A
P 2500 5150
F 0 "T601" V 2450 5150 40  0000 C CNN
F 1 "+ 12V -" V 2550 5150 40  0000 C CNN
F 2 "~" H 2500 5150 60  0000 C CNN
F 3 "~" H 2500 5150 60  0000 C CNN
F 4 "651-1725656" H 2500 5150 60  0001 C CNN "Mouser Part Number"
	1    2500 5150
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2850 5050 3300 5050
$Comp
L GND #PWR034
U 1 1 5E35CA2B
P 2850 5350
F 0 "#PWR034" H 2850 5350 30  0001 C CNN
F 1 "GND" H 2850 5280 30  0001 C CNN
F 2 "" H 2850 5350 60  0000 C CNN
F 3 "" H 2850 5350 60  0000 C CNN
	1    2850 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 5250 2850 5350
Text GLabel 3700 5050 2    100  Input ~ 0
CAN +12V
$EndSCHEMATC