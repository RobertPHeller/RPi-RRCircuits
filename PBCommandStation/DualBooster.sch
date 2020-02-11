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
LIBS:sn65hvd233-ht
LIBS:lmd18200
LIBS:sn75als174
LIBS:tmp36
LIBS:cl2
LIBS:dpdt
LIBS:gbu4a
LIBS:PBCommandStation-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
Title "BeagleBone Black based OpenLCB/DCC Command Station"
Date "10 nov 2019"
Rev "1.0"
Comp "Deepwoods Software"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L LMD18200 U401
U 1 1 5DBDEFD1
P 2800 2200
F 0 "U401" H 2800 2200 60  0000 C CNN
F 1 "LMD18200" H 2800 2300 60  0000 C CNN
F 2 "~" H 2800 2200 60  0000 C CNN
F 3 "~" H 2800 2200 60  0000 C CNN
	1    2800 2200
	1    0    0    -1  
$EndComp
$Comp
L LMD18200 U403
U 1 1 5DBDEFD2
P 6100 2200
F 0 "U403" H 6100 2200 60  0000 C CNN
F 1 "LMD18200" H 6100 2300 60  0000 C CNN
F 2 "~" H 6100 2200 60  0000 C CNN
F 3 "~" H 6100 2200 60  0000 C CNN
	1    6100 2200
	1    0    0    -1  
$EndComp
$Comp
L C C401
U 1 1 5DBDEFD3
P 2450 2700
F 0 "C401" H 2450 2800 40  0000 L CNN
F 1 ".01 uf" H 2456 2615 40  0000 L CNN
F 2 "~" H 2488 2550 30  0000 C CNN
F 3 "~" H 2450 2700 60  0000 C CNN
	1    2450 2700
	-1   0    0    1   
$EndComp
$Comp
L C C402
U 1 1 5DBDEFD4
P 3150 2700
F 0 "C402" H 3150 2800 40  0000 L CNN
F 1 ".01 uf" H 3156 2615 40  0000 L CNN
F 2 "~" H 3188 2550 30  0000 C CNN
F 3 "~" H 3150 2700 60  0000 C CNN
	1    3150 2700
	-1   0    0    1   
$EndComp
$Comp
L C C403
U 1 1 5DBDEFD5
P 5750 2700
F 0 "C403" H 5750 2800 40  0000 L CNN
F 1 ".01 uf" H 5756 2615 40  0000 L CNN
F 2 "~" H 5788 2550 30  0000 C CNN
F 3 "~" H 5750 2700 60  0000 C CNN
	1    5750 2700
	-1   0    0    1   
$EndComp
$Comp
L C C404
U 1 1 5DBDEFD6
P 6450 2700
F 0 "C404" H 6450 2800 40  0000 L CNN
F 1 ".01 uf" H 6456 2615 40  0000 L CNN
F 2 "~" H 6488 2550 30  0000 C CNN
F 3 "~" H 6450 2700 60  0000 C CNN
	1    6450 2700
	-1   0    0    1   
$EndComp
$Comp
L R R401
U 1 1 5DBDEFD7
P 2550 3850
F 0 "R401" V 2630 3850 40  0000 C CNN
F 1 "10K Ohms" V 2557 3851 40  0000 C CNN
F 2 "~" V 2480 3850 30  0000 C CNN
F 3 "~" H 2550 3850 30  0000 C CNN
	1    2550 3850
	1    0    0    -1  
$EndComp
$Comp
L R R402
U 1 1 5DBDEFD8
P 2900 3850
F 0 "R402" V 2980 3850 40  0000 C CNN
F 1 "1.591K Ohms" V 2907 3851 40  0000 C CNN
F 2 "~" V 2830 3850 30  0000 C CNN
F 3 "~" H 2900 3850 30  0000 C CNN
	1    2900 3850
	1    0    0    -1  
$EndComp
$Comp
L R R403
U 1 1 5DBDEFD9
P 6000 3850
F 0 "R403" V 6080 3850 40  0000 C CNN
F 1 "10K Ohms" V 6007 3851 40  0000 C CNN
F 2 "~" V 5930 3850 30  0000 C CNN
F 3 "~" H 6000 3850 30  0000 C CNN
	1    6000 3850
	1    0    0    -1  
