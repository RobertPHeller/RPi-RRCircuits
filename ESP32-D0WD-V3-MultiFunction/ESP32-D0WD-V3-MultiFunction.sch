EESchema Schematic File Version 2
LIBS:ESP32-D0WD-V3-MultiFunction-rescue
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
LIBS:ESP32-D0WD-V3
LIBS:W25Q32JVSSIQ
LIBS:umh3n
LIBS:lm2574n-5
LIBS:tc442x
LIBS:4814p-2
LIBS:pca9685
LIBS:tbd62x83a
LIBS:sn65hvd233-ht
LIBS:mechanical
LIBS:graphic_symbols
LIBS:ESP32-D0WD-V3-MultiFunction-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 8
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 5150 1350 3850 3150
U 62ACDEF5
F0 "ESP32 MCU" 59
F1 "ESP32-MCU.sch" 59
F2 "CAN_TX" O L 5150 1550 60 
F3 "CAN_RX" I L 5150 1700 60 
F4 "SCL" I L 5150 2750 60 
F5 "SDA" B L 5150 3000 60 
F6 "GPIO19(LEDS3)" O R 9000 4300 60 
F7 "GPIO23(LEDS4)" O R 9000 4400 60 
F8 "GPIO18(LEDS2)" O R 9000 4200 60 
F9 "GPIO17(LEDS1)" O R 9000 4100 60 
F10 "GPIO16(Buttons4)" I R 9000 3950 60 
F11 "GPIO0(Buttons1)" I R 9000 3650 60 
F12 "GPIO27(Points4)" I R 9000 3150 60 
F13 "GPIO26(Motor2)" O R 9000 2500 60 
F14 "GPIO25(Motor1)" O R 9000 2400 60 
F15 "GPIO34(OD1)" I L 5150 3850 60 
F16 "GPIO35(OD2)" I L 5150 3950 60 
F17 "GPIO36(OD3)" I L 5150 4050 60 
F18 "GPIO39(OD4)" I L 5150 4150 60 
F19 "GPIO32(Motor3)" O R 9000 2600 60 
F20 "GPIO33(Motor4)" O R 9000 2700 60 
F21 "GPIO12(Points1)" I R 9000 2850 60 
F22 "GPIO13(Points2)" I R 9000 2950 60 
F23 "GPIO14(Points3)" I R 9000 3050 60 
F24 "GPIO38(Buttons2)" I R 9000 3750 60 
F25 "GPIO37(Buttons3)" I R 9000 3850 60 
$EndSheet
$Sheet
S 1300 5500 1250 950 
U 62AF1057
F0 "Power Supply" 60
F1 "PowerSupply.sch" 60
$EndSheet
$Sheet
S 9450 3450 1350 1000
U 62AF1547
F0 "Buttons and LEDs" 60
F1 "Buttons4LEDs4.sch" 60
F2 "LED1" I L 9450 4100 60 
F3 "LED2" I L 9450 4200 60 
F4 "LED3" I L 9450 4300 60 
F5 "LED4" I L 9450 4400 60 
F6 "Button1" O L 9450 3650 60 
F7 "Button2" O L 9450 3750 60 
F8 "Button3" O L 9450 3850 60 
F9 "Button4" O L 9450 3950 60 
$EndSheet
$Sheet
S 9500 2350 1300 950 
U 62AF637B
F0 "Stall Motors" 60
F1 "StallMotors.sch" 60
F2 "Points1" O L 9500 2850 60 
F3 "Points2" O L 9500 2950 60 
F4 "Points3" O L 9500 3050 60 
F5 "Points4" O L 9500 3150 60 
F6 "Motor1" I L 9500 2400 60 
F7 "Motor2" I L 9500 2500 60 
F8 "Motor3" I L 9500 2600 60 
F9 "Motor4" I L 9500 2700 60 
$EndSheet
$Sheet
S 2700 3700 1650 850 
U 62AFE480
F0 "Occupancy Detectors" 60
F1 "OccDetect4.sch" 60
F2 "OD1" O R 4350 3850 60 
F3 "OD2" O R 4350 3950 60 
F4 "OD3" O R 4350 4050 60 
F5 "OD4" O R 4350 4150 60 
$EndSheet
$Sheet
S 2700 2450 1650 1000
U 62AFFF5E
F0 "PWM LED Signal Drivers" 60
F1 "PCA9685Driver.sch" 60
F2 "SCL" I R 4350 2750 60 
F3 "SDA" B R 4350 3000 60 
$EndSheet
Wire Wire Line
	9000 4400 9450 4400
Wire Wire Line
	9000 4300 9450 4300
Wire Wire Line
	9000 4200 9450 4200
