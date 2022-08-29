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
LIBS:ESP32-C3FN4
LIBS:USBLC6-2SC6
LIBS:sn65hvd233-ht
LIBS:lm2574n-5
LIBS:DRV8873HPWPR
LIBS:mcp9701at
LIBS:MCP9700T-E_LT
LIBS:ina180b
LIBS:mechanical
LIBS:ESP32-C3-Booster-cache
EELAYER 25 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 2 6
Title "ESP32-C3 Booster"
Date ""
Rev "1.0"
Comp "Deepwoods Software"
Comment1 "ESP32-C3 MCU"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L USBLC6-2SC6 IC202
U 1 1 630A2C56
P 7200 1650
F 0 "IC202" H 8350 1950 50  0000 L CNN
F 1 "USBLC6-2SC6" H 8350 1850 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-6" H 8350 1750 50  0001 L CNN
F 3 "https://componentsearchengine.com//USBLC6-2SC6.pdf" H 8350 1650 50  0001 L CNN
F 4 "TVS Diode Array Uni-Directional USBLC6-2SC6 17V, SOT-23 6-Pin" H 8350 1550 50  0001 L CNN "Description"
F 5 "1.45" H 8350 1450 50  0001 L CNN "Height"
F 6 "511-USBLC6-2SC6" H 8350 1350 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.com/Search/Refine.aspx?Keyword=511-USBLC6-2SC6" H 8350 1250 50  0001 L CNN "Mouser2 Price/Stock"
F 8 "STMicroelectronics" H 8350 1150 50  0001 L CNN "Manufacturer_Name"
F 9 "USBLC6-2SC6" H 8350 1050 50  0001 L CNN "Manufacturer_Part_Number"
	1    7200 1650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 630A2E8F
P 7050 1750
F 0 "#PWR08" H 7050 1500 50  0001 C CNN
F 1 "GND" H 7050 1600 50  0000 C CNN
F 2 "" H 7050 1750 50  0001 C CNN
F 3 "" H 7050 1750 50  0001 C CNN
	1    7050 1750
	0    1    1    0   
$EndComp
$Comp
L USB_OTG J201
U 1 1 630A2ECD
P 9600 1650
F 0 "J201" H 9400 2100 50  0000 L CNN
F 1 "USB_OTG" H 9400 2000 50  0000 L CNN
F 2 "Connectors_USB:USB_Micro-B_Molex_47346-0001" H 9750 1600 50  0001 C CNN
F 3 "" H 9750 1600 50  0001 C CNN
F 4 "538-47346-0001" H 9600 1650 60  0001 C CNN "Mouser Part Number"
	1    9600 1650
	-1   0    0    -1  
$EndComp
$Comp
L VBUS #PWR09
U 1 1 630A2F74
P 9050 1300
F 0 "#PWR09" H 9050 1150 50  0001 C CNN
F 1 "VBUS" H 9050 1450 50  0000 C CNN
F 2 "" H 9050 1300 50  0001 C CNN
F 3 "" H 9050 1300 50  0001 C CNN
	1    9050 1300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 630A2FB4
P 9700 2200
F 0 "#PWR010" H 9700 1950 50  0001 C CNN
F 1 "GND" H 9700 2050 50  0000 C CNN
F 2 "" H 9700 2200 50  0001 C CNN
F 3 "" H 9700 2200 50  0001 C CNN
	1    9700 2200
	1    0    0    -1  
$EndComp
$Comp
L C_Small C202
U 1 1 630A2FE3
P 6900 2000
F 0 "C202" H 6910 2070 50  0000 L CNN
F 1 "22pf" H 6910 1920 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 6900 2000 50  0001 C CNN
F 3 "" H 6900 2000 50  0001 C CNN
F 4 "187-CL05C220JB5NNND" H 6900 2000 60  0001 C CNN "Mouser Part Number"
	1    6900 2000
	1    0    0    -1  
$EndComp
$Comp
L C_Small C201
U 1 1 630A3006
P 6900 1500
F 0 "C201" H 6910 1570 50  0000 L CNN
F 1 "22pf" H 6910 1420 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 6900 1500 50  0001 C CNN
F 3 "" H 6900 1500 50  0001 C CNN
F 4 "187-CL05C220JB5NNND" H 6900 1500 60  0001 C CNN "Mouser Part Number"
	1    6900 1500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 630A3059