$EndComp
$Comp
L R R404
U 1 1 5DBDEFDA
P 6200 3800
F 0 "R404" V 6280 3800 40  0000 C CNN
F 1 "1.591K Ohms" V 6207 3801 40  0000 C CNN
F 2 "~" V 6130 3800 30  0000 C CNN
F 3 "~" H 6200 3800 30  0000 C CNN
	1    6200 3800
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 T401
U 1 1 5DBDEFDB
P 1250 2950
F 0 "T401" V 1200 2950 40  0000 C CNN
F 1 "+ Power In -" V 1300 2950 40  0000 C CNN
F 2 "~" H 1250 2950 60  0000 C CNN
F 3 "~" H 1250 2950 60  0000 C CNN
	1    1250 2950
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR021
U 1 1 5DBDEFDC
P 1600 3200
F 0 "#PWR021" H 1600 3200 30  0001 C CNN
F 1 "GND" H 1600 3130 30  0001 C CNN
F 2 "" H 1600 3200 60  0000 C CNN
F 3 "" H 1600 3200 60  0000 C CNN
	1    1600 3200
	1    0    0    -1  
$EndComp
$Comp
L VCOM #PWR401
U 1 1 5DBDEFDD
P 1600 2700
F 0 "#PWR401" H 1600 2900 40  0001 C CNN
F 1 "VCOM" H 1600 2850 40  0000 C CNN
F 2 "" H 1600 2700 60  0000 C CNN
F 3 "" H 1600 2700 60  0000 C CNN
	1    1600 2700
	1    0    0    -1  
$EndComp
$Comp
L VCOM #PWR410
U 1 1 5DBDEFDE
P 6100 2600
F 0 "#PWR410" H 6100 2800 40  0001 C CNN
F 1 "VCOM" H 6100 2750 40  0000 C CNN
F 2 "" H 6100 2600 60  0000 C CNN
F 3 "" H 6100 2600 60  0000 C CNN
	1    6100 2600
	-1   0    0    1   
$EndComp
$Comp
L VCOM #PWR404
U 1 1 5DBDEFDF
P 2800 2600
F 0 "#PWR404" H 2800 2800 40  0001 C CNN
F 1 "VCOM" H 2800 2750 40  0000 C CNN
F 2 "" H 2800 2600 60  0000 C CNN
F 3 "" H 2800 2600 60  0000 C CNN
	1    2800 2600
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR022
U 1 1 5DBDEFE0
P 2850 2800
F 0 "#PWR022" H 2850 2800 30  0001 C CNN
F 1 "GND" H 2850 2730 30  0001 C CNN
F 2 "" H 2850 2800 60  0000 C CNN
F 3 "" H 2850 2800 60  0000 C CNN
	1    2850 2800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR023
U 1 1 5DBDEFE1
P 6150 2800
F 0 "#PWR023" H 6150 2800 30  0001 C CNN
F 1 "GND" H 6150 2730 30  0001 C CNN
F 2 "" H 6150 2800 60  0000 C CNN
F 3 "" H 6150 2800 60  0000 C CNN
	1    6150 2800
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 T402
U 1 1 5DBDEFE2
P 3750 3000
F 0 "T402" V 3700 3000 40  0000 C CNN
F 1 "DCC 1" V 3800 3000 40  0000 C CNN
F 2 "~" H 3750 3000 60  0000 C CNN
F 3 "~" H 3750 3000 60  0000 C CNN
	1    3750 3000
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 T403
U 1 1 5DBDEFE3
P 7050 3000
F 0 "T403" V 7000 3000 40  0000 C CNN
F 1 "DCC 2" V 7100 3000 40  0000 C CNN
F 2 "~" H 7050 3000 60  0000 C CNN
F 3 "~" H 7050 3000 60  0000 C CNN
	1    7050 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 2500 2450 2500
Wire Wire Line
	3050 2500 3150 2500
Wire Wire Line
	2600 2500 2600 3100
Wire Wire Line
	2600 2900 2450 2900
Wire Wire Line
	3000 2500 3000 2900
Wire Wire Line
	3000 2900 3400 2900
Wire Wire Line
	5850 2500 5750 2500
Wire Wire Line
	6350 2500 6450 2500
Wire Wire Line
	6300 2500 6300 2900
Wire Wire Line
	6300 2900 6700 2900
Wire Wire Line
	5900 2500 5900 3100