Wire Wire Line
	9000 4100 9450 4100
Wire Wire Line
	9000 3950 9450 3950
Wire Wire Line
	9000 3850 9450 3850
Wire Wire Line
	9000 3750 9450 3750
Wire Wire Line
	9000 3650 9450 3650
Wire Wire Line
	9000 2400 9500 2400
Wire Wire Line
	9000 2500 9500 2500
Wire Wire Line
	9000 2600 9500 2600
Wire Wire Line
	9000 2700 9500 2700
Wire Wire Line
	9000 2850 9500 2850
Wire Wire Line
	9000 2950 9500 2950
Wire Wire Line
	9000 3050 9500 3050
Wire Wire Line
	9000 3150 9500 3150
Wire Wire Line
	4350 3850 5150 3850
Wire Wire Line
	5150 3950 4350 3950
Wire Wire Line
	5150 4050 4350 4050
Wire Wire Line
	5150 4150 4350 4150
Wire Wire Line
	5150 2750 4350 2750
Wire Wire Line
	5150 3000 4350 3000
$Sheet
S 2850 1100 1450 1050
U 62B03516
F0 "CAN Tranceiver" 60
F1 "CAN_Transceiver.sch" 60
F2 "CAN_TX" I R 4300 1550 60 
F3 "CAN_RX" O R 4300 1700 60 
$EndSheet
Wire Wire Line
	5150 1550 4300 1550
Wire Wire Line
	5150 1700 4300 1700
$Comp
L Mounting_Hole MK101
U 1 1 62AFDBC7
P 3300 5550
F 0 "MK101" H 3300 5750 50  0000 C CNN
F 1 "Mounting_Hole" H 3300 5675 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.7mm" H 3300 5550 50  0001 C CNN
F 3 "" H 3300 5550 50  0001 C CNN
	1    3300 5550
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole MK102
U 1 1 62AFDD12
P 3750 5600
F 0 "MK102" H 3750 5800 50  0000 C CNN
F 1 "Mounting_Hole" H 3750 5725 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.7mm" H 3750 5600 50  0001 C CNN
F 3 "" H 3750 5600 50  0001 C CNN
	1    3750 5600
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole MK103
U 1 1 62AFDE4B
P 4200 5550
F 0 "MK103" H 4200 5750 50  0000 C CNN
F 1 "Mounting_Hole" H 4200 5675 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.7mm" H 4200 5550 50  0001 C CNN
F 3 "" H 4200 5550 50  0001 C CNN
	1    4200 5550
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole_PAD MK104
U 1 1 62AFDF86
P 4650 5550
F 0 "MK104" H 4650 5800 50  0000 C CNN
F 1 "Mounting_Hole_PAD" H 4650 5725 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.7mm_Pad" H 4650 5550 50  0001 C CNN
F 3 "" H 4650 5550 50  0001 C CNN
	1    4650 5550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 62AFE0C3
P 4650 5850
F 0 "#PWR01" H 4650 5600 50  0001 C CNN
F 1 "GND" H 4650 5700 50  0000 C CNN
F 2 "" H 4650 5850 50  0001 C CNN
F 3 "" H 4650 5850 50  0001 C CNN
	1    4650 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 5650 4650 5850
$Comp
L Mounting_Hole FD101
U 1 1 62AFE875
P 5000 5650
F 0 "FD101" H 5000 5850 50  0000 C CNN
F 1 "FidMark" H 5000 5775 50  0000 C CNN
F 2 "Fiducials:Fiducial_1mm_Dia_2.54mm_Outer_CopperTop" H 5000 5650 50  0001 C CNN
F 3 "" H 5000 5650 50  0001 C CNN
	1    5000 5650
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole FD102
U 1 1 62AFEAF8
P 5400 5650
F 0 "FD102" H 5400 5850 50  0000 C CNN
F 1 "FidMark" H 5400 5775 50  0000 C CNN
F 2 "Fiducials:Fiducial_1mm_Dia_2.54mm_Outer_CopperTop" H 5400 5650 50  0001 C CNN
F 3 "" H 5400 5650 50  0001 C CNN
	1    5400 5650
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole FD103
U 1 1 62AFED78
P 5900 5700
F 0 "FD103" H 5900 5900 50  0000 C CNN
F 1 "FidMark" H 5900 5825 50  0000 C CNN
F 2 "Fiducials:Fiducial_1mm_Dia_2.54mm_Outer_CopperTop" H 5900 5700 50  0001 C CNN
F 3 "" H 5900 5700 50  0001 C CNN
	1    5900 5700
	1    0    0    -1  
$EndComp
$EndSCHEMATC
