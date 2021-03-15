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
LIBS:lm2574n-5
LIBS:lmd18200
LIBS:tmp36
LIBS:sn65hvd233-ht
LIBS:L298N
LIBS:PocketBeagleCommandStation-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 7
Title "Pocket Beagle DCC Command Station"
Date "3 mar 2019"
Rev "A"
Comp "Deepwoods Software"
Comment1 "Railcom Interface"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L 2N7000 Q6
U 1 1 603F9C3E
P 2150 1950
F 0 "Q6" H 2350 2025 50  0000 L CNN
F 1 "2N7000" H 2350 1950 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 2350 1875 50  0001 L CIN
F 3 "" H 2150 1950 50  0001 L CNN
	1    2150 1950
	-1   0    0    -1  
$EndComp
$Comp
L 2N7000 Q8
U 1 1 603F9CBE
P 2850 2150
F 0 "Q8" H 3050 2225 50  0000 L CNN
F 1 "2N7000" H 3050 2150 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 3050 2075 50  0001 L CIN
F 3 "" H 2850 2150 50  0001 L CNN
	1    2850 2150
	-1   0    0    -1  
$EndComp
$Comp
L 2N7000 Q10
U 1 1 603F9EB6
P 3800 1700
F 0 "Q10" H 4000 1775 50  0000 L CNN
F 1 "2N7000" H 4000 1700 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 4000 1625 50  0001 L CIN
F 3 "" H 3800 1700 50  0001 L CNN
	1    3800 1700
	-1   0    0    -1  
$EndComp
$Comp
L 2N7000 Q11
U 1 1 603F9F00
P 4250 2250
F 0 "Q11" H 4450 2325 50  0000 L CNN
F 1 "2N7000" H 4450 2250 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 4450 2175 50  0001 L CIN
F 3 "" H 4250 2250 50  0001 L CNN
	1    4250 2250
	-1   0    0    -1  
$EndComp
$Comp
L R R21
U 1 1 603FA058
P 2600 1750
F 0 "R21" V 2680 1750 50  0000 C CNN
F 1 "20K Ohms" V 2600 1750 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 2530 1750 50  0001 C CNN
F 3 "" H 2600 1750 50  0001 C CNN
	1    2600 1750
	0    1    1    0   
$EndComp
$Comp
L R R27
U 1 1 603FA1EB
P 4650 2550
F 0 "R27" V 4730 2550 50  0000 C CNN
F 1 "2.4K Ohms" V 4650 2550 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 4580 2550 50  0001 C CNN
F 3 "" H 4650 2550 50  0001 C CNN
	1    4650 2550
	1    0    0    -1  
$EndComp
$Comp
L R R26
U 1 1 603FA20E
P 4300 1400
F 0 "R26" V 4380 1400 50  0000 C CNN
F 1 "10K Ohms" V 4300 1400 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 4230 1400 50  0001 C CNN
F 3 "" H 4300 1400 50  0001 C CNN
	1    4300 1400
	0    1    1    0   
$EndComp
$Comp
L R R25
U 1 1 603FA246
P 3850 850
F 0 "R25" V 3930 850 50  0000 C CNN
F 1 "1.5K Ohms 1/2W" V 3850 850 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Vertical" V 3780 850 50  0001 C CNN
F 3 "" H 3850 850 50  0001 C CNN
	1    3850 850 
	0    1    1    0   
$EndComp
$Comp
L GND #PWR040
U 1 1 603FA319
P 3800 2800
F 0 "#PWR040" H 3800 2550 50  0001 C CNN
F 1 "GND" H 3800 2650 50  0000 C CNN
F 2 "" H 3800 2800 50  0001 C CNN
F 3 "" H 3800 2800 50  0001 C CNN
	1    3800 2800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR041