Wire Wire Line
	5900 2900 5750 2900
Wire Wire Line
	1600 3050 1600 3200
Wire Wire Line
	1600 2700 1600 2850
Wire Wire Line
	2800 2500 2800 2600
Wire Wire Line
	6100 2500 6100 2600
Wire Wire Line
	6150 2500 6150 2800
Wire Wire Line
	2850 2500 2850 2800
Connection ~ 3150 2900
Wire Wire Line
	2600 3100 3400 3100
Connection ~ 2600 2900
Connection ~ 6450 2900
Wire Wire Line
	5900 3100 6700 3100
Connection ~ 5900 2900
Wire Wire Line
	2650 2500 2650 3150
Wire Wire Line
	2650 3150 2300 3150
Wire Wire Line
	2700 2500 2700 3250
Wire Wire Line
	2700 3250 2300 3250
Wire Wire Line
	2750 3050 2750 2500
Wire Wire Line
	2300 3050 2750 3050
Wire Wire Line
	2900 2500 2900 3600
Wire Wire Line
	2900 3350 2300 3350
Wire Wire Line
	2950 3450 2950 2500
Connection ~ 2900 3350
Wire Wire Line
	2300 3450 2950 3450
Wire Wire Line
	2550 3600 2550 3450
Connection ~ 2550 3450
Wire Wire Line
	2550 4100 2400 4100
Wire Wire Line
	2400 4100 2400 3050
Connection ~ 2400 3050
Wire Wire Line
	5950 2500 5950 3150
Wire Wire Line
	5950 3150 5600 3150
Wire Wire Line
	6000 2500 6000 3250
Wire Wire Line
	6000 3250 5600 3250
Wire Wire Line
	6050 3000 6050 2500
Wire Wire Line
	5600 3000 6050 3000
Wire Wire Line
	6200 2500 6200 3550
Wire Wire Line
	5600 3350 6600 3350
Wire Wire Line
	6250 3450 6250 2500
Wire Wire Line
	5600 3450 6250 3450
Connection ~ 6200 3350
$Comp
L GND #PWR024
U 1 1 5DBDEFE8
P 2900 4200
F 0 "#PWR024" H 2900 4200 30  0001 C CNN
F 1 "GND" H 2900 4130 30  0001 C CNN
F 2 "" H 2900 4200 60  0000 C CNN
F 3 "" H 2900 4200 60  0000 C CNN
	1    2900 4200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR025
U 1 1 5DBDEFE9
P 6200 4150
F 0 "#PWR025" H 6200 4150 30  0001 C CNN
F 1 "GND" H 6200 4080 30  0001 C CNN
F 2 "" H 6200 4150 60  0000 C CNN
F 3 "" H 6200 4150 60  0000 C CNN
	1    6200 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 4050 6200 4150
Wire Wire Line
	6000 3600 6000 3450
Connection ~ 6000 3450
Wire Wire Line
	6000 4100 5800 4100
Wire Wire Line
	5800 4100 5800 3000
Connection ~ 5800 3000
Wire Wire Line
	2900 4100 2900 4200
$Comp
L TMP36 U402
U 1 1 5DBDEFEA
P 3800 5150
F 0 "U402" H 3900 5000 60  0000 C CNN
F 1 "TMP36" H 4000 5400 60  0000 C CNN
F 2 "~" H 3800 5150 60  0000 C CNN
F 3 "~" H 3800 5150 60  0000 C CNN
	1    3800 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 5150 4050 5150
$Comp
L TIP120 Q401
U 1 1 5DBDEFED
P 7600 5200
F 0 "Q401" H 7550 5050 40  0000 R CNN
F 1 "TIP120" H 7600 5350 40  0000 R CNN
F 2 "TO220" H 7480 5305 29  0001 C CNN
F 3 "" H 7600 5200 60  0000 C CNN
	1    7600 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 4100 6950 4100
Wire Wire Line
	6950 4100 6950 5400
Wire Wire Line
	6950 5400 7700 5400
Connection ~ 6200 4100
$Comp
L ZENER D401
U 1 1 5DBDEFEE
P 7850 4800
F 0 "D401" H 7850 4900 50  0000 C CNN
F 1 "1n5349" H 7850 4700 40  0000 C CNN
F 2 "~" H 7850 4800 60  0000 C CNN
F 3 "~" H 7850 4800 60  0000 C CNN
	1    7850 4800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7700 5000 8100 5000
