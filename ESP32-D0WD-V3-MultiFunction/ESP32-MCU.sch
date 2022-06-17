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
LIBS:ESP32-D0WD-V3
LIBS:W25Q32JVSSIQ
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
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
L ESP32-D0WD-V3 U?
U 1 1 62ACDF68
P 4850 3200
F 0 "U?" H 5600 2950 50  0000 L CNN
F 1 "ESP32-D0WD-V3" H 5400 3300 50  0000 L CNN
F 2 "QFN35P500X500X90-49N-D" H 6500 3700 50  0001 L CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf" H 6500 3600 50  0001 L CNN
F 4 "WiFi Development Tools (802.11) SMD IC WiFi Dual Core BT Combo" H 6500 3500 50  0001 L CNN "Description"
F 5 "0.9" H 6500 3400 50  0001 L CNN "Height"
F 6 "" H 6500 3300 50  0001 L CNN "Mouser2 Part Number"
F 7 "" H 6500 3200 50  0001 L CNN "Mouser2 Price/Stock"
F 8 "Espressif Systems" H 6500 3100 50  0001 L CNN "Manufacturer_Name"
F 9 "ESP32-D0WD-V3" H 6500 3000 50  0001 L CNN "Manufacturer_Part_Number"
	1    4850 3200
	1    0    0    -1  
$EndComp
$Comp
L W25Q32JVSSIQ U?
U 1 1 62ACEC4E
P 7100 3800
F 0 "U?" H 7900 3700 50  0000 L CNN
F 1 "W25Q32JVSSIQ" H 7500 3500 50  0000 L CNN
F 2 "SOIC127P790X216-8N" H 8450 3900 50  0001 L CNN
F 3 "https://componentsearchengine.com//W25Q32JVSSIQ.pdf" H 8450 3800 50  0001 L CNN
F 4 "NOR Flash spiFlash, 32M-bit, DTR, 4Kb Uniform Sector" H 8450 3700 50  0001 L CNN "Description"
F 5 "2.16" H 8450 3600 50  0001 L CNN "Height"
F 6 "454-W25Q32JVSSIQ" H 8450 3500 50  0001 L CNN "Mouser2 Part Number"
F 7 "https://www.mouser.com/Search/Refine.aspx?Keyword=454-W25Q32JVSSIQ" H 8450 3400 50  0001 L CNN "Mouser2 Price/Stock"
F 8 "Winbond" H 8450 3300 50  0001 L CNN "Manufacturer_Name"
F 9 "W25Q32JVSSIQ" H 8450 3200 50  0001 L CNN "Manufacturer_Part_Number"
	1    7100 3800
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 62ACEDF1
P 8850 3450
F 0 "#PWR?" H 8850 3300 50  0001 C CNN
F 1 "+3.3V" H 8850 3590 50  0000 C CNN
F 2 "" H 8850 3450 50  0001 C CNN
F 3 "" H 8850 3450 50  0001 C CNN
	1    8850 3450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 62ACEE0B
P 7000 4400
F 0 "#PWR?" H 7000 4150 50  0001 C CNN
F 1 "GND" H 7000 4250 50  0000 C CNN
F 2 "" H 7000 4400 50  0001 C CNN
F 3 "" H 7000 4400 50  0001 C CNN
	1    7000 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 3450 8850 3800
Wire Wire Line
	8850 3800 8600 3800
Wire Wire Line
	7000 4400 7000 4100
Wire Wire Line
	7000 4100 7100 4100
Wire Wire Line
	7100 3800 6900 3800
Wire Wire Line
	6900 3800 6900 4000
Wire Wire Line
	6900 4000 6650 4000
Wire Wire Line
	6650 3800 6800 3800
Wire Wire Line
	6800 3800 6800 3900
Wire Wire Line
	6800 3900 7100 3900
Wire Wire Line
	7100 4000 6950 4000
Wire Wire Line
	6950 4000 6950 4100
Wire Wire Line
	6950 4100 6650 4100
Wire Wire Line
	8600 4100 9000 4100
Wire Wire Line
	9000 4100 9000 3500
Wire Wire Line
	9000 3500 6900 3500
Wire Wire Line
	6900 3500 6900 3700
Wire Wire Line
	6900 3700 6650 3700
Wire Wire Line
	8600 4000 8850 4000
Wire Wire Line
	8850 4000 8850 4300
Wire Wire Line
	8850 4300 6750 4300
Wire Wire Line
	6750 4300 6750 3900
Wire Wire Line
	6750 3900 6650 3900
Wire Wire Line
	6650 4200 7150 4200
Wire Wire Line
	7150 4200 7150 4400
Wire Wire Line
	7150 4400 8900 4400
Wire Wire Line
	8900 4400 8900 3900
Wire Wire Line
	8900 3900 8600 3900
$EndSCHEMATC
