EESchema Schematic File Version 2
LIBS:digitizer-rescue
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
LIBS:ref196
LIBS:adr4533
LIBS:adt7410
LIBS:digitizer-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 7
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
L AD8620 U4
U 1 1 55A8C542
P 5250 3850
AR Path="/55A8BFF5/55A8C542" Ref="U4"  Part="1" 
AR Path="/55AAF91E/55A8C542" Ref="U5"  Part="1" 
AR Path="/55AAFE8D/55A8C542" Ref="U6"  Part="1" 
AR Path="/55AAFE9C/55A8C542" Ref="U7"  Part="1" 
F 0 "U7" H 5600 3650 60  0000 C CNN
F 1 "AD8672" H 5600 4050 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 5300 3850 60  0001 C CNN
F 3 "" H 5300 3850 60  0000 C CNN
	1    5250 3850
	1    0    0    -1  
$EndComp
$Comp
L AD8620 U4
U 2 1 55A8C549
P 8350 4050
AR Path="/55A8BFF5/55A8C549" Ref="U4"  Part="2" 
AR Path="/55AAF91E/55A8C549" Ref="U5"  Part="2" 
AR Path="/55AAFE8D/55A8C549" Ref="U6"  Part="2" 
AR Path="/55AAFE9C/55A8C549" Ref="U7"  Part="2" 
F 0 "U7" H 8700 3850 60  0000 C CNN
F 1 "AD8672" H 8700 4250 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 8400 4050 60  0001 C CNN
F 3 "" H 8400 4050 60  0000 C CNN
	2    8350 4050
	1    0    0    -1  
$EndComp
$Comp
L R R45
U 1 1 55A8C74B
P 5250 2800
AR Path="/55A8BFF5/55A8C74B" Ref="R45"  Part="1" 
AR Path="/55AAF91E/55A8C74B" Ref="R51"  Part="1" 
AR Path="/55AAFE8D/55A8C74B" Ref="R57"  Part="1" 
AR Path="/55AAFE9C/55A8C74B" Ref="R63"  Part="1" 
F 0 "R63" V 5330 2800 50  0000 C CNN
F 1 "16 5%" V 5250 2800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5180 2800 30  0001 C CNN
F 3 "" H 5250 2800 30  0000 C CNN
	1    5250 2800
	1    0    0    -1  
$EndComp
$Comp
L C C42
U 1 1 55A8C77D
P 5000 3050
AR Path="/55A8BFF5/55A8C77D" Ref="C42"  Part="1" 
AR Path="/55AAF91E/55A8C77D" Ref="C48"  Part="1" 
AR Path="/55AAFE8D/55A8C77D" Ref="C54"  Part="1" 
AR Path="/55AAFE9C/55A8C77D" Ref="C60"  Part="1" 
F 0 "C60" H 5025 3150 50  0000 L CNN
F 1 "100n" H 5025 2950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5038 2900 30  0001 C CNN
F 3 "" H 5000 3050 60  0000 C CNN
	1    5000 3050
	0    1    1    0   
$EndComp
Wire Wire Line
	5250 2950 5250 3450
Wire Wire Line
	5150 3050 5600 3050
Connection ~ 5250 3050
Wire Wire Line
	4850 3050 4650 3050
Wire Wire Line
	4650 3050 4650 3150
$Comp
L GNDA #PWR0131
U 1 1 55A8C85F
P 4650 3150
AR Path="/55A8BFF5/55A8C85F" Ref="#PWR0131"  Part="1" 
AR Path="/55AAF91E/55A8C85F" Ref="#PWR0137"  Part="1" 
AR Path="/55AAFE8D/55A8C85F" Ref="#PWR0143"  Part="1" 
AR Path="/55AAFE9C/55A8C85F" Ref="#PWR0149"  Part="1" 
F 0 "#PWR0149" H 4650 2900 50  0001 C CNN
F 1 "GNDA" H 4650 3000 50  0000 C CNN
F 2 "" H 4650 3150 60  0000 C CNN
F 3 "" H 4650 3150 60  0000 C CNN
	1    4650 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 2650 5250 2300
Text Label 5250 2450 1    60   ~ 0
v+
Wire Wire Line
	5250 4250 5250 4650
