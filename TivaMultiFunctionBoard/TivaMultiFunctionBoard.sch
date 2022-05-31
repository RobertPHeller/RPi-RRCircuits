EESchema Schematic File Version 2
LIBS:TivaMultiFunctionBoard-rescue
LIBS:boosterpack
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
LIBS:esp32_devboards
LIBS:sn65hvd233-ht
LIBS:lm2574n-5
LIBS:MCP23xxx
LIBS:tbd62x83a
LIBS:tc442x
LIBS:mct6h
LIBS:bridge
LIBS:pca9685
LIBS:4814p-2
LIBS:mechanical
LIBS:graphic_symbols
LIBS:references
LIBS:TivaMultiFunctionBoard-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 7
Title "Tive Multifunction Board"
Date "19 oct 2012"
Rev ""
Comp "Deepwoods Software"
Comment1 "Main Page: Tiva Headers, GPIO Header"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L GND #PWR01
U 1 1 5080AA99
P 9150 2300
F 0 "#PWR01" H 9150 2050 50  0001 C CNN
F 1 "GND" H 9150 2150 50  0000 C CNN
F 2 "" H 9150 2300 50  0000 C CNN
F 3 "" H 9150 2300 50  0000 C CNN
	1    9150 2300
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR02
U 1 1 5080AA9F
P 9150 750
F 0 "#PWR02" H 9150 600 50  0001 C CNN
F 1 "VCC" H 9150 900 50  0000 C CNN
F 2 "" H 9150 750 50  0000 C CNN
F 3 "" H 9150 750 50  0000 C CNN
	1    9150 750 
	1    0    0    -1  
$EndComp
$Comp
L Ti_Booster_40_J1 J1
U 1 1 5080DB5C
P 9750 1200
F 0 "J1" H 9700 1850 60  0000 C CNN
F 1 "TI_BOOSTER_40_J1" H 9750 550 60  0000 C CNN
F 2 "Connect:SIL-10" V 10200 1200 60  0000 C CNN
F 3 "" H 9750 1200 60  0001 C CNN
	1    9750 1200
	1    0    0    -1  
$EndComp
$Comp
L Ti_Booster_40_J2 J2
U 1 1 5080DBF4
P 9750 2750
F 0 "J2" H 9700 3400 60  0000 C CNN
F 1 "TI_BOOSTER_40_J2" H 9750 2100 60  0000 C CNN
F 2 "Connect:SIL-10" V 10200 2750 60  0000 C CNN
F 3 "" H 9750 2750 60  0001 C CNN
	1    9750 2750
	1    0    0    -1  
$EndComp
$Comp
L Ti_Booster_40_J3 J3
U 1 1 5080DC03
P 9750 4300
F 0 "J3" H 9700 4950 60  0000 C CNN
F 1 "TI_BOOSTER_40_J3" H 9750 3650 60  0000 C CNN
F 2 "Connect:SIL-10" V 10200 4300 60  0000 C CNN
F 3 "" H 9750 4300 60  0001 C CNN
	1    9750 4300
	1    0    0    -1  
$EndComp
$Comp
L Ti_Booster_40_J4 J4
U 1 1 5080DC12
P 9750 5850
F 0 "J4" H 9700 6500 60  0000 C CNN
F 1 "TI_BOOSTER_40_J4" H 9750 5200 60  0000 C CNN
F 2 "Connect:SIL-10" V 10200 5850 60  0000 C CNN
F 3 "" H 9750 5850 60  0001 C CNN
	1    9750 5850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 5080DC79
P 9150 3950
F 0 "#PWR03" H 9150 3700 50  0001 C CNN
F 1 "GND" H 9150 3800 50  0000 C CNN
F 2 "" H 9150 3950 50  0000 C CNN
F 3 "" H 9150 3950 50  0000 C CNN
	1    9150 3950
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR04
U 1 1 5080DC8B
P 8350 3650
F 0 "#PWR04" H 8350 3500 50  0001 C CNN
F 1 "+5V" H 8350 3790 50  0000 C CNN
F 2 "" H 8350 3650 50  0000 C CNN
F 3 "" H 8350 3650 50  0000 C CNN
	1    8350 3650
	1    0    0    -1  
