EESchema Schematic File Version 2
LIBS:ESP32-16PWMLedDriver-rescue
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
LIBS:valves
LIBS:esp32_devboards
LIBS:smalljumper
LIBS:sn65hvd233-ht
LIBS:lm2574n-5
LIBS:MCP23xxx
LIBS:tbd62x83a
LIBS:pca9685
LIBS:ESP32-16PWMLedDriver-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
Title ""
Date "30 jan 2020"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ESP32_DEVKIT_C U0
U 1 1 5C81CCAA
P 3100 2000
F 0 "U0" H 3100 2000 60  0000 C CNN
F 1 "ESP32_DEVKIT_C or TTGO-T1" H 3100 2550 60  0000 C CNN
F 2 "~" H 3100 2000 60  0000 C CNN
F 3 "~" H 3100 2000 60  0000 C CNN
	1    3100 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 1750 3850 2600
Wire Wire Line
	3850 2600 2650 2600
Wire Wire Line
	2650 2600 2650 2450
$Comp
L GND-RESCUE-ESP32-16PWMLedDriver #PWR01
U 1 1 5C81D000
P 2500 2200
F 0 "#PWR01" H 2500 2200 30  0001 C CNN
F 1 "GND" H 2500 2130 30  0001 C CNN
F 2 "" H 2500 2200 60  0000 C CNN
F 3 "" H 2500 2200 60  0000 C CNN
	1    2500 2200
	0    1    1    0   
$EndComp
Wire Wire Line
	2650 2200 2500 2200
$Comp
L +5V #PWR02
U 1 1 5C81D01A
P 3850 1750
F 0 "#PWR02" H 3850 1840 20  0001 C CNN
F 1 "+5V" H 3850 1840 30  0000 C CNN
F 2 "" H 3850 1750 60  0000 C CNN
F 3 "" H 3850 1750 60  0000 C CNN
	1    3850 1750
	1    0    0    -1  
$EndComp
$Comp
L GND-RESCUE-ESP32-16PWMLedDriver #PWR03
U 1 1 5C81D02F
P 3700 1550
F 0 "#PWR03" H 3700 1550 30  0001 C CNN
F 1 "GND" H 3700 1480 30  0001 C CNN
F 2 "" H 3700 1550 60  0000 C CNN
F 3 "" H 3700 1550 60  0000 C CNN
	1    3700 1550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3700 1550 3550 1550
$Comp
L 3V3 #PWR04
U 1 1 5C81D08D
P 2650 1450
F 0 "#PWR04" H 2650 1550 40  0001 C CNN
F 1 "3V3" H 2650 1575 40  0000 C CNN
F 2 "" H 2650 1450 60  0000 C CNN
F 3 "" H 2650 1450 60  0000 C CNN
	1    2650 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 1450 2650 1550
NoConn ~ 3550 2450
Text Label 3550 2150 0    25   ~ 0
CAN_RX
Text Label 3550 2000 0    25   ~ 0
CAN_TX
Text Label 3550 1800 0    25   ~ 0
SDA
Text Label 3550 1650 0    25   ~ 0
SCL
Text Label 3550 2300 0    25   ~ 0
MOSI
Text Label 3550 2250 0    25   ~ 0
MISO
Text Label 2650 2100 2    25   ~ 0
CLK
$Sheet
S 4300 1650 950  850 
U 5C828AE5
F0 "CAN Transceiver" 50
F1 "CAN_Transeiver.sch" 50
F2 "CAN_TX" I L 4300 2000 60 
F3 "CAN_RX" I L 4300 2150 60 
$EndSheet
$Sheet
S 2700 2800 1250 1050
U 5C828B13
F0 "Power Supply" 50
F1 "PowerSupply.sch" 50
$EndSheet
Wire Wire Line
	4300 2000 3550 2000
Wire Wire Line
	4300 2150 3550 2150
$Sheet
S 4450 3000 1950 1650
U 5E334987
F0 "PCA9685 Driver" 50
F1 "PCA9685Driver.sch" 50
F2 "SCL" I L 4450 3300 60 
F3 "SDA" B L 4450 3550 60 
F4 "OE" I L 4450 3750 60 
$EndSheet
Text Label 4450 3300 2    100  ~ 0
SCL
Text Label 4450 3550 2    100  ~ 0
SDA
Text Label 4450 3750 2    100  ~ 0
OE
Text Label 3550 2100 0    25   ~ 0
OE
$EndSCHEMATC