$Comp
L R R46
U 1 1 55A8C8A7
P 5250 4800
AR Path="/55A8BFF5/55A8C8A7" Ref="R46"  Part="1" 
AR Path="/55AAF91E/55A8C8A7" Ref="R52"  Part="1" 
AR Path="/55AAFE8D/55A8C8A7" Ref="R58"  Part="1" 
AR Path="/55AAFE9C/55A8C8A7" Ref="R64"  Part="1" 
F 0 "R64" V 5330 4800 50  0000 C CNN
F 1 "16 5%" V 5250 4800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5180 4800 30  0001 C CNN
F 3 "" H 5250 4800 30  0000 C CNN
	1    5250 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 4950 5250 5250
Text Label 5250 5200 1    60   ~ 0
v-
$Comp
L C C43
U 1 1 55A8CC9D
P 5000 4550
AR Path="/55A8BFF5/55A8CC9D" Ref="C43"  Part="1" 
AR Path="/55AAF91E/55A8CC9D" Ref="C49"  Part="1" 
AR Path="/55AAFE8D/55A8CC9D" Ref="C55"  Part="1" 
AR Path="/55AAFE9C/55A8CC9D" Ref="C61"  Part="1" 
F 0 "C61" H 5025 4650 50  0000 L CNN
F 1 "100n" H 5025 4450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5038 4400 30  0001 C CNN
F 3 "" H 5000 4550 60  0000 C CNN
	1    5000 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	4850 4550 4650 4550
Wire Wire Line
	4650 4550 4650 4650
$Comp
L GNDA #PWR0132
U 1 1 55A8CCA5
P 4650 4650
AR Path="/55A8BFF5/55A8CCA5" Ref="#PWR0132"  Part="1" 
AR Path="/55AAF91E/55A8CCA5" Ref="#PWR0138"  Part="1" 
AR Path="/55AAFE8D/55A8CCA5" Ref="#PWR0144"  Part="1" 
AR Path="/55AAFE9C/55A8CCA5" Ref="#PWR0150"  Part="1" 
F 0 "#PWR0150" H 4650 4400 50  0001 C CNN
F 1 "GNDA" H 4650 4500 50  0000 C CNN
F 2 "" H 4650 4650 60  0000 C CNN
F 3 "" H 4650 4650 60  0000 C CNN
	1    4650 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 4550 5600 4550
Connection ~ 5250 4550
$Comp
L R R43
U 1 1 55A8D6B2
P 3100 3650
AR Path="/55A8BFF5/55A8D6B2" Ref="R43"  Part="1" 
AR Path="/55AAF91E/55A8D6B2" Ref="R49"  Part="1" 
AR Path="/55AAFE8D/55A8D6B2" Ref="R55"  Part="1" 
AR Path="/55AAFE9C/55A8D6B2" Ref="R61"  Part="1" 
F 0 "R61" V 3180 3650 50  0000 C CNN
F 1 "806 1%" V 3100 3650 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3030 3650 30  0001 C CNN
F 3 "" H 3100 3650 30  0000 C CNN
	1    3100 3650
	0    1    1    0   
$EndComp
$Comp
L R R44
U 1 1 55A8D707
P 3600 3650
AR Path="/55A8BFF5/55A8D707" Ref="R44"  Part="1" 
AR Path="/55AAF91E/55A8D707" Ref="R50"  Part="1" 
AR Path="/55AAFE8D/55A8D707" Ref="R56"  Part="1" 
AR Path="/55AAFE9C/55A8D707" Ref="R62"  Part="1" 
F 0 "R62" V 3680 3650 50  0000 C CNN
F 1 "1k91 1%" V 3600 3650 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3530 3650 30  0001 C CNN
F 3 "" H 3600 3650 30  0000 C CNN
	1    3600 3650
	0    1    1    0   
$EndComp
$Comp
L C C41
U 1 1 55A8D733
P 3850 3900
AR Path="/55A8BFF5/55A8D733" Ref="C41"  Part="1" 
AR Path="/55AAF91E/55A8D733" Ref="C47"  Part="1" 
AR Path="/55AAFE8D/55A8D733" Ref="C53"  Part="1" 
AR Path="/55AAFE9C/55A8D733" Ref="C59"  Part="1" 
F 0 "C59" H 3875 4000 50  0000 L CNN
F 1 "22n" H 3875 3800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3888 3750 30  0001 C CNN
F 3 "" H 3850 3900 60  0000 C CNN
	1    3850 3900
	1    0    0    -1  
