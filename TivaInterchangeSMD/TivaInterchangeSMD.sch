EESchema Schematic File Version 2
LIBS:mmpq2222a
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
LIBS:TivaInterchangeSMD-cache
EELAYER 27 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 3
Title "Tiva Interchange"
Date "10 mar 2018"
Rev "1.0"
Comp "Deepwoods Software"
Comment1 "Master Sheet"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L GND #PWR01
U 1 1 5080AA99
P 9150 2450
F 0 "#PWR01" H 9150 2450 30  0001 C CNN
F 1 "GND" H 9150 2380 30  0001 C CNN
F 2 "" H 9150 2450 60  0001 C CNN
F 3 "" H 9150 2450 60  0001 C CNN
	1    9150 2450
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR02
U 1 1 5080AA9F
P 9150 900
F 0 "#PWR02" H 9150 1000 30  0001 C CNN
F 1 "VCC" H 9150 1000 30  0000 C CNN
F 2 "" H 9150 900 60  0001 C CNN
F 3 "" H 9150 900 60  0001 C CNN
	1    9150 900 
	0    -1   -1   0   
$EndComp
$Comp
L TI_BOOSTER_40_J1 J1
U 1 1 5080DB5C
P 9750 1350
F 0 "J1" H 9700 2000 60  0000 C CNN
F 1 "TI_BOOSTER_40_J1" H 9750 700 60  0000 C CNN
F 2 "" H 9750 1350 60  0001 C CNN
F 3 "" H 9750 1350 60  0001 C CNN
	1    9750 1350
	1    0    0    -1  
$EndComp
$Comp
L TI_BOOSTER_40_J2 J2
U 1 1 5080DBF4
P 9750 2900
F 0 "J2" H 9700 3550 60  0000 C CNN
F 1 "TI_BOOSTER_40_J2" H 9750 2250 60  0000 C CNN
F 2 "" H 9750 2900 60  0001 C CNN
F 3 "" H 9750 2900 60  0001 C CNN
	1    9750 2900
	1    0    0    -1  
$EndComp
$Comp
L TI_BOOSTER_40_J3 J3
U 1 1 5080DC03
P 9750 4450
F 0 "J3" H 9700 5100 60  0000 C CNN
F 1 "TI_BOOSTER_40_J3" H 9750 3800 60  0000 C CNN
F 2 "" H 9750 4450 60  0001 C CNN
F 3 "" H 9750 4450 60  0001 C CNN
	1    9750 4450
	1    0    0    -1  
$EndComp
$Comp
L TI_BOOSTER_40_J4 J4
U 1 1 5080DC12
P 9750 6000
F 0 "J4" H 9700 6650 60  0000 C CNN
F 1 "TI_BOOSTER_40_J4" H 9750 5350 60  0000 C CNN
F 2 "" H 9750 6000 60  0001 C CNN
F 3 "" H 9750 6000 60  0001 C CNN
	1    9750 6000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 5080DC79
P 9150 4100
F 0 "#PWR03" H 9150 4100 30  0001 C CNN
F 1 "GND" H 9150 4030 30  0001 C CNN
F 2 "" H 9150 4100 60  0001 C CNN
F 3 "" H 9150 4100 60  0001 C CNN
	1    9150 4100
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR04
U 1 1 5080DC8B
P 9150 4000
F 0 "#PWR04" H 9150 4090 20  0001 C CNN
F 1 "+5V" H 9150 4090 30  0000 C CNN
F 2 "" H 9150 4000 60  0001 C CNN
F 3 "" H 9150 4000 60  0001 C CNN
	1    9150 4000
	0    -1   -1   0   
$EndComp
$Comp
L +3.3V #PWR05
U 1 1 5A9F02BA
P 9150 900
F 0 "#PWR05" H 9150 860 30  0001 C CNN
F 1 "+3.3V" H 9150 1010 30  0000 C CNN
F 2 "" H 9150 900 60  0000 C CNN
F 3 "" H 9150 900 60  0000 C CNN
	1    9150 900 
	1    0    0    -1  
