EESchema Schematic File Version 2
LIBS:MetroMiniABS2-bidir-cache
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
LIBS:metromini
LIBS:mcp25xxx
LIBS:mct6h
LIBS:MetroMiniABS2-bidir-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "10 jun 2018"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCP2551 U3
U 1 1 5B021890
P 6000 5150
F 0 "U3" H 6000 5150 60  0000 C CNN
F 1 "MCP2551" H 6000 5250 60  0000 C CNN
F 2 "DIL-8_300" H 6000 5050 60  0001 C CNN
F 3 "~" H 6000 5150 60  0000 C CNN
F 4 "579-MCP2551-I/P" H 6000 5150 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 "1.31" H 6000 5150 60  0001 C CNN "Price"
	1    6000 5150
	1    0    0    -1  
$EndComp
Text GLabel 5400 2500 0    39   Input ~ 0
CS
Text GLabel 5400 2700 0    39   Input ~ 0
MISO
Text GLabel 5400 2600 0    39   Input ~ 0
MOSI
Text GLabel 5400 2800 0    39   Input ~ 0
SCLK
Text GLabel 5400 2400 0    39   Input ~ 0
INT
Text GLabel 2350 4750 0    39   Input ~ 0
CS
Text GLabel 2350 4900 0    39   Input ~ 0
SCLK
Text GLabel 2350 5000 0    39   Input ~ 0
MOSI
Text GLabel 2350 5100 0    39   Input ~ 0
MISO
$Comp
L +5V #PWR01
U 1 1 5B0219DD
P 2250 3800
F 0 "#PWR01" H 2250 3890 20  0001 C CNN
F 1 "+5V" H 2250 3890 30  0000 C CNN
F 2 "" H 2250 3800 60  0000 C CNN
F 3 "" H 2250 3800 60  0000 C CNN
	1    2250 3800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 5B021A12
P 3200 5950
F 0 "#PWR02" H 3200 5950 30  0001 C CNN
F 1 "GND" H 3200 5880 30  0001 C CNN
F 2 "" H 3200 5950 60  0000 C CNN
F 3 "" H 3200 5950 60  0000 C CNN
	1    3200 5950
	1    0    0    -1  
$EndComp
$Comp
L CRYSTAL X1
U 1 1 5B021A38
P 2000 5450
F 0 "X1" H 2000 5600 60  0000 C CNN
F 1 "20 Mhz" H 2000 5300 60  0000 C CNN
F 2 "~" H 2000 5450 60  0000 C CNN
F 3 "~" H 2000 5450 60  0000 C CNN
F 4 "449-LFXTAL027945BULK " H 2000 5450 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 ".21" H 2000 5450 60  0001 C CNN "Price"
	1    2000 5450
	0    -1   -1   0   
$EndComp
$Comp
L C C1
U 1 1 5B021A64
P 1700 5150
F 0 "C1" H 1700 5250 40  0000 L CNN
F 1 "22 pf" H 1706 5065 40  0000 L CNN
F 2 "~" H 1738 5000 30  0000 C CNN
F 3 "~" H 1700 5150 60  0000 C CNN
F 4 "581-SR151A220KAR" H 1700 5150 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 ".10" H 1700 5150 60  0001 C CNN "Price"
	1    1700 5150
	0    -1   -1   0   
$EndComp
$Comp
L C C2
U 1 1 5B021A73
P 1700 5750
F 0 "C2" H 1700 5850 40  0000 L CNN
F 1 "22 pf" H 1706 5665 40  0000 L CNN
F 2 "~" H 1738 5600 30  0000 C CNN
F 3 "~" H 1700 5750 60  0000 C CNN
F 4 "581-SR151A220KAR" H 1700 5750 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 ".10" H 1700 5750 60  0001 C CNN "Price"
	1    1700 5750
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR03
U 1 1 5B021AAF
P 1500 5850
F 0 "#PWR03" H 1500 5850 30  0001 C CNN
F 1 "GND" H 1500 5780 30  0001 C CNN
F 2 "" H 1500 5850 60  0000 C CNN
F 3 "" H 1500 5850 60  0000 C CNN
	1    1500 5850
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5B021B27
P 1450 4400
F 0 "C3" H 1450 4500 40  0000 L CNN
F 1 ".1 uf" H 1456 4315 40  0000 L CNN
F 2 "~" H 1488 4250 30  0000 C CNN
F 3 "~" H 1450 4400 60  0000 C CNN
F 4 "21RZ310-RC" H 1450 4400 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 ".08" H 1450 4400 60  0001 C CNN "Price"
	1    1450 4400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 5B021B52
P 1450 4700
F 0 "#PWR04" H 1450 4700 30  0001 C CNN
F 1 "GND" H 1450 4630 30  0001 C CNN
F 2 "" H 1450 4700 60  0000 C CNN
F 3 "" H 1450 4700 60  0000 C CNN
	1    1450 4700
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR05
U 1 1 5B021B9E
P 6000 4150
F 0 "#PWR05" H 6000 4240 20  0001 C CNN
F 1 "+5V" H 6000 4240 30  0000 C CNN
F 2 "" H 6000 4150 60  0000 C CNN
F 3 "" H 6000 4150 60  0000 C CNN
	1    6000 4150
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR06
U 1 1 5B021BAD
P 5200 5100
F 0 "#PWR06" H 5200 5190 20  0001 C CNN
F 1 "+5V" H 5200 5190 30  0000 C CNN
F 2 "" H 5200 5100 60  0000 C CNN
F 3 "" H 5200 5100 60  0000 C CNN
	1    5200 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR07