P 6900 1300
F 0 "#PWR011" H 6900 1050 50  0001 C CNN
F 1 "GND" H 6900 1150 50  0000 C CNN
F 2 "" H 6900 1300 50  0001 C CNN
F 3 "" H 6900 1300 50  0001 C CNN
	1    6900 1300
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR012
U 1 1 630A3077
P 6900 2250
F 0 "#PWR012" H 6900 2000 50  0001 C CNN
F 1 "GND" H 6900 2100 50  0000 C CNN
F 2 "" H 6900 2250 50  0001 C CNN
F 3 "" H 6900 2250 50  0001 C CNN
	1    6900 2250
	1    0    0    -1  
$EndComp
$Comp
L R R204
U 1 1 630AB75B
P 6000 2550
F 0 "R204" V 6080 2550 50  0000 C CNN
F 1 "0" V 6000 2550 50  0000 C CNN
F 2 "Resistors_SMD:R_0402" V 5930 2550 50  0001 C CNN
F 3 "" H 6000 2550 50  0001 C CNN
F 4 "71-CRCW04020000Z0EDC" V 6000 2550 60  0001 C CNN "Mouser Part Number"
	1    6000 2550
	1    0    0    -1  
$EndComp
$Comp
L Crystal_GND24 Y201
U 1 1 630AB8E9
P 6100 1700
F 0 "Y201" H 6150 1875 50  0000 L CNN
F 1 "40 Mhz +/- 10 ppm" H 6150 1800 50  0000 L CNN
F 2 "FA_20H:FA20_H" H 6100 1700 50  0001 C CNN
F 3 "" H 6100 1700 50  0001 C CNN
F 4 "732-FA-20H40MF10Z-W3" H 6100 1700 60  0001 C CNN "Mouser Part Number"
	1    6100 1700
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR013
U 1 1 630AB9C9
P 6100 1950
F 0 "#PWR013" H 6100 1700 50  0001 C CNN
F 1 "GND" H 6100 1800 50  0000 C CNN
F 2 "" H 6100 1950 50  0001 C CNN
F 3 "" H 6100 1950 50  0001 C CNN
	1    6100 1950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 630ABA23
P 6100 1450
F 0 "#PWR014" H 6100 1200 50  0001 C CNN
F 1 "GND" H 6100 1300 50  0000 C CNN
F 2 "" H 6100 1450 50  0001 C CNN
F 3 "" H 6100 1450 50  0001 C CNN
	1    6100 1450
	-1   0    0    1   
$EndComp
Wire Wire Line
	6500 2800 6500 1850
Wire Wire Line
	6500 1850 7200 1850
Wire Wire Line
	6400 2800 6400 1650
Wire Wire Line
	6400 1650 7200 1650
Wire Wire Line
	7200 1750 7050 1750
Wire Wire Line
	8500 1650 9300 1650
Wire Wire Line
	9300 1750 9000 1750
Wire Wire Line
	9000 1750 9000 1850
Wire Wire Line
	9000 1850 8500 1850
Wire Wire Line
	8800 1750 8500 1750
Wire Wire Line
	8800 1450 8800 1750
Wire Wire Line
	8800 1450 9300 1450
Wire Wire Line
	9050 1300 9050 1450
Wire Wire Line
	9050 1450 9000 1450
Connection ~ 9000 1450
Wire Wire Line
	9600 2050 9700 2050
Wire Wire Line
	9700 2050 9700 2200
Wire Wire Line
	6900 1900 6900 1850
Connection ~ 6900 1850
Wire Wire Line
	6900 1600 6900 1650
Connection ~ 6900 1650
Wire Wire Line
	6900 1300 6900 1400
Wire Wire Line
	6900 2100 6900 2250
Wire Wire Line
	6000 2700 6000 2800
Wire Wire Line
	6100 2800 6100 2300
Wire Wire Line
	6100 2300 6300 2300
Wire Wire Line
	6000 1900 6000 2400
Wire Wire Line
	5850 1900 6000 1900
Wire Wire Line
	5950 1900 5950 1700
Wire Wire Line
	6250 2300 6250 1700
Wire Wire Line
	6100 1950 6100 1900
Wire Wire Line
	6100 1500 6100 1450