$Comp
L R R406
U 1 1 5DBDEFEF
P 7850 4200
F 0 "R406" V 7930 4200 40  0000 C CNN
F 1 "33 Ohms 1W" V 7857 4201 40  0000 C CNN
F 2 "~" V 7780 4200 30  0000 C CNN
F 3 "~" H 7850 4200 30  0000 C CNN
	1    7850 4200
	1    0    0    -1  
$EndComp
$Comp
L VCOM #PWR413
U 1 1 5DBDEFF0
P 7850 3900
F 0 "#PWR413" H 7850 4100 40  0001 C CNN
F 1 "VCOM" H 7850 4050 40  0000 C CNN
F 2 "" H 7850 3900 60  0000 C CNN
F 3 "" H 7850 3900 60  0000 C CNN
	1    7850 3900
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 H401
U 1 1 5DBDEFF1
P 8450 4650
F 0 "H401" V 8400 4650 40  0000 C CNN
F 1 "Fan" V 8500 4650 40  0000 C CNN
F 2 "~" H 8450 4650 60  0000 C CNN
F 3 "~" H 8450 4650 60  0000 C CNN
	1    8450 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 5000 8100 4750
Connection ~ 7850 5000
Wire Wire Line
	7850 3900 7850 3950
Wire Wire Line
	7850 4450 7850 4600
Wire Wire Line
	8100 4550 7850 4550
Connection ~ 7850 4550
$Comp
L R R405
U 1 1 5DBDEFF2
P 7050 5200
F 0 "R405" V 7130 5200 40  0000 C CNN
F 1 "1K Ohms" V 7057 5201 40  0000 C CNN
F 2 "~" V 6980 5200 30  0000 C CNN
F 3 "~" H 7050 5200 30  0000 C CNN
	1    7050 5200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7400 5200 7300 5200
Wire Wire Line
	6800 5200 6750 5200
$Comp
L +3.3V #PWR026
U 1 1 5DBE4EED
P 2300 3050
F 0 "#PWR026" H 2300 3010 30  0001 C CNN
F 1 "+3.3V" H 2300 3160 30  0000 C CNN
F 2 "" H 2300 3050 60  0000 C CNN
F 3 "" H 2300 3050 60  0000 C CNN
	1    2300 3050
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR027
U 1 1 5DBE4F0A
P 5600 2950
F 0 "#PWR027" H 5600 2910 30  0001 C CNN
F 1 "+3.3V" H 5600 3060 30  0000 C CNN
F 2 "" H 5600 2950 60  0000 C CNN
F 3 "" H 5600 2950 60  0000 C CNN
	1    5600 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 3000 5600 2950
$Comp
L +3.3V #PWR028
U 1 1 5DBE4F83
P 3800 4700
F 0 "#PWR028" H 3800 4660 30  0001 C CNN
F 1 "+3.3V" H 3800 4810 30  0000 C CNN
F 2 "" H 3800 4700 60  0000 C CNN
F 3 "" H 3800 4700 60  0000 C CNN
	1    3800 4700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR029
U 1 1 5DBE4F92
P 3800 5600
F 0 "#PWR029" H 3800 5600 30  0001 C CNN
F 1 "GND" H 3800 5530 30  0001 C CNN
F 2 "" H 3800 5600 60  0000 C CNN
F 3 "" H 3800 5600 60  0000 C CNN
	1    3800 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 4700 3800 4850
Wire Wire Line
	3800 5450 3800 5600
Text HLabel 2300 3150 0    60   Input ~ 0
DCC Signal 1
Text HLabel 2300 3250 0    60   Input ~ 0
Enable 1
Text HLabel 2300 3350 0    60   Output ~ 0
Current Sense 1
Text HLabel 2300 3450 0    60   Output ~ 0
Thermal Flag 1
Text HLabel 5600 3150 0    60   Input ~ 0
DCC Signal 2
Text HLabel 5600 3250 0    60   Input ~ 0
Enable 2
Text HLabel 5600 3350 0    60   Output ~ 0
Current Sense 2
Text HLabel 5600 3450 0    60   Output ~ 0
Thermal Flag 2
Text HLabel 6750 5200 0    60   Input ~ 0
Fan Control
Text HLabel 4600 5150 2    60   Output ~ 0
Temp Sense
$Comp
L LM393 U404
U 1 1 5DC833F0
P 9050 1550
F 0 "U404" H 9200 1700 60  0000 C CNN
F 1 "LM393" H 9250 1350 60  0000 C CNN
F 2 "" H 9050 1550 60  0000 C CNN
F 3 "" H 9050 1550 60  0000 C CNN
	1    9050 1550
	1    0    0    -1  