U 1 1 5B021BBC
P 6000 6000
F 0 "#PWR07" H 6000 6000 30  0001 C CNN
F 1 "GND" H 6000 5930 30  0001 C CNN
F 2 "" H 6000 6000 60  0000 C CNN
F 3 "" H 6000 6000 60  0000 C CNN
	1    6000 6000
	1    0    0    -1  
$EndComp
$Comp
L METROMINI U1
U 1 1 5B021C9F
P 5850 2150
F 0 "U1" H 5850 2050 60  0000 C CNN
F 1 "METROMINI" H 5850 2250 60  0000 C CNN
F 2 "~" H 5850 2150 60  0000 C CNN
F 3 "~" H 5850 2150 60  0000 C CNN
F 4 "~" H 5850 2150 60  0001 C CNN "Mouser Part Number"
F 5 "2590" H 5850 2150 60  0001 C CNN "Adafruit Part Number"
F 6 "12.50" H 5850 2150 60  0001 C CNN "Price"
	1    5850 2150
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR08
U 1 1 5B021CAE
P 6600 2550
F 0 "#PWR08" H 6600 2640 20  0001 C CNN
F 1 "+5V" H 6600 2640 30  0000 C CNN
F 2 "" H 6600 2550 60  0000 C CNN
F 3 "" H 6600 2550 60  0000 C CNN
	1    6600 2550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 5B021CDC
P 6450 2450
F 0 "#PWR09" H 6450 2450 30  0001 C CNN
F 1 "GND" H 6450 2380 30  0001 C CNN
F 2 "" H 6450 2450 60  0000 C CNN
F 3 "" H 6450 2450 60  0000 C CNN
	1    6450 2450
	1    0    0    -1  
$EndComp
$Comp
L RJ45 J2
U 1 1 5B02214E
P 9100 4250
F 0 "J2" H 9300 4750 60  0000 C CNN
F 1 "RJ45" H 8950 4750 60  0000 C CNN
F 2 "~" H 9100 4250 60  0000 C CNN
F 3 "~" H 9100 4250 60  0000 C CNN
F 4 "710-615008144221" H 9100 4250 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 "1.17" H 9100 4250 60  0001 C CNN "Price"
	1    9100 4250
	0    1    1    0   
$EndComp
$Comp
L RJ45 J1
U 1 1 5B02213F
P 9100 2650
F 0 "J1" H 9300 3150 60  0000 C CNN
F 1 "RJ45" H 8950 3150 60  0000 C CNN
F 2 "~" H 9100 2650 60  0000 C CNN
F 3 "~" H 9100 2650 60  0000 C CNN
F 4 "710-615008144221" H 9100 2650 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 "1.17" H 9100 2650 60  0001 C CNN "Price"
	1    9100 2650
	0    1    1    0   
$EndComp
$Comp
L DIODESCH D2
U 1 1 5B0223A4
P 8300 1650
F 0 "D2" H 8300 1750 40  0000 C CNN
F 1 "SB240-E3/54" H 8300 1550 40  0000 C CNN
F 2 "~" H 8300 1650 60  0000 C CNN
F 3 "~" H 8300 1650 60  0000 C CNN
F 4 "625-SB240-E3" H 8300 1650 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 ".46" H 8300 1650 60  0001 C CNN "Price"
	1    8300 1650
	-1   0    0    1   
$EndComp
$Comp
L DIODESCH D1
U 1 1 5B0223B3
P 7700 1650
F 0 "D1" H 7700 1750 40  0000 C CNN
F 1 "SB240-E3/54" H 7700 1550 40  0000 C CNN
F 2 "~" H 7700 1650 60  0000 C CNN
F 3 "~" H 7700 1650 60  0000 C CNN
F 4 "625-SB240-E3 " H 7700 1650 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 ".46" H 7700 1650 60  0001 C CNN "Price"
	1    7700 1650
	-1   0    0    1   
$EndComp
$Comp
L CONN_2 P2
U 1 1 5B0223C2
P 9000 1750
F 0 "P2" V 8950 1750 40  0000 C CNN
F 1 "+ 12V (IN) -" V 9050 1750 40  0000 C CNN
F 2 "~" H 9000 1750 60  0000 C CNN
F 3 "~" H 9000 1750 60  0000 C CNN
F 4 "710-691249510002 " H 9000 1750 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 "1.61" H 9000 1750 60  0001 C CNN "Price"
	1    9000 1750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 5B02253C
P 8450 1950
F 0 "#PWR010" H 8450 1950 30  0001 C CNN
F 1 "GND" H 8450 1880 30  0001 C CNN
F 2 "" H 8450 1950 60  0000 C CNN
F 3 "" H 8450 1950 60  0000 C CNN
	1    8450 1950
	1    0    0    -1  
