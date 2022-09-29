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
LIBS:sn65hvd233-ht
LIBS:lm2574n-5
LIBS:UniversalLCCNodeBreakout-cache
EELAYER 25 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 1
Title "ESP32 S3 Multifunction Board"
Date ""
Rev "1.0"
Comp "Deepwoods Software"
Comment1 "Power Supply"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L TCAN332DR U101
U 1 1 63359A12
P 3750 3375
F 0 "U101" H 3750 3325 30  0000 C CNN
F 1 "TCAN332DR" H 3750 3425 30  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 3750 3375 60  0001 C CNN
F 3 "~" H 3750 3375 60  0000 C CNN
F 4 "595-TCAN332DR" H 3750 3375 60  0001 C CNN "Mouser Part Number"
	1    3750 3375
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR01
U 1 1 63359A13
P 3750 2875
F 0 "#PWR01" H 3750 2975 40  0001 C CNN
F 1 "+3V3" H 3750 3000 40  0000 C CNN
F 2 "" H 3750 2875 60  0000 C CNN
F 3 "" H 3750 2875 60  0000 C CNN
	1    3750 2875
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 63359A14
P 3750 3775
F 0 "#PWR02" H 3750 3775 30  0001 C CNN
F 1 "GND" H 3750 3705 30  0001 C CNN
F 2 "" H 3750 3775 60  0000 C CNN
F 3 "" H 3750 3775 60  0000 C CNN
	1    3750 3775
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 2875 3750 3025
Wire Wire Line
	3750 3725 3750 3775
Wire Wire Line
	3750 4875 3750 4925
Wire Wire Line
	4300 4875 3750 4875
Wire Wire Line
	3500 3175 3450 3175
Wire Wire Line
	3450 3175 3450 4275
Wire Wire Line
	3500 3275 3400 3275
$Comp
L C C102
U 1 1 63359A15
P 4300 3775
F 0 "C102" H 4300 3875 40  0000 L CNN
F 1 ".1 uf" H 4306 3690 40  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4338 3625 30  0001 C CNN
F 3 "~" H 4300 3775 60  0000 C CNN
F 4 "710-885012206095" H 4300 3775 60  0001 C CNN "Mouser Part Number"
	1    4300 3775
	1    0    0    -1  
$EndComp
$Comp
L CP1 C104
U 1 1 63359A16
P 4650 3775
F 0 "C104" H 4700 3875 50  0000 L CNN
F 1 "15 uf 15V" H 4700 3675 50  0000 L CNN
F 2 "Capacitors_SMD:CP_Elec_4x5.7" H 4650 3775 60  0001 C CNN
F 3 "~" H 4650 3775 60  0000 C CNN
F 4 "647-UUD1C150MCL" H 4650 3775 60  0001 C CNN "Mouser Part Number"
	1    4650 3775
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 2975 4300 2975
Wire Wire Line
	4300 2975 4300 3625
Connection ~ 3750 2975
Wire Wire Line
	4300 3575 4650 3575
Wire Wire Line
	4650 3975 4300 3975
Wire Wire Line
	4300 3925 4300 4875
Wire Wire Line
	4000 3225 7400 3225
Wire Wire Line
	4000 3525 4100 3525
Wire Wire Line
	4100 3525 4100 3075
Wire Wire Line
	4100 3075 7450 3075
Text Label 5050 3075 0    25   ~ 0
CANH
Text Label 5050 3225 0    25   ~ 0
CANL
$Comp
L R R101
U 1 1 63359A17
P 5500 3625
F 0 "R101" V 5580 3625 40  0000 C CNN
F 1 "60 Ohms" V 5507 3626 40  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 5430 3625 30  0001 C CNN
F 3 "~" H 5500 3625 30  0000 C CNN
F 4 "71-CRCW120660R0KNAIF" H 5500 3625 60  0001 C CNN "Mouser Part Number"
	1    5500 3625
	1    0    0    -1  
$EndComp
$Comp
L R R102
U 1 1 63359A18
P 6200 3625
F 0 "R102" V 6280 3625 40  0000 C CNN
F 1 "60 Ohms" V 6207 3626 40  0000 C CNN
F 2 "Resistors_SMD:R_1206" V 6130 3625 30  0001 C CNN
F 3 "~" H 6200 3625 30  0000 C CNN
F 4 "71-CRCW120660R0KNAIF" H 6200 3625 60  0001 C CNN "Mouser Part Number"
	1    6200 3625
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 3075 5500 3475
Connection ~ 5500 3075
Wire Wire Line
	6200 3225 6200 3475
