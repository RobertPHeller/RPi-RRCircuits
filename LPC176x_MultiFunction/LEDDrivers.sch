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
LIBS:LPC1768FBD100K
LIBS:4814p-2
LIBS:tc442x
LIBS:pca9685
LIBS:tbd62x83a
LIBS:mechanical
LIBS:LPC176x_MultiFunction-cache
EELAYER 25 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 8 11
Title "ESP32 S3 Multifunction Board"
Date ""
Rev "1.0"
Comp "Deepwoods Software"
Comment1 "Generic Buttons and LEDs"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L TC4426 U802
U 2 1 632FBBDD
P 2625 2900
F 0 "U802" H 2625 2600 60  0000 C CNN
F 1 "TC4426" H 2625 3200 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 2625 2900 60  0001 C CNN
F 3 "" H 2625 2900 60  0000 C CNN
F 4 "579-TC1426COA713" H 2625 2900 60  0001 C CNN "Mouser Part Number"
	2    2625 2900
	1    0    0    -1  
$EndComp
$Comp
L TC4426 U802
U 1 1 632FBBDE
P 2625 3750
F 0 "U802" H 2625 3450 60  0000 C CNN
F 1 "TC4426" H 2625 4050 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 2625 3750 60  0001 C CNN
F 3 "" H 2625 3750 60  0000 C CNN
F 4 "579-TC1426COA713" H 2625 3750 60  0001 C CNN "Mouser Part Number"
	1    2625 3750
	1    0    0    -1  
$EndComp
$Comp
L R R802
U 1 1 632FBBDF
P 3825 1300
F 0 "R802" V 3905 1300 50  0000 C CNN
F 1 "1.5K Ohms" V 3825 1300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3755 1300 50  0001 C CNN
F 3 "" H 3825 1300 50  0001 C CNN
F 4 "71-CRCW06031K50JNEAC" V 3825 1300 60  0001 C CNN "Mouser Part Number"
	1    3825 1300
	0    1    1    0   
$EndComp
$Comp
L R R801
U 1 1 632FBBE0
P 3825 1150
F 0 "R801" V 3905 1150 50  0000 C CNN
F 1 "R" V 3825 1150 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3755 1150 50  0001 C CNN
F 3 "" H 3825 1150 50  0001 C CNN
	1    3825 1150
	0    1    1    0   
$EndComp
$Comp
L R R804
U 1 1 632FBBE1
P 3825 2100
F 0 "R804" V 3905 2100 50  0000 C CNN
F 1 "1.5K Ohms" V 3825 2100 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3755 2100 50  0001 C CNN
F 3 "" H 3825 2100 50  0001 C CNN
F 4 "71-CRCW06031K50JNEAC" V 3825 2100 60  0001 C CNN "Mouser Part Number"
	1    3825 2100
	0    1    1    0   
$EndComp
$Comp
L R R803
U 1 1 632FBBE2
P 3825 1950
F 0 "R803" V 3905 1950 50  0000 C CNN
F 1 "R" V 3825 1950 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3755 1950 50  0001 C CNN
F 3 "" H 3825 1950 50  0001 C CNN
	1    3825 1950
	0    1    1    0   
$EndComp
$Comp
L R R806
U 1 1 632FBBE3
P 3825 2900
F 0 "R806" V 3905 2900 50  0000 C CNN
F 1 "1.5K Ohms" V 3825 2900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3755 2900 50  0001 C CNN
F 3 "" H 3825 2900 50  0001 C CNN
F 4 "71-CRCW06031K50JNEAC" V 3825 2900 60  0001 C CNN "Mouser Part Number"
	1    3825 2900
	0    1    1    0   
$EndComp
$Comp
L R R805
U 1 1 632FBBE4
P 3825 2750
F 0 "R805" V 3905 2750 50  0000 C CNN
F 1 "R" V 3825 2750 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3755 2750 50  0001 C CNN
F 3 "" H 3825 2750 50  0001 C CNN
	1    3825 2750
	0    1    1    0   
$EndComp
$Comp
L R R808
U 1 1 632FBBE5
P 3825 3750
F 0 "R808" V 3905 3750 50  0000 C CNN
F 1 "1.5K Ohms" V 3825 3750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3755 3750 50  0001 C CNN
F 3 "" H 3825 3750 50  0001 C CNN
F 4 "71-CRCW06031K50JNEAC" V 3825 3750 60  0001 C CNN "Mouser Part Number"
	1    3825 3750
	0    1    1    0   