$EndComp
$Sheet
S 1250 950  1250 900 
U 60CCDA7F
F0 "CAN Transceiver" 60
F1 "CAN_Transeiver.sch" 60
F2 "CAN_TX" I L 1250 1150 60 
F3 "CAN_RX" I L 1250 1300 60 
$EndSheet
$Sheet
S 3100 900  1600 900 
U 60CCFE36
F0 "Power Supply" 60
F1 "PowerSupply.sch" 60
$EndSheet
$Sheet
S 4550 3800 1850 1400
U 60CD053F
F0 "PWM Driver" 60
F1 "PCA9685Driver.sch" 60
F2 "SCL" I L 4550 4000 60 
F3 "SDA" B L 4550 4200 60 
$EndSheet
$Sheet
S 1200 2200 2250 1350
U 60CD421F
F0 "Occupancy Detectors" 60
F1 "OccDetect4.sch" 60
F2 "OD1" O R 3450 2450 60 
F3 "OD2" O R 3450 2600 60 
F4 "OD3" O R 3450 2750 60 
F5 "OD4" O R 3450 2900 60 
$EndSheet
$Sheet
S 1150 3800 2350 1300
U 60CD422F
F0 "Stall Motors" 60
F1 "StallMotors4.sch" 60
F2 "Points1" O R 3500 3900 60 
F3 "Points2" O R 3500 4050 60 
F4 "Points3" O R 3500 4200 60 
F5 "Points4" O R 3500 4350 60 
F6 "Motor1" I R 3500 4500 60 
F7 "Motor2" I R 3500 4650 60 
F8 "Motor3" I R 3500 4800 60 
F9 "Motor4" I R 3500 4950 60 
$EndSheet
$Sheet
S 1100 5750 2500 1400
U 60CD423F
F0 "Buttons And LEDs" 60
F1 "Buttons4LEDs4.sch" 60
F2 "LED1" I R 3600 6050 60 
F3 "LED2" I R 3600 6150 60 
F4 "LED3" I R 3600 6250 60 
F5 "LED4" I R 3600 6350 60 
F6 "Button1" O R 3600 6500 60 
F7 "Button2" O R 3600 6600 60 
F8 "Button3" O R 3600 6700 60 
F9 "Button4" O R 3600 6800 60 
$EndSheet
$Comp
L D_Shockley D16
U 1 1 6287E9F7
P 8750 3850
F 0 "D16" H 8750 3950 50  0000 C CNN
F 1 "DFLS240-7" H 8750 3750 50  0000 C CNN
F 2 "Diodes_SMD:D_SMB" H 8750 3850 50  0001 C CNN
F 3 "" H 8750 3850 50  0001 C CNN
F 4 "621-DFLS240-7" H 8750 3850 60  0001 C CNN "Mouser Part Number"
	1    8750 3850
	-1   0    0    1   
$EndComp
$Comp
L +3.3V #PWR05
U 1 1 6287F808
P 8800 750
F 0 "#PWR05" H 8800 600 50  0001 C CNN
F 1 "+3.3V" H 8800 890 50  0000 C CNN
F 2 "" H 8800 750 50  0001 C CNN
F 3 "" H 8800 750 50  0001 C CNN
	1    8800 750 
	1    0    0    -1  