$EndComp
Text GLabel 7500 1650 0    39   Input ~ 0
Vin
Text GLabel 6400 2200 2    39   Input ~ 0
Vin
$Comp
L GND #PWR011
U 1 1 5B02271C
P 7550 4100
F 0 "#PWR011" H 7550 4100 30  0001 C CNN
F 1 "GND" H 7550 4030 30  0001 C CNN
F 2 "" H 7550 4100 60  0000 C CNN
F 3 "" H 7550 4100 60  0000 C CNN
	1    7550 4100
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 5B02277B
P 7100 5700
F 0 "R2" V 7180 5700 40  0000 C CNN
F 1 "60 Ohms" V 7107 5701 40  0000 C CNN
F 2 "~" V 7030 5700 30  0000 C CNN
F 3 "~" H 7100 5700 30  0000 C CNN
F 4 "71-RN60C60R0B/R " H 7100 5700 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 ".54" H 7100 5700 60  0001 C CNN "Price"
	1    7100 5700
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 5B02278A
P 7800 5700
F 0 "R3" V 7880 5700 40  0000 C CNN
F 1 "60 Ohms" V 7807 5701 40  0000 C CNN
F 2 "~" V 7730 5700 30  0000 C CNN
F 3 "~" H 7800 5700 30  0000 C CNN
F 4 "71-RN60C60R0B/R " H 7800 5700 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 ".54" H 7800 5700 60  0001 C CNN "Price"
	1    7800 5700
	1    0    0    -1  
$EndComp
$Comp
L CONN_2X2 P1
U 1 1 5B02283D
P 7450 6400
F 0 "P1" H 7450 6550 50  0000 C CNN
F 1 "Termination" H 7460 6270 40  0000 C CNN
F 2 "~" H 7450 6400 60  0000 C CNN
F 3 "~" H 7450 6400 60  0000 C CNN
F 4 "855-M20-9980246" H 7450 6400 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 ".16" H 7450 6400 60  0001 C CNN "Price"
	1    7450 6400
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 5B022954
P 6850 6650
F 0 "C4" H 6850 6750 40  0000 L CNN
F 1 "47 nf" H 6856 6565 40  0000 L CNN
F 2 "~" H 6888 6500 30  0000 C CNN
F 3 "~" H 6850 6650 60  0000 C CNN
F 4 "75-1C10Z5U473M050B" H 6850 6650 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 ".10" H 6850 6650 60  0001 C CNN "Price"
	1    6850 6650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 5B022979
P 6850 7000
F 0 "#PWR012" H 6850 7000 30  0001 C CNN
F 1 "GND" H 6850 6930 30  0001 C CNN
F 2 "" H 6850 7000 60  0000 C CNN
F 3 "" H 6850 7000 60  0000 C CNN
	1    6850 7000
	1    0    0    -1  
$EndComp
$Comp
L BRIDGE D3
U 1 1 5B02D40D
P 2350 1900
F 0 "D3" H 2350 1950 70  0000 C CNN
F 1 "KBL 401-G" H 2350 1850 70  0000 C CNN
F 2 "~" H 2350 1900 60  0000 C CNN
F 3 "~" H 2350 1900 60  0000 C CNN
F 4 "750-KBL401-G " H 2350 1900 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 ".88" H 2350 1900 60  0001 C CNN "Price"
	1    2350 1900
	0    -1   -1   0   
$EndComp
$Comp
L MCT6H IC1
U 1 1 5B02D421
P 4150 1450
F 0 "IC1" H 3940 1640 40  0000 C CNN
F 1 "MCT6H" H 4260 1260 40  0000 C CNN
F 2 "DIP-8__300" H 3990 1270 30  0001 C CIN
F 3 "~" H 4150 1450 60  0000 C CNN
F 4 "782-MCT6H" H 4150 1450 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 "1.35" H 4150 1450 60  0001 C CNN "Price"
	1    4150 1450
	1    0    0    -1  
$EndComp
$Comp
L MCT6H IC1
U 2 1 5B02D430
P 4150 2200
F 0 "IC1" H 3940 2390 40  0000 C CNN
F 1 "MCT6H" H 4260 2010 40  0000 C CNN
F 2 "DIP-8__300" H 3990 2020 30  0001 C CIN
F 3 "~" H 4150 2200 60  0000 C CNN
F 4 "782-MCT6H" H 4150 1450 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 "1.35" H 4150 1450 60  0001 C CNN "Price"
	2    4150 2200
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P3
U 1 1 5B02D43F
P 1200 800
F 0 "P3" V 1150 800 40  0000 C CNN
F 1 "In Track Out" V 1250 800 40  0000 C CNN
F 2 "~" H 1200 800 60  0000 C CNN
F 3 "~" H 1200 800 60  0000 C CNN
F 4 "710-691249510002" H 1200 800 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 "1.61" H 1200 800 60  0001 C CNN "Price"
	1    1200 800 
	0    -1   -1   0   