$Comp
L Conn_02x02_Odd_Even JP101
U 1 1 63359A19
P 5850 4175
F 0 "JP101" H 5850 4325 50  0000 C CNN
F 1 "Termination" H 5860 4045 40  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x02_Pitch2.54mm" H 5850 4175 60  0001 C CNN
F 3 "" H 5850 4175 60  0000 C CNN
F 4 "649-67997-404HLF" H 5850 4175 60  0001 C CNN "Mouser Part Number"
	1    5850 4175
	1    0    0    -1  
$EndComp
$Comp
L C C105
U 1 1 63359A1A
P 5850 4725
F 0 "C105" H 5850 4825 40  0000 L CNN
F 1 "47 nf" H 5856 4640 40  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5888 4575 30  0001 C CNN
F 3 "~" H 5850 4725 60  0000 C CNN
F 4 "710-885012206093" H 5850 4725 60  0001 C CNN "Mouser Part Number"
	1    5850 4725
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 63359A1B
P 5850 5075
F 0 "#PWR03" H 5850 5075 30  0001 C CNN
F 1 "GND" H 5850 5005 30  0001 C CNN
F 2 "" H 5850 5075 60  0000 C CNN
F 3 "" H 5850 5075 60  0000 C CNN
	1    5850 5075
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 4425 5850 4575
Connection ~ 5850 4425
Wire Wire Line
	5850 4875 5850 5075
$Comp
L RJ45 J103
U 1 1 63359A1C
P 8000 4675
F 0 "J103" H 8200 5175 60  0000 C CNN
F 1 "RJ45" H 7850 5175 60  0000 C CNN
F 2 "RJ45-8N-S:RJ45_8N-S" H 8000 4675 60  0001 C CNN
F 3 "" H 8000 4675 60  0000 C CNN
F 4 "710-615008144221" H 8000 4675 60  0001 C CNN "Mouser Part Number"
	1    8000 4675
	0    1    1    0   
$EndComp
$Comp
L RJ45 J102
U 1 1 63359A1D
P 8000 3025
F 0 "J102" H 8200 3525 60  0000 C CNN
F 1 "RJ45" H 7850 3525 60  0000 C CNN
F 2 "RJ45-8N-S:RJ45_8N-S" H 8000 3025 60  0001 C CNN
F 3 "" H 8000 3025 60  0000 C CNN
F 4 "710-615008144221" H 8000 3025 60  0001 C CNN "Mouser Part Number"
	1    8000 3025
	0    1    1    0   
$EndComp
NoConn ~ 8350 3575
NoConn ~ 8350 5225
Wire Wire Line
	7450 2675 7450 4325
Wire Wire Line
	7450 2675 7550 2675
Wire Wire Line
	7450 4325 7550 4325
Connection ~ 7450 3075
Wire Wire Line
	7550 2775 7400 2775
Wire Wire Line
	7400 2775 7400 4425
Wire Wire Line
	7400 4425 7550 4425
Connection ~ 7400 3225
Connection ~ 6200 3225
Wire Wire Line
	7150 2875 7550 2875
Wire Wire Line
	7350 2875 7350 4525
Wire Wire Line
	7350 4525 7550 4525
Wire Wire Line
	7550 4625 7300 4625
Wire Wire Line
	7300 4625 7300 2975
Wire Wire Line
	7300 2975 7550 2975
Wire Wire Line
	7550 3075 7550 3125
Wire Wire Line
	7550 3125 7250 3125
Wire Wire Line
	7250 3125 7250 4725
Wire Wire Line
	7250 4725 7550 4725
Wire Wire Line
	7550 3175 7200 3175
Wire Wire Line
	7200 2875 7200 4825
Wire Wire Line
	7200 4825 7550 4825
Wire Wire Line
	7150 3275 7550 3275
Wire Wire Line
	7150 2875 7150 4925
Wire Wire Line
	7150 4925 7550 4925
Wire Wire Line
	7100 5025 7550 5025
Wire Wire Line
	7100 3375 7550 3375
Connection ~ 7350 2875
Connection ~ 7200 3175
Connection ~ 7200 2875
Connection ~ 7150 3275
Wire Wire Line
	6100 2925 7150 2925
Connection ~ 7100 3375
Connection ~ 7150 2925
Wire Wire Line
	5450 2975 7100 2975
Wire Wire Line
	7100 2975 7100 5025
Text GLabel 5450 2975 0    100  Input ~ 0
CAN +12V
Wire Wire Line
	3450 4275 2275 4275