$Comp
L C_Small C212
U 1 1 630AC083
P 5850 2000
F 0 "C212" H 5860 2070 50  0000 L CNN
F 1 "22pf" H 5860 1920 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 5850 2000 50  0001 C CNN
F 3 "" H 5850 2000 50  0001 C CNN
F 4 "187-CL05C220JB5NNND" H 5850 2000 60  0001 C CNN "Mouser Part Number"
	1    5850 2000
	1    0    0    -1  
$EndComp
$Comp
L C_Small C213
U 1 1 630AC0CE
P 6300 2450
F 0 "C213" H 6310 2520 50  0000 L CNN
F 1 "22pf" H 6310 2370 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 6300 2450 50  0001 C CNN
F 3 "" H 6300 2450 50  0001 C CNN
F 4 "187-CL05C220JB5NNND" H 6300 2450 60  0001 C CNN "Mouser Part Number"
	1    6300 2450
	1    0    0    -1  
$EndComp
Connection ~ 5950 1900
Wire Wire Line
	6300 2300 6300 2350
Connection ~ 6250 2300
$Comp
L GND #PWR015
U 1 1 630AC14F
P 5850 2200
F 0 "#PWR015" H 5850 1950 50  0001 C CNN
F 1 "GND" H 5850 2050 50  0000 C CNN
F 2 "" H 5850 2200 50  0001 C CNN
F 3 "" H 5850 2200 50  0001 C CNN
	1    5850 2200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 630AC1B4
P 6200 2550
F 0 "#PWR016" H 6200 2300 50  0001 C CNN
F 1 "GND" H 6200 2400 50  0000 C CNN
F 2 "" H 6200 2550 50  0001 C CNN
F 3 "" H 6200 2550 50  0001 C CNN
	1    6200 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 2550 6200 2550
Wire Wire Line
	5850 2100 5850 2200
$Comp
L GND #PWR017
U 1 1 630AC738
P 5200 2850
F 0 "#PWR017" H 5200 2600 50  0001 C CNN
F 1 "GND" H 5200 2700 50  0000 C CNN
F 2 "" H 5200 2850 50  0001 C CNN
F 3 "" H 5200 2850 50  0001 C CNN
	1    5200 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 2800 5200 2800
Wire Wire Line
	5200 2800 5200 2850
$Comp
L +3.3V #PWR018
U 1 1 630AC8B0
P 5800 2600
F 0 "#PWR018" H 5800 2450 50  0001 C CNN
F 1 "+3.3V" H 5800 2740 50  0000 C CNN
F 2 "" H 5800 2600 50  0001 C CNN
F 3 "" H 5800 2600 50  0001 C CNN
	1    5800 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 2600 5800 2800
$Comp
L C_Small C204
U 1 1 630ACA49
P 3650 2650
F 0 "C204" H 3660 2720 50  0000 L CNN
F 1 "100nf" H 3660 2570 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 3650 2650 50  0001 C CNN
F 3 "" H 3650 2650 50  0001 C CNN
F 4 "710-885012105001" H 3650 2650 60  0001 C CNN "Mouser Part Number"
	1    3650 2650
	1    0    0    -1  
$EndComp
$Comp
L C_Small C207
U 1 1 630ACAEC
P 4050 2650
F 0 "C207" H 4060 2720 50  0000 L CNN
F 1 "1uf" H 4060 2570 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 4050 2650 50  0001 C CNN
F 3 "" H 4050 2650 50  0001 C CNN
F 4 "80-C0402C101K8RAUTO" H 4050 2650 60  0001 C CNN "Mouser Part Number"
	1    4050 2650
	1    0    0    -1  
$EndComp
$Comp
L C_Small C209
U 1 1 630ACB17
P 4250 2650
F 0 "C209" H 4260 2720 50  0000 L CNN
F 1 "100pf" H 4260 2570 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 4250 2650 50  0001 C CNN
F 3 "" H 4250 2650 50  0001 C CNN
F 4 "80-C0402C101K3RAUTO" H 4250 2650 60  0001 C CNN "Mouser Part Number"
	1    4250 2650
	1    0    0    -1  
