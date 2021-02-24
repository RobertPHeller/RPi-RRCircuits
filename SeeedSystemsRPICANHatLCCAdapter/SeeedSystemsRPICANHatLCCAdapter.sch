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
LIBS:SeeedSystemsRPICANHatLCCAdapter-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L Conn_01x03_Male J1
U 1 1 6035406B
P 3250 2550
F 0 "J1" H 3250 2750 50  0000 C CNN
F 1 "PC 1737022 " H 3250 2350 50  0000 C CNN
F 2 "MyPinHeaders:PinArray_3_RA_3.5MM" H 3250 2550 50  0001 C CNN
F 3 "" H 3250 2550 50  0001 C CNN
F 4 "651-1737022" H 3250 2550 60  0001 C CNN "Mouser Part Number"
	1    3250 2550
	1    0    0    -1  
$EndComp
$Comp
L RJ45 J2
U 1 1 603540A0
P 4550 2500
F 0 "J2" H 4750 3000 50  0000 C CNN
F 1 "Wurth 615008144221" H 4400 3000 50  0000 C CNN
F 2 "Connectors:RJ45_8" H 4550 2500 50  0001 C CNN
F 3 "" H 4550 2500 50  0001 C CNN
F 4 "710-615008144221 " H 4550 2500 60  0001 C CNN "Mouser Part Number"
	1    4550 2500
	0    1    1    0   
$EndComp
$Comp
L RJ45 J3
U 1 1 60354103
P 6450 2500
F 0 "J3" H 6650 3000 50  0000 C CNN
F 1 "Wurth 615008144221" H 6300 3000 50  0000 C CNN
F 2 "Connectors:RJ45_8" H 6450 2500 50  0001 C CNN
F 3 "" H 6450 2500 50  0001 C CNN
F 4 "710-615008144221 " H 6450 2500 60  0001 C CNN "Mouser Part Number"
	1    6450 2500
	0    1    1    0   
$EndComp
Wire Wire Line
	3700 2150 6000 2150
Wire Wire Line
	3750 2250 6000 2250
Wire Wire Line
	3850 2350 6000 2350
Wire Wire Line
	6000 2450 4100 2450
Wire Wire Line
	6000 2550 4100 2550
Wire Wire Line
	3450 2650 6000 2650
Wire Wire Line
	6000 2750 4100 2750
Wire Wire Line
	6000 2850 4100 2850
Text Label 3450 2450 0    60   ~ 0
CAN_H
Text Label 3450 2550 0    60   ~ 0
CAN_L
$Comp
L GND #PWR01
U 1 1 603542C4
P 3450 2750
F 0 "#PWR01" H 3450 2500 50  0001 C CNN
F 1 "GND" H 3450 2600 50  0000 C CNN
F 2 "" H 3450 2750 50  0001 C CNN
F 3 "" H 3450 2750 50  0001 C CNN
	1    3450 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 2650 3450 2750
Wire Wire Line
	3450 2450 3700 2450
Wire Wire Line
	3700 2450 3700 2150
Connection ~ 4100 2150
Wire Wire Line
	3450 2550 3750 2550
Wire Wire Line
	3750 2550 3750 2250
Connection ~ 4100 2250
Wire Wire Line
	3850 2650 3850 2350
Connection ~ 4100 2350
Connection ~ 3850 2650
Connection ~ 4100 2650
$EndSCHEMATC