U 1 1 603FA33D
P 2300 2700
F 0 "#PWR041" H 2300 2450 50  0001 C CNN
F 1 "GND" H 2300 2550 50  0000 C CNN
F 2 "" H 2300 2700 50  0001 C CNN
F 3 "" H 2300 2700 50  0001 C CNN
	1    2300 2700
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR042
U 1 1 603FA3C4
P 2950 1750
F 0 "#PWR042" H 2950 1600 50  0001 C CNN
F 1 "+3.3V" H 2950 1890 50  0000 C CNN
F 2 "" H 2950 1750 50  0001 C CNN
F 3 "" H 2950 1750 50  0001 C CNN
	1    2950 1750
	0    1    1    0   
$EndComp
Text Label 2650 1300 0    60   ~ 0
GATE-EN
Text HLabel 4400 600  2    60   Input ~ 0
Power
Text HLabel 3200 2150 2    60   Output ~ 0
SHORT
Text HLabel 4950 2250 2    60   Input ~ 0
ENABLE
Text Label 3050 2300 0    60   ~ 0
SHORT_
$Comp
L Q_NMOS_GDS Q5
U 1 1 603FACFA
P 1800 6100
F 0 "Q5" H 2000 6150 50  0000 L CNN
F 1 "STD12NF06" H 2000 6050 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 2000 6200 50  0001 C CNN
F 3 "" H 1800 6100 50  0001 C CNN
	1    1800 6100
	0    -1   -1   0   
$EndComp
$Comp
L Q_NPN_EBC Q7
U 1 1 603FB141
P 2750 6350
F 0 "Q7" H 2950 6400 50  0000 L CNN
F 1 "DTC143E" H 3400 6550 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 2950 6450 50  0001 C CNN
F 3 "" H 2750 6350 50  0001 C CNN
	1    2750 6350
	1    0    0    1   
$EndComp
$Comp
L Q_NPN_EBC Q9
U 1 1 603FB1FF
P 3450 6350
F 0 "Q9" H 3650 6400 50  0000 L CNN
F 1 "DTC143E" H 4100 6550 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 3650 6450 50  0001 C CNN
F 3 "" H 3450 6350 50  0001 C CNN
	1    3450 6350
	-1   0    0    1   
$EndComp
$Comp
L Q_NMOS_GDS Q12
U 1 1 603FB48D
P 4300 6050
F 0 "Q12" H 4500 6100 50  0000 L CNN
F 1 "STD12NF06" H 4500 6000 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 4500 6150 50  0001 C CNN
F 3 "" H 4300 6050 50  0001 C CNN
	1    4300 6050
	0    1    -1   0   
$EndComp
$Comp
L LM339 U6
U 1 1 603FB77F
P 4100 4100
F 0 "U6" H 4250 4250 50  0000 C CNN
F 1 "LM339" H 4350 3950 50  0000 C CNN
F 2 "Housings_DIP:DIP-14_W7.62mm" H 4100 4100 50  0001 C CNN
F 3 "" H 4100 4100 50  0001 C CNN
	1    4100 4100
	1    0    0    1   
$EndComp
$Comp
L LM339 U6
U 2 1 603FB7C5
P 4100 4900
F 0 "U6" H 4250 5050 50  0000 C CNN
F 1 "LM339" H 4350 4750 50  0000 C CNN
F 2 "Housings_DIP:DIP-14_W7.62mm" H 4100 4900 50  0001 C CNN
F 3 "" H 4100 4900 50  0001 C CNN
	2    4100 4900
	1    0    0    -1  
$EndComp
Text Label 3400 6850 0    60   ~ 0
SHORT_
Text Label 1950 6600 0    60   ~ 0
GATE-EN
Text Label 4400 6500 0    60   ~ 0
GATE-EN
Text HLabel 1400 6000 0    60   Input ~ 0
OPS-A
Text HLabel 4850 5950 2    60   Input ~ 0
OPS-B
Text HLabel 3500 4400 0    60   Input ~ 0
VREF
Text HLabel 5350 4100 2    60   Output ~ 0
DATA
Text HLabel 5350 4900 2    60   Output ~ 0
DIR
$Comp
L D_Shockley D7
U 1 1 603FBAFB
P 4550 4450
F 0 "D7" H 4550 4550 50  0000 C CNN
F 1 "1N581" H 4550 4350 50  0000 C CNN
F 2 "Diodes_THT:D_DO-41_SOD81_P7.62mm_Horizontal" H 4550 4450 50  0001 C CNN
F 3 "" H 4550 4450 50  0001 C CNN
	1    4550 4450
	0    -1   -1   0   