$EndComp
$Comp
L C_Small C211
U 1 1 630ACBBE
P 4550 2650
F 0 "C211" H 4560 2720 50  0000 L CNN
F 1 "100nf" H 4560 2570 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 4550 2650 50  0001 C CNN
F 3 "" H 4550 2650 50  0001 C CNN
F 4 "710-885012105001" H 4550 2650 60  0001 C CNN "Mouser Part Number"
	1    4550 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2550 4550 2550
Connection ~ 4250 2550
Connection ~ 4050 2550
$Comp
L +3.3V #PWR019
U 1 1 630ACCC1
P 3650 2400
F 0 "#PWR019" H 3650 2250 50  0001 C CNN
F 1 "+3.3V" H 3650 2540 50  0000 C CNN
F 2 "" H 3650 2400 50  0001 C CNN
F 3 "" H 3650 2400 50  0001 C CNN
	1    3650 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2400 3650 2550
$Comp
L GND #PWR020
U 1 1 630ACD2A
P 3650 2900
F 0 "#PWR020" H 3650 2650 50  0001 C CNN
F 1 "GND" H 3650 2750 50  0000 C CNN
F 2 "" H 3650 2900 50  0001 C CNN
F 3 "" H 3650 2900 50  0001 C CNN
	1    3650 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2750 4550 2750
Connection ~ 4250 2750
Connection ~ 4050 2750
Wire Wire Line
	3650 2750 3650 2900
Text Notes 3950 2900 0    60   ~ 0
Pins 31,32
Text Notes 3500 2900 0    60   ~ 0
Pin 17
Text Notes 4550 2900 0    60   ~ 0
Pin 11
$Comp
L L_Small L201
U 1 1 630AD286
P 4800 3250
F 0 "L201" H 4830 3290 50  0000 L CNN
F 1 "2.0nH" H 4830 3210 50  0000 L CNN
F 2 "Inductors_SMD:L_0402" H 4800 3250 50  0001 C CNN
F 3 "" H 4800 3250 50  0001 C CNN
F 4 "810-MLG1005S2N0ST000" H 4800 3250 60  0001 C CNN "Mouser Part Number"
	1    4800 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 3600 4800 3600
Wire Wire Line
	4800 3600 4800 3350
$Comp
L +3.3V #PWR021
U 1 1 630AD330
P 4800 3050
F 0 "#PWR021" H 4800 2900 50  0001 C CNN
F 1 "+3.3V" H 4800 3190 50  0000 C CNN
F 2 "" H 4800 3050 50  0001 C CNN
F 3 "" H 4800 3050 50  0001 C CNN
	1    4800 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 3050 4800 3150
Text Label 4800 3600 0    60   ~ 0
VDD3P3
Text Label 3500 3550 0    60   ~ 0
VDD3P3
$Comp
L C_Small C203
U 1 1 630AD7EA
P 3500 3650
F 0 "C203" H 3510 3720 50  0000 L CNN
F 1 "10uf" H 3510 3570 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 3500 3650 50  0001 C CNN
F 3 "" H 3500 3650 50  0001 C CNN
F 4 "187-CL21A106MQFNNNE" H 3500 3650 60  0001 C CNN "Mouser Part Number"
	1    3500 3650
	1    0    0    -1  
$EndComp
$Comp
L C_Small C205
U 1 1 630AD848
P 3700 3650
F 0 "C205" H 3710 3720 50  0000 L CNN
F 1 "10uf" H 3710 3570 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 3700 3650 50  0001 C CNN
F 3 "" H 3700 3650 50  0001 C CNN
F 4 "187-CL21A106MQFNNNE" H 3700 3650 60  0001 C CNN "Mouser Part Number"
	1    3700 3650
	1    0    0    -1  
$EndComp
$Comp
L C_Small C206
U 1 1 630AD8A0
P 3900 3650
F 0 "C206" H 3910 3720 50  0000 L CNN
F 1 "1uf" H 3910 3570 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 3900 3650 50  0001 C CNN
F 3 "" H 3900 3650 50  0001 C CNN
F 4 "80-C0402C101K8RAUTO" H 3900 3650 60  0001 C CNN "Mouser Part Number"
	1    3900 3650
	1    0    0    -1  
