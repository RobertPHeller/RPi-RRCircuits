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
LIBS:FT231XS-R
LIBS:ssm3j117tu
LIBS:TC1263-3.3VOATR
LIBS:umh3n
LIBS:ESP32-D0WD-V3-USBProgBoard-cache
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
L USB_OTG J1
U 1 1 62B0CE14
P 2000 2050
F 0 "J1" H 1800 2500 50  0000 L CNN
F 1 "USB_OTG" H 1800 2400 50  0000 L CNN
F 2 "Connectors_USB:USB_Micro-B_Molex_47346-0001" H 2150 2000 50  0001 C CNN
F 3 "" H 2150 2000 50  0001 C CNN
F 4 "538-47346-0001" H 2000 2050 60  0001 C CNN "Mouser Part Number"
	1    2000 2050
	1    0    0    -1  
$EndComp
$Comp
L FT231XS-R U1
U 1 1 62B0D37F
P 4400 1750
F 0 "U1" H 5550 2050 50  0000 L CNN
F 1 "FT231XS-R" H 5550 1950 50  0000 L CNN
F 2 "Housings_SSOP:SSOP-20_5.3x7.2mm_Pitch0.65mm" H 5550 1850 50  0001 L CNN
F 3 "https://datasheet.lcsc.com/szlcsc/Future-Designs-FT231XS-R_C132160.pdf" H 5550 1750 50  0001 L CNN
F 4 "USB SSOP-20 RoHS" H 5550 1650 50  0001 L CNN "Description"
F 5 "1.753" H 5550 1550 50  0001 L CNN "Height"
F 6 "Future Designs" H 5550 1250 50  0001 L CNN "Manufacturer_Name"
F 7 "FT231XS-R" H 5550 1150 50  0001 L CNN "Manufacturer_Part_Number"
	1    4400 1750
	-1   0    0    -1  
$EndComp
$Comp
L SSM3J117TU Q1
U 1 1 62B0D8E3
P 3300 900
F 0 "Q1" H 3500 975 50  0000 L CNN
F 1 "SSM3J117TU" H 3500 900 50  0000 L CNN
F 2 "SOTFL65P210X75-3N:SOTFL65P210X75-3N" H 3500 825 50  0001 L CIN
F 3 "" H 3300 900 50  0001 L CNN
F 4 "757-SSM3J117TULF " H 3300 900 60  0001 C CNN "Mouser Part Number"
	1    3300 900 
	0    1    -1   0   
$EndComp
$Comp
L Ferrite_Bead_Small L1
U 1 1 62B0DA0C
P 2450 1850
F 0 "L1" H 2525 1900 50  0000 L CNN
F 1 "Ferrite Bead" H 2525 1800 50  0000 L CNN
F 2 "Inductors_SMD:L_0402" V 2380 1850 50  0001 C CNN
F 3 "" H 2450 1850 50  0001 C CNN
F 4 "810-MMZ1005S800CTD25" H 2450 1850 60  0001 C CNN "Mouser Part Number"
	1    2450 1850
	0    1    1    0   
$EndComp
$Comp
L R R1
U 1 1 62B0DBDE
P 2750 2550
F 0 "R1" V 2830 2550 50  0000 C CNN
F 1 "27" V 2750 2550 50  0000 C CNN
F 2 "Resistors_SMD:R_0402" V 2680 2550 50  0001 C CNN
F 3 "" H 2750 2550 50  0001 C CNN
F 4 "603-RT0402FRE0727RL" V 2750 2550 60  0001 C CNN "Mouser Part Number"
	1    2750 2550
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 62B0DC21
P 2750 2650
F 0 "R2" V 2830 2650 50  0000 C CNN
F 1 "27" V 2750 2650 50  0000 C CNN
F 2 "Resistors_SMD:R_0402" V 2680 2650 50  0001 C CNN
F 3 "" H 2750 2650 50  0001 C CNN
F 4 "603-RT0402FRE0727RL" V 2750 2650 60  0001 C CNN "Mouser Part Number"
	1    2750 2650
	0    1    1    0   
$EndComp
$Comp
L C_Small C3
U 1 1 62B0DCCA
P 2500 2900
F 0 "C3" H 2510 2970 50  0000 L CNN
F 1 "47pf" H 2510 2820 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 2500 2900 50  0001 C CNN
F 3 "" H 2500 2900 50  0001 C CNN
F 4 "80-C0402C470K5RAUTO" H 2500 2900 60  0001 C CNN "Mouser Part Number"
	1    2500 2900
	1    0    0    -1  