$EndComp
$Comp
L R R19
U 1 1 603FBCBD
P 2250 4000
F 0 "R19" V 2330 4000 50  0000 C CNN
F 1 "10K Ohms" V 2250 4000 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 2180 4000 50  0001 C CNN
F 3 "" H 2250 4000 50  0001 C CNN
	1    2250 4000
	0    1    1    0   
$EndComp
$Comp
L R R23
U 1 1 603FBD63
P 3300 5000
F 0 "R23" V 3380 5000 50  0000 C CNN
F 1 "10K Ohms" V 3300 5000 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 3230 5000 50  0001 C CNN
F 3 "" H 3300 5000 50  0001 C CNN
	1    3300 5000
	0    1    1    0   
$EndComp
$Comp
L R R24
U 1 1 603FBDDE
P 3350 5950
F 0 "R24" V 3430 5950 50  0000 C CNN
F 1 "2.4 Ohms" V 3350 5950 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 3280 5950 50  0001 C CNN
F 3 "" H 3350 5950 50  0001 C CNN
	1    3350 5950
	0    1    1    0   
$EndComp
$Comp
L R R20
U 1 1 603FBEAC
P 2550 5950
F 0 "R20" V 2630 5950 50  0000 C CNN
F 1 "2.4 Ohms" V 2550 5950 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 2480 5950 50  0001 C CNN
F 3 "" H 2550 5950 50  0001 C CNN
	1    2550 5950
	0    1    1    0   
$EndComp
$Comp
L R R28
U 1 1 603FBF1C
P 4900 4300
F 0 "R28" V 4980 4300 50  0000 C CNN
F 1 "2.4K Ohms" V 4900 4300 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 4830 4300 50  0001 C CNN
F 3 "" H 4900 4300 50  0001 C CNN
	1    4900 4300
	0    1    1    0   
$EndComp
$Comp
L R R29
U 1 1 603FBF92
P 4900 4550
F 0 "R29" V 4980 4550 50  0000 C CNN
F 1 "2.4K Ohms" V 4900 4550 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 4830 4550 50  0001 C CNN
F 3 "" H 4900 4550 50  0001 C CNN
	1    4900 4550
	0    1    1    0   
$EndComp
$Comp
L R R22
U 1 1 603FC067
P 2600 6700
F 0 "R22" V 2680 6700 50  0000 C CNN
F 1 "20K Ohms" V 2600 6700 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 2530 6700 50  0001 C CNN
F 3 "" H 2600 6700 50  0001 C CNN
	1    2600 6700
	0    1    1    0   
$EndComp
Wire Wire Line
	2350 1950 2750 1950
Wire Wire Line
	2450 1750 2450 1950
Connection ~ 2450 1950
Wire Wire Line
	2050 2150 2050 2350
Wire Wire Line
	2050 2350 2750 2350
Wire Wire Line
	2300 2700 2300 2350
Connection ~ 2300 2350
Wire Wire Line
	2950 1750 2750 1750
Wire Wire Line
	2050 1400 3700 1400
Wire Wire Line
	2050 1400 2050 1750
Wire Wire Line
	3700 850  3700 1500
Connection ~ 3700 1400
Wire Wire Line
	2650 1300 2650 1400
Connection ~ 2650 1400
Wire Wire Line
	4000 850  4450 850 
Wire Wire Line
	4000 1700 4150 1700
Connection ~ 4150 1700
Wire Wire Line
	3700 2700 4650 2700
Wire Wire Line
	3800 2700 3800 2800
Wire Wire Line
	4150 2450 4150 2700
Connection ~ 4150 2700
Wire Wire Line
	4450 2250 4950 2250