$EndComp
$Comp
L C_Small C208
U 1 1 630AD8FE
P 4100 3650
F 0 "C208" H 4110 3720 50  0000 L CNN
F 1 "100nf" H 4110 3570 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 4100 3650 50  0001 C CNN
F 3 "" H 4100 3650 50  0001 C CNN
F 4 "710-885012105001" H 4100 3650 60  0001 C CNN "Mouser Part Number"
	1    4100 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 3550 4100 3550
Connection ~ 3900 3550
Connection ~ 3700 3550
Wire Wire Line
	3500 3750 4100 3750
Connection ~ 3900 3750
Connection ~ 3700 3750
$Comp
L GND #PWR022
U 1 1 630ADB4B
P 3500 3850
F 0 "#PWR022" H 3500 3600 50  0001 C CNN
F 1 "GND" H 3500 3700 50  0000 C CNN
F 2 "" H 3500 3850 50  0001 C CNN
F 3 "" H 3500 3850 50  0001 C CNN
	1    3500 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 3750 3500 3850
Text Notes 3600 3850 0    60   ~ 0
Pins 2,3
$Comp
L +3.3V #PWR023
U 1 1 630AE930
P 7200 4200
F 0 "#PWR023" H 7200 4050 50  0001 C CNN
F 1 "+3.3V" H 7200 4340 50  0000 C CNN
F 2 "" H 7200 4200 50  0001 C CNN
F 3 "" H 7200 4200 50  0001 C CNN
	1    7200 4200
	0    1    1    0   
$EndComp
Wire Wire Line
	7200 4200 7000 4200
$Comp
L +3.3V #PWR024
U 1 1 630AEABC
P 5900 5350
F 0 "#PWR024" H 5900 5200 50  0001 C CNN
F 1 "+3.3V" H 5900 5490 50  0000 C CNN
F 2 "" H 5900 5350 50  0001 C CNN
F 3 "" H 5900 5350 50  0001 C CNN
	1    5900 5350
	-1   0    0    1   
$EndComp
Wire Wire Line
	5900 5100 5900 5350
Text HLabel 5200 3800 0    60   Input ~ 0
GPIO0/A1/CurrentSense
Text HLabel 5200 3900 0    60   Input ~ 0
GPIO1/A2/TermpSense
Text HLabel 4450 4000 0    60   Output ~ 0
GPIO2/Fan
$Comp
L R R202
U 1 1 630AFC33
P 4500 4150
F 0 "R202" V 4580 4150 50  0000 C CNN
F 1 "10K" V 4500 4150 50  0000 C CNN
F 2 "Resistors_SMD:R_0402" V 4430 4150 50  0001 C CNN
F 3 "" H 4500 4150 50  0001 C CNN
F 4 "603-RC0402JR-1310KL" V 4500 4150 60  0001 C CNN "Mouser Part Number"
	1    4500 4150
	-1   0    0    1   
$EndComp
Wire Wire Line
	4450 4000 5200 4000
Connection ~ 4500 4000
$Comp
L +3.3V #PWR025
U 1 1 630AFDA0
P 4500 4400
F 0 "#PWR025" H 4500 4250 50  0001 C CNN
F 1 "+3.3V" H 4500 4540 50  0000 C CNN
F 2 "" H 4500 4400 50  0001 C CNN
F 3 "" H 4500 4400 50  0001 C CNN
	1    4500 4400
	-1   0    0    1   
$EndComp
Wire Wire Line
	4500 4300 4500 4400
$Comp
L R R203
U 1 1 630B03B1
P 4950 4200
F 0 "R203" V 5030 4200 50  0000 C CNN
F 1 "10K" V 4950 4200 50  0000 C CNN
F 2 "Resistors_SMD:R_0402" V 4880 4200 50  0001 C CNN
F 3 "" H 4950 4200 50  0001 C CNN
F 4 "603-RC0402JR-1310KL" V 4950 4200 60  0001 C CNN "Mouser Part Number"
	1    4950 4200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5100 4200 5200 4200
Wire Wire Line
	4800 4400 4800 4200
Wire Wire Line
	4400 4400 4800 4400
$Comp
L R R201
U 1 1 630B1004
P 4400 4850
F 0 "R201" V 4480 4850 50  0000 C CNN
F 1 "10K" V 4400 4850 50  0000 C CNN
F 2 "Resistors_SMD:R_0402" V 4330 4850 50  0001 C CNN
F 3 "" H 4400 4850 50  0001 C CNN
F 4 "603-RC0402JR-1310KL" V 4400 4850 60  0001 C CNN "Mouser Part Number"
	1    4400 4850
	1    0    0    -1  