$EndComp
Text Label 9150 1150 2    60   ~ 0
CAN0RX
Text Label 9150 1250 2    60   ~ 0
CAN0TX
Text Label 1250 1150 2    60   ~ 0
CAN0TX
Text Label 1250 1300 2    60   ~ 0
CAN0RX
Text Label 9150 1550 2    60   ~ 0
SCL
Text Label 9150 1650 2    60   ~ 0
SDA
Text Label 4550 4000 2    60   ~ 0
SCL
Text Label 4550 4200 2    60   ~ 0
SDA
Text Label 3450 2450 0    60   ~ 0
OD1
Text Label 3450 2600 0    60   ~ 0
OD2
Text Label 3450 2750 0    60   ~ 0
OD3
Text Label 3450 2900 0    60   ~ 0
OD4
Text Label 9150 6200 2    60   ~ 0
OD1
Text Label 9150 6300 2    60   ~ 0
OD2
Text Label 9150 1450 2    60   ~ 0
OD3
Text Label 9150 2400 2    60   ~ 0
OD4
Text Label 3500 3900 0    60   ~ 0
Points1
Text Label 3500 4050 0    60   ~ 0
Points2
Text Label 3500 4200 0    60   ~ 0
Points3
Text Label 3500 4350 0    60   ~ 0
Points4
Text Label 3500 4500 0    60   ~ 0
Motor1
Text Label 3500 4650 0    60   ~ 0
Motor2
Text Label 3500 4800 0    60   ~ 0
Motor3
Text Label 3500 4950 0    60   ~ 0
Motor4
Text Label 3600 6050 0    60   ~ 0
LED1
Text Label 3600 6150 0    60   ~ 0
LED2
Text Label 3600 6250 0    60   ~ 0
LED3
Text Label 3600 6350 0    60   ~ 0
LED4
Text Label 3600 6500 0    60   ~ 0
Button1
Text Label 3600 6800 0    60   ~ 0
Button4
Text Label 3600 6600 0    60   ~ 0
Button2
Text Label 3600 6700 0    60   ~ 0
Button3
Text Label 9150 5400 2    60   ~ 0
Points1
Text Label 9150 5500 2    60   ~ 0
Points2
Text Label 9150 5600 2    60   ~ 0
Points3
Text Label 9150 5700 2    60   ~ 0
Points4
Text Label 9150 5800 2    60   ~ 0
Motor1
Text Label 9150 5900 2    60   ~ 0
Motor2
Text Label 9150 6000 2    60   ~ 0
Motor3
Text Label 9150 6100 2    60   ~ 0
Motor4
Text Label 9150 4050 2    60   ~ 0
LED1
Text Label 9150 4150 2    60   ~ 0
LED2
Text Label 9150 4250 2    60   ~ 0
LED3
Text Label 9150 4350 2    60   ~ 0
LED4
Text Label 9150 4450 2    60   ~ 0
Button1
Text Label 9150 4550 2    60   ~ 0
Button2
Text Label 9150 4650 2    60   ~ 0
Button3
Text Label 9150 4750 2    60   ~ 0
Button4
$Comp
L Conn_02x05_Odd_Even J5
U 1 1 62895D18
P 8400 2750
F 0 "J5" H 8450 3050 50  0000 C CNN
F 1 "GPIO Header" H 8450 2450 50  0000 C CNN
F 2 "Connectors:IDC_Header_Straight_10pins" H 8400 2750 50  0001 C CNN
F 3 "" H 8400 2750 50  0001 C CNN
	1    8400 2750
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR06
U 1 1 628975DC
P 8100 2750
F 0 "#PWR06" H 8100 2600 50  0001 C CNN
F 1 "+3.3V" H 8100 2890 50  0000 C CNN
F 2 "" H 8100 2750 50  0001 C CNN
F 3 "" H 8100 2750 50  0001 C CNN
	1    8100 2750
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR07
U 1 1 628988E0
P 8750 2750
F 0 "#PWR07" H 8750 2500 50  0001 C CNN
F 1 "GND" H 8750 2600 50  0000 C CNN
F 2 "" H 8750 2750 50  0001 C CNN
F 3 "" H 8750 2750 50  0001 C CNN
	1    8750 2750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9150 3850 8900 3850
Wire Wire Line
	8350 3650 8350 3850
Wire Wire Line
	8350 3850 8600 3850
Wire Wire Line
	8800 750  9150 750 
