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
LIBS:pocketbeagle
LIBS:sn65hvd233-ht
LIBS:lm2574n-5
LIBS:pca9685
LIBS:tbd62x83a
LIBS:tc442x
LIBS:mct6h
LIBS:bridge
LIBS:sn75lbc179
LIBS:trs202e
LIBS:PocketBeaglePWMHalfSiding+ABSSlave-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 9 9
Title ""
Date "22 jul 2019"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MAX3490AE U901
U 1 1 5D36DC92
P 5350 3300
F 0 "U901" H 5350 3450 60  0000 C CNN
F 1 "MAX3490AE" H 5350 3300 60  0000 C CNN
F 2 "~" H 5350 3300 60  0000 C CNN
F 3 "~" H 5350 3300 60  0000 C CNN
	1    5350 3300
	1    0    0    -1  
$EndComp
$Comp
L 3V3 #PWR042
U 1 1 5D36DCA1
P 5350 2500
F 0 "#PWR042" H 5350 2600 40  0001 C CNN
F 1 "3V3" H 5350 2625 40  0000 C CNN
F 2 "" H 5350 2500 60  0000 C CNN
F 3 "" H 5350 2500 60  0000 C CNN
	1    5350 2500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR043
U 1 1 5D36DCB0
P 5350 4050
F 0 "#PWR043" H 5350 4050 30  0001 C CNN
F 1 "GND" H 5350 3980 30  0001 C CNN
F 2 "" H 5350 4050 60  0000 C CNN
F 3 "" H 5350 4050 60  0000 C CNN
	1    5350 4050
	1    0    0    -1  
$EndComp
$Comp
L R R901
U 1 1 5D36DCBF
P 6150 2850
F 0 "R901" V 6230 2850 40  0000 C CNN
F 1 "60 Ohms" V 6157 2851 40  0000 C CNN
F 2 "~" V 6080 2850 30  0000 C CNN
F 3 "~" H 6150 2850 30  0000 C CNN
	1    6150 2850
	0    -1   -1   0   
$EndComp
$Comp
L R R902
U 1 1 5D36DCCE
P 6150 3750
F 0 "R902" V 6230 3750 40  0000 C CNN
F 1 "60 Ohms" V 6157 3751 40  0000 C CNN
F 2 "~" V 6080 3750 30  0000 C CNN
F 3 "~" H 6150 3750 30  0000 C CNN
	1    6150 3750
	0    -1   -1   0   
$EndComp
$Comp
L CONN_5 P901
U 1 1 5D36DCDD
P 7000 3300
F 0 "P901" V 6950 3300 50  0000 C CNN
F 1 "ABS Slave Bus" V 7050 3300 50  0000 C CNN
F 2 "" H 7000 3300 60  0000 C CNN
F 3 "" H 7000 3300 60  0000 C CNN
	1    7000 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 3050 6100 3050
Wire Wire Line
	6100 3050 6100 3400
Wire Wire Line
	6100 3400 6600 3400
Wire Wire Line
	6600 3300 6200 3300
Wire Wire Line
	6200 3300 6200 3150
Wire Wire Line
	5850 3150 6400 3150
Wire Wire Line
	6600 3200 5950 3200
Wire Wire Line
	5950 3200 5950 3450
Wire Wire Line
	5850 3450 6400 3450
Wire Wire Line
	6600 3100 6450 3100
Wire Wire Line
	6450 3100 6450 3550
Wire Wire Line
	6450 3550 5850 3550
Wire Wire Line
	5350 4050 5350 3850
Wire Wire Line
	5350 2750 5350 2500
Wire Wire Line
	5900 3750 5900 3550
Connection ~ 5900 3550
Wire Wire Line
	6400 3450 6400 3750
Connection ~ 5950 3450
Wire Wire Line
	5900 2850 5900 3050
Connection ~ 5900 3050
Wire Wire Line
	6400 3150 6400 2850
Connection ~ 6200 3150
Wire Wire Line
	6600 3500 6600 3950
Wire Wire Line
	6600 3950 5350 3950
Connection ~ 5350 3950
Text HLabel 4850 3050 0    60   Output ~ 0
RX
Text HLabel 4850 3450 0    60   Input ~ 0
TX
$EndSCHEMATC