$EndComp
$Comp
L R R807
U 1 1 632FBBE6
P 3825 3600
F 0 "R807" V 3905 3600 50  0000 C CNN
F 1 "R" V 3825 3600 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3755 3600 50  0001 C CNN
F 3 "" H 3825 3600 50  0001 C CNN
	1    3825 3600
	0    1    1    0   
$EndComp
$Comp
L Screw_Terminal_01x10 Term801
U 1 1 632FBBE7
P 5225 3000
F 0 "Term801" H 5275 3550 50  0000 C CNN
F 1 "GND 8 7 6 5 4 3 2 1 +" V 5425 2875 50  0000 C CNN
F 2 "Connectors_Terminal_Blocks:TerminalBlock_Pheonix_MPT-2.54mm_10pol" H 5225 3000 50  0001 C CNN
F 3 "" H 5225 3000 50  0001 C CNN
F 4 "651-1725737" H 5225 3000 60  0001 C CNN "Mouser Part Number"
	1    5225 3000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR079
U 1 1 632FBBE8
P 5025 3625
F 0 "#PWR079" H 5025 3375 50  0001 C CNN
F 1 "GND" H 5025 3475 50  0000 C CNN
F 2 "" H 5025 3625 50  0001 C CNN
F 3 "" H 5025 3625 50  0001 C CNN
	1    5025 3625
	1    0    0    -1  
$EndComp
Text HLabel 2325 1300 0    60   Input ~ 0
LED1
Text HLabel 2325 2100 0    60   Input ~ 0
LED2
Text HLabel 2375 2900 0    60   Input ~ 0
LED3
Text HLabel 2375 3750 0    60   Input ~ 0
LED4
$Comp
L TC4426 U801
U 2 1 632FBC04
P 2575 1300
F 0 "U801" H 2575 1000 60  0000 C CNN
F 1 "TC4426" H 2575 1600 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 2575 1300 60  0001 C CNN
F 3 "" H 2575 1300 60  0000 C CNN
F 4 "579-TC1426COA713" H 2575 1300 60  0001 C CNN "Mouser Part Number"
	2    2575 1300
	1    0    0    -1  
$EndComp
$Comp
L TC4426 U801
U 1 1 632FBC05
P 2575 2100
F 0 "U801" H 2575 1800 60  0000 C CNN
F 1 "TC4426" H 2575 2400 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 2575 2100 60  0001 C CNN
F 3 "" H 2575 2100 60  0000 C CNN
F 4 "579-TC1426COA713" H 2575 2100 60  0001 C CNN "Mouser Part Number"
	1    2575 2100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR080
U 1 1 632FBC06
P 2525 2450
F 0 "#PWR080" H 2525 2200 50  0001 C CNN
F 1 "GND" H 2525 2300 50  0000 C CNN
F 2 "" H 2525 2450 50  0001 C CNN
F 3 "" H 2525 2450 50  0001 C CNN
	1    2525 2450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR081
U 1 1 632FBC07
P 2575 4200
F 0 "#PWR081" H 2575 3950 50  0001 C CNN
F 1 "GND" H 2575 4050 50  0000 C CNN
F 2 "" H 2575 4200 50  0001 C CNN
F 3 "" H 2575 4200 50  0001 C CNN
	1    2575 4200
	1    0    0    -1  
$EndComp
Text Label 4975 2600 1    60   ~ 0
LEDPower
$Comp
L TC4426 U804
U 2 1 632FD544
P 2750 6325
F 0 "U804" H 2750 6025 60  0000 C CNN
F 1 "TC4426" H 2750 6625 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 2750 6325 60  0001 C CNN
F 3 "" H 2750 6325 60  0000 C CNN
F 4 "579-TC1426COA713" H 2750 6325 60  0001 C CNN "Mouser Part Number"
	2    2750 6325
	1    0    0    -1  
$EndComp
$Comp
L TC4426 U804
U 1 1 632FD54B
P 2750 7175
F 0 "U804" H 2750 6875 60  0000 C CNN
F 1 "TC4426" H 2750 7475 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 2750 7175 60  0001 C CNN
F 3 "" H 2750 7175 60  0000 C CNN
F 4 "579-TC1426COA713" H 2750 7175 60  0001 C CNN "Mouser Part Number"
	1    2750 7175
	1    0    0    -1  