$EndComp
$Comp
L R R4
U 1 1 5B02D44E
P 2850 900
F 0 "R4" V 2930 900 40  0000 C CNN
F 1 "10 Ohms" V 2857 901 40  0000 C CNN
F 2 "~" V 2780 900 30  0000 C CNN
F 3 "~" H 2850 900 30  0000 C CNN
F 4 "782-MCT6H" H 4150 1450 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 ".10" H 2850 900 60  0001 C CNN "Price"
	1    2850 900 
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR013
U 1 1 5B02D93B
P 4600 2450
F 0 "#PWR013" H 4600 2450 30  0001 C CNN
F 1 "GND" H 4600 2380 30  0001 C CNN
F 2 "" H 4600 2450 60  0000 C CNN
F 3 "" H 4600 2450 60  0000 C CNN
	1    4600 2450
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 5B02D94A
P 4700 950
F 0 "R5" V 4780 950 40  0000 C CNN
F 1 "10K Ohms" V 4707 951 40  0000 C CNN
F 2 "~" V 4630 950 30  0000 C CNN
F 3 "~" H 4700 950 30  0000 C CNN
F 4 "782-MCT6H" H 4150 1450 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 ".10" H 4700 950 60  0001 C CNN "Price"
	1    4700 950 
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR014
U 1 1 5B02D9CF
P 4700 600
F 0 "#PWR014" H 4700 690 20  0001 C CNN
F 1 "+5V" H 4700 690 30  0000 C CNN
F 2 "" H 4700 600 60  0000 C CNN
F 3 "" H 4700 600 60  0000 C CNN
	1    4700 600 
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P5
U 1 1 5B05D1DD
P 7600 1050
F 0 "P5" V 7550 1050 40  0000 C CNN
F 1 "- 12V (out) +" V 7650 1050 40  0000 C CNN
F 2 "~" H 7600 1050 60  0000 C CNN
F 3 "~" H 7600 1050 60  0000 C CNN
F 4 "782-MCT6H" H 4150 1450 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 1200 800 60  0001 C CNN "Adafruit Part Number"
F 6 "0.52" H 7600 1050 60  0001 C CNN "Price"
	1    7600 1050
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR015
U 1 1 5B05D274
P 8200 1150
F 0 "#PWR015" H 8200 1150 30  0001 C CNN
F 1 "GND" H 8200 1080 30  0001 C CNN
F 2 "" H 8200 1150 60  0000 C CNN
F 3 "" H 8200 1150 60  0000 C CNN
	1    8200 1150
	1    0    0    -1  
$EndComp
$Comp
L LED D4
U 1 1 5B0718CA
P 3550 6800
F 0 "D4" H 3550 6900 50  0000 C CNN
F 1 "Blue" H 3550 6700 50  0000 C CNN
F 2 "~" H 3550 6800 60  0000 C CNN
F 3 "~" H 3550 6800 60  0000 C CNN
F 4 "859-LTL17KTBS3KS" H 3550 6800 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 3550 6800 60  0001 C CNN "Adafruit Part Number"
F 6 "0.27" H 3550 6800 60  0001 C CNN "Price"
	1    3550 6800
	1    0    0    -1  
$EndComp
$Comp
L LED D5
U 1 1 5B0718DA
P 3550 7200
F 0 "D5" H 3550 7300 50  0000 C CNN
F 1 "Gold" H 3550 7100 50  0000 C CNN
F 2 "~" H 3550 7200 60  0000 C CNN
F 3 "~" H 3550 7200 60  0000 C CNN
F 4 "859-LTL-4251N" H 3550 7200 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 3550 7200 60  0001 C CNN "Adafruit Part Number"
F 6 "0.12" H 3550 7200 60  0001 C CNN "Price"
	1    3550 7200
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 5B071901
P 2950 6800
F 0 "R9" V 3030 6800 40  0000 C CNN
F 1 "1K Ohms" V 2957 6801 40  0000 C CNN
F 2 "~" V 2880 6800 30  0000 C CNN
F 3 "~" H 2950 6800 30  0000 C CNN
F 4 "660-MF1/4LCT52R102J " H 2950 6800 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 2950 6800 60  0001 C CNN "Adafruit Part Number"
F 6 ".10" H 2950 6800 60  0001 C CNN "Price"
	1    2950 6800
	0    -1   -1   0   
$EndComp
$Comp
L R R10
U 1 1 5B071910
P 2950 7200
F 0 "R10" V 3030 7200 40  0000 C CNN
F 1 "1K Ohms" V 2957 7201 40  0000 C CNN
F 2 "~" V 2880 7200 30  0000 C CNN
F 3 "~" H 2950 7200 30  0000 C CNN
F 4 "660-MF1/4LCT52R102J " H 2950 7200 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 2950 7200 60  0001 C CNN "Adafruit Part Number"
F 6 ".10" H 2950 7200 60  0001 C CNN "Price"
	1    2950 7200
	0    -1   -1   0   
