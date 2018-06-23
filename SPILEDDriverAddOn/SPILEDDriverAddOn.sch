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
LIBS:uln2803a
LIBS:SPILEDDriverAddOn-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 5
Title ""
Date "21 jun 2018"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 2050 1950 1850 1750
U 5B2ADE19
F0 "Port F" 50
F1 "PortF.sch" 50
F2 "Serial In" I L 2050 2150 60 
F3 "SCK" I L 2050 2400 60 
F4 "OLAT" I L 2050 2650 60 
F5 "Serial Out" I R 3900 2700 60 
$EndSheet
$Sheet
S 4600 1950 1850 1750
U 5B2AF5D2
F0 "Port G" 50
F1 "PortG.sch" 50
F2 "Serial In" I L 4600 2150 60 
F3 "SCK" I L 4600 2400 60 
F4 "OLAT" I L 4600 2650 60 
F5 "Serial Out" I R 6450 2700 60 
$EndSheet
$Sheet
S 2000 4750 1850 1750
U 5B2B0868
F0 "Port H" 50
F1 "PortH.sch" 50
F2 "Serial In" I L 2000 4950 60 
F3 "SCK" I L 2000 5200 60 
F4 "OLAT" I L 2000 5450 60 
F5 "Serial Out" I R 3850 5500 60 
$EndSheet
$Sheet
S 4550 4750 1850 1750
U 5B2B086E
F0 "Port I" 50
F1 "PortI.sch" 50
F2 "Serial In" I L 4550 4950 60 
F3 "SCK" I L 4550 5200 60 
F4 "OLAT" I L 4550 5450 60 
F5 "Serial Out" I R 6400 5500 60 
$EndSheet
$Comp
L CONN_8 P1
U 1 1 5B2B1C05
P 9350 3850
F 0 "P1" V 9300 3850 60  0000 C CNN
F 1 "CONN_8" V 9400 3850 60  0000 C CNN
F 2 "~" H 9350 3850 60  0000 C CNN
F 3 "~" H 9350 3850 60  0000 C CNN
	1    9350 3850
	1    0    0    -1  
$EndComp
Text Label 9000 3500 2    60   ~ 0
MISO
Text Label 9000 3600 2    60   ~ 0
MOSI
Text Label 9000 3700 2    60   ~ 0
SCK
Text Label 9000 3800 2    60   ~ 0
E_CS
Text Label 9000 3900 2    60   ~ 0
E_LAT
$Comp
L +5V #PWR01
U 1 1 5B2B2B14
P 9000 4000
F 0 "#PWR01" H 9000 4090 20  0001 C CNN
F 1 "+5V" H 9000 4090 30  0000 C CNN
F 2 "" H 9000 4000 60  0000 C CNN
F 3 "" H 9000 4000 60  0000 C CNN
	1    9000 4000
	0    -1   -1   0   
$EndComp
$Comp
L +12V #PWR02
U 1 1 5B2B2BB8
P 9000 4100
F 0 "#PWR02" H 9000 4050 20  0001 C CNN
F 1 "+12V" H 9000 4200 30  0000 C CNN
F 2 "" H 9000 4100 60  0000 C CNN
F 3 "" H 9000 4100 60  0000 C CNN
	1    9000 4100
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR03
U 1 1 5B2B2BF7
P 9000 4200
F 0 "#PWR03" H 9000 4200 30  0001 C CNN
F 1 "GND" H 9000 4130 30  0001 C CNN
F 2 "" H 9000 4200 60  0000 C CNN
F 3 "" H 9000 4200 60  0000 C CNN
	1    9000 4200
	0    1    1    0   
$EndComp
Wire Wire Line
	1650 2150 2050 2150
Wire Wire Line
	1650 2400 2050 2400
Wire Wire Line
	1650 2650 2050 2650
Text Label 1650 2150 2    60   ~ 0
MOSI
Text Label 1650 2400 2    60   ~ 0
SCK
Text Label 1650 2650 2    60   ~ 0
E_LAT
Wire Wire Line
	3900 2700 4050 2700
Wire Wire Line
	4050 2700 4050 2150
Wire Wire Line
	4050 2150 4600 2150
Wire Wire Line
	4400 2400 4600 2400
Wire Wire Line
	4400 2650 4600 2650
Text Label 4400 2400 2    60   ~ 0
SCK
Text Label 4400 2650 2    60   ~ 0
E_LAT
Wire Wire Line
	6450 2700 6600 2700
Wire Wire Line
	6600 2700 6600 3850
Wire Wire Line
	6600 3850 1500 3850
Wire Wire Line
	1500 3850 1500 4950
Wire Wire Line
	1500 4950 2000 4950
Wire Wire Line
	3850 5500 4100 5500
Wire Wire Line
	4100 5500 4100 4950
Wire Wire Line
	4100 4950 4550 4950
Wire Wire Line
	1700 5200 2000 5200
Wire Wire Line
	1700 5450 2000 5450
Wire Wire Line
	4350 5200 4550 5200
Wire Wire Line
	4350 5450 4550 5450
Text Label 1700 5200 2    60   ~ 0
SCK
Text Label 1700 5450 2    60   ~ 0
E_LAT
Text Label 4350 5200 2    60   ~ 0
SCK
Text Label 4350 5450 2    60   ~ 0
E_LAT
$EndSCHEMATC