$EndComp
$Comp
L C_Small C2
U 1 1 62B0DD86
P 2350 2900
F 0 "C2" H 2360 2970 50  0000 L CNN
F 1 "47pf" H 2360 2820 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 2350 2900 50  0001 C CNN
F 3 "" H 2350 2900 50  0001 C CNN
F 4 "80-C0402C470K5RAUTO" H 2350 2900 60  0001 C CNN "Mouser Part Number"
	1    2350 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 62B0DDFB
P 2500 3200
F 0 "#PWR01" H 2500 2950 50  0001 C CNN
F 1 "GND" H 2500 3050 50  0000 C CNN
F 2 "" H 2500 3200 50  0001 C CNN
F 3 "" H 2500 3200 50  0001 C CNN
	1    2500 3200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 62B0DE4B
P 2000 2600
F 0 "#PWR02" H 2000 2350 50  0001 C CNN
F 1 "GND" H 2000 2450 50  0000 C CNN
F 2 "" H 2000 2600 50  0001 C CNN
F 3 "" H 2000 2600 50  0001 C CNN
	1    2000 2600
	1    0    0    -1  
$EndComp
$Comp
L C_Small C1
U 1 1 62B0DEAA
P 2300 1600
F 0 "C1" H 2310 1670 50  0000 L CNN
F 1 "10pf" H 2310 1520 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 2300 1600 50  0001 C CNN
F 3 "" H 2300 1600 50  0001 C CNN
F 4 "80-C0402C100K5RAUTO" H 2300 1600 60  0001 C CNN "Mouser Part Number"
	1    2300 1600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 62B0DF2C
P 2300 1400
F 0 "#PWR03" H 2300 1150 50  0001 C CNN
F 1 "GND" H 2300 1250 50  0000 C CNN
F 2 "" H 2300 1400 50  0001 C CNN
F 3 "" H 2300 1400 50  0001 C CNN
	1    2300 1400
	-1   0    0    1   
$EndComp
$Comp
L R R3
U 1 1 62B0E093
P 2900 1050
F 0 "R3" V 2980 1050 50  0000 C CNN
F 1 "10K" V 2900 1050 50  0000 C CNN
F 2 "Resistors_SMD:R_0402" V 2830 1050 50  0001 C CNN
F 3 "" H 2900 1050 50  0001 C CNN
F 4 "603-AT0402FRE0710KL" V 2900 1050 60  0001 C CNN "Mouser Part Number"
	1    2900 1050
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 62B0E128
P 2900 1500
F 0 "R4" V 2980 1500 50  0000 C CNN
F 1 "1K" V 2900 1500 50  0000 C CNN
F 2 "Resistors_SMD:R_0402" V 2830 1500 50  0001 C CNN
F 3 "" H 2900 1500 50  0001 C CNN
F 4 "603-RT0402FRE071KL" V 2900 1500 60  0001 C CNN "Mouser Part Number"
	1    2900 1500
	-1   0    0    1   
$EndComp
$Comp
L C_Small C7
U 1 1 62B0E1D5
P 3600 1000
F 0 "C7" H 3610 1070 50  0000 L CNN
F 1 ".1uf" H 3610 920 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 3600 1000 50  0001 C CNN
F 3 "" H 3600 1000 50  0001 C CNN
F 4 "80-C0402C104K8R" H 3600 1000 60  0001 C CNN "Mouser Part Number"
	1    3600 1000
	1    0    0    -1  
$EndComp
$Comp
L C_Small C8
U 1 1 62B0E20C
P 3950 1000
F 0 "C8" H 3960 1070 50  0000 L CNN
F 1 ".1uf" H 3960 920 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 3950 1000 50  0001 C CNN
F 3 "" H 3950 1000 50  0001 C CNN
F 4 "80-C0402C104K8R" H 3950 1000 60  0001 C CNN "Mouser Part Number"
	1    3950 1000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 62B0E549
P 3950 1200
F 0 "#PWR04" H 3950 950 50  0001 C CNN
F 1 "GND" H 3950 1050 50  0000 C CNN
F 2 "" H 3950 1200 50  0001 C CNN
F 3 "" H 3950 1200 50  0001 C CNN
	1    3950 1200
	1    0    0    -1  