Wire Wire Line
	4650 2250 4650 2400
Wire Wire Line
	3700 1900 3700 2700
Connection ~ 3800 2700
Wire Wire Line
	4150 1400 4150 2050
Wire Wire Line
	4450 850  4450 1400
Wire Wire Line
	4400 600  4400 850 
Connection ~ 4400 850 
Wire Wire Line
	3200 2150 3050 2150
Connection ~ 4650 2250
Wire Wire Line
	3050 2150 3050 2300
Wire Wire Line
	2400 4000 3800 4000
Wire Wire Line
	2900 4100 2900 4000
Connection ~ 2900 4000
Wire Wire Line
	3800 4200 3800 4800
Wire Wire Line
	3500 4400 3800 4400
Connection ~ 3800 4400
Wire Wire Line
	3450 5000 3800 5000
Wire Wire Line
	3600 5050 3600 5000
Connection ~ 3600 5000
Wire Wire Line
	2700 5950 3200 5950
Wire Wire Line
	2850 6150 3350 6150
Wire Wire Line
	3000 5950 3000 6150
Connection ~ 3000 6150
Connection ~ 3000 5950
Wire Wire Line
	2300 5950 2300 6350
Wire Wire Line
	2100 5950 2400 5950
Wire Wire Line
	2100 4000 2100 5950
Connection ~ 2300 5950
Wire Wire Line
	1600 6000 1400 6000
Wire Wire Line
	1800 6300 1800 6600
Wire Wire Line
	1800 6600 1950 6600
Wire Wire Line
	3500 5950 4100 5950
Wire Wire Line
	4300 6250 4300 6500
Wire Wire Line
	4300 6500 4400 6500
Wire Wire Line
	4850 5950 4500 5950
Wire Wire Line
	2300 6350 2550 6350
Wire Wire Line
	2000 6000 2300 6000
Connection ~ 2300 6000
Wire Wire Line
	3650 5650 3650 6350
Connection ~ 3650 5950
Wire Wire Line
	2850 6550 3350 6550
Wire Wire Line
	2750 6850 3400 6850
Wire Wire Line
	2750 6850 2750 6700
Wire Wire Line
	3100 6850 3100 6550
Connection ~ 3100 6550
Connection ~ 3100 6850
$Comp
L +3.3V #PWR043
U 1 1 603FDC6B
P 2300 6700
F 0 "#PWR043" H 2300 6550 50  0001 C CNN
F 1 "+3.3V" H 2300 6840 50  0000 C CNN
F 2 "" H 2300 6700 50  0001 C CNN
F 3 "" H 2300 6700 50  0001 C CNN
	1    2300 6700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2300 6700 2450 6700
$Comp
L GND #PWR044
U 1 1 603FDE0F
P 3100 6250
F 0 "#PWR044" H 3100 6000 50  0001 C CNN
F 1 "GND" H 3100 6100 50  0000 C CNN
F 2 "" H 3100 6250 50  0001 C CNN
F 3 "" H 3100 6250 50  0001 C CNN
	1    3100 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 6250 3100 6150
Connection ~ 3100 6150
Wire Wire Line
	3650 5650 2950 5650
Wire Wire Line
	2950 5650 2950 5000
Wire Wire Line
	2950 5000 3150 5000
$Comp
L GND #PWR045
U 1 1 603FE247
P 2900 4500
F 0 "#PWR045" H 2900 4250 50  0001 C CNN
F 1 "GND" H 2900 4350 50  0000 C CNN
F 2 "" H 2900 4500 50  0001 C CNN
F 3 "" H 2900 4500 50  0001 C CNN
	1    2900 4500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR046
U 1 1 603FE28B
P 3600 5400
F 0 "#PWR046" H 3600 5150 50  0001 C CNN
F 1 "GND" H 3600 5250 50  0000 C CNN
F 2 "" H 3600 5400 50  0001 C CNN
F 3 "" H 3600 5400 50  0001 C CNN
	1    3600 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 4400 2900 4500
Wire Wire Line
	3600 5350 3600 5400
