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
LIBS:sn65hvd233-ht
LIBS:lm2574n-5
LIBS:MCP23xxx
LIBS:tbd62x83a
LIBS:tc442x
LIBS:pca9685
LIBS:4814p-2
LIBS:mechanical
LIBS:graphic_symbols
LIBS:references
LIBS:PocketBeagleMultifuntion-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 7
Title "Pocket Beagle Multifunction Board"
Date "3 mar 2019"
Rev ""
Comp "Deepwoods Software"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L POCKETBEAGLE-P1 P1
U 1 1 5C7C1FE0
P 2850 2550
F 0 "P1" H 2850 2550 60  0000 C CNN
F 1 "POCKETBEAGLE-P1" H 2850 3700 60  0000 C CNN
F 2 "" H 2850 2550 60  0001 C CNN
F 3 "" H 2850 2550 60  0000 C CNN
	1    2850 2550
	1    0    0    -1  
$EndComp
$Comp
L POCKETBEAGLE-P2 P2
U 1 1 5C7C1FEF
P 5150 2550
F 0 "P2" H 5150 2550 60  0000 C CNN
F 1 "POCKETBEAGLE-P2" H 5150 3700 60  0000 C CNN
F 2 "" H 5150 2550 60  0001 C CNN
F 3 "" H 5150 2550 60  0000 C CNN
	1    5150 2550
	1    0    0    -1  
$EndComp
Text Label 4700 2050 2    60   ~ 0
SCL
Text Label 4700 2150 2    60   ~ 0
SDA
Text Label 4700 2850 2    60   ~ 0
CAN1_RX
Text Label 4700 2950 2    60   ~ 0
CAN1_TX
$Comp
L Conn_01x06 P3
U 1 1 60D232F7
P 3900 2950
F 0 "P3" H 3900 3250 50  0000 C CNN
F 1 "Console" H 3900 2550 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06_Pitch2.54mm" H 3900 2950 50  0001 C CNN
F 3 "" H 3900 2950 50  0001 C CNN
	1    3900 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 2650 3550 2650
Wire Wire Line
	3550 2650 3550 2750
Wire Wire Line
	3550 2750 3700 2750
Wire Wire Line
	3450 3050 3700 3050
Wire Wire Line
	3450 3050 3450 3150
Wire Wire Line
	3450 3150 3300 3150
Wire Wire Line
	3300 3050 3400 3050
Wire Wire Line
	3400 3050 3400 3100
Wire Wire Line
	3400 3100 3550 3100
Wire Wire Line
	3550 3100 3550 3150
Wire Wire Line
	3550 3150 3700 3150
NoConn ~ 3700 2850
NoConn ~ 3700 2950
NoConn ~ 3700 3250
$Comp
L USB_A J3
U 1 1 60D2347F
P 1850 1950
F 0 "J3" H 1650 2400 50  0000 L CNN
F 1 "USB_A" H 1650 2300 50  0000 L CNN
F 2 "Connectors:USB_A" H 2000 1900 50  0001 C CNN
F 3 "" H 2000 1900 50  0001 C CNN
	1    1850 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 2050 2150 2050
Wire Wire Line
	2400 2150 2250 2150
Wire Wire Line
	2250 2150 2250 1950
Wire Wire Line
	2250 1950 2150 1950
Wire Wire Line
	2150 1750 2300 1750
Wire Wire Line
	2300 1750 2300 1950
Wire Wire Line
	2300 1950 2400 1950
Wire Wire Line
	2400 1850 2300 1850
Connection ~ 2300 1850
Wire Wire Line
	1750 2350 2400 2350
Connection ~ 1850 2350
$Comp
L GND #PWR01
U 1 1 60D23593
P 2100 2450
F 0 "#PWR01" H 2100 2200 50  0001 C CNN
F 1 "GND" H 2100 2300 50  0000 C CNN
F 2 "" H 2100 2450 50  0001 C CNN
F 3 "" H 2100 2450 50  0001 C CNN
	1    2100 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 2450 2100 2350
Connection ~ 2100 2350
Wire Wire Line
	2400 2250 2300 2250
Wire Wire Line
	2300 2250 2300 2350
Connection ~ 2300 2350
NoConn ~ 2400 1750
NoConn ~ 2400 2450
NoConn ~ 2400 2550
NoConn ~ 2400 2650
NoConn ~ 2400 2750
NoConn ~ 2400 2850
NoConn ~ 2400 2950
NoConn ~ 3300 2450
$Comp
L +5V #PWR02
U 1 1 60D23667
P 2300 1500
F 0 "#PWR02" H 2300 1350 50  0001 C CNN
F 1 "+5V" H 2300 1640 50  0000 C CNN
F 2 "" H 2300 1500 50  0001 C CNN
F 3 "" H 2300 1500 50  0001 C CNN
	1    2300 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 1500 2300 1650