$EndComp
$Comp
L C_Small C6
U 1 1 62B1C72D
P 3400 3350
F 0 "C6" H 3410 3420 50  0000 L CNN
F 1 "100nf" H 3410 3270 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 3400 3350 50  0001 C CNN
F 3 "" H 3400 3350 50  0001 C CNN
F 4 "80-C0402C104K8R" H 3400 3350 60  0001 C CNN "Mouser Part Number"
	1    3400 3350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 62B1C816
P 3400 3600
F 0 "#PWR05" H 3400 3350 50  0001 C CNN
F 1 "GND" H 3400 3450 50  0000 C CNN
F 2 "" H 3400 3600 50  0001 C CNN
F 3 "" H 3400 3600 50  0001 C CNN
	1    3400 3600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 62B1C9B8
P 2950 2150
F 0 "#PWR06" H 2950 1900 50  0001 C CNN
F 1 "GND" H 2950 2000 50  0000 C CNN
F 2 "" H 2950 2150 50  0001 C CNN
F 3 "" H 2950 2150 50  0001 C CNN
	1    2950 2150
	0    1    1    0   
$EndComp
$Comp
L GND #PWR07
U 1 1 62B1CA2C
P 4550 2250
F 0 "#PWR07" H 4550 2000 50  0001 C CNN
F 1 "GND" H 4550 2100 50  0000 C CNN
F 2 "" H 4550 2250 50  0001 C CNN
F 3 "" H 4550 2250 50  0001 C CNN
	1    4550 2250
	0    -1   -1   0   
$EndComp
$Comp
L C_Small C4
U 1 1 62B1CB2D
P 2600 2050
F 0 "C4" H 2610 2120 50  0000 L CNN
F 1 "100nf" H 2610 1970 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 2600 2050 50  0001 C CNN
F 3 "" H 2600 2050 50  0001 C CNN
F 4 "80-C0402C104K8R" H 2600 2050 60  0001 C CNN "Mouser Part Number"
	1    2600 2050
	1    0    0    -1  
$EndComp
$Comp
L CP1_Small C5
U 1 1 62B1CB60
P 2700 2250
F 0 "C5" H 2710 2320 50  0000 L CNN
F 1 "4.7uf" H 2710 2170 50  0000 L CNN
F 2 "Capacitors_SMD:CP_Elec_4x5.7" H 2700 2250 50  0001 C CNN
F 3 "" H 2700 2250 50  0001 C CNN
F 4 "710-865080540002" H 2700 2250 60  0001 C CNN "Mouser Part Number"
	1    2700 2250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 62B1CC82
P 2600 2400
F 0 "#PWR08" H 2600 2150 50  0001 C CNN
F 1 "GND" H 2600 2250 50  0000 C CNN
F 2 "" H 2600 2400 50  0001 C CNN
F 3 "" H 2600 2400 50  0001 C CNN
	1    2600 2400
	1    0    0    -1  
$EndComp
$Comp
L TC1263-3.3VOATR U2
U 1 1 62B1D1E4
P 6050 800
F 0 "U2" H 7200 1100 50  0000 L CNN
F 1 "TC1263-3.3VOATR" H 7200 1000 50  0000 L CNN
F 2 "Housings_SOIC:SO-8_5.3x6.2mm_Pitch1.27mm" H 7200 900 50  0001 L CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21374D.pdf" H 7200 800 50  0001 L CNN
F 4 "LDO Voltage Regulators 500mA Fixed Out Adj" H 7200 700 50  0001 L CNN "Description"
F 5 "1.75" H 7200 600 50  0001 L CNN "Height"
F 6 "Microchip" H 7200 300 50  0001 L CNN "Manufacturer_Name"
F 7 "TC1263-3.3VOATR" H 7200 200 50  0001 L CNN "Manufacturer_Part_Number"
	1    6050 800 
	-1   0    0    -1  
$EndComp
NoConn ~ 4750 900 
NoConn ~ 4750 1100
NoConn ~ 6050 1000
$Comp
L C_Small C9
U 1 1 62B1D556
P 6100 1400
F 0 "C9" H 6110 1470 50  0000 L CNN
F 1 "470pf" H 6110 1320 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 6100 1400 50  0001 C CNN
F 3 "" H 6100 1400 50  0001 C CNN
F 4 "80-C0402S471K3RAUTO" H 6100 1400 60  0001 C CNN "Mouser Part Number"
	1    6100 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 1850 2350 1850
