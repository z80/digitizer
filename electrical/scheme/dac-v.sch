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
LIBS:my_comps
LIBS:stm32f103cbt6
LIBS:stm32f100rct6b
LIBS:ncp3063
LIBS:crystal-4pin
LIBS:cp2102
LIBS:mcp1703t-3302e
LIBS:mini-usb-b
LIBS:ad7683
LIBS:ad8610
LIBS:adg5204
LIBS:ad8620
LIBS:ad5543
LIBS:bat54s
LIBS:digitizer-cache
EELAYER 25 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 7 7
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
L AD5543 U?
U 1 1 55A0AE22
P 7650 8100
AR Path="/55A0A529/55A0AE22" Ref="U?"  Part="1" 
AR Path="/55A5B809/55A0AE22" Ref="U?"  Part="1" 
F 0 "U?" H 8400 7900 60  0000 C CNN
F 1 "AD5543" H 8500 8350 60  0000 C CNN
F 2 "" H 7650 8100 60  0000 C CNN
F 3 "" H 7650 8100 60  0000 C CNN
	1    7650 8100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 7900 6300 7900
Wire Wire Line
	7150 8100 6300 8100
Wire Wire Line
	7150 8300 6300 8300
Wire Wire Line
	7600 7350 7600 7650
Wire Wire Line
	7600 7450 7450 7450
Connection ~ 7600 7450
$Comp
L C C?
U 1 1 55A0AE2F
P 7300 7450
AR Path="/55A0A529/55A0AE2F" Ref="C?"  Part="1" 
AR Path="/55A5B809/55A0AE2F" Ref="C?"  Part="1" 
F 0 "C?" H 7325 7550 50  0000 L CNN
F 1 "100n" H 7325 7350 50  0000 L CNN
F 2 "" H 7338 7300 30  0000 C CNN
F 3 "" H 7300 7450 60  0000 C CNN
	1    7300 7450
	0    1    1    0   
$EndComp
Wire Wire Line
	7150 7450 7050 7450
Wire Wire Line
	7050 7450 7050 7550
$Comp
L VDD #PWR?
U 1 1 55A0AE38
P 7600 7350
AR Path="/55A0A529/55A0AE38" Ref="#PWR?"  Part="1" 
AR Path="/55A5B809/55A0AE38" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7600 7450 30  0001 C CNN
F 1 "VDD" H 7600 7460 30  0000 C CNN
F 2 "" H 7600 7350 60  0000 C CNN
F 3 "" H 7600 7350 60  0000 C CNN
	1    7600 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 6000 7800 7650
$Comp
L AD8620 U?
U 2 1 55A0AE3F
P 9400 8300
AR Path="/55A0A529/55A0AE3F" Ref="U?"  Part="2" 
AR Path="/55A5B809/55A0AE3F" Ref="U?"  Part="2" 
F 0 "U?" H 9750 8100 60  0000 C CNN
F 1 "AD8620" H 9750 8500 60  0000 C CNN
F 2 "" H 9450 8300 60  0000 C CNN
F 3 "" H 9450 8300 60  0000 C CNN
	2    9400 8300
	1    0    0    1   
$EndComp
Wire Wire Line
	8950 8100 8500 8100
Wire Wire Line
	8950 8500 8850 8500
Wire Wire Line
	8850 8500 8850 8600
$Comp
L GNDA #PWR?
U 1 1 55A0AE49
P 8850 8600
AR Path="/55A0A529/55A0AE49" Ref="#PWR?"  Part="1" 
AR Path="/55A5B809/55A0AE49" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8850 8350 50  0001 C CNN
F 1 "GNDA" H 8850 8450 50  0000 C CNN
F 2 "" H 8850 8600 60  0000 C CNN
F 3 "" H 8850 8600 60  0000 C CNN
	1    8850 8600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 8550 7700 8750
Wire Wire Line
	9950 8300 10450 8300
Wire Wire Line
	10100 6600 10100 8300
Wire Wire Line
	8100 6600 8100 7650
Connection ~ 10100 8300
$Comp
L C C?
U 1 1 55A0AE5A
P 8800 7800
AR Path="/55A0A529/55A0AE5A" Ref="C?"  Part="1" 
AR Path="/55A5B809/55A0AE5A" Ref="C?"  Part="1" 
F 0 "C?" H 8825 7900 50  0000 L CNN
F 1 "1n" H 8825 7700 50  0000 L CNN
F 2 "" H 8838 7650 30  0000 C CNN
F 3 "" H 8800 7800 60  0000 C CNN
	1    8800 7800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 6600 8800 7650
Wire Wire Line
	8800 7950 8800 8100
Connection ~ 8800 8100
Wire Wire Line
	8100 6600 10100 6600