$Comp
L GND #PWR04
U 1 1 63359A23
P 3750 4925
F 0 "#PWR04" H 3750 4925 30  0001 C CNN
F 1 "GND" H 3750 4855 30  0001 C CNN
F 2 "" H 3750 4925 60  0000 C CNN
F 3 "" H 3750 4925 60  0000 C CNN
	1    3750 4925
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 3575 4650 3625
Connection ~ 4300 3575
Wire Wire Line
	4650 3925 4650 3975
Connection ~ 4300 3975
Wire Wire Line
	5500 3775 5500 4175
Wire Wire Line
	5500 4175 5650 4175
Wire Wire Line
	6200 3775 6200 4175
Wire Wire Line
	6200 4175 6150 4175
Wire Wire Line
	6150 4425 6150 4275
Wire Wire Line
	5600 4425 6150 4425
Wire Wire Line
	5600 4425 5600 4275
Wire Wire Line
	5600 4275 5650 4275
NoConn ~ 3500 3475
NoConn ~ 3500 3575
$Comp
L LM2574HVM-5.0 U102
U 1 1 6335AD2C
P 3825 6725
F 0 "U102" H 3825 6775 60  0000 C CNN
F 1 "LM2574N-5" H 3825 6875 21  0000 C CNN
F 2 "SOIC_Wide:SOIC-14_7.4x8.7mm_Pitch1.27mm" H 3825 6725 60  0001 C CNN
F 3 "~" H 3825 6725 60  0000 C CNN
F 4 "926-LM2574MX-5.0NOPB" H 3825 6725 60  0001 C CNN "Mouser Part Number"
	1    3825 6725
	1    0    0    -1  
$EndComp
$Comp
L CP1 C103
U 1 1 6335AD2D
P 4575 6825
F 0 "C103" H 4625 6925 50  0000 L CNN
F 1 "22 uf 100V" H 4625 6725 50  0000 L CNN
F 2 "Capacitors_SMD:CP_Elec_8x10" H 4575 6825 60  0001 C CNN
F 3 "~" H 4575 6825 60  0000 C CNN
F 4 "647-UUX2A220MNL6GS" H 4575 6825 60  0001 C CNN "Mouser Part Number"
	1    4575 6825
	1    0    0    -1  
$EndComp
$Comp
L CP1 C101
U 1 1 6335AD2E
P 2325 6725
F 0 "C101" H 2375 6825 50  0000 L CNN
F 1 "220 uf 25V" H 2375 6625 50  0000 L CNN
F 2 "Capacitors_SMD:CP_Elec_8x10" H 2325 6725 60  0001 C CNN
F 3 "~" H 2325 6725 60  0000 C CNN
F 4 "647-UWT1E221MNL1GS" H 2325 6725 60  0001 C CNN "Mouser Part Number"
	1    2325 6725
	1    0    0    -1  
$EndComp
$Comp
L L_Small L101
U 1 1 6335AD2F
P 3175 6825
F 0 "L101" H 3175 6925 50  0000 C CNN
F 1 "330 uh" H 3175 6775 50  0000 C CNN
F 2 "SRN1060:SRN1060" H 3175 6825 60  0001 C CNN
F 3 "~" H 3175 6825 60  0000 C CNN
F 4 "652-SRN1060-331M" H 3175 6825 60  0001 C CNN "Mouser Part Number"
	1    3175 6825
	0    1    1    0   
$EndComp
$Comp
L D_Schottky D101
U 1 1 6335AD30
P 3475 7025
F 0 "D101" H 3475 7125 40  0000 C CNN
F 1 "CMSH1-60 TR13" H 3475 6925 40  0000 C CNN
F 2 "Diodes_SMD:D_SMB" H 3475 7025 60  0001 C CNN
F 3 "~" H 3475 7025 60  0000 C CNN
F 4 "610-CMSH1-60TR13" H 3475 7025 60  0001 C CNN "Mouser Part Number"
	1    3475 7025
	0    1    1    0   
$EndComp
Wire Wire Line
	4575 6625 4125 6625
Wire Wire Line
	2325 6525 3525 6525
Wire Wire Line
	2925 6825 2925 6525
Connection ~ 2925 6525
Wire Wire Line
	3275 6825 3525 6825
Connection ~ 3475 6825
Wire Wire Line
	4575 7075 4575 6975
Wire Wire Line
	3775 7075 4575 7075
Connection ~ 3975 7075
Connection ~ 3875 7075
Wire Wire Line
	3775 7225 3775 7075