$EndComp
$Comp
L R R11
U 1 1 5B07191F
P 4200 6800
F 0 "R11" V 4280 6800 40  0000 C CNN
F 1 "510 Ohms" V 4207 6801 40  0000 C CNN
F 2 "~" V 4130 6800 30  0000 C CNN
F 3 "~" H 4200 6800 30  0000 C CNN
F 4 "660-MF1/4LCT52R511J" H 4200 6800 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 4200 6800 60  0001 C CNN "Adafruit Part Number"
F 6 ".10" H 4200 6800 60  0001 C CNN "Price"
	1    4200 6800
	0    -1   -1   0   
$EndComp
$Comp
L R R12
U 1 1 5B07192E
P 4200 7200
F 0 "R12" V 4280 7200 40  0000 C CNN
F 1 "510 Ohms" V 4207 7201 40  0000 C CNN
F 2 "~" V 4130 7200 30  0000 C CNN
F 3 "~" H 4200 7200 30  0000 C CNN
F 4 "660-MF1/4LCT52R511J" H 4200 7200 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 4200 7200 60  0001 C CNN "Adafruit Part Number"
F 6 ".10" H 4200 7200 60  0001 C CNN "Price"
	1    4200 7200
	0    -1   -1   0   
$EndComp
$Comp
L SW_PUSH SW1
U 1 1 5B071951
P 4900 6800
F 0 "SW1" H 5050 6910 50  0000 C CNN
F 1 "Blue" H 4900 6720 50  0000 C CNN
F 2 "~" H 4900 6800 60  0000 C CNN
F 3 "~" H 4900 6800 60  0000 C CNN
F 4 "506-FSM4JH" H 4900 6800 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 4900 6800 60  0001 C CNN "Adafruit Part Number"
F 6 ".10" H 4900 6800 60  0001 C CNN "Price"
	1    4900 6800
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH SW2
U 1 1 5B071960
P 4900 7200
F 0 "SW2" H 5050 7310 50  0000 C CNN
F 1 "Gold" H 4900 7120 50  0000 C CNN
F 2 "~" H 4900 7200 60  0000 C CNN
F 3 "~" H 4900 7200 60  0000 C CNN
F 4 "506-FSM4JH" H 4900 7200 60  0001 C CNN "Mouser Part Number"
F 5 "~" H 4900 7200 60  0001 C CNN "Adafruit Part Number"
F 6 ".10" H 4900 7200 60  0001 C CNN "Price"
	1    4900 7200
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR016
U 1 1 5B0719C5
P 2700 6600
F 0 "#PWR016" H 2700 6690 20  0001 C CNN
F 1 "+5V" H 2700 6690 30  0000 C CNN
F 2 "" H 2700 6600 60  0000 C CNN
F 3 "" H 2700 6600 60  0000 C CNN
	1    2700 6600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR017
U 1 1 5B0719D4
P 5250 7450
F 0 "#PWR017" H 5250 7450 30  0001 C CNN
F 1 "GND" H 5250 7380 30  0001 C CNN
F 2 "" H 5250 7450 60  0000 C CNN
F 3 "" H 5250 7450 60  0000 C CNN
	1    5250 7450
	1    0    0    -1  
$EndComp
$Comp
L MCP2517 U2
U 1 1 5B0D98F9
P 3200 4800
F 0 "U2" H 3200 4500 60  0000 C CNN
F 1 "MCP2517" H 3200 4900 60  0000 C CNN
F 2 "DIL-18_300" H 3100 4700 60  0001 C CNN
F 3 "~" H 3200 4800 60  0000 C CNN
	1    3200 4800
	1    0    0    -1  
$EndComp
Text GLabel 2350 4600 0    39   Input ~ 0
INT
$Comp
L C C5
U 1 1 5B0D9F08
P 1850 4400
F 0 "C5" H 1850 4500 40  0000 L CNN
F 1 ".1 uf" H 1856 4315 40  0000 L CNN
F 2 "~" H 1888 4250 30  0000 C CNN
F 3 "~" H 1850 4400 60  0000 C CNN
	1    1850 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 3950 3200 3950
Wire Wire Line
	3200 3950 3200 4050
Wire Wire Line
	3200 5950 3200 5900
Wire Wire Line
	2350 5350 2350 5200
Wire Wire Line
	2350 5200 2100 5200
Wire Wire Line
	2100 5200 2100 5150
Wire Wire Line
	2100 5150 1900 5150
Wire Wire Line
	2350 5750 2350 5500
Wire Wire Line
	1900 5750 2350 5750
Connection ~ 2000 5150
Connection ~ 2000 5750
Wire Wire Line
	1500 5150 1500 5850
Connection ~ 1500 5750
Wire Wire Line
	2250 3800 2250 3950
Wire Wire Line
	1450 3950 1450 4200
Connection ~ 2250 3950
Wire Wire Line
	1450 4700 1450 4600
Wire Wire Line
	4050 4650 5150 4650
Wire Wire Line
	5150 4650 5150 4800
Wire Wire Line
	5150 4800 5350 4800
Wire Wire Line
	5350 4900 4950 4900
Wire Wire Line
	4950 4900 4950 4750
Wire Wire Line
	4950 4750 4050 4750
Wire Wire Line
	5350 5350 5350 5850
Wire Wire Line
	5350 5850 6000 5850
Wire Wire Line
	6000 5850 6000 6000
Wire Wire Line
	5350 5200 5200 5200