Connection ~ 8800 6600
Text Label 6350 7900 0    60   ~ 0
dac_sck
Text Label 6350 8100 0    60   ~ 0
dac_mosi
$Comp
L C C?
U 1 1 55A0AE9E
P 7500 6500
AR Path="/55A0A529/55A0AE9E" Ref="C?"  Part="1" 
AR Path="/55A5B809/55A0AE9E" Ref="C?"  Part="1" 
F 0 "C?" H 7525 6600 50  0000 L CNN
F 1 "100n" H 7525 6400 50  0000 L CNN
F 2 "" H 7538 6350 30  0000 C CNN
F 3 "" H 7500 6500 60  0000 C CNN
	1    7500 6500
	0    1    1    0   
$EndComp
Wire Wire Line
	7650 6500 7800 6500
Connection ~ 7800 6500
Wire Wire Line
	7350 6500 7150 6500
Wire Wire Line
	7150 6500 7150 6700
$Comp
L GNDA #PWR?
U 1 1 55A0AEA9
P 7150 6700
AR Path="/55A0A529/55A0AEA9" Ref="#PWR?"  Part="1" 
AR Path="/55A5B809/55A0AEA9" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7150 6450 50  0001 C CNN
F 1 "GNDA" H 7150 6550 50  0000 C CNN
F 2 "" H 7150 6700 60  0000 C CNN
F 3 "" H 7150 6700 60  0000 C CNN
	1    7150 6700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55A0AEAF
P 7050 7550
AR Path="/55A0A529/55A0AEAF" Ref="#PWR?"  Part="1" 
AR Path="/55A5B809/55A0AEAF" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7050 7300 50  0001 C CNN
F 1 "GND" H 7050 7400 50  0000 C CNN
F 2 "" H 7050 7550 60  0000 C CNN
F 3 "" H 7050 7550 60  0000 C CNN
	1    7050 7550
	1    0    0    -1  
$EndComp
Text HLabel 6300 8300 0    60   Input ~ 0
cs-2
Text HLabel 10450 8300 2    60   Input ~ 0
out-2
$Comp
L AD5543 U?
U 1 1 55A0B77B
P 7950 3800
AR Path="/55A0A529/55A0B77B" Ref="U?"  Part="1" 
AR Path="/55A5B809/55A0B77B" Ref="U?"  Part="1" 
F 0 "U?" H 8700 3600 60  0000 C CNN
F 1 "AD5543" H 8800 4050 60  0000 C CNN
F 2 "" H 7950 3800 60  0000 C CNN
F 3 "" H 7950 3800 60  0000 C CNN
	1    7950 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 3600 6600 3600
Wire Wire Line
	7450 3800 6600 3800
Wire Wire Line
	7450 4000 6600 4000
Wire Wire Line
	7900 3050 7900 3350
Wire Wire Line
	7900 3150 7750 3150
Connection ~ 7900 3150
$Comp
L C C?
U 1 1 55A0B787
P 7600 3150
AR Path="/55A0A529/55A0B787" Ref="C?"  Part="1" 
AR Path="/55A5B809/55A0B787" Ref="C?"  Part="1" 
F 0 "C?" H 7625 3250 50  0000 L CNN
F 1 "100n" H 7625 3050 50  0000 L CNN
F 2 "" H 7638 3000 30  0000 C CNN
F 3 "" H 7600 3150 60  0000 C CNN
	1    7600 3150
	0    1    1    0   
$EndComp
Wire Wire Line
	7450 3150 7350 3150
Wire Wire Line
	7350 3150 7350 3250
$Comp
L VDD #PWR?
U 1 1 55A0B78F
P 7900 3050
AR Path="/55A0A529/55A0B78F" Ref="#PWR?"  Part="1" 
AR Path="/55A5B809/55A0B78F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7900 3150 30  0001 C CNN
F 1 "VDD" H 7900 3160 30  0000 C CNN
F 2 "" H 7900 3050 60  0000 C CNN
F 3 "" H 7900 3050 60  0000 C CNN
	1    7900 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 1700 8100 3350
$Comp
L AD8620 U?
U 1 1 55A0B796
P 9700 4000
AR Path="/55A0A529/55A0B796" Ref="U?"  Part="1" 
AR Path="/55A5B809/55A0B796" Ref="U?"  Part="1" 
F 0 "U?" H 10050 3800 60  0000 C CNN
F 1 "AD8620" H 10050 4200 60  0000 C CNN
F 2 "" H 9750 4000 60  0000 C CNN
F 3 "" H 9750 4000 60  0000 C CNN
	1    9700 4000
	1    0    0    1   
$EndComp
Wire Wire Line
	9250 3800 8800 3800