$EndComp
$Comp
L C_Small C210
U 1 1 630B1049
P 4400 5250
F 0 "C210" H 4410 5320 50  0000 L CNN
F 1 "1uf" H 4410 5170 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 4400 5250 50  0001 C CNN
F 3 "" H 4400 5250 50  0001 C CNN
F 4 "80-C0402C101K8RAUTO" H 4400 5250 60  0001 C CNN "Mouser Part Number"
	1    4400 5250
	1    0    0    -1  
$EndComp
$Comp
L SW_Push SW201
U 1 1 630B10D0
P 4000 5050
F 0 "SW201" H 4050 5150 50  0000 L CNN
F 1 "RESET" H 4000 4990 50  0000 C CNN
F 2 "PTS647SN70SMTR2LFS:PTS647SM38SMTR2LFS" H 4000 5250 50  0001 C CNN
F 3 "" H 4000 5250 50  0001 C CNN
F 4 "611-PTS647SN70SMTR2L" H 4000 5050 60  0001 C CNN "Mouser Part Number"
	1    4000 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 4700 4400 4400
Connection ~ 4500 4400
Wire Wire Line
	4400 5000 4400 5150
Wire Wire Line
	4200 5050 4400 5050
Connection ~ 4400 5050
Wire Wire Line
	3800 5050 3800 5350
Wire Wire Line
	3800 5350 4400 5350
$Comp
L GND #PWR026
U 1 1 630B12F5
P 4400 5400
F 0 "#PWR026" H 4400 5150 50  0001 C CNN
F 1 "GND" H 4400 5250 50  0000 C CNN
F 2 "" H 4400 5400 50  0001 C CNN
F 3 "" H 4400 5400 50  0001 C CNN
	1    4400 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 5350 4400 5400
Text Label 4400 5050 0    60   ~ 0
RESET
Text Label 5200 4100 2    60   ~ 0
RESET
$Comp
L SW_Push SW202
U 1 1 630B1B53
P 6500 5400
F 0 "SW202" H 6550 5500 50  0000 L CNN
F 1 "BOOT" H 6500 5340 50  0000 C CNN
F 2 "PTS647SN70SMTR2LFS:PTS647SM38SMTR2LFS" H 6500 5600 50  0001 C CNN
F 3 "" H 6500 5600 50  0001 C CNN
F 4 "611-PTS647SN70SMTR2L" H 6500 5400 60  0001 C CNN "Mouser Part Number"
	1    6500 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 5100 6300 5400
$Comp
L GND #PWR027
U 1 1 630B1E0A
P 6700 5500
F 0 "#PWR027" H 6700 5250 50  0001 C CNN
F 1 "GND" H 6700 5350 50  0000 C CNN
F 2 "" H 6700 5500 50  0001 C CNN
F 3 "" H 6700 5500 50  0001 C CNN
	1    6700 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 5400 6700 5500
$Comp
L R R205
U 1 1 630B1FBD
P 7100 4650
F 0 "R205" V 7180 4650 50  0000 C CNN
F 1 "270" V 7100 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_0402" V 7030 4650 50  0001 C CNN
F 3 "" H 7100 4650 50  0001 C CNN
F 4 "754-RR0510P-271D" V 7100 4650 60  0001 C CNN "Mouser Part Number"
	1    7100 4650
	1    0    0    -1  
$EndComp
$Comp
L R R206
U 1 1 630B2004
P 7350 4650
F 0 "R206" V 7430 4650 50  0000 C CNN
F 1 "270" V 7350 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_0402" V 7280 4650 50  0001 C CNN
F 3 "" H 7350 4650 50  0001 C CNN
F 4 "754-RR0510P-271D" V 7350 4650 60  0001 C CNN "Mouser Part Number"
	1    7350 4650
	1    0    0    -1  
$EndComp
$Comp
L LED D201
U 1 1 630B204D
P 7100 5050
F 0 "D201" H 7100 5150 50  0000 C CNN
F 1 "ACT1" H 7100 4950 50  0000 C CNN
F 2 "LEDs:LED_0402" H 7100 5050 50  0001 C CNN
F 3 "" H 7100 5050 50  0001 C CNN
F 4 "710-150040GS73220" H 7100 5050 60  0001 C CNN "Mouser Part Number"
	1    7100 5050
	0    -1   -1   0   
