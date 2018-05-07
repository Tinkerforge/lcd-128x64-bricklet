EESchema Schematic File Version 2
LIBS:tinkerforge
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
LIBS:lcd-128x64-bricklet-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "LCD 128x64 Bricklet"
Date "2018-03-27"
Rev "1.0"
Comp "Tinkerforge GmbH"
Comment1 "Licensed under CERN OHL v.1.1"
Comment2 "Copyright (©) 2018, L.Lauer <lukas@tinkerforge.com>"
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 600  7700 0    40   ~ 0
Copyright Tinkerforge GmbH 2018.\nThis documentation describes Open Hardware and is licensed under the\nCERN OHL v. 1.1.\nYou may redistribute and modify this documentation under the terms of the\nCERN OHL v.1.1. (http://ohwr.org/cernohl). This documentation is distributed\nWITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING OF\nMERCHANTABILITY, SATISFACTORY QUALITY AND FITNESS FOR A\nPARTICULAR PURPOSE. Please see the CERN OHL v.1.1 for applicable\nconditions\n
$Comp
L DRILL U4
U 1 1 4C6050A5
P 10650 6150
F 0 "U4" H 10700 6200 60  0001 C CNN
F 1 "DRILL" H 10650 6150 60  0000 C CNN
F 2 "kicad-libraries:DRILL_NP" H 10650 6150 60  0001 C CNN
F 3 "" H 10650 6150 60  0001 C CNN
	1    10650 6150
	1    0    0    -1  
$EndComp
$Comp
L DRILL U5
U 1 1 4C6050A2
P 10650 6350
F 0 "U5" H 10700 6400 60  0001 C CNN
F 1 "DRILL" H 10650 6350 60  0000 C CNN
F 2 "kicad-libraries:DRILL_NP" H 10650 6350 60  0001 C CNN
F 3 "" H 10650 6350 60  0001 C CNN
	1    10650 6350
	1    0    0    -1  
$EndComp
$Comp
L DRILL U7
U 1 1 4C60509F
P 11000 6350
F 0 "U7" H 11050 6400 60  0001 C CNN
F 1 "DRILL" H 11000 6350 60  0000 C CNN
F 2 "kicad-libraries:DRILL_NP" H 11000 6350 60  0001 C CNN
F 3 "" H 11000 6350 60  0001 C CNN
	1    11000 6350
	1    0    0    -1  
$EndComp
$Comp
L DRILL U6
U 1 1 4C605099
P 11000 6150
F 0 "U6" H 11050 6200 60  0001 C CNN
F 1 "DRILL" H 11000 6150 60  0000 C CNN
F 2 "kicad-libraries:DRILL_NP" H 11000 6150 60  0001 C CNN
F 3 "" H 11000 6150 60  0001 C CNN
	1    11000 6150
	1    0    0    -1  
$EndComp
Text GLabel 2550 1700 2    39   Output ~ 0
S-CS
$Comp
L VCC #PWR01
U 1 1 4C5FCFB4
P 2300 850
F 0 "#PWR01" H 2300 950 30  0001 C CNN
F 1 "VCC" H 2300 950 30  0000 C CNN
F 2 "" H 2300 850 60  0001 C CNN
F 3 "" H 2300 850 60  0001 C CNN
	1    2300 850 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 4C5FCF5E
P 1100 2500
F 0 "#PWR02" H 1100 2500 30  0001 C CNN
F 1 "GND" H 1100 2430 30  0001 C CNN
F 2 "" H 1100 2500 60  0001 C CNN
F 3 "" H 1100 2500 60  0001 C CNN
	1    1100 2500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 4C5FCF4F
P 1600 2500
F 0 "#PWR03" H 1600 2500 30  0001 C CNN
F 1 "GND" H 1600 2430 30  0001 C CNN
F 2 "" H 1600 2500 60  0001 C CNN
F 3 "" H 1600 2500 60  0001 C CNN
	1    1600 2500
	1    0    0    -1  
$EndComp
$Comp
L CON-SENSOR2 P1
U 1 1 59A678E0
P 1100 1700
F 0 "P1" H 950 2100 60  0000 C CNN
F 1 "CON-SENSOR2" V 1250 1700 60  0000 C CNN
F 2 "kicad-libraries:CON-SENSOR2" H 1200 1550 60  0001 C CNN
F 3 "" H 1200 1550 60  0000 C CNN
	1    1100 1700
	-1   0    0    -1  
$EndComp
Text GLabel 2550 1800 2    39   Output ~ 0
S-CLK
Text GLabel 2550 1900 2    39   Output ~ 0
S-MOSI
Text GLabel 2550 2000 2    39   Input ~ 0
S-MISO
$Comp
L C C2
U 1 1 59A688A5
P 1850 1150
F 0 "C2" V 1900 1200 50  0000 L CNN
F 1 "10uF" V 1800 1200 50  0000 L CNN
F 2 "kicad-libraries:C0805" H 1850 1150 60  0001 C CNN
F 3 "" H 1850 1150 60  0001 C CNN
	1    1850 1150
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 59A68920
P 2300 1150
F 0 "C3" V 2350 1200 50  0000 L CNN
F 1 "1uF" V 2250 1200 50  0000 L CNN
F 2 "kicad-libraries:C0603F" H 2300 1150 60  0001 C CNN
F 3 "" H 2300 1150 60  0001 C CNN
	1    2300 1150
	1    0    0    -1  
$EndComp
$Comp
L R_PACK4 RP1
U 1 1 59A68BF3
P 2000 2050
F 0 "RP1" H 2000 2500 50  0000 C CNN
F 1 "82" H 2000 2000 50  0000 C CNN
F 2 "kicad-libraries:4X0402" H 2000 2050 50  0001 C CNN
F 3 "" H 2000 2050 50  0000 C CNN
	1    2000 2050
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 59A68F1D
P 1600 2250
F 0 "C1" V 1650 2300 50  0000 L CNN
F 1 "220pF" V 1550 2300 50  0000 L CNN
F 2 "kicad-libraries:C0402F" H 1600 2250 60  0001 C CNN
F 3 "" H 1600 2250 60  0001 C CNN
	1    1600 2250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 59A69103
P 2300 1550
F 0 "#PWR04" H 2300 1550 30  0001 C CNN
F 1 "GND" H 2300 1480 30  0001 C CNN
F 2 "" H 2300 1550 60  0001 C CNN
F 3 "" H 2300 1550 60  0001 C CNN
	1    2300 1550
	1    0    0    -1  
$EndComp
$Comp
L XMC1XXX24 U1
U 1 1 59A6925A
P 4500 6550
F 0 "U1" H 4350 6950 60  0000 C CNN
F 1 "XMC130024" H 4500 6150 60  0000 C CNN
F 2 "kicad-libraries:QFN24-4x4mm-0.5mm" H 4650 7300 60  0001 C CNN
F 3 "" H 4650 7300 60  0000 C CNN
	1    4500 6550
	1    0    0    -1  
$EndComp
$Comp
L XMC1XXX24 U1
U 3 1 59A6930E
P 4500 2800
F 0 "U1" H 4350 3100 60  0000 C CNN
F 1 "XMC130024" H 4500 2550 60  0000 C CNN
F 2 "kicad-libraries:QFN24-4x4mm-0.5mm" H 4650 3550 60  0001 C CNN
F 3 "" H 4650 3550 60  0000 C CNN
	3    4500 2800
	1    0    0    -1  
$EndComp
$Comp
L XMC1XXX24 U1
U 4 1 59A69353
P 4500 5150
F 0 "U1" H 4350 5650 60  0000 C CNN
F 1 "XMC130024" H 4500 4700 60  0000 C CNN
F 2 "kicad-libraries:QFN24-4x4mm-0.5mm" H 4650 5900 60  0001 C CNN
F 3 "" H 4650 5900 60  0000 C CNN
	4    4500 5150
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 59A697EC
P 4050 6500
F 0 "C4" H 4100 6600 50  0000 L CNN
F 1 "100n" H 4100 6400 50  0000 L CNN
F 2 "kicad-libraries:C0603F" H 4050 6500 60  0001 C CNN
F 3 "" H 4050 6500 60  0000 C CNN
	1    4050 6500
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR05
U 1 1 59A69992
P 4050 6950
F 0 "#PWR05" H 4050 6950 30  0001 C CNN
F 1 "GND" H 4050 6880 30  0001 C CNN
F 2 "" H 4050 6950 60  0001 C CNN
F 3 "" H 4050 6950 60  0001 C CNN
	1    4050 6950
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR06
U 1 1 59A69AFC
P 4050 6200
F 0 "#PWR06" H 4050 6300 30  0001 C CNN
F 1 "VCC" H 4050 6300 30  0000 C CNN
F 2 "" H 4050 6200 60  0001 C CNN
F 3 "" H 4050 6200 60  0001 C CNN
	1    4050 6200
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 59A6A2DF
P 3800 4900
F 0 "R1" V 3800 5000 50  0000 C CNN
F 1 "1k" V 3800 4800 50  0000 C CNN
F 2 "kicad-libraries:R0603F" H 3800 4900 60  0001 C CNN
F 3 "" H 3800 4900 60  0000 C CNN
	1    3800 4900
	0    1    1    0   
$EndComp
$Comp
L LED D1
U 1 1 59A6A408
P 3300 4900
F 0 "D1" H 3300 5000 50  0000 C CNN
F 1 "blue" H 3300 4800 50  0000 C CNN
F 2 "kicad-libraries:D0603E" H 3300 4900 50  0001 C CNN
F 3 "" H 3300 4900 50  0000 C CNN
	1    3300 4900
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 P3
U 1 1 59A6A44B
P 3700 5150
F 0 "P3" H 3700 5300 50  0000 C CNN
F 1 "BOOT" V 3800 5150 50  0000 C CNN
F 2 "kicad-libraries:SolderJumper" H 3700 5050 50  0001 C CNN
F 3 "" H 3700 5050 50  0000 C CNN
	1    3700 5150
	-1   0    0    1   
$EndComp
NoConn ~ 4150 5500
NoConn ~ 4150 5200
NoConn ~ 4150 5000
NoConn ~ 4150 5300
Text GLabel 4000 3700 0    39   Output ~ 0
S-MISO
$Comp
L GND #PWR07
U 1 1 59A6AA4C
P 3950 5250
F 0 "#PWR07" H 3950 5250 30  0001 C CNN
F 1 "GND" H 3950 5180 30  0001 C CNN
F 2 "" H 3950 5250 60  0001 C CNN
F 3 "" H 3950 5250 60  0001 C CNN
	1    3950 5250
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR08
U 1 1 59A6AAC7
P 3050 4600
F 0 "#PWR08" H 3050 4700 30  0001 C CNN
F 1 "VCC" H 3050 4700 30  0000 C CNN
F 2 "" H 3050 4600 60  0001 C CNN
F 3 "" H 3050 4600 60  0001 C CNN
	1    3050 4600
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X01 P2
U 1 1 59A6B0EA
P 3600 3400
F 0 "P2" H 3600 3500 50  0000 C CNN
F 1 "Debug" V 3700 3400 50  0000 C CNN
F 2 "kicad-libraries:DEBUG_PAD" H 3600 3400 50  0001 C CNN
F 3 "" H 3600 3400 50  0000 C CNN
	1    3600 3400
	-1   0    0    1   
$EndComp
Text GLabel 4000 3900 0    39   Input ~ 0
S-CS
Text GLabel 3900 2950 0    39   Input ~ 0
S-CLK
Text GLabel 3900 2850 0    39   Input ~ 0
S-MOSI
Text GLabel 3350 4300 0    39   Input ~ 0
M-MISO
Text GLabel 3350 4200 0    39   Output ~ 0
M-CLK
Text GLabel 3350 4100 0    39   Output ~ 0
M-CS-LCD
Text GLabel 3450 4650 0    39   Output ~ 0
M-MOSI
$Comp
L XMC1XXX24 U1
U 2 1 59A692AF
P 4500 3850
F 0 "U1" H 4350 4400 60  0000 C CNN
F 1 "XMC130024" H 4500 3300 60  0000 C CNN
F 2 "kicad-libraries:QFN24-4x4mm-0.5mm" H 4650 4600 60  0001 C CNN
F 3 "" H 4650 4600 60  0000 C CNN
	2    4500 3850
	1    0    0    -1  
$EndComp
Text GLabel 8850 1800 0    39   Input ~ 0
M-CLK
Text GLabel 8850 1900 0    39   Input ~ 0
M-MOSI
Text GLabel 8850 700  0    39   Input ~ 0
M-CS-LCD
$Comp
L R_PACK4 RP2
U 1 1 59A6D565
P 3650 4350
F 0 "RP2" H 3650 4300 50  0000 C CNN
F 1 "82" H 3650 4800 50  0000 C CNN
F 2 "kicad-libraries:4X0402" H 3650 4350 50  0001 C CNN
F 3 "" H 3650 4350 50  0000 C CNN
	1    3650 4350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 4CE29748
P 8100 3550
F 0 "#PWR09" H 8100 3550 30  0001 C CNN
F 1 "GND" H 8100 3480 30  0001 C CNN
F 2 "" H 8100 3550 60  0001 C CNN
F 3 "" H 8100 3550 60  0001 C CNN
	1    8100 3550
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR010
U 1 1 59A6F279
P 10500 1950
F 0 "#PWR010" H 10500 2050 30  0001 C CNN
F 1 "VCC" H 10500 2050 30  0000 C CNN
F 2 "" H 10500 1950 60  0001 C CNN
F 3 "" H 10500 1950 60  0001 C CNN
	1    10500 1950
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 5A255E0E
P 8550 2400
F 0 "C9" H 8650 2400 50  0000 L CNN
F 1 "2,2uF" H 8600 2300 50  0000 L CNN
F 2 "kicad-libraries:C0603F" H 8550 2400 60  0001 C CNN
F 3 "" H 8550 2400 60  0000 C CNN
	1    8550 2400
	1    0    0    -1  
$EndComp
$Comp
L C C10
U 1 1 5A256D22
P 8850 2300
F 0 "C10" H 8950 2300 50  0000 L CNN
F 1 "2,2uF" H 8900 2200 50  0000 L CNN
F 2 "kicad-libraries:C0603F" H 8850 2300 60  0001 C CNN
F 3 "" H 8850 2300 60  0000 C CNN
	1    8850 2300
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 5A257C2E
P 8100 2250
F 0 "C7" V 8050 2300 50  0000 L CNN
F 1 "0,33uF" V 8050 1950 50  0000 L CNN
F 2 "kicad-libraries:R0603F" H 8100 2250 60  0001 C CNN
F 3 "" H 8100 2250 60  0000 C CNN
	1    8100 2250
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 5A257C98
P 8300 2300
F 0 "R6" V 8300 2200 50  0000 C CNN
F 1 "10M" V 8300 2350 50  0000 C CNN
F 2 "kicad-libraries:R0603F" H 8300 2300 60  0001 C CNN
F 3 "" H 8300 2300 60  0000 C CNN
	1    8300 2300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 5A258EDD
P 8300 3550
F 0 "#PWR011" H 8300 3550 30  0001 C CNN
F 1 "GND" H 8300 3480 30  0001 C CNN
F 2 "" H 8300 3550 60  0001 C CNN
F 3 "" H 8300 3550 60  0001 C CNN
	1    8300 3550
	1    0    0    -1  
$EndComp
$Comp
L C C11
U 1 1 5A25A69E
P 10000 2350
F 0 "C11" H 10050 2450 50  0000 L CNN
F 1 "100n" H 10000 2250 50  0000 L CNN
F 2 "kicad-libraries:C0603F" H 10000 2350 60  0001 C CNN
F 3 "" H 10000 2350 60  0000 C CNN
	1    10000 2350
	-1   0    0    1   
$EndComp
$Comp
L C C12
U 1 1 5A25ADC6
P 10300 2350
F 0 "C12" H 10350 2450 50  0000 L CNN
F 1 "10uF" H 10300 2250 50  0000 L CNN
F 2 "kicad-libraries:C0805" H 10300 2350 60  0001 C CNN
F 3 "" H 10300 2350 60  0000 C CNN
	1    10300 2350
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR012
U 1 1 5A25B4C7
P 10450 3550
F 0 "#PWR012" H 10450 3550 30  0001 C CNN
F 1 "GND" H 10450 3480 30  0001 C CNN
F 2 "" H 10450 3550 60  0001 C CNN
F 3 "" H 10450 3550 60  0001 C CNN
	1    10450 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 5A25D61C
P 8950 2750
F 0 "#PWR013" H 8950 2750 30  0001 C CNN
F 1 "GND" H 8950 2680 30  0001 C CNN
F 2 "" H 8950 2750 60  0001 C CNN
F 3 "" H 8950 2750 60  0001 C CNN
	1    8950 2750
	0    1    1    0   
$EndComp
$Comp
L MOSFET_N_CH Q1
U 1 1 5A25F36E
P 8900 3300
F 0 "Q1" H 9200 3350 50  0000 R CNN
F 1 "2N7002" H 9400 3250 50  0000 R CNN
F 2 "kicad-libraries:SOT23GDS" H 9100 3400 50  0001 C CNN
F 3 "" H 8900 3300 50  0000 C CNN
	1    8900 3300
	1    0    0    -1  
$EndComp
$Comp
L CP1 C8
U 1 1 5A25F87E
P 8500 3150
F 0 "C8" H 8500 3250 50  0000 L CNN
F 1 "100µF/6.3V" H 8300 3050 50  0000 L CNN
F 2 "kicad-libraries:3528-21" H 8500 3150 60  0001 C CNN
F 3 "" H 8500 3150 60  0000 C CNN
	1    8500 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 5A260EE7
P 8500 3550
F 0 "#PWR014" H 8500 3550 30  0001 C CNN
F 1 "GND" H 8500 3480 30  0001 C CNN
F 2 "" H 8500 3550 60  0001 C CNN
F 3 "" H 8500 3550 60  0001 C CNN
	1    8500 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 5A2618F9
P 9000 3550
F 0 "#PWR015" H 9000 3550 30  0001 C CNN
F 1 "GND" H 9000 3480 30  0001 C CNN
F 2 "" H 9000 3550 60  0001 C CNN
F 3 "" H 9000 3550 60  0001 C CNN
	1    9000 3550
	1    0    0    -1  
$EndComp
Text GLabel 8650 3400 3    39   Input ~ 0
SW-BACK
$Comp
L CON_4Wire_ER-TP028-1 P4
U 1 1 5A265705
P 10150 5200
F 0 "P4" H 10000 5500 39  0000 C CNN
F 1 "CON_4Wire_ER-TP028-1" H 10300 4850 39  0000 C CNN
F 2 "kicad-libraries:ER-CON04HB-2" H 11000 5850 39  0001 C CNN
F 3 "" H 11000 5850 39  0000 C CNN
	1    10150 5200
	1    0    0    -1  
$EndComp
Text GLabel 3900 2650 0    39   Input ~ 0
SW-BACK
$Comp
L TSC2046E U2
U 1 1 5A256B60
P 8450 5500
F 0 "U2" H 8650 6350 60  0000 C CNN
F 1 "TSC2046E" H 8700 5000 60  0000 C CNN
F 2 "kicad-libraries:TSSOP16" H 9400 6400 60  0001 C CNN
F 3 "" H 9400 6400 60  0000 C CNN
	1    8450 5500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 5A2578F5
P 8450 6350
F 0 "#PWR016" H 8450 6350 30  0001 C CNN
F 1 "GND" H 8450 6280 30  0001 C CNN
F 2 "" H 8450 6350 60  0001 C CNN
F 3 "" H 8450 6350 60  0001 C CNN
	1    8450 6350
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 5A25E881
P 8050 4450
F 0 "C6" H 8100 4550 50  0000 L CNN
F 1 "100n" H 8050 4350 50  0000 L CNN
F 2 "kicad-libraries:C0603F" H 8050 4450 60  0001 C CNN
F 3 "" H 8050 4450 60  0000 C CNN
	1    8050 4450
	0    -1   -1   0   
$EndComp
$Comp
L C C5
U 1 1 5A25ED48
P 8050 4150
F 0 "C5" H 8100 4250 50  0000 L CNN
F 1 "10uF" H 8050 4050 50  0000 L CNN
F 2 "kicad-libraries:C0805" H 8050 4150 60  0001 C CNN
F 3 "" H 8050 4150 60  0000 C CNN
	1    8050 4150
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR017
U 1 1 5A25F4D9
P 7750 4650
F 0 "#PWR017" H 7750 4650 30  0001 C CNN
F 1 "GND" H 7750 4580 30  0001 C CNN
F 2 "" H 7750 4650 60  0001 C CNN
F 3 "" H 7750 4650 60  0001 C CNN
	1    7750 4650
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR018
U 1 1 5A25FBC4
P 8350 4000
F 0 "#PWR018" H 8350 4100 30  0001 C CNN
F 1 "VCC" H 8350 4100 30  0000 C CNN
F 2 "" H 8350 4000 60  0001 C CNN
F 3 "" H 8350 4000 60  0001 C CNN
	1    8350 4000
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR019
U 1 1 5A260972
P 8550 4000
F 0 "#PWR019" H 8550 4100 30  0001 C CNN
F 1 "VCC" H 8550 4100 30  0000 C CNN
F 2 "" H 8550 4000 60  0001 C CNN
F 3 "" H 8550 4000 60  0001 C CNN
	1    8550 4000
	1    0    0    -1  
$EndComp
NoConn ~ 8850 5600
NoConn ~ 8850 5700
Text GLabel 7850 5600 0    39   Input ~ 0
M-CS-TS
Text GLabel 3350 4000 0    39   Output ~ 0
M-CS-TS
Text GLabel 7850 5400 0    39   Output ~ 0
M-MISO
Text GLabel 7850 5500 0    39   Input ~ 0
M-MOSI
Text GLabel 7850 5700 0    39   Input ~ 0
M-CLK
$Comp
L R R7
U 1 1 5A257295
P 3800 4800
F 0 "R7" V 3800 4700 50  0000 C CNN
F 1 "82" V 3800 4900 50  0000 C CNN
F 2 "kicad-libraries:R0402F" H 3800 4800 60  0001 C CNN
F 3 "" H 3800 4800 60  0000 C CNN
	1    3800 4800
	0    1    1    0   
$EndComp
$Comp
L LCD-ERC12864 U3
U 1 1 5A254726
P 9200 1850
F 0 "U3" H 9250 3100 60  0000 C CNN
F 1 "LCD-ERC12864" H 9550 600 60  0000 C CNN
F 2 "kicad-libraries:ERC12864-655_128x64" H 9200 2250 60  0001 C CNN
F 3 "" H 9200 2250 60  0000 C CNN
	1    9200 1850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR020
U 1 1 5A25D56C
P 8700 1400
F 0 "#PWR020" H 8700 1400 30  0001 C CNN
F 1 "GND" H 8700 1330 30  0001 C CNN
F 2 "" H 8700 1400 60  0001 C CNN
F 3 "" H 8700 1400 60  0001 C CNN
	1    8700 1400
	1    0    0    -1  
$EndComp
Text GLabel 7850 5150 0    39   UnSpc ~ 0
TS-BUSY
Text GLabel 7850 5050 0    39   UnSpc ~ 0
PENIRQ
Text GLabel 3900 2750 0    39   UnSpc ~ 0
PENIRQ
Text GLabel 4000 3600 0    39   UnSpc ~ 0
TS-BUSY
Text GLabel 8850 800  0    39   Input ~ 0
LCD-RST
Text GLabel 4000 3800 0    39   Output ~ 0
LCD-RST
Text GLabel 8850 900  0    39   Input ~ 0
LCD-CD
Text GLabel 4000 3500 0    39   UnSpc ~ 0
LCD-CD
Wire Wire Line
	4000 3600 4150 3600
Wire Wire Line
	4000 3800 4150 3800
Wire Wire Line
	4000 3900 4150 3900
Wire Wire Line
	4000 3700 4150 3700
Wire Wire Line
	3050 4600 3050 5400
Wire Wire Line
	3050 5400 3100 5400
Wire Wire Line
	3550 5400 3500 5400
Wire Wire Line
	4050 5400 4150 5400
Wire Wire Line
	3950 5200 3950 5250
Wire Wire Line
	3900 5200 3950 5200
Wire Wire Line
	4150 5100 3900 5100
Connection ~ 4050 6850
Connection ~ 4050 6250
Connection ~ 4050 6750
Wire Wire Line
	4050 6850 4150 6850
Wire Wire Line
	4050 6700 4050 6950
Wire Wire Line
	4150 6750 4050 6750
Wire Wire Line
	4050 6250 4150 6250
Wire Wire Line
	4050 6200 4050 6300
Wire Wire Line
	1600 2500 1600 2450
Connection ~ 1600 2000
Wire Wire Line
	1600 2000 1600 2050
Wire Wire Line
	2200 2000 2550 2000
Wire Wire Line
	2200 1900 2550 1900
Wire Wire Line
	2200 1800 2550 1800
Wire Wire Line
	2200 1700 2550 1700
Wire Wire Line
	1450 2000 1800 2000
Wire Wire Line
	1450 1900 1800 1900
Wire Wire Line
	1800 1800 1450 1800
Wire Wire Line
	1450 1700 1800 1700
Connection ~ 2300 900 
Connection ~ 1850 900 
Wire Wire Line
	1850 950  1850 900 
Wire Wire Line
	2300 850  2300 950 
Wire Wire Line
	1650 900  2300 900 
Wire Wire Line
	1650 1600 1650 900 
Wire Wire Line
	1450 1600 1650 1600
Connection ~ 1850 1500
Wire Wire Line
	1850 1350 1850 1500
Connection ~ 2300 1500
Wire Wire Line
	2300 1350 2300 1550
Wire Wire Line
	1450 1500 2300 1500
Wire Wire Line
	1100 2150 1100 2500
Wire Wire Line
	8100 2450 8100 3550
Wire Wire Line
	9050 2100 8850 2100
Wire Wire Line
	8850 2500 9050 2500
Wire Wire Line
	9050 2600 8550 2600
Wire Wire Line
	8550 2200 9050 2200
Wire Wire Line
	8100 2000 9050 2000
Wire Wire Line
	8100 2000 8100 2050
Wire Wire Line
	8300 2050 8300 2000
Connection ~ 8300 2000
Wire Wire Line
	8300 2550 8300 3550
Wire Wire Line
	9700 2300 9600 2300
Wire Wire Line
	9700 2100 9700 2300
Wire Wire Line
	9700 2100 10500 2100
Wire Wire Line
	10500 2100 10500 1950
Wire Wire Line
	9600 2400 9700 2400
Wire Wire Line
	9700 2400 9700 2600
Wire Wire Line
	9700 2600 10450 2600
Wire Wire Line
	10450 2600 10450 3550
Wire Wire Line
	10000 2550 10000 2600
Connection ~ 10000 2600
Wire Wire Line
	10300 2550 10300 2600
Connection ~ 10300 2600
Wire Wire Line
	10000 2150 10000 2100
Connection ~ 10000 2100
Wire Wire Line
	10300 2150 10300 2100
Connection ~ 10300 2100
Wire Wire Line
	9050 3000 9000 3000
Wire Wire Line
	9000 3000 9000 3100
Wire Wire Line
	8500 2650 8500 2950
Wire Wire Line
	8500 3350 8500 3550
Wire Wire Line
	9000 3500 9000 3550
Wire Wire Line
	8700 3300 8650 3300
Wire Wire Line
	8650 3300 8650 3400
Wire Wire Line
	8850 700  9050 700 
Wire Wire Line
	8850 1800 9050 1800
Wire Wire Line
	4150 2650 3900 2650
Wire Wire Line
	8850 5050 9800 5050
Wire Wire Line
	9800 5150 8850 5150
Wire Wire Line
	8850 5250 9800 5250
Wire Wire Line
	9800 5350 8850 5350
Wire Wire Line
	8450 6100 8450 6350
Wire Wire Line
	8350 4000 8350 4550
Wire Wire Line
	8350 4450 8250 4450
Wire Wire Line
	8250 4150 8350 4150
Connection ~ 8350 4450
Wire Wire Line
	7850 4450 7750 4450
Wire Wire Line
	7750 4150 7750 4650
Wire Wire Line
	7850 4150 7750 4150
Connection ~ 7750 4450
Connection ~ 8350 4150
Wire Wire Line
	8450 4550 8450 4300
Wire Wire Line
	8450 4300 8350 4300
Connection ~ 8350 4300
Wire Wire Line
	8550 4550 8550 4000
Wire Wire Line
	7850 5400 8050 5400
Wire Wire Line
	8050 5500 7850 5500
Wire Wire Line
	7850 5600 8050 5600
Wire Wire Line
	8050 5700 7850 5700
Wire Notes Line
	11200 3850 6250 3850
Wire Notes Line
	6250 500  6250 7800
Wire Wire Line
	8950 1700 9050 1700
Wire Wire Line
	8950 1000 8950 1700
Wire Wire Line
	8950 1000 9050 1000
Wire Wire Line
	9050 1100 8950 1100
Connection ~ 8950 1100
Wire Wire Line
	9050 1200 8950 1200
Connection ~ 8950 1200
Wire Wire Line
	9050 1300 8950 1300
Connection ~ 8950 1300
Wire Wire Line
	9050 1400 8950 1400
Connection ~ 8950 1400
Wire Wire Line
	9050 1500 8950 1500
Connection ~ 8950 1500
Wire Wire Line
	9050 1600 8950 1600
Connection ~ 8950 1600
Wire Wire Line
	8950 1350 8700 1350
Wire Wire Line
	8700 1350 8700 1400
Connection ~ 8950 1350
Wire Wire Line
	7850 5050 8050 5050
Wire Wire Line
	8050 5150 7850 5150
Wire Wire Line
	4150 2750 3900 2750
Wire Wire Line
	3900 2850 4150 2850
Wire Wire Line
	4150 2950 3900 2950
Wire Wire Line
	9050 800  8850 800 
Wire Wire Line
	8850 900  9050 900 
Wire Wire Line
	3350 4000 3450 4000
Wire Wire Line
	3450 4100 3350 4100
Wire Wire Line
	3350 4200 3450 4200
Wire Wire Line
	3450 4300 3350 4300
Wire Wire Line
	3850 4300 4150 4300
Wire Wire Line
	4150 4200 3850 4200
Wire Wire Line
	4150 4000 3850 4000
Wire Wire Line
	4150 3500 4000 3500
Wire Wire Line
	3850 4100 4150 4100
Wire Wire Line
	9050 1900 8850 1900
Wire Wire Line
	4150 3400 3800 3400
$Comp
L R R8
U 1 1 5A298F20
P 3800 5400
F 0 "R8" V 3800 5500 50  0000 C CNN
F 1 "1k" V 3800 5300 50  0000 C CNN
F 2 "kicad-libraries:R0603F" H 3800 5400 60  0001 C CNN
F 3 "" H 3800 5400 60  0000 C CNN
	1    3800 5400
	0    1    1    0   
$EndComp
$Comp
L LED D2
U 1 1 5A299070
P 3300 5400
F 0 "D2" H 3300 5500 50  0000 C CNN
F 1 "green" H 3300 5300 50  0000 C CNN
F 2 "kicad-libraries:D0603E" H 3300 5400 50  0001 C CNN
F 3 "" H 3300 5400 50  0000 C CNN
	1    3300 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 4800 4050 4800
Wire Wire Line
	3550 4800 3500 4800
Wire Wire Line
	3500 4800 3500 4650
Wire Wire Line
	3500 4650 3450 4650
Wire Wire Line
	3550 4900 3500 4900
Wire Wire Line
	4050 4900 4150 4900
Wire Wire Line
	3100 4900 3050 4900
Connection ~ 3050 4900
Text Notes 5000 3750 0    39   ~ 0
SPI Slave\nP1.2 : USIC0_CH1-DOUT0 : MOSI\nP0.7 : USIC0_CH1-DX0D : MISO\nP1.3 : USIC0_CH1-SCLKOUT: CLK\nP0.9 : USIC0_CH1-DX2B : SEL\n
Text Notes 5000 4150 0    39   ~ 0
SPI Master\nP0.12 : USIC0_CH0-SELO3    : SEL\nP0.13 : USIC0_CH0-SELO4    : SEL\nP0.14 : USIC0_CH0-SCLKOUT : CLK\nP0.15 : USIC0_CH0-DX0B    : MISO\nP2.0  : USIC0_CH0-DOUT0   : MOSI
$Comp
L +5V #PWR021
U 1 1 5ABA5F3F
P 1500 850
F 0 "#PWR021" H 1500 700 50  0001 C CNN
F 1 "+5V" H 1500 950 30  0000 C CNN
F 2 "" H 1500 850 50  0000 C CNN
F 3 "" H 1500 850 50  0000 C CNN
	1    1500 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 850  1500 1400
Wire Wire Line
	1500 1400 1450 1400
$Comp
L +5V #PWR022
U 1 1 5ABA77E6
P 8500 2650
F 0 "#PWR022" H 8500 2500 50  0001 C CNN
F 1 "+5V" H 8500 2750 30  0000 C CNN
F 2 "" H 8500 2650 50  0000 C CNN
F 3 "" H 8500 2650 50  0000 C CNN
	1    8500 2650
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 5ABA9079
P 8800 2900
F 0 "R9" V 8880 2900 50  0000 C CNN
F 1 "27|22" V 8800 2900 50  0000 C CNN
F 2 "R1206" H 8800 2900 60  0001 C CNN
F 3 "" H 8800 2900 60  0000 C CNN
	1    8800 2900
	0    1    1    0   
$EndComp
Wire Wire Line
	8550 2900 8500 2900
Connection ~ 8500 2900
Wire Wire Line
	8950 2750 9000 2750
Wire Wire Line
	9000 2700 9000 2800
Wire Wire Line
	9000 2700 9050 2700
Wire Wire Line
	9000 2800 9050 2800
Connection ~ 9000 2750
$EndSCHEMATC