Wire Wire Line
	9250 4200 9150 4200
Wire Wire Line
	9150 4200 9150 4300
$Comp
L GNDA #PWR?
U 1 1 55A0B79F
P 9150 4300
AR Path="/55A0A529/55A0B79F" Ref="#PWR?"  Part="1" 
AR Path="/55A5B809/55A0B79F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 9150 4050 50  0001 C CNN
F 1 "GNDA" H 9150 4150 50  0000 C CNN
F 2 "" H 9150 4300 60  0000 C CNN
F 3 "" H 9150 4300 60  0000 C CNN
	1    9150 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 4250 8000 4450
Wire Wire Line
	10250 4000 10750 4000
Wire Wire Line
	10400 2300 10400 4000
Wire Wire Line
	8400 2300 8400 3350
Connection ~ 10400 4000
$Comp
L C C?
U 1 1 55A0B7B0
P 9100 3500
AR Path="/55A0A529/55A0B7B0" Ref="C?"  Part="1" 
AR Path="/55A5B809/55A0B7B0" Ref="C?"  Part="1" 
F 0 "C?" H 9125 3600 50  0000 L CNN
F 1 "1n" H 9125 3400 50  0000 L CNN
F 2 "" H 9138 3350 30  0000 C CNN
F 3 "" H 9100 3500 60  0000 C CNN
	1    9100 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 2300 9100 3350
Wire Wire Line
	9100 3650 9100 3800
Connection ~ 9100 3800
Wire Wire Line
	8400 2300 10400 2300
Connection ~ 9100 2300
Wire Wire Line
	9700 3600 9700 3250
$Comp
L R R?
U 1 1 55A0B7BC
P 9700 3100
AR Path="/55A0A529/55A0B7BC" Ref="R?"  Part="1" 
AR Path="/55A5B809/55A0B7BC" Ref="R?"  Part="1" 
F 0 "R?" V 9780 3100 50  0000 C CNN
F 1 "16 5%" V 9700 3100 50  0000 C CNN
F 2 "" V 9630 3100 30  0000 C CNN
F 3 "" H 9700 3100 30  0000 C CNN
	1    9700 3100
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 55A0B7C2
P 9950 3350
AR Path="/55A0A529/55A0B7C2" Ref="C?"  Part="1" 
AR Path="/55A5B809/55A0B7C2" Ref="C?"  Part="1" 
F 0 "C?" H 9975 3450 50  0000 L CNN
F 1 "C" H 9975 3250 50  0000 L CNN
F 2 "" H 9988 3200 30  0000 C CNN
F 3 "" H 9950 3350 60  0000 C CNN
	1    9950 3350
	0    1    1    0   
$EndComp
Wire Wire Line
	9800 3350 9700 3350
Connection ~ 9700 3350
Wire Wire Line
	10100 3350 10200 3350
Wire Wire Line
	10200 3350 10200 3450
$Comp
L GNDA #PWR?
U 1 1 55A0B7CC
P 10200 3450
AR Path="/55A0A529/55A0B7CC" Ref="#PWR?"  Part="1" 
AR Path="/55A5B809/55A0B7CC" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10200 3200 50  0001 C CNN
F 1 "GNDA" H 10200 3300 50  0000 C CNN
F 2 "" H 10200 3450 60  0000 C CNN
F 3 "" H 10200 3450 60  0000 C CNN
	1    10200 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 2950 9700 2650
Text Label 9700 2800 1    60   ~ 0
v-
Wire Wire Line
	9700 4400 9700 4650
$Comp
L R R?
U 1 1 55A0B7D5
P 9700 4800
AR Path="/55A0A529/55A0B7D5" Ref="R?"  Part="1" 
AR Path="/55A5B809/55A0B7D5" Ref="R?"  Part="1" 
F 0 "R?" V 9780 4800 50  0000 C CNN
F 1 "16 5%" V 9700 4800 50  0000 C CNN
F 2 "" V 9630 4800 30  0000 C CNN
F 3 "" H 9700 4800 30  0000 C CNN
	1    9700 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 4950 9700 5250
Text Label 9700 5200 1    60   ~ 0
v+
Wire Wire Line
	9700 4550 10000 4550
Wire Wire Line
	10000 4550 10000 4650
Connection ~ 9700 4550
$Comp
L C C?
U 1 1 55A0B7E0
P 10000 4800
AR Path="/55A0A529/55A0B7E0" Ref="C?"  Part="1" 
AR Path="/55A5B809/55A0B7E0" Ref="C?"  Part="1" 
F 0 "C?" H 10025 4900 50  0000 L CNN
F 1 "100n" H 10025 4700 50  0000 L CNN
F 2 "" H 10038 4650 30  0000 C CNN
F 3 "" H 10000 4800 60  0000 C CNN
	1    10000 4800
	-1   0    0    1   
