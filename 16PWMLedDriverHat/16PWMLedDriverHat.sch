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
LIBS:pca9685
LIBS:tbd62x83a
LIBS:16PWMLedDriverHat-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
Title ""
Date "9 mar 2019"
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
P 1950 1550
F 0 "J0" H 2700 1800 60  0000 C CNN
F 1 "RPi_GPIO" H 2700 1700 60  0000 C CNN
F 2 "RPi_Hat:Pin_Header_Straight_2x20" H 1950 1550 60  0001 C CNN
F 3 "" H 1950 1550 60  0000 C CNN
	1    1950 1550
	1    0    0    -1  
$EndComp
Text Label 3650 3450 0    60   ~ 0
GPIO.21
Text Label 1750 1650 2    60   ~ 0
SDA
Text Label 1750 1750 2    60   ~ 0
SCL
$Comp
L +3.3V #PWR01
U 1 1 5C840943
P 1750 1250
F 0 "#PWR01" H 1750 1210 30  0001 C CNN
F 1 "+3.3V" H 1750 1360 30  0000 C CNN
F 2 "" H 1750 1250 60  0000 C CNN
F 3 "" H 1750 1250 60  0000 C CNN
	1    1750 1250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 5C840952
P 2800 3900
F 0 "#PWR02" H 2800 3900 30  0001 C CNN
F 1 "GND" H 2800 3830 30  0001 C CNN
F 2 "" H 2800 3900 60  0000 C CNN
F 3 "" H 2800 3900 60  0000 C CNN
	1    2800 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1750 1250 1750 1550
Wire Wire Line
	1750 2350 1500 2350
Wire Wire Line
	1500 2350 1500 1400
Wire Wire Line
	1500 1400 1750 1400
Connection ~ 1750 1400
Wire Wire Line
	1750 1950 1550 1950
Wire Wire Line
	1550 1950 1550 3700
Wire Wire Line
	1550 3700 4150 3700
Wire Wire Line
	4150 3700 4150 1750
Wire Wire Line
	4150 3150 3650 3150
Wire Wire Line
	2800 3900 2800 3700
Connection ~ 2800 3700
Wire Wire Line
	1750 2750 1550 2750
Connection ~ 1550 2750
Wire Wire Line
	1750 3450 1550 3450
Connection ~ 1550 3450
Wire Wire Line
	4150 2950 3650 2950
Connection ~ 4150 3150
Wire Wire Line
	4150 2450 3650 2450
Connection ~ 4150 2950
Wire Wire Line
	4150 2150 3650 2150
Connection ~ 4150 2450
Wire Wire Line
	4150 1750 3650 1750
Connection ~ 4150 2150
$Sheet
S 5400 1700 2100 2200
U 5C840A32
F0 "PCA9685 Driver" 50
F1 "PCA9685Driver.sch" 50
F2 "SCL" I L 5400 2000 60 
F3 "SDA" B L 5400 2250 60 
F4 "OE" I L 5400 2550 60 
$EndSheet
Text Label 5400 2000 2    100  ~ 0
SCL
Text Label 5400 2250 2    100  ~ 0
SDA
Text Label 5400 2550 2    100  ~ 0
GPIO.21
$Comp
L +5V #PWR03
U 1 1 5C84263F
P 3650 1350
F 0 "#PWR03" H 3650 1440 20  0001 C CNN
F 1 "+5V" H 3650 1440 30  0000 C CNN
F 2 "" H 3650 1350 60  0000 C CNN
F 3 "" H 3650 1350 60  0000 C CNN
	1    3650 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 1350 3650 1650
Connection ~ 3650 1550
$EndSCHEMATC