$EndComp
$Comp
L C C40
U 1 1 55A8D76D
P 3350 3350
AR Path="/55A8BFF5/55A8D76D" Ref="C40"  Part="1" 
AR Path="/55AAF91E/55A8D76D" Ref="C46"  Part="1" 
AR Path="/55AAFE8D/55A8D76D" Ref="C52"  Part="1" 
AR Path="/55AAFE9C/55A8D76D" Ref="C58"  Part="1" 
F 0 "C58" H 3375 3450 50  0000 L CNN
F 1 "3u3" H 3375 3250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3388 3200 30  0001 C CNN
F 3 "" H 3350 3350 60  0000 C CNN
	1    3350 3350
	-1   0    0    1   
$EndComp
$Comp
L R R47
U 1 1 55A8D8D4
P 6200 3850
AR Path="/55A8BFF5/55A8D8D4" Ref="R47"  Part="1" 
AR Path="/55AAF91E/55A8D8D4" Ref="R53"  Part="1" 
AR Path="/55AAFE8D/55A8D8D4" Ref="R59"  Part="1" 
AR Path="/55AAFE9C/55A8D8D4" Ref="R65"  Part="1" 
F 0 "R65" V 6280 3850 50  0000 C CNN
F 1 "9k31 1%" V 6200 3850 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6130 3850 30  0001 C CNN
F 3 "" H 6200 3850 30  0000 C CNN
	1    6200 3850
	0    1    1    0   
$EndComp
$Comp
L R R48
U 1 1 55A8DA7D
P 6700 3850
AR Path="/55A8BFF5/55A8DA7D" Ref="R48"  Part="1" 
AR Path="/55AAF91E/55A8DA7D" Ref="R54"  Part="1" 
AR Path="/55AAFE8D/55A8DA7D" Ref="R60"  Part="1" 
AR Path="/55AAFE9C/55A8DA7D" Ref="R66"  Part="1" 
F 0 "R66" V 6780 3850 50  0000 C CNN
F 1 "21k 1%" V 6700 3850 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6630 3850 30  0001 C CNN
F 3 "" H 6700 3850 30  0000 C CNN
	1    6700 3850
	0    1    1    0   
$EndComp
$Comp
L C C44
U 1 1 55A8DD2D
P 6450 3600
AR Path="/55A8BFF5/55A8DD2D" Ref="C44"  Part="1" 
AR Path="/55AAF91E/55A8DD2D" Ref="C50"  Part="1" 
AR Path="/55AAFE8D/55A8DD2D" Ref="C56"  Part="1" 
AR Path="/55AAFE9C/55A8DD2D" Ref="C62"  Part="1" 
F 0 "C62" H 6475 3700 50  0000 L CNN
F 1 "120n" H 6475 3500 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6488 3450 30  0001 C CNN
F 3 "" H 6450 3600 60  0000 C CNN
	1    6450 3600
	-1   0    0    1   
$EndComp
$Comp
L C C45
U 1 1 55A8DF47
P 7050 4200
AR Path="/55A8BFF5/55A8DF47" Ref="C45"  Part="1" 
AR Path="/55AAF91E/55A8DF47" Ref="C51"  Part="1" 
AR Path="/55AAFE8D/55A8DF47" Ref="C57"  Part="1" 
AR Path="/55AAFE9C/55A8DF47" Ref="C63"  Part="1" 
F 0 "C63" H 7075 4300 50  0000 L CNN
F 1 "22n" H 7075 4100 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7088 4050 30  0001 C CNN
F 3 "" H 7050 4200 60  0000 C CNN
	1    7050 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 3650 4800 3650
Wire Wire Line
	3850 3750 3850 3650
Connection ~ 3850 3650
Wire Wire Line
	3450 3650 3250 3650
Wire Wire Line
	3350 3500 3350 3650
Connection ~ 3350 3650
Wire Wire Line
	2950 3650 2500 3650
Wire Wire Line
	5800 3850 6050 3850
Wire Wire Line
	6350 3850 6550 3850
Wire Wire Line
	6450 3750 6450 3850
Connection ~ 6450 3850
Wire Wire Line
	6850 3850 7900 3850
Wire Wire Line
	7050 4050 7050 3850
Connection ~ 7050 3850
Wire Wire Line
	7050 4350 7050 4550
Wire Wire Line
	3850 4050 3850 4250
Wire Wire Line
	5900 1950 5900 5450
Wire Wire Line
	5900 1950 3350 1950
Wire Wire Line
	3350 1950 3350 3200
Connection ~ 5900 3850
Wire Wire Line
	8900 4050 9700 4050