$EndComp
Wire Wire Line
	10000 4950 10000 5100
$Comp
L GNDA #PWR?
U 1 1 55A0B7E7
P 10000 5100
AR Path="/55A0A529/55A0B7E7" Ref="#PWR?"  Part="1" 
AR Path="/55A5B809/55A0B7E7" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 10000 4850 50  0001 C CNN
F 1 "GNDA" H 10000 4950 50  0000 C CNN
F 2 "" H 10000 5100 60  0000 C CNN
F 3 "" H 10000 5100 60  0000 C CNN
	1    10000 5100
	1    0    0    -1  
$EndComp
Text Label 6650 3600 0    60   ~ 0
dac_sck
Text Label 6650 3800 0    60   ~ 0
dac_mosi
$Comp
L C C?
U 1 1 55A0B7EF
P 7800 2200
AR Path="/55A0A529/55A0B7EF" Ref="C?"  Part="1" 
AR Path="/55A5B809/55A0B7EF" Ref="C?"  Part="1" 
F 0 "C?" H 7825 2300 50  0000 L CNN
F 1 "100n" H 7825 2100 50  0000 L CNN
F 2 "" H 7838 2050 30  0000 C CNN
F 3 "" H 7800 2200 60  0000 C CNN
	1    7800 2200
	0    1    1    0   
$EndComp
Wire Wire Line
	7950 2200 8100 2200
Connection ~ 8100 2200
Wire Wire Line
	7650 2200 7450 2200
Wire Wire Line
	7450 2200 7450 2400
$Comp
L GNDA #PWR?
U 1 1 55A0B7F9
P 7450 2400
AR Path="/55A0A529/55A0B7F9" Ref="#PWR?"  Part="1" 
AR Path="/55A5B809/55A0B7F9" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7450 2150 50  0001 C CNN
F 1 "GNDA" H 7450 2250 50  0000 C CNN
F 2 "" H 7450 2400 60  0000 C CNN
F 3 "" H 7450 2400 60  0000 C CNN
	1    7450 2400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55A0B7FF
P 7350 3250
AR Path="/55A0A529/55A0B7FF" Ref="#PWR?"  Part="1" 
AR Path="/55A5B809/55A0B7FF" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7350 3000 50  0001 C CNN
F 1 "GND" H 7350 3100 50  0000 C CNN
F 2 "" H 7350 3250 60  0000 C CNN
F 3 "" H 7350 3250 60  0000 C CNN
	1    7350 3250
	1    0    0    -1  
$EndComp
Text Label 8100 2050 1    60   ~ 0
dac-ref
Text HLabel 9700 2650 1    60   Input ~ 0
v-
Text HLabel 9700 5250 3    60   Input ~ 0
v+
Text HLabel 6600 3600 0    60   Input ~ 0
sck
Text HLabel 6600 3800 0    60   Input ~ 0
mosi
Text HLabel 6600 4000 0    60   Input ~ 0
cs-1
Text HLabel 10750 4000 2    60   Input ~ 0
out-1
$Comp
L GNDA #PWR?
U 1 1 55A0D1BD
P 7700 8750
AR Path="/55A0A529/55A0D1BD" Ref="#PWR?"  Part="1" 
AR Path="/55A5B809/55A0D1BD" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7700 8500 50  0001 C CNN
F 1 "GNDA" H 7700 8600 50  0000 C CNN
F 2 "" H 7700 8750 60  0000 C CNN
F 3 "" H 7700 8750 60  0000 C CNN
	1    7700 8750
	1    0    0    -1  
$EndComp
Wire Notes Line
	7900 8850 8200 9250
Text Notes 7500 9350 0    60   ~ 0
Here is "agnd" according to datasheet.
$Comp
L GNDA #PWR?
U 1 1 55A0D66F
P 8000 4450
AR Path="/55A0A529/55A0D66F" Ref="#PWR?"  Part="1" 
AR Path="/55A5B809/55A0D66F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8000 4200 50  0001 C CNN
F 1 "GNDA" H 8000 4300 50  0000 C CNN
F 2 "" H 8000 4450 60  0000 C CNN
F 3 "" H 8000 4450 60  0000 C CNN
	1    8000 4450
	1    0    0    -1  
$EndComp
Wire Notes Line
	8150 4650 8450 5050
Text Notes 7750 5150 0    60   ~ 0
Here is "agnd" according to datasheet.
Text HLabel 7800 6000 1    60   Input ~ 0
dac-ref
Text Label 7800 6400 1    60   ~ 0
dac-ref
$EndSCHEMATC