NoConn ~ 9150 950 
NoConn ~ 9150 1050
NoConn ~ 9150 2700
$Comp
L Mounting_Hole MK1
U 1 1 62893518
P 4750 6100
F 0 "MK1" H 4750 6300 50  0000 C CNN
F 1 "Mounting_Hole" H 4750 6225 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3" H 4750 6100 50  0001 C CNN
F 3 "" H 4750 6100 50  0001 C CNN
	1    4750 6100
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole MK2
U 1 1 6289367E
P 5100 6100
F 0 "MK2" H 5100 6300 50  0000 C CNN
F 1 "Mounting_Hole" H 5100 6225 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3" H 5100 6100 50  0001 C CNN
F 3 "" H 5100 6100 50  0001 C CNN
	1    5100 6100
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole MK3
U 1 1 62893733
P 5400 6100
F 0 "MK3" H 5400 6300 50  0000 C CNN
F 1 "Mounting_Hole" H 5400 6225 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3" H 5400 6100 50  0001 C CNN
F 3 "" H 5400 6100 50  0001 C CNN
	1    5400 6100
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole_PAD MK4
U 1 1 628937EA
P 5850 6100
F 0 "MK4" H 5850 6350 50  0000 C CNN
F 1 "Mounting_Hole_PAD" H 5850 6275 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_Pad" H 5850 6100 50  0001 C CNN
F 3 "" H 5850 6100 50  0001 C CNN
	1    5850 6100
	1    0    0    -1  
$EndComp
$Comp
L R R27
U 1 1 62895B4A
P 8450 1550
F 0 "R27" V 8530 1550 50  0000 C CNN
F 1 "2.4K" V 8450 1550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8380 1550 50  0001 C CNN
F 3 "" H 8450 1550 50  0001 C CNN
F 4 "71-CRCW06032K40JN" V 8450 1550 60  0001 C CNN "Mouser Part Number"
	1    8450 1550
	0    1    1    0   
$EndComp
$Comp
L R R28
U 1 1 62895C7F
P 8450 1650
F 0 "R28" V 8530 1650 50  0000 C CNN
F 1 "2.4K" V 8450 1650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8380 1650 50  0001 C CNN
F 3 "" H 8450 1650 50  0001 C CNN
F 4 "71-CRCW06032K40JN" V 8450 1650 60  0001 C CNN "Mouser Part Number"
	1    8450 1650
	0    1    1    0   
$EndComp
Wire Wire Line
	8600 1550 9150 1550
Wire Wire Line
	8600 1650 9150 1650
Wire Wire Line
	8300 1450 8300 1650
$Comp
L +3.3V #PWR08
U 1 1 628962E1
P 8300 1450
F 0 "#PWR08" H 8300 1300 50  0001 C CNN
F 1 "+3.3V" H 8300 1590 50  0000 C CNN
F 2 "" H 8300 1450 50  0001 C CNN
F 3 "" H 8300 1450 50  0001 C CNN
	1    8300 1450
	1    0    0    -1  
$EndComp
Connection ~ 8300 1550
Wire Wire Line
	8200 2750 8100 2750
Wire Wire Line
	8750 2750 8700 2750
Wire Wire Line
	8200 2550 8200 2350
Wire Wire Line
	8200 2350 9000 2350
Wire Wire Line
	9000 2350 9000 2500
Wire Wire Line
	9000 2500 9150 2500
Wire Wire Line
	8700 2550 9150 2550
Wire Wire Line
	9150 2550 9150 2600
Wire Wire Line
	8200 2650 8000 2650
Wire Wire Line
	8700 2650 9050 2650
Wire Wire Line
	9050 2650 9050 2800
Wire Wire Line
	9050 2800 9150 2800
Wire Wire Line
	8200 2850 7850 2850
Wire Wire Line
	7850 2850 7850 3300
Wire Wire Line
	7850 3300 9000 3300
Wire Wire Line
	9000 3300 9000 2900
Wire Wire Line
	9000 2900 9150 2900
Wire Wire Line
	8700 2850 8950 2850
Wire Wire Line
	8950 2850 8950 3000
Wire Wire Line
	8950 3000 9150 3000
Wire Wire Line
	8200 2950 8200 3150
Wire Wire Line
	8200 3150 9150 3150
Wire Wire Line
	9150 3150 9150 3100
Wire Wire Line
	8700 2950 8700 3200
Wire Wire Line
	8700 3200 9150 3200
NoConn ~ 9150 850 
Wire Wire Line
	8000 2650 8000 1350
Wire Wire Line
	8000 1350 9150 1350
$EndSCHEMATC