Wire Wire Line
	2550 1850 2800 1850
Wire Wire Line
	2800 800  2800 2250
Wire Wire Line
	2800 800  3100 800 
Wire Wire Line
	2800 2250 3100 2250
Connection ~ 2800 1850
Wire Wire Line
	3100 2550 2900 2550
Wire Wire Line
	3100 2650 2900 2650
Wire Wire Line
	2350 2550 2600 2550
Wire Wire Line
	2450 2550 2450 2150
Wire Wire Line
	2450 2150 2300 2150
Wire Wire Line
	2600 2650 2500 2650
Wire Wire Line
	2500 2050 2500 2800
Wire Wire Line
	2500 2050 2300 2050
Connection ~ 2500 2650
Wire Wire Line
	2350 2800 2350 2550
Connection ~ 2450 2550
Wire Wire Line
	2350 3000 2500 3000
Wire Wire Line
	2500 3000 2500 3200
Wire Wire Line
	1900 2450 2000 2450
Wire Wire Line
	2000 2450 2000 2600
Wire Wire Line
	2300 1700 2300 1850
Wire Wire Line
	2300 1400 2300 1500
Wire Wire Line
	2900 900  2900 800 
Connection ~ 2900 800 
Wire Wire Line
	2900 1200 3600 1200
Connection ~ 3300 1200
Wire Wire Line
	3500 800  4750 800 
Wire Wire Line
	3600 800  3600 900 
Wire Wire Line
	3600 1200 3600 1100
Wire Wire Line
	3300 1100 3300 1300
Wire Wire Line
	3300 1300 2900 1300
Wire Wire Line
	2900 1300 2900 1350
Wire Wire Line
	2900 1650 2900 1850
Wire Wire Line
	2900 1850 3100 1850
Wire Wire Line
	3950 800  3950 900 
Connection ~ 3600 800 
Wire Wire Line
	3950 1100 3950 1200
Wire Wire Line
	3100 2350 3100 2450
Wire Wire Line
	4400 1950 4750 1950
Wire Wire Line
	4750 1950 4750 3150
Wire Wire Line
	4750 3150 3000 3150
Wire Wire Line
	3000 3150 3000 2350
Wire Wire Line
	3000 2350 3100 2350
Wire Wire Line
	3400 3250 3400 3150
Connection ~ 3400 3150
Wire Wire Line
	3400 3600 3400 3450
Wire Wire Line
	4550 2250 4400 2250
Wire Wire Line
	2950 2150 3100 2150
Wire Wire Line
	2600 1950 2600 1850
Connection ~ 2600 1850
Wire Wire Line
	2700 2150 2700 1850
Connection ~ 2700 1850
Wire Wire Line
	2600 2150 2600 2400
Wire Wire Line
	2600 2400 2700 2400
Wire Wire Line
	2700 2400 2700 2350
Connection ~ 3950 800 
Wire Wire Line
	4750 1000 4450 1000
Wire Wire Line
	4450 1000 4450 800 
Connection ~ 4450 800 
Wire Wire Line
	6050 1100 6100 1100
Wire Wire Line
	6100 1100 6100 1300
$Comp
L GND #PWR09
U 1 1 62B1D70A
P 6100 1600
F 0 "#PWR09" H 6100 1350 50  0001 C CNN
F 1 "GND" H 6100 1450 50  0000 C CNN
F 2 "" H 6100 1600 50  0001 C CNN
F 3 "" H 6100 1600 50  0001 C CNN
	1    6100 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 1500 6100 1600
$Comp
L GND #PWR010
U 1 1 62B1D890
P 6200 900
F 0 "#PWR010" H 6200 650 50  0001 C CNN
F 1 "GND" H 6200 750 50  0000 C CNN
F 2 "" H 6200 900 50  0001 C CNN
F 3 "" H 6200 900 50  0001 C CNN
	1    6200 900 
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6200 900  6050 900 
$Comp
L CP1_Small C10
U 1 1 62B1D9BD
P 6200 700
F 0 "C10" H 6210 770 50  0000 L CNN
F 1 "1uf" H 6210 620 50  0000 L CNN
F 2 "Capacitors_SMD:CP_Elec_4x4.5" H 6200 700 50  0001 C CNN
F 3 "" H 6200 700 50  0001 C CNN
F 4 "647-UZG1H010MCL1GB" H 6200 700 60  0001 C CNN "Mouser Part Number"
	1    6200 700 
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 800  6050 600 
Wire Wire Line
	6050 600  6400 600 
