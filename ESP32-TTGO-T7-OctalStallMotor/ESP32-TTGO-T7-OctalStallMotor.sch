EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
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
LIBS:ESP32_mini
LIBS:sn65hvd233-ht
LIBS:lm2574n-5
LIBS:tc442x
LIBS:4814p-2
LIBS:ESP32-TTGO-T7-OctalStallMotor-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L TTGO-T7-v13 U0
U 1 1 621A4385
P 2150 3750
F 0 "U0" H 2400 1800 50  0000 C CNN
F 1 "TTGO-T7-v13" H 2350 1400 50  0000 C CNN
F 2 "ESP32_mini:ESP32_mini" H 2300 3850 50  0001 C CNN
F 3 "" H 2300 3850 50  0001 C CNN
	1    2150 3750
	1    0    0    1   
$EndComp
Text Label 1650 3450 2    60   ~ 0
CAN_RX
Text Label 1650 3350 2    60   ~ 0
CAN_TX
NoConn ~ 1650 1750
NoConn ~ 1650 1650
NoConn ~ 2700 2550
NoConn ~ 2700 2450
$Comp
L GND #PWR01
U 1 1 621A45D6
P 2900 3350
F 0 "#PWR01" H 2900 3100 50  0001 C CNN
F 1 "GND" H 2900 3200 50  0000 C CNN
F 2 "" H 2900 3350 50  0001 C CNN
F 3 "" H 2900 3350 50  0001 C CNN
	1    2900 3350
	0    -1   1    0   
$EndComp
Wire Wire Line
	2700 3350 2900 3350
Connection ~ 2800 3350
$Comp
L +3V3 #PWR02
U 1 1 621A4618
P 2800 3550
F 0 "#PWR02" H 2800 3400 50  0001 C CNN
F 1 "+3V3" H 2800 3690 50  0000 C CNN
F 2 "" H 2800 3550 50  0001 C CNN
F 3 "" H 2800 3550 50  0001 C CNN
	1    2800 3550
	0    1    -1   0   
$EndComp
Wire Wire Line
	2800 3550 2700 3550
$Comp
L +5V #PWR03
U 1 1 621A463E
P 2800 3650
F 0 "#PWR03" H 2800 3500 50  0001 C CNN
F 1 "+5V" H 2800 3790 50  0000 C CNN
F 2 "" H 2800 3650 50  0001 C CNN
F 3 "" H 2800 3650 50  0001 C CNN
	1    2800 3650
	0    1    -1   0   
$EndComp
Wire Wire Line
	2700 3650 2800 3650
Text Label 1650 3650 2    60   ~ 0
Motor1
Text Label 1650 3550 2    60   ~ 0
Motor2
Text Label 1650 2750 2    60   ~ 0
Motor3
Text Label 1650 2650 2    60   ~ 0
Motor4
Text Label 1650 2550 2    60   ~ 0
Motor5
Text Label 1650 2450 2    60   ~ 0
Motor6
Text Label 1650 2350 2    60   ~ 0
Motor7
Text Label 1650 2250 2    60   ~ 0
Motor8
Text Label 1650 2150 2    60   ~ 0
Sense1
Text Label 1650 2050 2    60   ~ 0
Sense2
Text Label 1650 1950 2    60   ~ 0
Sense3
Text Label 1650 1850 2    60   ~ 0
Sense4
Text Label 1650 1550 2    60   ~ 0
Sense5
Text Label 1650 1450 2    60   ~ 0
Sense6
Text Label 1650 1350 2    60   ~ 0
Sense7
Text Label 1650 1250 2    60   ~ 0
Sense8
NoConn ~ 1650 3250
NoConn ~ 1650 3150
NoConn ~ 1650 3050
NoConn ~ 1650 2950
NoConn ~ 2700 2050
NoConn ~ 2700 2150
NoConn ~ 2700 2250
NoConn ~ 2700 2350
NoConn ~ 2700 2650
NoConn ~ 2700 2900
NoConn ~ 2700 3000
NoConn ~ 2700 3150
$Sheet
S 4650 800  950  850 
U 5C828AE5
F0 "CAN Transceiver" 50
F1 "CAN_Transeiver.sch" 50
F2 "CAN_TX" I L 4650 1150 60 
F3 "CAN_RX" I L 4650 1300 60 
$EndSheet
$Sheet
S 6400 550  1250 1050
U 5C828B13
F0 "Power Supply" 50
F1 "PowerSupply.sch" 50
$EndSheet
Text Label 4650 1150 2    60   ~ 0
CAN_TX
Text Label 4650 1300 2    60   ~ 0
CAN_RX
$Sheet
S 1950 4550 2950 2400
U 621AAE2E
F0 "Motors 5-8" 60
F1 "Motors5-8.sch" 60
F2 "Motor5" I L 1950 4650 60 
F3 "Sense5" O L 1950 4800 60 
F4 "Motor6" I L 1950 4950 60 
F5 "Sense6" O L 1950 5100 60 
F6 "Sense7" O L 1950 5400 60 
F7 "Sense8" O L 1950 5700 60 
F8 "Motor7" I L 1950 5250 60 
F9 "Motor8" I L 1950 5600 60 
$EndSheet
$Sheet
S 5800 2350 2850 2450
U 621AAEFF
F0 "Motors 1-4" 60
F1 "Motors1-4.sch" 60
F2 "Motor1" I L 5800 2550 60 
F3 "Sense1" O L 5800 2750 60 
F4 "Motor2" I L 5800 2900 60 
F5 "Sense2" O L 5800 3050 60 
F6 "Sense3" O L 5800 3350 60 
F7 "Sense4" O L 5800 3600 60 
F8 "Motor3" I L 5800 3200 60 
F9 "Motor4" I L 5800 3450 60 
$EndSheet
Text Label 5800 2550 2    60   ~ 0
Motor1
Text Label 5800 2750 2    60   ~ 0
Sense1
Text Label 5800 2900 2    60   ~ 0
Motor2
Text Label 5800 3050 2    60   ~ 0
Sense2
Text Label 5800 3200 2    60   ~ 0
Motor3
Text Label 5800 3350 2    60   ~ 0
Sense3
Text Label 5800 3450 2    60   ~ 0
Motor4
Text Label 5800 3600 2    60   ~ 0
Sense4
Text Label 1950 4650 2    60   ~ 0
Motor5
Text Label 1950 4800 2    60   ~ 0
Sense5
Text Label 1950 4950 2    60   ~ 0
Motor6
Text Label 1950 5100 2    60   ~ 0
Sense6
Text Label 1950 5250 2    60   ~ 0
Motor7
Text Label 1950 5400 2    60   ~ 0
Sense7
Text Label 1950 5600 2    60   ~ 0
Motor8
Text Label 1950 5700 2    60   ~ 0
Sense8
Wire Wire Line
	2700 3250 2800 3250
Wire Wire Line
	2800 3250 2800 3450
Wire Wire Line
	2800 3450 2700 3450
$EndSCHEMATC