$EndComp
$Comp
L R R810
U 1 1 632FD552
P 3950 4725
F 0 "R810" V 4030 4725 50  0000 C CNN
F 1 "1.5K Ohms" V 3950 4725 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3880 4725 50  0001 C CNN
F 3 "" H 3950 4725 50  0001 C CNN
F 4 "71-CRCW06031K50JNEAC" V 3950 4725 60  0001 C CNN "Mouser Part Number"
	1    3950 4725
	0    1    1    0   
$EndComp
$Comp
L R R809
U 1 1 632FD558
P 3950 4575
F 0 "R809" V 4030 4575 50  0000 C CNN
F 1 "R" V 3950 4575 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3880 4575 50  0001 C CNN
F 3 "" H 3950 4575 50  0001 C CNN
	1    3950 4575
	0    1    1    0   
$EndComp
$Comp
L R R812
U 1 1 632FD55F
P 3950 5525
F 0 "R812" V 4030 5525 50  0000 C CNN
F 1 "1.5K Ohms" V 3950 5525 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3880 5525 50  0001 C CNN
F 3 "" H 3950 5525 50  0001 C CNN
F 4 "71-CRCW06031K50JNEAC" V 3950 5525 60  0001 C CNN "Mouser Part Number"
	1    3950 5525
	0    1    1    0   
$EndComp
$Comp
L R R811
U 1 1 632FD565
P 3950 5375
F 0 "R811" V 4030 5375 50  0000 C CNN
F 1 "R" V 3950 5375 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3880 5375 50  0001 C CNN
F 3 "" H 3950 5375 50  0001 C CNN
	1    3950 5375
	0    1    1    0   
$EndComp
$Comp
L R R814
U 1 1 632FD56C
P 3950 6325
F 0 "R814" V 4030 6325 50  0000 C CNN
F 1 "1.5K Ohms" V 3950 6325 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3880 6325 50  0001 C CNN
F 3 "" H 3950 6325 50  0001 C CNN
F 4 "71-CRCW06031K50JNEAC" V 3950 6325 60  0001 C CNN "Mouser Part Number"
	1    3950 6325
	0    1    1    0   
$EndComp
$Comp
L R R813
U 1 1 632FD572
P 3950 6175
F 0 "R813" V 4030 6175 50  0000 C CNN
F 1 "R" V 3950 6175 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3880 6175 50  0001 C CNN
F 3 "" H 3950 6175 50  0001 C CNN
	1    3950 6175
	0    1    1    0   
$EndComp
$Comp
L R R816
U 1 1 632FD579
P 3950 7175
F 0 "R816" V 4030 7175 50  0000 C CNN
F 1 "1.5K Ohms" V 3950 7175 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3880 7175 50  0001 C CNN
F 3 "" H 3950 7175 50  0001 C CNN
F 4 "71-CRCW06031K50JNEAC" V 3950 7175 60  0001 C CNN "Mouser Part Number"
	1    3950 7175
	0    1    1    0   
$EndComp
$Comp
L R R815
U 1 1 632FD57F
P 3950 7025
F 0 "R815" V 4030 7025 50  0000 C CNN
F 1 "R" V 3950 7025 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3880 7025 50  0001 C CNN
F 3 "" H 3950 7025 50  0001 C CNN
	1    3950 7025
	0    1    1    0   
$EndComp
Text HLabel 2450 4725 0    60   Input ~ 0
LED5
Text HLabel 2450 5525 0    60   Input ~ 0
LED6
Text HLabel 2500 6325 0    60   Input ~ 0
LED7
Text HLabel 2500 7175 0    60   Input ~ 0
LED8
$Comp
L TC4426 U803
U 2 1 632FD58A
P 2700 4725
F 0 "U803" H 2700 4425 60  0000 C CNN
F 1 "TC4426" H 2700 5025 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 2700 4725 60  0001 C CNN
F 3 "" H 2700 4725 60  0000 C CNN
F 4 "579-TC1426COA713" H 2700 4725 60  0001 C CNN "Mouser Part Number"
	2    2700 4725
	1    0    0    -1  
$EndComp
$Comp
L TC4426 U803
U 1 1 632FD591
P 2700 5525
F 0 "U803" H 2700 5225 60  0000 C CNN
F 1 "TC4426" H 2700 5825 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 2700 5525 60  0001 C CNN
F 3 "" H 2700 5525 60  0000 C CNN
F 4 "579-TC1426COA713" H 2700 5525 60  0001 C CNN "Mouser Part Number"
	1    2700 5525
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR082
U 1 1 632FD597
P 2650 5875
F 0 "#PWR082" H 2650 5625 50  0001 C CNN
F 1 "GND" H 2650 5725 50  0000 C CNN
F 2 "" H 2650 5875 50  0001 C CNN
F 3 "" H 2650 5875 50  0001 C CNN
	1    2650 5875
	1    0    0    -1  
