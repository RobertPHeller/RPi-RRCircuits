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
LIBS:mechanical
LIBS:graphic_symbols
LIBS:sn65hvd233-ht
LIBS:lm2574n-5
LIBS:tc442x
LIBS:ESP32-TTGO-T7-OctalCTSense-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 6
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
L TTGO-T7-v13 U2
U 1 1 62224C29
P 2400 2300
F 0 "U2" H 2100 300 50  0000 C CNN
F 1 "TTGO-T7-v13" H 2600 -50 50  0000 C CNN
F 2 "ESP32_mini:ESP32_mini" H 2550 2400 50  0001 C CNN
F 3 "" H 2550 2400 50  0001 C CNN
	1    2400 2300
	1    0    0    -1  
$EndComp
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
Text Label 1900 2600 2    60   ~ 0
CAN_RX
Text Label 1900 2700 2    60   ~ 0
CAN_TX
Text Label 1900 2400 2    60   ~ 0
OD_1
Text Label 1900 2500 2    60   ~ 0
OD_2
Text Label 1900 3300 2    60   ~ 0
OD_3
Text Label 1900 3400 2    60   ~ 0
OD_4
Text Label 1900 3500 2    60   ~ 0
OD_5
Text Label 1900 3600 2    60   ~ 0
OD_6
Text Label 1900 3700 2    60   ~ 0
OD_7
Text Label 1900 3800 2    60   ~ 0
OD_8
$Comp
L +5V #PWR01
U 1 1 622258C1
P 3100 2300
F 0 "#PWR01" H 3100 2150 50  0001 C CNN
F 1 "+5V" H 3100 2440 50  0000 C CNN
F 2 "" H 3100 2300 50  0001 C CNN
F 3 "" H 3100 2300 50  0001 C CNN
	1    3100 2300
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR02
U 1 1 622258E3
P 3300 2300
F 0 "#PWR02" H 3300 2150 50  0001 C CNN
F 1 "+3V3" H 3300 2440 50  0000 C CNN
F 2 "" H 3300 2300 50  0001 C CNN
F 3 "" H 3300 2300 50  0001 C CNN
	1    3300 2300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 62225905
P 3200 2900
F 0 "#PWR03" H 3200 2650 50  0001 C CNN
F 1 "GND" H 3200 2750 50  0000 C CNN
F 2 "" H 3200 2900 50  0001 C CNN
F 3 "" H 3200 2900 50  0001 C CNN
	1    3200 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 2400 3100 2400
Wire Wire Line
	3100 2400 3100 2300
Wire Wire Line
	2950 2500 3300 2500
Wire Wire Line
	3300 2500 3300 2300
Wire Wire Line
	2950 2600 3200 2600
Wire Wire Line
	3200 2600 3200 2900
Wire Wire Line
	2950 2700 3200 2700
Connection ~ 3200 2700
Wire Wire Line
	2950 2800 3200 2800