Wire Wire Line
	2325 7225 3775 7225
Wire Wire Line
	2325 7225 2325 6875
Connection ~ 3475 7225
$Comp
L GND #PWR05
U 1 1 6335AD31
P 3475 7325
F 0 "#PWR05" H 3475 7325 30  0001 C CNN
F 1 "GND" H 3475 7255 30  0001 C CNN
F 2 "" H 3475 7325 60  0000 C CNN
F 3 "" H 3475 7325 60  0000 C CNN
	1    3475 7325
	1    0    0    -1  
$EndComp
Wire Wire Line
	4575 6425 4575 6675
Wire Wire Line
	2325 6375 2325 6575
Wire Wire Line
	3475 7175 3475 7325
Text GLabel 4875 6425 2    50   Input ~ 0
CAN +12V
Connection ~ 2325 6525
Wire Wire Line
	3075 6825 2925 6825
Wire Wire Line
	3475 6875 3475 6825
Connection ~ 4575 6625
NoConn ~ 4125 6825
NoConn ~ 4125 6725
NoConn ~ 3975 6375
NoConn ~ 3925 6375
NoConn ~ 3875 6375
NoConn ~ 3825 6375
NoConn ~ 3775 6375
NoConn ~ 3725 6375
$Comp
L +5V #PWR06
U 1 1 6335BFCA
P 2325 6375
F 0 "#PWR06" H 2325 6225 50  0001 C CNN
F 1 "+5V" H 2325 6515 50  0000 C CNN
F 2 "" H 2325 6375 50  0001 C CNN
F 3 "" H 2325 6375 50  0001 C CNN
	1    2325 6375
	1    0    0    -1  
$EndComp
Text Label 3250 4275 2    60   ~ 0
CAN_TX
Text Label 3250 4175 2    60   ~ 0
CAN_RX
$Comp
L Conn_01x05_Male J101
U 1 1 6335C5F9
P 2075 4175
F 0 "J101" H 2075 4475 50  0000 C CNN
F 1 "5V 3V CR CT G" H 2075 3875 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x05_Pitch2.54mm" H 2075 4175 50  0001 C CNN
F 3 "" H 2075 4175 50  0001 C CNN
	1    2075 4175
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR07
U 1 1 6335C6B0
P 2275 3975
F 0 "#PWR07" H 2275 3825 50  0001 C CNN
F 1 "+5V" H 2275 4115 50  0000 C CNN
F 2 "" H 2275 3975 50  0001 C CNN
F 3 "" H 2275 3975 50  0001 C CNN
	1    2275 3975
	0    1    1    0   
$EndComp
$Comp
L +3.3V #PWR08
U 1 1 6335C71A
P 2275 4075
F 0 "#PWR08" H 2275 3925 50  0001 C CNN
F 1 "+3.3V" H 2275 4215 50  0000 C CNN
F 2 "" H 2275 4075 50  0001 C CNN
F 3 "" H 2275 4075 50  0001 C CNN
	1    2275 4075
	0    1    1    0   
$EndComp
$Comp
L GND #PWR09
U 1 1 6335C784
P 2275 4375
F 0 "#PWR09" H 2275 4125 50  0001 C CNN
F 1 "GND" H 2275 4225 50  0000 C CNN
F 2 "" H 2275 4375 50  0001 C CNN
F 3 "" H 2275 4375 50  0001 C CNN
	1    2275 4375
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2275 4175 3400 4175
Wire Wire Line
	3400 4175 3400 3275
Wire Wire Line
	4875 6425 4575 6425
$Comp
L Conn_01x02_Female J104
U 1 1 6335EE91
P 7750 3725
F 0 "J104" H 7750 3825 50  0000 C CNN
F 1 "5 4" H 7750 3525 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x02_Pitch2.54mm" H 7750 3725 50  0001 C CNN
F 3 "" H 7750 3725 50  0001 C CNN
	1    7750 3725
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 3725 7300 3725
Connection ~ 7300 3725
Wire Wire Line
	7550 3825 7250 3825
Connection ~ 7250 3825
$Comp
L GND #PWR010
U 1 1 63360580
P 6100 2800
F 0 "#PWR010" H 6100 2550 50  0001 C CNN
F 1 "GND" H 6100 2650 50  0000 C CNN
F 2 "" H 6100 2800 50  0001 C CNN
F 3 "" H 6100 2800 50  0001 C CNN
	1    6100 2800
	-1   0    0    1   
$EndComp
Wire Wire Line
	6100 2800 6100 2925
$EndSCHEMATC