$EndComp
Wire Wire Line
	3675 1150 3675 1300
Wire Wire Line
	3975 1150 3975 1300
Wire Wire Line
	3675 1950 3675 2100
Wire Wire Line
	3975 1950 3975 2100
Wire Wire Line
	3675 2750 3675 2900
Wire Wire Line
	3975 2750 3975 2900
Wire Wire Line
	3675 3600 3675 3750
Wire Wire Line
	3975 3600 3975 3750
Wire Wire Line
	2525 1150 2525 850 
Wire Wire Line
	2525 850  4625 850 
Wire Wire Line
	4625 850  4625 2600
Wire Wire Line
	4625 2600 5025 2600
Wire Wire Line
	2575 2750 2575 2650
Wire Wire Line
	2575 2650 3500 2650
Wire Wire Line
	3425 2650 3425 850 
Connection ~ 3425 850 
Wire Wire Line
	3675 1300 2975 1300
Wire Wire Line
	3975 1300 4525 1300
Wire Wire Line
	4525 1300 4525 2700
Wire Wire Line
	4525 2700 5025 2700
Wire Wire Line
	3675 2100 2975 2100
Wire Wire Line
	3975 2100 4375 2100
Wire Wire Line
	4375 2100 4375 2800
Wire Wire Line
	4375 2800 5025 2800
Wire Wire Line
	3675 2900 3025 2900
Wire Wire Line
	3975 2900 5025 2900
Wire Wire Line
	3675 3750 3025 3750
Wire Wire Line
	5025 3000 4375 3000
Wire Wire Line
	4375 3000 4375 3750
Wire Wire Line
	4375 3750 3975 3750
Wire Wire Line
	2525 2450 2525 2300
Wire Wire Line
	2575 4200 2575 3950
Wire Wire Line
	5025 3500 5025 3625
Wire Wire Line
	3800 4575 3800 4725
Wire Wire Line
	4100 4575 4100 4725
Wire Wire Line
	3800 5375 3800 5525
Wire Wire Line
	4100 5375 4100 5525
Wire Wire Line
	3800 6175 3800 6325
Wire Wire Line
	4100 6175 4100 6325
Wire Wire Line
	3800 7025 3800 7175
Wire Wire Line
	4100 7025 4100 7175
Wire Wire Line
	2700 6175 2700 6075
Wire Wire Line
	2700 6075 3550 6075
Wire Wire Line
	3800 4725 3100 4725
Wire Wire Line
	4100 4725 4425 4725
Wire Wire Line
	3800 5525 3100 5525
Wire Wire Line
	4100 5525 4500 5525
Wire Wire Line
	3800 6325 3150 6325
Wire Wire Line
	3800 7175 3150 7175
Wire Wire Line
	4100 7175 4600 7175
Wire Wire Line
	2650 5875 2650 5725
Wire Wire Line
	2700 7625 2700 7375
Wire Wire Line
	2625 4575 2625 4500
Wire Wire Line
	2625 4500 3550 4500
Wire Wire Line
	3500 2650 3500 4500
Connection ~ 3425 2650
Wire Wire Line
	3550 4500 3550 6075
Connection ~ 3500 4500
$Comp
L GND #PWR083
U 1 1 632FF3CD
P 2700 7625
F 0 "#PWR083" H 2700 7375 50  0001 C CNN
F 1 "GND" H 2700 7475 50  0000 C CNN
F 2 "" H 2700 7625 50  0001 C CNN
F 3 "" H 2700 7625 50  0001 C CNN
	1    2700 7625
	1    0    0    -1  
$EndComp
Wire Wire Line
	5025 3100 4425 3100
Wire Wire Line
	4425 3100 4425 4725
Wire Wire Line
	5025 3200 4500 3200
Wire Wire Line
	4500 3200 4500 5525
Wire Wire Line
	5025 3300 4550 3300
Wire Wire Line
	4550 3300 4550 6325
Wire Wire Line
	4550 6325 4100 6325
Wire Wire Line
	5025 3400 4600 3400
Wire Wire Line
	4600 3400 4600 7175
$EndSCHEMATC