$EndComp
$Sheet
S 6400 3950 1450 1200
U 5A9FF518
F0 "SMCSense" 100
F1 "SMCSense.sch" 100
F2 "Motor Select 1" I R 7850 4200 60 
F3 "Motor Select 2" I R 7850 4300 60 
F4 "Point Sense 1" O R 7850 4400 60 
F5 "Point Sense 2" O R 7850 4500 60 
$EndSheet
$Sheet
S 5750 800  1600 1850
U 5AA03023
F0 "QuadSignalHead" 100
F1 "QuadSignalHead.sch" 100
F2 "SCL" I R 7350 1100 60 
F3 "SDA" B R 7350 1450 60 
$EndSheet
$Comp
L CONN_10 P3
U 1 1 5AA1384C
P 7600 6050
F 0 "P3" V 7550 6050 60  0000 C CNN
F 1 "CONN_10" V 7650 6050 60  0000 C CNN
F 2 "~" H 7600 6050 60  0000 C CNN
F 3 "~" H 7600 6050 60  0000 C CNN
	1    7600 6050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7350 1100 8650 1100
Wire Wire Line
	8650 1100 8650 1700
Wire Wire Line
	8650 1700 9150 1700
Wire Wire Line
	9150 1800 8150 1800
Wire Wire Line
	8150 1800 8150 1450
Wire Wire Line
	8150 1450 7350 1450
Wire Wire Line
	9150 4200 7850 4200
Wire Wire Line
	7850 4300 9150 4300
Wire Wire Line
	9150 4400 7850 4400
Wire Wire Line
	7850 4500 9150 4500
Wire Wire Line
	7950 5600 7950 4600
Wire Wire Line
	7950 4600 9150 4600
Wire Wire Line
	7950 5700 8100 5700
Wire Wire Line
	8100 5700 8100 4700
Wire Wire Line
	8100 4700 9150 4700
Wire Wire Line
	7950 5800 8200 5800
Wire Wire Line
	8200 5800 8200 4800
Wire Wire Line
	8200 4800 9150 4800
Wire Wire Line
	7950 5900 8300 5900
Wire Wire Line
	8300 5900 8300 4900
Wire Wire Line
	8300 4900 9150 4900
Wire Wire Line
	7950 6500 9150 6500
Wire Wire Line
	9150 6500 9150 6450
Wire Wire Line
	7950 6400 9150 6400
Wire Wire Line
	9150 6400 9150 6350
Wire Wire Line
	7950 6300 9150 6300
Wire Wire Line
	9150 6300 9150 6250
Wire Wire Line
	7950 6200 9150 6200
Wire Wire Line
	9150 6200 9150 6150
$Comp
L +5V #PWR06
U 1 1 5AA1A6ED
P 8500 5900
F 0 "#PWR06" H 8500 5990 20  0001 C CNN
F 1 "+5V" H 8500 5990 30  0000 C CNN
F 2 "" H 8500 5900 60  0000 C CNN
F 3 "" H 8500 5900 60  0000 C CNN
	1    8500 5900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR07
U 1 1 5AA1A70E
P 8500 6050
F 0 "#PWR07" H 8500 6050 30  0001 C CNN
F 1 "GND" H 8500 5980 30  0001 C CNN
F 2 "" H 8500 6050 60  0000 C CNN
F 3 "" H 8500 6050 60  0000 C CNN
	1    8500 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 6000 8200 6000
Wire Wire Line
	8200 6000 8200 5950
Wire Wire Line
	8200 5950 8500 5950
Wire Wire Line
	8500 5950 8500 5900
Wire Wire Line
	7950 6100 8250 6100
Wire Wire Line
	8250 6100 8250 6000
Wire Wire Line
	8250 6000 8500 6000
Wire Wire Line
	8500 6000 8500 6050
$EndSCHEMATC