Connection ~ 3200 2800
Text Label 4650 1150 2    60   ~ 0
CAN_TX
Text Label 4650 1300 2    60   ~ 0
CAN_RX
$Sheet
S 5700 2100 2250 1350
U 622276BC
F0 "Occupancy Detectors 1 to 4" 60
F1 "OccDetect1-4.sch" 60
F2 "OD1" O R 7950 2350 60 
F3 "OD2" O R 7950 2500 60 
F4 "OD3" O R 7950 2650 60 
F5 "OD4" O R 7950 2800 60 
$EndSheet
$Sheet
S 4750 4650 2250 1350
U 60CD421F
F0 "Occupancy Detectors 5 to 8" 60
F1 "OccDetect5-8.sch" 60
F2 "OD5" O R 7000 4900 60 
F3 "OD6" O R 7000 5050 60 
F4 "OD7" O R 7000 5200 60 
F5 "OD8" O R 7000 5350 60 
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
Text Label 7950 2350 0    60   ~ 0
OD_1
Text Label 7950 2500 0    60   ~ 0
OD_2
Text Label 7950 2650 0    60   ~ 0
OD_3
Text Label 7950 2800 0    60   ~ 0
OD_4
Text Label 7000 4900 0    60   ~ 0
OD_5
Text Label 7000 5050 0    60   ~ 0
OD_6
Text Label 7000 5200 0    60   ~ 0
OD_7
Text Label 7000 5350 0    60   ~ 0
OD_8
$Comp
L Mounting_Hole_PAD MK1
U 1 1 62238A8E
P 4150 6800
F 0 "MK1" H 4150 7050 50  0000 C CNN
F 1 "Mounting_Hole_PAD" H 4150 6975 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_Pad_Via" H 4150 6800 50  0001 C CNN
F 3 "" H 4150 6800 50  0001 C CNN
	1    4150 6800
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole MK2
U 1 1 62238B15
P 4550 6750
F 0 "MK2" H 4550 6950 50  0000 C CNN
F 1 "Mounting_Hole" H 4550 6875 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3" H 4550 6750 50  0001 C CNN
F 3 "" H 4550 6750 50  0001 C CNN
	1    4550 6750
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole MK3
U 1 1 62238B9C
P 4950 6750
F 0 "MK3" H 4950 6950 50  0000 C CNN
F 1 "Mounting_Hole" H 4950 6875 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3" H 4950 6750 50  0001 C CNN
F 3 "" H 4950 6750 50  0001 C CNN
	1    4950 6750
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole MK4
U 1 1 62238C25
P 5400 6750
F 0 "MK4" H 5400 6950 50  0000 C CNN
F 1 "Mounting_Hole" H 5400 6875 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3" H 5400 6750 50  0001 C CNN
F 3 "" H 5400 6750 50  0001 C CNN
	1    5400 6750
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole FD1
U 1 1 62238CB0
P 4150 7350
F 0 "FD1" H 4150 7550 50  0000 C CNN
F 1 "Mounting_Hole" H 4150 7475 50  0000 C CNN
F 2 "Fiducials:Fiducial_Modern_CopperTop" H 4150 7350 50  0001 C CNN
F 3 "" H 4150 7350 50  0001 C CNN
	1    4150 7350
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole FD2
U 1 1 62238E3C
P 4500 7400
F 0 "FD2" H 4500 7600 50  0000 C CNN
F 1 "Mounting_Hole" H 4500 7525 50  0000 C CNN
F 2 "Fiducials:Fiducial_Modern_CopperTop" H 4500 7400 50  0001 C CNN
F 3 "" H 4500 7400 50  0001 C CNN
	1    4500 7400
	1    0    0    -1  
$EndComp
$Comp
L Mounting_Hole FD3
U 1 1 62238F8C
P 4950 7400
F 0 "FD3" H 4950 7600 50  0000 C CNN
F 1 "Mounting_Hole" H 4950 7525 50  0000 C CNN
F 2 "Fiducials:Fiducial_Modern_CopperTop" H 4950 7400 50  0001 C CNN
F 3 "" H 4950 7400 50  0001 C CNN
	1    4950 7400
	1    0    0    -1  
$EndComp
Text Label 1900 3900 2    60   ~ 0
LED_1
Text Label 1900 4000 2    60   ~ 0
LED_2
Text Label 1900 4100 2    60   ~ 0
LED_3
Text Label 1900 4200 2    60   ~ 0
LED_4
Text Label 1900 4500 2    60   ~ 0
Button_1
Text Label 1900 4600 2    60   ~ 0
Button_2
Text Label 1900 4700 2    60   ~ 0
Button_3
Text Label 1900 4800 2    60   ~ 0
Button_4
Text Label 3600 6050 0    60   ~ 0
LED_1
Text Label 3600 6150 0    60   ~ 0
LED_2
Text Label 3600 6250 0    60   ~ 0
LED_3
Text Label 3600 6350 0    60   ~ 0
LED_4
Text Label 3600 6500 0    60   ~ 0
Button_1
Text Label 3600 6600 0    60   ~ 0
Button_2
Text Label 3600 6700 0    60   ~ 0
Button_3
Text Label 3600 6800 0    60   ~ 0
Button_4
$EndSCHEMATC