Wire Wire Line
	2300 1650 2400 1650
$Sheet
S 5150 5900 1250 900 
U 60CCDA7F
F0 "CAN Transceiver" 60
F1 "CAN_Transeiver.sch" 60
F2 "CAN_TX" I L 5150 6100 60 
F3 "CAN_RX" I L 5150 6250 60 
$EndSheet
$Sheet
S 6600 2300 1600 900 
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
S 6800 3800 2250 1350
U 60CD421F
F0 "Occupancy Detectors" 60
F1 "OccDetect4.sch" 60
F2 "OD1" O R 9050 4050 60 
F3 "OD2" O R 9050 4200 60 
F4 "OD3" O R 9050 4350 60 
F5 "OD4" O R 9050 4500 60 
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
L Mounting_Hole MK1
U 1 1 60CD4484
P 10450 1000
F 0 "MK1" H 10450 1200 50  0000 C CNN
F 1 "Mounting_Hole" H 10450 1125 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3" H 10450 1000 50  0001 C CNN
F 3 "" H 10450 1000 50  0001 C CNN
	1    10450 1000
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole MK2
U 1 1 60CD44E3
P 10450 1550
F 0 "MK2" H 10450 1750 50  0000 C CNN
F 1 "Mounting_Hole" H 10450 1675 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3" H 10450 1550 50  0001 C CNN
F 3 "" H 10450 1550 50  0001 C CNN
	1    10450 1550
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole MK4
U 1 1 60CD4516
P 10500 2150
F 0 "MK4" H 10500 2350 50  0000 C CNN
F 1 "Mounting_Hole" H 10500 2275 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3" H 10500 2150 50  0001 C CNN
F 3 "" H 10500 2150 50  0001 C CNN
	1    10500 2150
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole_PAD MK3
U 1 1 60CD454B
P 10450 2750
F 0 "MK3" H 10450 3000 50  0000 C CNN
F 1 "Mounting_Hole_PAD" H 10450 2925 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_Pad" H 10450 2750 50  0001 C CNN
F 3 "" H 10450 2750 50  0001 C CNN
	1    10450 2750
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole FID1
U 1 1 60CD48B9
P 9450 1700
F 0 "FID1" H 9450 1900 50  0000 C CNN
F 1 "FidMark" H 9450 1825 50  0000 C CNN
F 2 "Fiducials:Fiducial_1mm_Dia_2.54mm_Outer_CopperBottom" H 9450 1700 50  0001 C CNN
F 3 "" H 9450 1700 50  0001 C CNN
	1    9450 1700
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole FID2
U 1 1 60CD49DA
P 9450 2050
F 0 "FID2" H 9450 2250 50  0000 C CNN
F 1 "FidMark" H 9450 2175 50  0000 C CNN
F 2 "Fiducials:Fiducial_1mm_Dia_2.54mm_Outer_CopperBottom" H 9450 2050 50  0001 C CNN
F 3 "" H 9450 2050 50  0001 C CNN
	1    9450 2050
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole FID3
U 1 1 60CD4A15
P 9450 2350
F 0 "FID3" H 9450 2550 50  0000 C CNN
F 1 "FidMark" H 9450 2475 50  0000 C CNN
F 2 "Fiducials:Fiducial_1mm_Dia_2.54mm_Outer_CopperBottom" H 9450 2350 50  0001 C CNN
F 3 "" H 9450 2350 50  0001 C CNN
	1    9450 2350
	1    0    0    -1  