$EndComp
$Comp
L LM393 U404
U 2 1 5DC833FF
P 10600 2350
F 0 "U404" H 10750 2500 60  0000 C CNN
F 1 "LM393" H 10800 2150 60  0000 C CNN
F 2 "" H 10600 2350 60  0000 C CNN
F 3 "" H 10600 2350 60  0000 C CNN
	2    10600 2350
	1    0    0    -1  
$EndComp
$Comp
L C C408
U 1 1 5DC8340E
P 9950 2350
F 0 "C408" H 9950 2450 40  0000 L CNN
F 1 "1000 nf" H 9956 2265 40  0000 L CNN
F 2 "~" H 9988 2200 30  0000 C CNN
F 3 "~" H 9950 2350 60  0000 C CNN
	1    9950 2350
	1    0    0    -1  
$EndComp
$Comp
L C C407
U 1 1 5DC83427
P 8400 1550
F 0 "C407" H 8400 1650 40  0000 L CNN
F 1 "10 nf" H 8406 1465 40  0000 L CNN
F 2 "~" H 8438 1400 30  0000 C CNN
F 3 "~" H 8400 1550 60  0000 C CNN
	1    8400 1550
	1    0    0    -1  
$EndComp
$Comp
L C C406
U 1 1 5DC83436
P 8200 1900
F 0 "C406" H 8200 2000 40  0000 L CNN
F 1 "1 uf" H 8206 1815 40  0000 L CNN
F 2 "~" H 8238 1750 30  0000 C CNN
F 3 "~" H 8200 1900 60  0000 C CNN
	1    8200 1900
	1    0    0    -1  
$EndComp
$Comp
L C C405
U 1 1 5DC83445
P 7900 1650
F 0 "C405" H 7900 1750 40  0000 L CNN
F 1 "10 nf" H 7906 1565 40  0000 L CNN
F 2 "~" H 7938 1500 30  0000 C CNN
F 3 "~" H 7900 1650 60  0000 C CNN
	1    7900 1650
	1    0    0    -1  
$EndComp
$Comp
L R R411
U 1 1 5DC83454
P 9550 1050
F 0 "R411" V 9630 1050 40  0000 C CNN
F 1 "10K Ohms" V 9557 1051 40  0000 C CNN
F 2 "~" V 9480 1050 30  0000 C CNN
F 3 "~" H 9550 1050 30  0000 C CNN
	1    9550 1050
	1    0    0    -1  
$EndComp
$Comp
L R R410
U 1 1 5DC83463
P 8200 950
F 0 "R410" V 8280 950 40  0000 C CNN
F 1 "1M Ohms" V 8207 951 40  0000 C CNN
F 2 "~" V 8130 950 30  0000 C CNN
F 3 "~" H 8200 950 30  0000 C CNN
	1    8200 950 
	1    0    0    -1  
$EndComp
$Comp
L R R409
U 1 1 5DC83472
P 7900 950
F 0 "R409" V 7980 950 40  0000 C CNN
F 1 "1M Ohms" V 7907 951 40  0000 C CNN
F 2 "~" V 7830 950 30  0000 C CNN
F 3 "~" H 7900 950 30  0000 C CNN
	1    7900 950 
	1    0    0    -1  
$EndComp
$Comp
L R R407
U 1 1 5DC83481
P 7350 1450
F 0 "R407" V 7430 1450 40  0000 C CNN
F 1 "10K Ohms" V 7357 1451 40  0000 C CNN
F 2 "~" V 7280 1450 30  0000 C CNN
F 3 "~" H 7350 1450 30  0000 C CNN
	1    7350 1450
	0    -1   -1   0   