$Comp
L GND #PWR047
U 1 1 603FE578
P 4000 5350
F 0 "#PWR047" H 4000 5100 50  0001 C CNN
F 1 "GND" H 4000 5200 50  0000 C CNN
F 2 "" H 4000 5350 50  0001 C CNN
F 3 "" H 4000 5350 50  0001 C CNN
	1    4000 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 5200 4000 5350
$Comp
L +3.3V #PWR048
U 1 1 603FE622
P 4150 4550
F 0 "#PWR048" H 4150 4400 50  0001 C CNN
F 1 "+3.3V" H 4150 4690 50  0000 C CNN
F 2 "" H 4150 4550 50  0001 C CNN
F 3 "" H 4150 4550 50  0001 C CNN
	1    4150 4550
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR049
U 1 1 603FE666
P 5200 4400
F 0 "#PWR049" H 5200 4250 50  0001 C CNN
F 1 "+3.3V" H 5200 4540 50  0000 C CNN
F 2 "" H 5200 4400 50  0001 C CNN
F 3 "" H 5200 4400 50  0001 C CNN
	1    5200 4400
	0    1    1    0   
$EndComp
Wire Wire Line
	4150 4550 4000 4550
Wire Wire Line
	4000 4400 4000 4600
Wire Wire Line
	5200 4400 5050 4400
Wire Wire Line
	5050 4300 5050 4550
Connection ~ 5050 4400
Wire Wire Line
	4400 4100 5350 4100
Wire Wire Line
	4550 4100 4550 4300
Wire Wire Line
	4550 4600 4550 4900
Wire Wire Line
	4400 4900 5350 4900
Connection ~ 4550 4900
Connection ~ 4550 4100
Wire Wire Line
	4750 4300 4750 4100
Connection ~ 4750 4100
Wire Wire Line
	4750 4550 4750 4900
Connection ~ 4750 4900
Connection ~ 4000 4550
$Comp
L GND #PWR050
U 1 1 603FEEBA
P 4000 3650
F 0 "#PWR050" H 4000 3400 50  0001 C CNN
F 1 "GND" H 4000 3500 50  0000 C CNN
F 2 "" H 4000 3650 50  0001 C CNN
F 3 "" H 4000 3650 50  0001 C CNN
	1    4000 3650
	-1   0    0    1   
$EndComp
Wire Wire Line
	4000 3650 4000 3800
$Comp
L Q_NMOS_GDS Q14
U 1 1 604F91D8
P 6500 2100
F 0 "Q14" H 6700 2150 50  0000 L CNN
F 1 "STD12NF06" H 6700 2050 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 6700 2200 50  0001 C CNN
F 3 "" H 6500 2100 50  0001 C CNN
	1    6500 2100
	0    -1   -1   0   
$EndComp
$Comp
L Q_NPN_EBC Q15
U 1 1 604F91DE
P 8000 2350
F 0 "Q15" H 8200 2400 50  0000 L CNN
F 1 "DTC143E" H 8650 2550 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 8200 2450 50  0001 C CNN
F 3 "" H 8000 2350 50  0001 C CNN
	1    8000 2350
	1    0    0    1   
$EndComp
$Comp
L Q_NPN_EBC Q16
U 1 1 604F91E4
P 8700 2350
F 0 "Q16" H 8900 2400 50  0000 L CNN
F 1 "DTC143E" H 9350 2550 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 8900 2450 50  0001 C CNN
F 3 "" H 8700 2350 50  0001 C CNN
	1    8700 2350
	-1   0    0    1   
$EndComp
$Comp
L Q_NMOS_GDS Q17
U 1 1 604F91EA
P 10150 2050
F 0 "Q17" H 10350 2100 50  0000 L CNN
F 1 "STD12NF06" H 10350 2000 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 10350 2150 50  0001 C CNN
F 3 "" H 10150 2050 50  0001 C CNN
	1    10150 2050
	0    1    -1   0   