$EndComp
Text Label 4550 4000 2    60   ~ 0
SCL
Text Label 4550 4200 2    60   ~ 0
SDA
Text Label 5150 6100 2    60   ~ 0
CAN1_TX
Text Label 5150 6250 2    60   ~ 0
CAN1_RX
Text Label 3300 1750 0    60   ~ 0
GPIO89
Text Label 3300 1650 0    60   ~ 0
GPIO87
Text Label 3300 2550 0    60   ~ 0
GPIO20
Text Label 3300 3250 0    60   ~ 0
GPIO26
Text Label 4700 3250 2    60   ~ 0
GPIO45
Text Label 4700 2550 2    60   ~ 0
GPIO27
Text Label 4700 2450 2    60   ~ 0
GPIO65
Text Label 4700 1750 2    60   ~ 0
GPIO23
Text Label 5600 1650 0    60   ~ 0
GPIO59
Text Label 5600 1750 0    60   ~ 0
GPIO58
Text Label 5600 1850 0    60   ~ 0
GPIO57
Text Label 5600 1950 0    60   ~ 0
GPIO60
Text Label 5600 2050 0    60   ~ 0
GPIO52
Text Label 5600 2450 0    60   ~ 0
GPIO47
Text Label 5600 2550 0    60   ~ 0
GPIO64
Text Label 5600 2650 0    60   ~ 0
GPIO46
Text Label 5600 2750 0    60   ~ 0
GPIO48
Text Label 2400 3050 2    60   ~ 0
GPIO117
Text Label 2400 3150 2    60   ~ 0
GPIO114
Text Label 2400 3250 2    60   ~ 0
GPIO111
Text Label 2400 3350 2    60   ~ 0
GPIO88
Text Label 3300 3350 0    60   ~ 0
GPIO110
Text Label 4700 1650 2    60   ~ 0
GPIO50
Text Label 4700 3050 2    60   ~ 0
GPIO7
Text Label 4700 3150 2    60   ~ 0
GPIO19
Text Label 4700 3350 2    60   ~ 0
GPIO86
Text Label 5600 2950 0    60   ~ 0
GPIO116
Text Label 5600 3050 0    60   ~ 0
GPIO113
Text Label 5600 3150 0    60   ~ 0
GPIO112
Text Label 5600 3250 0    60   ~ 0
GPIO115
Text Notes 6800 1500 0    60   ~ 0
OD:       7,  19,  20,  23,\nMotor:   26,  27,  45,  46,\nPoints:  47,  48,  50,  52,\nButtons: 57,  58,  59   60,\nLEDs:    64,  65,  86   87,\nSignals: I2C1\nGPIO Header: 110, 111, 112, 113,\n              114, 115, 116, 117
Text Label 9050 4050 0    60   ~ 0
GOIO7
Text Label 9050 4200 0    60   ~ 0
GPIO19
Text Label 9050 4350 0    60   ~ 0
GPIO20
Text Label 9050 4500 0    60   ~ 0
GPIO23
Text Label 3500 4500 0    60   ~ 0
GPIO26
Text Label 3500 4650 0    60   ~ 0
GPIO27
Text Label 3500 4800 0    60   ~ 0
GPIO45
Text Label 3500 4950 0    60   ~ 0
GPIO46
Text Label 3500 3900 0    60   ~ 0
GPIO47
Text Label 3500 4050 0    60   ~ 0
GPIO48
Text Label 3500 4200 0    60   ~ 0
GPIO50
Text Label 3500 4350 0    60   ~ 0
GPIO52
Text Label 3600 6500 0    60   ~ 0
GPIO57
Text Label 3600 6600 0    60   ~ 0
GPIO58
Text Label 3600 6700 0    60   ~ 0
GPIO59
Text Label 3600 6800 0    60   ~ 0
GPio60
Text Label 3600 6050 0    60   ~ 0
GPIO64
Text Label 3600 6150 0    60   ~ 0
GPIO65
Text Label 3600 6250 0    60   ~ 0
GPIO86
Text Label 3600 6350 0    60   ~ 0
GPIO87
$Comp
L Conn_02x05_Odd_Even J4
U 1 1 60D3EF93
P 7700 5800
F 0 "J4" H 7750 6100 50  0000 C CNN
F 1 "GPIO Header" H 7750 5500 50  0000 C CNN
F 2 "Connectors:IDC_Header_Straight_10pins" H 7700 5800 50  0001 C CNN
F 3 "" H 7700 5800 50  0001 C CNN
	1    7700 5800
	1    0    0    -1  
$EndComp
Text Label 7500 5600 2    60   ~ 0
GPIO110
Text Label 8000 5600 0    60   ~ 0
GPIO111
Text Label 7500 5700 2    60   ~ 0
GPIO112
Text Label 8000 5700 0    60   ~ 0
GPIO113
$Comp
L +3V3 #PWR03
U 1 1 60D3F534
P 7500 5800
F 0 "#PWR03" H 7500 5650 50  0001 C CNN
F 1 "+3V3" H 7500 5940 50  0000 C CNN
F 2 "" H 7500 5800 50  0001 C CNN
F 3 "" H 7500 5800 50  0001 C CNN
	1    7500 5800
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR04
U 1 1 60D3F8A8
P 8000 5800
F 0 "#PWR04" H 8000 5550 50  0001 C CNN
F 1 "GND" H 8000 5650 50  0000 C CNN
F 2 "" H 8000 5800 50  0001 C CNN
F 3 "" H 8000 5800 50  0001 C CNN
	1    8000 5800
	0    -1   -1   0   
$EndComp
Text Label 7500 5900 2    60   ~ 0
GPIO114
Text Label 8000 5900 0    60   ~ 0
GPIO115
Text Label 7500 6000 2    60   ~ 0
GPIO116
Text Label 8000 6000 0    60   ~ 0
GPIO117
$EndSCHEMATC