Wire Wire Line
	5200 5200 5200 5100
Wire Wire Line
	6000 4400 6000 4150
Wire Wire Line
	6600 2550 6300 2550
Wire Wire Line
	6300 2550 6300 2500
Wire Wire Line
	6450 2450 6450 2400
Wire Wire Line
	6450 2400 6300 2400
Wire Wire Line
	6300 2400 6300 2300
Wire Wire Line
	8650 2300 8550 2300
Wire Wire Line
	8550 2300 8550 5300
Wire Wire Line
	8550 3900 8650 3900
Wire Wire Line
	8650 2400 8450 2400
Wire Wire Line
	8450 2400 8450 4950
Wire Wire Line
	8450 4000 8650 4000
Wire Wire Line
	8650 2500 8350 2500
Wire Wire Line
	8350 2500 8350 4100
Wire Wire Line
	8350 4100 8650 4100
Wire Wire Line
	8650 2600 8250 2600
Wire Wire Line
	8250 2600 8250 4200
Wire Wire Line
	8250 4200 8650 4200
Wire Wire Line
	8650 4300 8150 4300
Wire Wire Line
	8150 4300 8150 2700
Wire Wire Line
	8150 2700 8650 2700
Wire Wire Line
	8650 2800 8050 2800
Wire Wire Line
	8050 2800 8050 4400
Wire Wire Line
	8050 4400 8650 4400
Wire Wire Line
	7950 4500 8650 4500
Wire Wire Line
	7950 2200 7950 4500
Wire Wire Line
	7950 2900 8650 2900
Wire Wire Line
	8650 3000 7850 3000
Wire Wire Line
	7850 4600 8650 4600
Wire Wire Line
	7850 2050 8000 2050
Wire Wire Line
	8000 2050 8000 1650
Wire Wire Line
	7900 1650 8100 1650
Connection ~ 7850 3000
Connection ~ 8000 1650
Wire Wire Line
	7950 2200 8200 2200
Wire Wire Line
	8200 2200 8200 1850
Wire Wire Line
	8200 1850 8650 1850
Connection ~ 7950 2900
Wire Wire Line
	8450 1850 8450 1950
Wire Wire Line
	6400 2200 6300 2200
Wire Wire Line
	8550 5300 6650 5300
Connection ~ 8550 3900
Wire Wire Line
	8450 4950 6650 4950
Connection ~ 8450 4000
Wire Wire Line
	7550 3950 8350 3950
Connection ~ 8050 3950
Connection ~ 8350 3950
Wire Wire Line
	7550 3950 7550 4100
Wire Wire Line
	7800 5450 7800 4950
Connection ~ 7800 4950
Wire Wire Line
	7100 5450 7100 5300
Connection ~ 7100 5300
Wire Wire Line
	7100 5950 7050 5950
Wire Wire Line
	7050 5950 7050 6350
Wire Wire Line
	7800 5950 7800 6100
Wire Wire Line
	7800 6100 7850 6100
Wire Wire Line
	7850 6100 7850 6350
Wire Wire Line
	7050 6450 7050 6650
Wire Wire Line
	7050 6650 7850 6650
Wire Wire Line
	7850 6650 7850 6450
Wire Wire Line
	6850 7000 6850 6850
Wire Wire Line
	6850 6450 7050 6450
Wire Wire Line
	2350 1050 2350 2750
Wire Wire Line
	3800 1550 3800 2100
Wire Wire Line
	1500 1900 1500 900 
Wire Wire Line
	1500 900  2600 900 
Wire Wire Line
	3100 900  3100 1350
Wire Wire Line
	3100 1350 3800 1350
Wire Wire Line
	3200 1900 3800 1900
Connection ~ 3800 1900
Wire Wire Line
	3800 2300 3500 2300
Wire Wire Line
	3500 2300 3500 1350
Connection ~ 3500 1350
Wire Wire Line
	4500 1550 4600 1550
Wire Wire Line
	4600 1550 4600 2450
Wire Wire Line
	4600 2300 4500 2300
Wire Wire Line
	4700 2100 4500 2100
Wire Wire Line
	4700 1200 4700 2300
Wire Wire Line
	4700 1350 4500 1350
Wire Wire Line
	1500 1900 1100 1900
Wire Wire Line
	1100 1900 1100 1150
Wire Wire Line
	3200 700  3200 1900
Wire Wire Line
	3200 700  1400 700 
Wire Wire Line
	1400 700  1400 1150
Wire Wire Line
	1400 1150 1300 1150
Connection ~ 4700 1350
Wire Wire Line
	4700 600  4700 700 
Connection ~ 4600 2300
Wire Wire Line
	4700 2300 5400 2300
Connection ~ 4700 2100
Wire Wire Line
	7950 1150 7950 1650
Connection ~ 7950 1650
Wire Wire Line
	7950 950  8200 950 
Wire Wire Line
	8200 950  8200 1150
Wire Wire Line
	5200 6800 5200 7250
Wire Wire Line
	5200 7250 5250 7250
Wire Wire Line
	5250 7250 5250 7450
Connection ~ 5200 7200
Wire Wire Line
	4450 6800 4600 6800