$EndComp
Text Label 8650 2850 0    60   ~ 0
SHORT_
Text Label 6500 2650 0    60   ~ 0
GATE-EN
Text Label 10450 2500 0    60   ~ 0
GATE-EN
Text HLabel 6000 2000 0    60   Input ~ 0
OLBC-A
Text HLabel 10700 1950 2    60   Input ~ 0
OLBC-B
$Comp
L R R35
U 1 1 604F91F5
P 8600 1950
F 0 "R35" V 8680 1950 50  0000 C CNN
F 1 "24.9 Ohms" V 8600 1950 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 8530 1950 50  0001 C CNN
F 3 "" H 8600 1950 50  0001 C CNN
	1    8600 1950
	0    1    1    0   
$EndComp
$Comp
L R R33
U 1 1 604F91FB
P 7800 1950
F 0 "R33" V 7880 1950 50  0000 C CNN
F 1 "24.9 Ohms" V 7800 1950 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 7730 1950 50  0001 C CNN
F 3 "" H 7800 1950 50  0001 C CNN
	1    7800 1950
	0    1    1    0   
$EndComp
Wire Wire Line
	7950 1950 8450 1950
Wire Wire Line
	8100 2150 8600 2150
Wire Wire Line
	8250 1950 8250 2150
Connection ~ 8250 2150
Connection ~ 8250 1950
Wire Wire Line
	7550 1950 7550 2350
Wire Wire Line
	6300 2000 6000 2000
Wire Wire Line
	8750 1950 9350 1950
Wire Wire Line
	10150 2250 10150 2500
Wire Wire Line
	10150 2500 10450 2500
Wire Wire Line
	10700 1950 10350 1950
Wire Wire Line
	7550 2350 7800 2350
Connection ~ 7550 2000
Connection ~ 8900 1950
Wire Wire Line
	8100 2550 8600 2550
Connection ~ 8350 2550
$Comp
L GND #PWR051
U 1 1 604F9229
P 8350 2250
F 0 "#PWR051" H 8350 2000 50  0001 C CNN
F 1 "GND" H 8450 2300 50  0000 C CNN
F 2 "" H 8350 2250 50  0001 C CNN
F 3 "" H 8350 2250 50  0001 C CNN
	1    8350 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 2250 8350 2150
Connection ~ 8350 2150
Wire Wire Line
	8650 2850 8350 2850
Wire Wire Line
	8350 2850 8350 2550
Wire Wire Line
	6500 2300 6500 2650
$Comp
L R R32
U 1 1 604FA2AF
P 7100 2000
F 0 "R32" V 7180 2000 50  0000 C CNN
F 1 "24.9" V 7100 2000 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P1.90mm_Vertical" V 7030 2000 50  0001 C CNN
F 3 "" H 7100 2000 50  0001 C CNN
	1    7100 2000
	0    1    1    0   
$EndComp
Wire Wire Line
	7550 2000 7250 2000
Wire Wire Line
	6950 2000 6700 2000
$Comp
L R R36
U 1 1 604FA777
P 9500 1950
F 0 "R36" V 9580 1950 50  0000 C CNN
F 1 "24.9 Ohms" V 9500 1950 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P1.90mm_Vertical" V 9430 1950 50  0001 C CNN
F 3 "" H 9500 1950 50  0001 C CNN
	1    9500 1950
	0    1    1    0   
$EndComp
Wire Wire Line
	9950 1950 9650 1950
Wire Wire Line
	7550 1950 7650 1950
$Comp
L LM339 U6
U 3 1 604FBDB2
P 8600 1000
F 0 "U6" H 8600 1200 50  0000 L CNN
F 1 "LM339" H 8600 800 50  0000 L CNN
F 2 "Housings_DIP:DIP-14_W7.62mm" H 8550 1100 50  0001 C CNN
F 3 "" H 8650 1200 50  0001 C CNN
	3    8600 1000
	1    0    0    -1  