Wire Wire Line
	6200 800  6200 900 
$Comp
L +3.3V #PWR011
U 1 1 62B1DB62
P 6400 600
F 0 "#PWR011" H 6400 450 50  0001 C CNN
F 1 "+3.3V" H 6400 740 50  0000 C CNN
F 2 "" H 6400 600 50  0001 C CNN
F 3 "" H 6400 600 50  0001 C CNN
	1    6400 600 
	0    1    1    0   
$EndComp
Connection ~ 6200 600 
$Comp
L UMH3N Q2
U 1 1 62B1DE35
P 6450 2300
F 0 "Q2" H 6650 2350 50  0000 L CNN
F 1 "UMH3N" H 6650 2250 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-363_SC-70-6" H 6650 2400 50  0001 C CNN
F 3 "" H 6450 2300 50  0001 C CNN
F 4 "755-UMH3NFHATN " H 6450 2300 60  0001 C CNN "Mouser Part Number"
	1    6450 2300
	1    0    0    -1  
$EndComp
$Comp
L UMH3N Q2
U 2 1 62B1DE8C
P 6300 2600
F 0 "Q2" H 6500 2650 50  0000 L CNN
F 1 "UMH3N" H 6500 2550 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-363_SC-70-6" H 6500 2700 50  0001 C CNN
F 3 "" H 6300 2600 50  0001 C CNN
	2    6300 2600
	-1   0    0    1   
$EndComp
Wire Wire Line
	6250 2300 6200 2300
Wire Wire Line
	6200 2300 6200 2400
Wire Wire Line
	6550 2500 6550 2600
Wire Wire Line
	6550 2600 6500 2600
$Comp
L Conn_02x03_Odd_Even J2
U 1 1 62B1E7D8
P 7050 3200
F 0 "J2" H 7100 3400 50  0000 C CNN
F 1 "ProgHeader" H 7100 3000 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_2x03_Pitch2.54mm" H 7050 3200 50  0001 C CNN
F 3 "" H 7050 3200 50  0001 C CNN
	1    7050 3200
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR012
U 1 1 62B1F076
P 6850 3000
F 0 "#PWR012" H 6850 2850 50  0001 C CNN
F 1 "+3.3V" H 6850 3140 50  0000 C CNN
F 2 "" H 6850 3000 50  0001 C CNN
F 3 "" H 6850 3000 50  0001 C CNN
	1    6850 3000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 62B1F0B6
P 7350 3450
F 0 "#PWR013" H 7350 3200 50  0001 C CNN
F 1 "GND" H 7350 3300 50  0000 C CNN
F 2 "" H 7350 3450 50  0001 C CNN
F 3 "" H 7350 3450 50  0001 C CNN
	1    7350 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 3300 7350 3450
Wire Wire Line
	6850 3000 6850 3100
Text Label 7350 3100 0    60   ~ 0
BOOTMODE
Text Label 7350 3200 0    60   ~ 0
RESET
Text Label 6850 3200 2    60   ~ 0
TX
Text Label 6850 3300 2    60   ~ 0
RX
Text Label 6200 2800 2    60   ~ 0
BOOTMODE
Text Label 6550 2100 2    60   ~ 0
RESET
Text Label 6550 2600 0    60   ~ 0
RTS
Text Label 6200 2300 2    60   ~ 0
DTR
Text Label 4400 1750 0    60   ~ 0
DTR
Text Label 4400 1850 0    60   ~ 0
RTS
Text Label 4400 2050 0    60   ~ 0
RX
Text Label 3100 1750 2    60   ~ 0
TX
$Comp
L R R7
U 1 1 62B20181
P 6950 900
F 0 "R7" V 7030 900 50  0000 C CNN
F 1 "270" V 6950 900 50  0000 C CNN
F 2 "Resistors_SMD:R_0402" V 6880 900 50  0001 C CNN
F 3 "" H 6950 900 50  0001 C CNN
F 4 "754-RR0510P-271D" V 6950 900 60  0001 C CNN "Mouser Part Number"
	1    6950 900 
	1    0    0    -1  