Wire Wire Line
	4450 7200 4600 7200
Wire Wire Line
	3350 6800 3200 6800
Wire Wire Line
	3350 7200 3200 7200
Wire Wire Line
	2700 6600 2700 7200
Connection ~ 2700 6800
Wire Wire Line
	3750 6800 3950 6800
Wire Wire Line
	3750 7200 3950 7200
Wire Wire Line
	5400 2200 5100 2200
Connection ~ 3850 7200
Wire Wire Line
	5400 1900 4900 1900
Wire Wire Line
	4900 1900 4900 6350
Connection ~ 3850 6800
Wire Wire Line
	1850 4200 1850 3950
Connection ~ 1850 3950
Wire Wire Line
	1450 4600 1850 4600
Wire Wire Line
	8500 1650 8650 1650
Wire Wire Line
	9500 1650 9500 1400
Wire Wire Line
	9500 1400 8600 1400
Wire Wire Line
	8600 1400 8600 1650
Connection ~ 8600 1650
Wire Wire Line
	9500 1850 9500 2100
Wire Wire Line
	9500 2100 8550 2100
Wire Wire Line
	8550 2100 8550 1850
Connection ~ 8450 1850
Connection ~ 8550 1850
$Comp
L BARREL_JACK_NOSWITCH CON1
U 1 1 5B0DB85B
P 9800 1750
F 0 "CON1" H 9800 2000 60  0000 C CNN
F 1 "PJ_037A" H 9800 1550 60  0000 C CNN
F 2 "~" H 9800 1750 60  0000 C CNN
F 3 "~" H 9800 1750 60  0000 C CNN
	1    9800 1750
	-1   0    0    -1  
$EndComp
$Comp
L JUMPER JP1
U 1 1 5B0E8562
P 7850 2500
F 0 "JP1" H 7850 2650 60  0000 C CNN
F 1 "JUMPER" H 7850 2420 40  0000 C CNN
F 2 "~" H 7850 2500 60  0000 C CNN
F 3 "~" H 7850 2500 60  0000 C CNN
	1    7850 2500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7850 2050 7850 2200
Wire Wire Line
	7850 2800 7850 4600
Text GLabel 6300 1600 2    39   Input ~ 0
SDA
Text GLabel 6300 1500 2    39   Input ~ 0
SCL
$Comp
L TLC59208F U4
U 1 1 5B1C151B
P 3600 3350
F 0 "U4" H 3600 3350 60  0000 C CNN
F 1 "TLC59208F" H 3550 3250 30  0000 C CNN
F 2 "" H 3600 3350 60  0000 C CNN
F 3 "" H 3600 3350 60  0000 C CNN
	1    3600 3350
	-1   0    0    -1  
$EndComp
Text GLabel 4000 3100 2    20   Input ~ 0
SCL
Text GLabel 4000 3150 2    20   Input ~ 0
SDA
$Comp
L R R15
U 1 1 5B1C1530
P 4250 3000
F 0 "R15" V 4330 3000 40  0000 C CNN
F 1 "10K Ohms" V 4257 3001 40  0000 C CNN
F 2 "~" V 4180 3000 30  0000 C CNN
F 3 "~" H 4250 3000 30  0000 C CNN
	1    4250 3000
	-1   0    0    1   
$EndComp
$Comp
L +5V #PWR018
U 1 1 5B1C1544
P 3600 2650
F 0 "#PWR018" H 3600 2740 20  0001 C CNN
F 1 "+5V" H 3600 2740 30  0000 C CNN
F 2 "" H 3600 2650 60  0000 C CNN
F 3 "" H 3600 2650 60  0000 C CNN
	1    3600 2650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR019
U 1 1 5B1C1553
P 3600 3850
F 0 "#PWR019" H 3600 3850 30  0001 C CNN
F 1 "GND" H 3600 3780 30  0001 C CNN
F 2 "" H 3600 3850 60  0000 C CNN
F 3 "" H 3600 3850 60  0000 C CNN
	1    3600 3850
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5B1C1598
P 2800 2900
F 0 "R1" V 2880 2900 40  0000 C CNN
F 1 "330 Ohms" V 2807 2901 40  0000 C CNN
F 2 "~" V 2730 2900 30  0000 C CNN
F 3 "~" H 2800 2900 30  0000 C CNN
	1    2800 2900
	0    -1   -1   0   
$EndComp
$Comp
L R R6
U 1 1 5B1C15A8
P 2800 3050
F 0 "R6" V 2880 3050 40  0000 C CNN
F 1 "330 Ohms" V 2807 3051 40  0000 C CNN
F 2 "~" V 2730 3050 30  0000 C CNN
F 3 "~" H 2800 3050 30  0000 C CNN
	1    2800 3050
	0    -1   -1   0   
$EndComp
$Comp
L R R7
U 1 1 5B1C15B7
P 2800 3200
F 0 "R7" V 2880 3200 40  0000 C CNN
F 1 "330 Ohms" V 2807 3201 40  0000 C CNN
F 2 "~" V 2730 3200 30  0000 C CNN
F 3 "~" H 2800 3200 30  0000 C CNN
	1    2800 3200
	0    -1   -1   0   
