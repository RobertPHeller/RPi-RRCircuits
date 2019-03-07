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
LIBS:MCP23xxx
LIBS:tbd62x83a
LIBS:PocketBeagle16LedDriver-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
Title ""
Date "7 mar 2019"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L LM2574N-5.0 U3
U 1 1 5C7D307C
P 5850 3450
F 0 "U3" H 5850 3500 60  0000 C CNN
F 1 "LM2574N-5.0" H 5850 3600 21  0000 C CNN
F 2 "~" H 5850 3450 60  0000 C CNN
F 3 "~" H 5850 3450 60  0000 C CNN
	1    5850 3450
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR_SMALL L1
U 1 1 5C7D34C6
P 5150 3550
F 0 "L1" H 5150 3650 50  0000 C CNN
F 1 "330 uh" H 5150 3500 50  0000 C CNN
F 2 "PE-LowProfile" H 5150 3550 60  0001 C CNN
F 3 "~" H 5150 3550 60  0000 C CNN
F 4 "PE-52627NL" H 5150 3550 60  0001 C CNN "Part Number"
	1    5150 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 3550 4900 3250
Wire Wire Line
	4300 3250 5550 3250
$Comp
L CP1 C4
U 1 1 5C7D358B
P 4650 3450
F 0 "C4" H 4700 3550 50  0000 L CNN
F 1 "220 uf 25V" V 4500 3250 50  0000 L CNN
F 2 "C1-3V8" H 4650 3450 60  0001 C CNN
F 3 "~" H 4650 3450 60  0000 C CNN
F 4 "REA221M1EBK-0811P" H 4650 3450 60  0001 C CNN "Part Number"
	1    4650 3450
	1    0    0    -1  
$EndComp
Connection ~ 4900 3250
$Comp
L GND #PWR014
U 1 1 5C7D35B7
P 5500 4050
F 0 "#PWR014" H 5500 4050 30  0001 C CNN
F 1 "GND" H 5500 3980 30  0001 C CNN
F 2 "" H 5500 4050 60  0000 C CNN
F 3 "" H 5500 4050 60  0000 C CNN
	1    5500 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 3800 6550 3800
Connection ~ 5900 3800
Connection ~ 5800 3800
$Comp
L CP1 C5
U 1 1 5C7D3DB0
P 6550 3550
F 0 "C5" H 6600 3650 50  0000 L CNN
F 1 "22 uf 100V" H 6600 3450 50  0000 L CNN
F 2 "C1-3V8" H 6550 3550 60  0001 C CNN
F 3 "~" H 6550 3550 60  0000 C CNN
F 4 "REA220M2ABK-0811P" H 6550 3550 60  0001 C CNN "Part Number"
	1    6550 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 3350 6850 3350
Wire Wire Line
	6550 3800 6550 3750
Connection ~ 6000 3800
$Comp
L DIODESCH D3
U 1 1 5C7D3F8E
P 5500 3750
F 0 "D3" H 5500 3850 40  0000 C CNN
F 1 "SB160-E3/54" H 5500 3650 40  0000 C CNN
F 2 "DO-41" H 5500 3750 60  0001 C CNN
F 3 "~" H 5500 3750 60  0000 C CNN
F 4 "SB160-E3/54 " H 5500 3750 60  0001 C CNN "Part Number"
	1    5500 3750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5400 3550 5550 3550
Connection ~ 5500 3550
Wire Wire Line
	5800 3950 5800 3800
Wire Wire Line
	4650 3950 5800 3950
Wire Wire Line
	5500 3950 5500 4050
Wire Wire Line
	4650 3950 4650 3650
Connection ~ 5500 3950
Text HLabel 4300 3250 0    60   Output ~ 0
5V Out
Connection ~ 4650 3250
Text HLabel 6850 3350 2    60   Input ~ 0
12-15 Volt input
Connection ~ 6550 3350
$EndSCHEMATC