$EndComp
$Comp
L LED D3
U 1 1 62B201C8
P 6950 1250
F 0 "D3" H 6950 1350 50  0000 C CNN
F 1 "Power" H 6950 1150 50  0000 C CNN
F 2 "LEDs:LED_0402" H 6950 1250 50  0001 C CNN
F 3 "" H 6950 1250 50  0001 C CNN
F 4 "710-150040AS73220" H 6950 1250 60  0001 C CNN "Mouser Part Number"
	1    6950 1250
	0    -1   -1   0   
$EndComp
$Comp
L +3.3V #PWR014
U 1 1 62B202CA
P 6950 650
F 0 "#PWR014" H 6950 500 50  0001 C CNN
F 1 "+3.3V" H 6950 790 50  0000 C CNN
F 2 "" H 6950 650 50  0001 C CNN
F 3 "" H 6950 650 50  0001 C CNN
	1    6950 650 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 62B2030E
P 6950 1550
F 0 "#PWR015" H 6950 1300 50  0001 C CNN
F 1 "GND" H 6950 1400 50  0000 C CNN
F 2 "" H 6950 1550 50  0001 C CNN
F 3 "" H 6950 1550 50  0001 C CNN
	1    6950 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 650  6950 750 
Wire Wire Line
	6950 1050 6950 1100
Wire Wire Line
	6950 1400 6950 1550
$Comp
L LED D1
U 1 1 62B20C19
P 2950 3700
F 0 "D1" H 2950 3800 50  0000 C CNN
F 1 "TX" H 2950 3600 50  0000 C CNN
F 2 "LEDs:LED_0402" H 2950 3700 50  0001 C CNN
F 3 "" H 2950 3700 50  0001 C CNN
F 4 "710-150040GS73220" H 2950 3700 60  0001 C CNN "Mouser Part Number"
	1    2950 3700
	0    1    1    0   
$EndComp
$Comp
L LED D2
U 1 1 62B20CDF
P 4500 3650
F 0 "D2" H 4500 3750 50  0000 C CNN
F 1 "RX" H 4500 3550 50  0000 C CNN
F 2 "LEDs:LED_0402" H 4500 3650 50  0001 C CNN
F 3 "" H 4500 3650 50  0001 C CNN
F 4 "710-150040SS73220 " H 4500 3650 60  0001 C CNN "Mouser Part Number"
	1    4500 3650
	0    1    1    0   
$EndComp
Wire Wire Line
	2950 3550 2950 2050
Wire Wire Line
	2950 2050 3100 2050
Wire Wire Line
	4500 3500 4500 2650
Wire Wire Line
	4500 2650 4400 2650
$Comp
L R R5
U 1 1 62B21401
P 2950 4000
F 0 "R5" V 3030 4000 50  0000 C CNN
F 1 "270" V 2950 4000 50  0000 C CNN
F 2 "Resistors_SMD:R_0402" V 2880 4000 50  0001 C CNN
F 3 "" H 2950 4000 50  0001 C CNN
F 4 "754-RR0510P-271D" V 2950 4000 60  0001 C CNN "Mouser Part Number"
	1    2950 4000
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 62B2144C
P 4500 4000
F 0 "R6" V 4580 4000 50  0000 C CNN
F 1 "270" V 4500 4000 50  0000 C CNN
F 2 "Resistors_SMD:R_0402" V 4430 4000 50  0001 C CNN
F 3 "" H 4500 4000 50  0001 C CNN
F 4 "754-RR0510P-271D" V 4500 4000 60  0001 C CNN "Mouser Part Number"
	1    4500 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 3800 4500 3850
Wire Wire Line
	2950 3850 2950 3900
Wire Wire Line
	4500 4150 2950 4150
Wire Wire Line
	2850 2250 2850 4250
Wire Wire Line
	2850 4250 2950 4250
Connection ~ 2850 2250
Wire Wire Line
	2950 4250 2950 4150
NoConn ~ 3100 1950
NoConn ~ 4400 2150
NoConn ~ 4400 2350
NoConn ~ 4400 2450
NoConn ~ 4400 2550
$EndSCHEMATC
