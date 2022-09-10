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
LIBS:LPC1754FBD80_551
LIBS:217175-0001
LIBS:gct_usb4105
LIBS:USBLC6-2SC6
LIBS:sn65hvd233-ht
LIBS:lm2574n-5
LIBS:LPC1754-MultiIOBoard-cache
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
$Sheet
S 3700 2450 3325 2575
U 631B4A0D
F0 "Micro Processor Unit" 60
F1 "MCU.sch" 60
F2 "CAN_RX" I L 3700 3050 60 
F3 "CAN_TX" O L 3700 3250 60 
$EndSheet
$Sheet
S 1100 2450 1650 2250
U 631BC9FA
F0 "CAN Transceiver" 60
F1 "CANTransceiver.sch" 60
F2 "CAN_TX" I R 2750 3250 60 
F3 "CAN_RX" O R 2750 3050 60 
$EndSheet
Wire Wire Line
	3700 3050 2750 3050
Wire Wire Line
	3700 3250 2750 3250
$Sheet
S 1050 5425 2025 1725
U 631BD2EB
F0 "Power Supply" 60
F1 "PowerSupply.sch" 60
$EndSheet
$Sheet
S 7525 1300 2575 2025
U 631BF9DD
F0 "Output Ports" 60
F1 "OutputPorts.sch" 60
$EndSheet
$Sheet
S 7450 3800 2850 1950
U 631BF9EC
F0 "Input Ports" 60
F1 "InputPorts.sch" 60
$EndSheet
$EndSCHEMATC