Connection ~ 9100 4050
$Comp
L GNDA #PWR0133
U 1 1 55A8F5C0
P 7050 4550
AR Path="/55A8BFF5/55A8F5C0" Ref="#PWR0133"  Part="1" 
AR Path="/55AAF91E/55A8F5C0" Ref="#PWR0139"  Part="1" 
AR Path="/55AAFE8D/55A8F5C0" Ref="#PWR0145"  Part="1" 
AR Path="/55AAFE9C/55A8F5C0" Ref="#PWR0151"  Part="1" 
F 0 "#PWR0151" H 7050 4300 50  0001 C CNN
F 1 "GNDA" H 7050 4400 50  0000 C CNN
F 2 "" H 7050 4550 60  0000 C CNN
F 3 "" H 7050 4550 60  0000 C CNN
	1    7050 4550
	1    0    0    -1  
$EndComp
$Comp
L GNDA #PWR0134
U 1 1 55A8F5EF
P 3850 4250
AR Path="/55A8BFF5/55A8F5EF" Ref="#PWR0134"  Part="1" 
AR Path="/55AAF91E/55A8F5EF" Ref="#PWR0140"  Part="1" 
AR Path="/55AAFE8D/55A8F5EF" Ref="#PWR0146"  Part="1" 
AR Path="/55AAFE9C/55A8F5EF" Ref="#PWR0152"  Part="1" 
F 0 "#PWR0152" H 3850 4000 50  0001 C CNN
F 1 "GNDA" H 3850 4100 50  0000 C CNN
F 2 "" H 3850 4250 60  0000 C CNN
F 3 "" H 3850 4250 60  0000 C CNN
	1    3850 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 2400 2400 2400
Text HLabel 2500 3650 0    60   Input ~ 0
in
Text HLabel 9700 4050 2    60   Input ~ 0
out
Text HLabel 2150 2400 0    60   Input ~ 0
v+
Text Label 2300 2400 0    60   ~ 0
v+
Wire Wire Line
	2150 2600 2400 2600
Text HLabel 2150 2600 0    60   Input ~ 0
v-
Text Label 2300 2600 0    60   ~ 0
v-
Wire Wire Line
	4800 4050 4300 4050
Wire Wire Line
	4300 4050 4300 5450
Wire Wire Line
	4300 5450 5900 5450
Wire Wire Line
	9100 3300 9100 4700
Wire Wire Line
	9100 3300 6450 3300
Wire Wire Line
	6450 3300 6450 3450
Wire Wire Line
	9100 4700 7700 4700
Wire Wire Line
	7700 4700 7700 4250
Wire Wire Line
	7700 4250 7900 4250
$Comp
L PWR_FLAG #FLG0135
U 1 1 55BB3EFD
P 5600 2950
AR Path="/55A8BFF5/55BB3EFD" Ref="#FLG0135"  Part="1" 
AR Path="/55AAF91E/55BB3EFD" Ref="#FLG0141"  Part="1" 
AR Path="/55AAFE8D/55BB3EFD" Ref="#FLG0147"  Part="1" 
AR Path="/55AAFE9C/55BB3EFD" Ref="#FLG0153"  Part="1" 
F 0 "#FLG0153" H 5600 3045 50  0001 C CNN
F 1 "PWR_FLAG" H 5600 3130 50  0000 C CNN
F 2 "" H 5600 2950 60  0000 C CNN
F 3 "" H 5600 2950 60  0000 C CNN
	1    5600 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 3050 5600 2950
$Comp
L PWR_FLAG #FLG0142
U 1 1 55BB3F68
P 5600 4650
AR Path="/55AAF91E/55BB3F68" Ref="#FLG0142"  Part="1" 
AR Path="/55AAFE8D/55BB3F68" Ref="#FLG0148"  Part="1" 
AR Path="/55A8BFF5/55BB3F68" Ref="#FLG0136"  Part="1" 
AR Path="/55AAFE9C/55BB3F68" Ref="#FLG0154"  Part="1" 
F 0 "#FLG0154" H 5600 4745 50  0001 C CNN
F 1 "PWR_FLAG" H 5600 4830 50  0000 C CNN
F 2 "" H 5600 4650 60  0000 C CNN
F 3 "" H 5600 4650 60  0000 C CNN
	1    5600 4650
	-1   0    0    1   
$EndComp
Wire Wire Line
	5600 4550 5600 4650
$EndSCHEMATC