$EndComp
$Comp
L LM339 U6
U 4 1 604FC03C
P 7850 3700
F 0 "U6" H 7850 3900 50  0000 L CNN
F 1 "LM339" H 7850 3500 50  0000 L CNN
F 2 "Housings_DIP:DIP-14_W7.62mm" H 7800 3800 50  0001 C CNN
F 3 "" H 7900 3900 50  0001 C CNN
	4    7850 3700
	1    0    0    1   
$EndComp
$Comp
L C C14
U 1 1 604FCB11
P 2900 4250
F 0 "C14" H 2925 4350 50  0000 L CNN
F 1 "33pf" H 2925 4150 50  0000 L CNN
F 2 "Capacitors_THT:C_Rect_L4.0mm_W2.5mm_P2.50mm" H 2938 4100 50  0001 C CNN
F 3 "" H 2900 4250 50  0001 C CNN
	1    2900 4250
	1    0    0    -1  
$EndComp
$Comp
L C C15
U 1 1 604FCC0D
P 3600 5200
F 0 "C15" H 3625 5300 50  0000 L CNN
F 1 "33pf" H 3625 5100 50  0000 L CNN
F 2 "Capacitors_THT:C_Rect_L4.0mm_W2.5mm_P2.50mm" H 3638 5050 50  0001 C CNN
F 3 "" H 3600 5200 50  0001 C CNN
	1    3600 5200
	1    0    0    -1  
$EndComp
$Comp
L R R34
U 1 1 604FD4AC
P 8000 1100
F 0 "R34" V 8080 1100 50  0000 C CNN
F 1 "10K Ohms" V 8000 1100 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P1.90mm_Vertical" V 7930 1100 50  0001 C CNN
F 3 "" H 8000 1100 50  0001 C CNN
	1    8000 1100
	0    1    1    0   
$EndComp
$Comp
L C C16
U 1 1 604FD558
P 8250 1350
F 0 "C16" H 8275 1450 50  0000 L CNN
F 1 "33pf" H 8275 1250 50  0000 L CNN
F 2 "Capacitors_THT:C_Rect_L4.0mm_W2.5mm_P2.50mm" H 8288 1200 50  0001 C CNN
F 3 "" H 8250 1350 50  0001 C CNN
	1    8250 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 1100 7600 1800
Wire Wire Line
	7600 1100 7850 1100
Wire Wire Line
	8150 1100 8300 1100
Wire Wire Line
	8250 1200 8250 1100
Connection ~ 8250 1100
$Comp
L GND #PWR052
U 1 1 604FD7D9
P 8250 1600
F 0 "#PWR052" H 8250 1350 50  0001 C CNN
F 1 "GND" H 8250 1450 50  0000 C CNN
F 2 "" H 8250 1600 50  0001 C CNN
F 3 "" H 8250 1600 50  0001 C CNN
	1    8250 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 1800 8900 2350
Wire Wire Line
	7600 1800 8900 1800
Wire Wire Line
	8250 1500 8250 1600
Text HLabel 7950 900  0    60   Input ~ 0
VREF-OLBC
Wire Wire Line
	8300 900  7950 900 
Text HLabel 9300 1000 2    60   Output ~ 0
DATA
Wire Wire Line
	9300 1000 8900 1000
$Comp
L +3.3V #PWR053
U 1 1 604FEB47
P 7550 3400
F 0 "#PWR053" H 7550 3250 50  0001 C CNN
F 1 "+3.3V" H 7550 3540 50  0000 C CNN
F 2 "" H 7550 3400 50  0001 C CNN
F 3 "" H 7550 3400 50  0001 C CNN
	1    7550 3400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR054
U 1 1 604FEBA3
P 7550 4000
F 0 "#PWR054" H 7550 3750 50  0001 C CNN
F 1 "GND" H 7550 3850 50  0000 C CNN
F 2 "" H 7550 4000 50  0001 C CNN
F 3 "" H 7550 4000 50  0001 C CNN
	1    7550 4000
	1    0    0    -1  
$EndComp
NoConn ~ 8150 3700
Wire Wire Line
	7550 3800 7550 4000
Wire Wire Line
	7550 3600 7550 3400
$EndSCHEMATC