$EndComp
$Comp
L R R8
U 1 1 5B1C15C6
P 2800 3350
F 0 "R8" V 2880 3350 40  0000 C CNN
F 1 "330 Ohms" V 2807 3351 40  0000 C CNN
F 2 "~" V 2730 3350 30  0000 C CNN
F 3 "~" H 2800 3350 30  0000 C CNN
	1    2800 3350
	0    -1   -1   0   
$EndComp
$Comp
L R R13
U 1 1 5B1C15D5
P 2800 3500
F 0 "R13" V 2880 3500 40  0000 C CNN
F 1 "330 Ohms" V 2807 3501 40  0000 C CNN
F 2 "~" V 2730 3500 30  0000 C CNN
F 3 "~" H 2800 3500 30  0000 C CNN
	1    2800 3500
	0    -1   -1   0   
$EndComp
$Comp
L R R14
U 1 1 5B1C15E4
P 2800 3650
F 0 "R14" V 2880 3650 40  0000 C CNN
F 1 "330 Ohms" V 2807 3651 40  0000 C CNN
F 2 "~" V 2730 3650 30  0000 C CNN
F 3 "~" H 2800 3650 30  0000 C CNN
	1    2800 3650
	0    -1   -1   0   
$EndComp
$Comp
L CONN_8 P4
U 1 1 5B1C15F3
P 1250 3300
F 0 "P4" V 1200 3300 60  0000 C CNN
F 1 "+5 WR WY WG ER EY EG +5" V 1300 3300 60  0000 C CNN
F 2 "~" H 1250 3300 60  0000 C CNN
F 3 "~" H 1250 3300 60  0000 C CNN
	1    1250 3300
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4900 6350 3850 6350
Wire Wire Line
	3850 6350 3850 6800
Wire Wire Line
	5100 2200 5100 7000
Wire Wire Line
	5100 7000 3850 7000
Wire Wire Line
	3850 7000 3850 7200
Wire Wire Line
	3050 2900 3250 2900
Wire Wire Line
	3250 2900 3250 3100
Wire Wire Line
	3250 3150 3200 3150
Wire Wire Line
	3200 3150 3200 3050
Wire Wire Line
	3200 3050 3050 3050
Wire Wire Line
	3250 3200 3050 3200
Wire Wire Line
	3250 3250 3100 3250
Wire Wire Line
	3100 3250 3100 3350
Wire Wire Line
	3100 3350 3050 3350
Wire Wire Line
	3250 3300 3150 3300
Wire Wire Line
	3150 3300 3150 3500
Wire Wire Line
	3150 3500 3050 3500
Wire Wire Line
	3250 3350 3200 3350
Wire Wire Line
	3200 3350 3200 3650
Wire Wire Line
	3200 3650 3050 3650
Wire Wire Line
	2550 2900 1750 2900
Wire Wire Line
	1750 2900 1750 3050
Wire Wire Line
	1750 3050 1600 3050
Wire Wire Line
	2550 3050 1850 3050
Wire Wire Line
	1850 3050 1850 3150
Wire Wire Line
	1850 3150 1600 3150
Wire Wire Line
	2550 3250 1600 3250
Wire Wire Line
	2550 3250 2550 3200
Wire Wire Line
	2550 3350 1600 3350
Wire Wire Line
	2550 3500 1750 3500
Wire Wire Line
	1750 3500 1750 3450
Wire Wire Line
	1750 3450 1600 3450
Wire Wire Line
	2550 3650 1650 3650
Wire Wire Line
	1650 3650 1650 3550
Wire Wire Line
	1650 3550 1600 3550
$Comp
L +5V #PWR020
U 1 1 5B1C21E0
P 1600 2650
F 0 "#PWR020" H 1600 2740 20  0001 C CNN
F 1 "+5V" H 1600 2740 30  0000 C CNN
F 2 "" H 1600 2650 60  0000 C CNN
F 3 "" H 1600 2650 60  0000 C CNN
	1    1600 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 2950 1600 2650
Wire Wire Line
	1600 2950 1700 2950
Wire Wire Line
	1700 2950 1700 3750
Wire Wire Line
	1700 3750 1600 3750
Wire Wire Line
	1600 3750 1600 3650
Wire Wire Line
	3600 2950 3600 2650
Wire Wire Line
	4250 2750 3600 2750
Connection ~ 3600 2750
Wire Wire Line
	4250 3250 4000 3250
Wire Wire Line
	3600 3750 3600 3850
Wire Wire Line
	4000 3450 4000 3800
Wire Wire Line
	4000 3800 3600 3800
Connection ~ 3600 3800
Connection ~ 4000 3550
Connection ~ 4000 3500
Text Label 1800 2900 0    39   ~ 0
East Green
Text Label 1900 3050 0    39   ~ 0
East Yellow
Text Label 1850 3250 0    39   ~ 0
East Red
Text Label 1850 3350 0    39   ~ 0
West Green
Text Label 1850 3500 0    39   ~ 0
West Yellow
Text Label 1850 3650 0    39   ~ 0
West Red
$EndSCHEMATC