$EndComp
$Comp
L R R408
U 1 1 5DC83490
P 7350 1700
F 0 "R408" V 7430 1700 40  0000 C CNN
F 1 "13K Ohms" V 7357 1701 40  0000 C CNN
F 2 "~" V 7280 1700 30  0000 C CNN
F 3 "~" H 7350 1700 30  0000 C CNN
	1    7350 1700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10100 2250 10100 1950
Wire Wire Line
	10100 1950 10500 1950
Wire Wire Line
	9950 2150 10100 2150
Connection ~ 10100 2150
Wire Wire Line
	10500 2750 10100 2750
Wire Wire Line
	10100 2750 10100 2450
Wire Wire Line
	9950 2550 10100 2550
Wire Wire Line
	10100 2550 10100 2600
Connection ~ 10100 2600
$Comp
L GND #PWR030
U 1 1 5DC835EE
P 10500 2850
F 0 "#PWR030" H 10500 2850 30  0001 C CNN
F 1 "GND" H 10500 2780 30  0001 C CNN
F 2 "" H 10500 2850 60  0000 C CNN
F 3 "" H 10500 2850 60  0000 C CNN
	1    10500 2850
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR031
U 1 1 5DC835FD
P 10500 1800
F 0 "#PWR031" H 10500 1760 30  0001 C CNN
F 1 "+3.3V" H 10500 1910 30  0000 C CNN
F 2 "" H 10500 1800 60  0000 C CNN
F 3 "" H 10500 1800 60  0000 C CNN
	1    10500 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	10500 1950 10500 1800
Wire Wire Line
	10500 2750 10500 2850
Wire Wire Line
	9550 1300 9550 3500
Wire Wire Line
	7600 1450 8550 1450
Connection ~ 7900 1450
Wire Wire Line
	8550 1650 8200 1650
Wire Wire Line
	8200 1200 8200 1700
Wire Wire Line
	8200 1700 7600 1700
Wire Wire Line
	8400 1350 8500 1350
Wire Wire Line
	8500 1350 8500 1450
Connection ~ 8500 1450
Wire Wire Line
	8400 1750 8500 1750
Wire Wire Line
	8500 1750 8500 1650
Connection ~ 8500 1650
$Comp
L GND #PWR032
U 1 1 5DC83992
P 8200 2200
F 0 "#PWR032" H 8200 2200 30  0001 C CNN
F 1 "GND" H 8200 2130 30  0001 C CNN
F 2 "" H 8200 2200 60  0000 C CNN
F 3 "" H 8200 2200 60  0000 C CNN
	1    8200 2200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR033
U 1 1 5DC839A1
P 7900 2000
F 0 "#PWR033" H 7900 2000 30  0001 C CNN
F 1 "GND" H 7900 1930 30  0001 C CNN
F 2 "" H 7900 2000 60  0000 C CNN
F 3 "" H 7900 2000 60  0000 C CNN
	1    7900 2000
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR034
U 1 1 5DC839B0
P 8050 600
F 0 "#PWR034" H 8050 560 30  0001 C CNN
F 1 "+3.3V" H 8050 710 30  0000 C CNN
F 2 "" H 8050 600 60  0000 C CNN
F 3 "" H 8050 600 60  0000 C CNN
	1    8050 600 
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR035
U 1 1 5DC839BF
P 9550 700
F 0 "#PWR035" H 9550 660 30  0001 C CNN
F 1 "+3.3V" H 9550 810 30  0000 C CNN
F 2 "" H 9550 700 60  0000 C CNN
F 3 "" H 9550 700 60  0000 C CNN
	1    9550 700 
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 700  9550 800 
Wire Wire Line
	8050 600  8050 700 
Wire Wire Line
	7900 700  8200 700 
Connection ~ 8050 700 
Connection ~ 8200 1650
Wire Wire Line
	7900 1200 7900 1450
Wire Wire Line
	8200 2100 8200 2200
Wire Wire Line
	7900 1850 7900 2000
Wire Wire Line
	7100 1450 7100 2700
Wire Wire Line
	7100 2700 6600 2700
Wire Wire Line
	6600 2700 6600 3350
Connection ~ 7100 1700
Wire Wire Line
	9550 3500 5750 3500
Wire Wire Line
	5750 3500 5750 3550
Wire Wire Line
	5750 3550 5600 3550
Connection ~ 9550 1550
Text HLabel 5600 3550 0    60   Output ~ 0
Ack Detect
$EndSCHEMATC