$EndComp
$Comp
L LED D202
U 1 1 630B2168
P 7350 5050
F 0 "D202" H 7350 5150 50  0000 C CNN
F 1 "ACT2" H 7350 4950 50  0000 C CNN
F 2 "LEDs:LED_0402" H 7350 5050 50  0001 C CNN
F 3 "" H 7350 5050 50  0001 C CNN
F 4 "710-150040SS73220" H 7350 5050 60  0001 C CNN "Mouser Part Number"
	1    7350 5050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7200 4200 7200 4500
Wire Wire Line
	7100 4500 7350 4500
Connection ~ 7200 4500
Wire Wire Line
	7100 4800 7100 4900
Wire Wire Line
	7350 4800 7350 4900
Wire Wire Line
	6200 5100 6200 5300
Wire Wire Line
	6200 5300 7100 5300
Wire Wire Line
	7100 5300 7100 5200
Wire Wire Line
	6400 5100 6400 5250
$Comp
L ESP32-C3FN4 IC201
U 1 1 630A2B4E
P 5200 3500
F 0 "IC201" H 6850 4200 50  0000 L CNN
F 1 "ESP32-C3FN4" H 6850 4100 50  0000 L CNN
F 2 "Housings_DFN_QFN:QFN-32-1EP_5x5mm_Pitch0.5mm" H 6850 4000 50  0001 L CNN
F 3 "https://www.mouser.co.uk/datasheet/2/891/esp32_c3_datasheet_en-1989865.pdf" H 6850 3900 50  0001 L CNN
F 4 "RF System on a Chip - SoC SMD IC ESP32-C3FN4, single-core MCU, 2.4G Wi-Fi & BLE 5.0 combo, QFN 32-pin, 5*5 mm, 4 MB flash inside, -40C +85C" H 6850 3800 50  0001 L CNN "Description"
F 5 "0.9" H 6850 3700 50  0001 L CNN "Height"
F 6 "356-ESP32-C3FN4" H 6850 3600 50  0001 L CNN "Mouser Part Number"
F 7 "Espressif Systems" H 6850 3400 50  0001 L CNN "Manufacturer_Name"
F 8 "ESP32-C3FN4" H 6850 3300 50  0001 L CNN "Manufacturer_Part_Number"
	1    5200 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 2800 5900 2800
Wire Wire Line
	6400 5250 7350 5250
Wire Wire Line
	7350 5250 7350 5200
Wire Wire Line
	5200 3700 5200 3600
NoConn ~ 5200 3500
NoConn ~ 5700 5100
NoConn ~ 7000 4000
NoConn ~ 7000 3900
NoConn ~ 7000 3800
NoConn ~ 7000 3700
NoConn ~ 7000 3600
NoConn ~ 7000 3500
NoConn ~ 6300 2800
NoConn ~ 6200 2800
$Comp
L C_Small C214
U 1 1 630B4185
P 7200 4000
F 0 "C214" H 7210 4070 50  0000 L CNN
F 1 "1uf" H 7210 3920 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 7200 4000 50  0001 C CNN
F 3 "" H 7200 4000 50  0001 C CNN
F 4 "80-C0402C101K8RAUTO" H 7200 4000 60  0001 C CNN "Mouser Part Number"
	1    7200 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 4100 7200 4100
$Comp
L GND #PWR028
U 1 1 630B426E
P 7200 3800
F 0 "#PWR028" H 7200 3550 50  0001 C CNN
F 1 "GND" H 7200 3650 50  0000 C CNN
F 2 "" H 7200 3800 50  0001 C CNN
F 3 "" H 7200 3800 50  0001 C CNN
	1    7200 3800
	-1   0    0    1   
$EndComp
Wire Wire Line
	7200 3900 7200 3800
Text HLabel 6000 5100 3    60   Output ~ 0
CAN_TX
Text HLabel 6100 5100 3    60   Input ~ 0
CAN_RX
Text HLabel 5800 5100 3    60   Output ~ 0
GPIO5/Brake
$EndSCHEMATC
