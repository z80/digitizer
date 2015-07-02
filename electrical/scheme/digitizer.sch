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
LIBS:digitizer-cache
EELAYER 25 0
EELAYER END
$Descr A1 33110 23386
encoding utf-8
Sheet 1 1
Title ""
Date "12 jan 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L STM32F103CBT6 stm32?
U 1 1 55963EB4
P -10400 -6650
F 0 "stm32?" H -10400 -6850 60  0000 C CNN
F 1 "STM32F103CBT6" H -10400 -6450 60  0000 C CNN
F 2 "" H -10400 -6650 60  0000 C CNN
F 3 "" H -10400 -6650 60  0000 C CNN
	1    -10400 -6650
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 55965F01
P -12450 -8750
F 0 "R?" V -12370 -8750 50  0000 C CNN
F 1 "10k 5%" V -12450 -8750 50  0000 C CNN
F 2 "" V -12520 -8750 30  0000 C CNN
F 3 "" H -12450 -8750 30  0000 C CNN
	1    -12450 -8750
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 55965F3A
P -12450 -8350
F 0 "C?" H -12425 -8250 50  0000 L CNN
F 1 "100n" H -12425 -8450 50  0000 L CNN
F 2 "" H -12412 -8500 30  0000 C CNN
F 3 "" H -12450 -8350 60  0000 C CNN
	1    -12450 -8350
	1    0    0    -1  
$EndComp
Wire Wire Line
	-12100 -8550 -12450 -8550
Wire Wire Line
	-12450 -8600 -12450 -8500
Connection ~ -12450 -8550
Wire Wire Line
	-12450 -8900 -12450 -9100
Wire Wire Line
	-12450 -8200 -12450 -8100
$Comp
L GND #PWR?
U 1 1 55965FE7
P -12450 -8100
F 0 "#PWR?" H -12450 -8350 50  0001 C CNN
F 1 "GND" H -12450 -8250 50  0000 C CNN
F 2 "" H -12450 -8100 60  0000 C CNN
F 3 "" H -12450 -8100 60  0000 C CNN
	1    -12450 -8100
	1    0    0    -1  
$EndComp
$Comp
L VDD #PWR?
U 1 1 55965FFE
P -12450 -9100
F 0 "#PWR?" H -12450 -9250 50  0001 C CNN
F 1 "VDD" H -12450 -8950 50  0000 C CNN
F 2 "" H -12450 -9100 60  0000 C CNN
F 3 "" H -12450 -9100 60  0000 C CNN
	1    -12450 -9100
	1    0    0    -1  
$EndComp
$Comp
L crystal-4pin U?
U 1 1 55966025
P -14100 -7400
F 0 "U?" H -14100 -7700 60  0000 C CNN
F 1 "crystal-4pin" H -14100 -7050 60  0000 C CNN
F 2 "" H -14150 -7400 60  0000 C CNN
F 3 "" H -14150 -7400 60  0000 C CNN
	1    -14100 -7400
	-1   0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 559660A4
P -15000 -8000
F 0 "C?" H -14975 -7900 50  0000 L CNN
F 1 "C" H -14975 -8100 50  0000 L CNN
F 2 "" H -14962 -8150 30  0000 C CNN
F 3 "" H -15000 -8000 60  0000 C CNN
	1    -15000 -8000
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5596633A
P -13300 -6800
F 0 "C?" H -13275 -6700 50  0000 L CNN
F 1 "C" H -13275 -6900 50  0000 L CNN
F 2 "" H -13262 -6950 30  0000 C CNN
F 3 "" H -13300 -6800 60  0000 C CNN
	1    -13300 -6800
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 559682F7
P -12900 -7050
F 0 "R?" V -12820 -7050 50  0000 C CNN
F 1 "100 5%" V -12900 -7050 50  0000 C CNN
F 2 "" V -12970 -7050 30  0000 C CNN
F 3 "" H -12900 -7050 30  0000 C CNN
	1    -12900 -7050
	0    1    1    0   
$EndComp
Wire Wire Line
	-12100 -7050 -12750 -7050
Wire Wire Line
	-13300 -6950 -13300 -7200
Wire Wire Line
	-13300 -7200 -13400 -7200
Wire Wire Line
	-13050 -7050 -13300 -7050
Connection ~ -13300 -7050
Wire Wire Line
	-12100 -7750 -15000 -7750
Wire Wire Line
	-15000 -7850 -15000 -7600
Wire Wire Line
	-15000 -7600 -14800 -7600
Connection ~ -15000 -7750
Wire Wire Line
	-13400 -7600 -13100 -7600
Wire Wire Line
	-13100 -7600 -13100 -7500
$Comp
L GND #PWR?
U 1 1 55968517
P -13100 -7500
F 0 "#PWR?" H -13100 -7750 50  0001 C CNN
F 1 "GND" H -13100 -7650 50  0000 C CNN
F 2 "" H -13100 -7500 60  0000 C CNN
F 3 "" H -13100 -7500 60  0000 C CNN
	1    -13100 -7500
	1    0    0    -1  
$EndComp
Wire Wire Line
	-14800 -7200 -15000 -7200
Wire Wire Line
	-15000 -7200 -15000 -7050
$Comp
L GND #PWR?
U 1 1 55968551
P -15000 -7050
F 0 "#PWR?" H -15000 -7300 50  0001 C CNN
F 1 "GND" H -15000 -7200 50  0000 C CNN
F 2 "" H -15000 -7050 60  0000 C CNN
F 3 "" H -15000 -7050 60  0000 C CNN
	1    -15000 -7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	-11600 -10550 -11600 -10750
Wire Wire Line
	-11600 -10750 -9200 -10750
Wire Wire Line
	-10900 -10750 -10900 -10550
Wire Wire Line
	-10400 -10950 -10400 -10550
Connection ~ -10900 -10750
Wire Wire Line
	-9800 -10750 -9800 -10550
Connection ~ -10400 -10750
Wire Wire Line
	-9200 -10750 -9200 -10550
Connection ~ -9800 -10750
$Comp
L VDD #PWR?
U 1 1 5596874E
P -10400 -10950
F 0 "#PWR?" H -10400 -11100 50  0001 C CNN
F 1 "VDD" H -10400 -10800 50  0000 C CNN
F 2 "" H -10400 -10950 60  0000 C CNN
F 3 "" H -10400 -10950 60  0000 C CNN
	1    -10400 -10950
	1    0    0    -1  
$EndComp
Wire Wire Line
	-11600 -1650 -11600 -1450
Wire Wire Line
	-11600 -1450 -9200 -1450
Wire Wire Line
	-10400 -1650 -10400 -1250
Wire Wire Line
	-9800 -1450 -9800 -1650
Connection ~ -10400 -1450
Wire Wire Line
	-9200 -1450 -9200 -1650
Connection ~ -9800 -1450
$Comp
L GND #PWR?
U 1 1 55968B2A
P -10400 -1250
F 0 "#PWR?" H -10400 -1500 50  0001 C CNN
F 1 "GND" H -10400 -1400 50  0000 C CNN
F 2 "" H -10400 -1250 60  0000 C CNN
F 3 "" H -10400 -1250 60  0000 C CNN
	1    -10400 -1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	-7500 -6250 -6300 -6250
Wire Wire Line
	-7500 -6450 -6300 -6450
Text Label -6900 -6250 0    60   ~ 0
swd-clk
Text Label -6900 -6450 0    60   ~ 0
swd-io
$Comp
L CONN_3 K?
U 1 1 5597574C
P -5950 -6350
F 0 "K?" V -6000 -6350 50  0000 C CNN
F 1 "swd" V -5900 -6350 40  0000 C CNN
F 2 "" H -5950 -6350 60  0000 C CNN
F 3 "" H -5950 -6350 60  0000 C CNN
	1    -5950 -6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	-6300 -6350 -7100 -6350
Wire Wire Line
	-7100 -6350 -7100 -6150
$Comp
L GND-RESCUE-voltamper #PWR?
U 1 1 55975754
P -7100 -6150
F 0 "#PWR?" H -7100 -6150 30  0001 C CNN
F 1 "GND" H -7100 -6220 30  0001 C CNN
F 2 "" H -7100 -6150 60  0001 C CNN
F 3 "" H -7100 -6150 60  0001 C CNN
	1    -7100 -6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	-7500 -4350 -5500 -4350
Wire Wire Line
	-7500 -4150 -5200 -4150
Text Label -6300 -4350 0    60   ~ 0
i2c1-scl
Text Label -6300 -4150 0    60   ~ 0
i2c1-sda
$Comp
L R-RESCUE-voltamper R?
U 1 1 55978DDA
P -5500 -4700
F 0 "R?" V -5420 -4700 40  0000 C CNN
F 1 "10k" V -5493 -4699 40  0000 C CNN
F 2 "~" V -5570 -4700 30  0000 C CNN
F 3 "~" H -5500 -4700 30  0000 C CNN
	1    -5500 -4700
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-voltamper R?
U 1 1 55978DE0
P -5200 -4700
F 0 "R?" V -5120 -4700 40  0000 C CNN
F 1 "10k" V -5193 -4699 40  0000 C CNN
F 2 "~" V -5270 -4700 30  0000 C CNN
F 3 "~" H -5200 -4700 30  0000 C CNN
	1    -5200 -4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	-5500 -4350 -5500 -4450
Wire Wire Line
	-5500 -4950 -5500 -5150
Wire Wire Line
	-5200 -4150 -5200 -4450
Wire Wire Line
	-5200 -4950 -5200 -5150
$Comp
L VDD #PWR?
U 1 1 55978DEA
P -5500 -5150
F 0 "#PWR?" H -5500 -5050 30  0001 C CNN
F 1 "VDD" H -5500 -5040 30  0000 C CNN
F 2 "" H -5500 -5150 60  0000 C CNN
F 3 "" H -5500 -5150 60  0000 C CNN
	1    -5500 -5150
	1    0    0    -1  
$EndComp
$Comp
L VDD #PWR?
U 1 1 55978DF0
P -5200 -5150
F 0 "#PWR?" H -5200 -5050 30  0001 C CNN
F 1 "VDD" H -5200 -5040 30  0000 C CNN
F 2 "" H -5200 -5150 60  0000 C CNN
F 3 "" H -5200 -5150 60  0000 C CNN
	1    -5200 -5150
	1    0    0    -1  
$EndComp
$Comp
L NCP3063 U?
U 1 1 5597C322
P -400 -8800
F 0 "U?" H -500 -8500 60  0000 C CNN
F 1 "NCP3063" H -450 -9100 60  0000 C CNN
F 2 "~" H -400 -8800 60  0000 C CNN
F 3 "~" H -400 -8800 60  0000 C CNN
	1    -400 -8800
	1    0    0    -1  
$EndComp
Wire Wire Line
	-1050 -8850 -1200 -8850
Wire Wire Line
	-1200 -8850 -1200 -9550
Wire Wire Line
	-1050 -8750 -1800 -8750
Wire Wire Line
	-1400 -9000 -1400 -8500
Connection ~ -1400 -8750
Wire Wire Line
	-1400 -9550 -1400 -9300
Connection ~ -1200 -9550
Text Label -1750 -8750 0    60   ~ 0
Vin
Wire Wire Line
	-1400 -8200 -1400 -7950
Wire Wire Line
	100  -8650 200  -8650
Wire Wire Line
	200  -8650 200  -7950
Wire Wire Line
	100  -8750 500  -8750
Wire Wire Line
	500  -8750 500  -8500
Wire Wire Line
	500  -8200 500  -7950
Wire Wire Line
	100  -8850 800  -8850
Wire Wire Line
	800  -8850 800  -7950
Wire Wire Line
	100  -8950 200  -8950
Wire Wire Line
	200  -8950 200  -9550
Connection ~ 200  -9550
Wire Wire Line
	850  -9550 2500 -9550
Wire Wire Line
	2500 -9550 2500 -10050
Text Label 2500 -9900 1    60   ~ 0
v+
Wire Wire Line
	1400 -9550 1400 -9300
Connection ~ 1400 -9550
Wire Wire Line
	1800 -9550 1800 -9300
Connection ~ 1800 -9550
Wire Wire Line
	1400 -9000 1400 -8750
Wire Wire Line
	1800 -9000 1800 -8750
Wire Wire Line
	-1050 -8650 -1150 -8650
Wire Wire Line
	-1150 -8650 -1150 -7250
Wire Wire Line
	-1150 -6950 -1150 -6650
Wire Wire Line
	-1150 -7500 -900 -7500
Connection ~ -1150 -7500
Wire Wire Line
	-600 -7500 -200 -7500
Text Label -300 -7500 0    60   ~ 0
v+
$Comp
L NCP3063 U?
U 1 1 5597C3A7
P 6350 -8750
F 0 "U?" H 6250 -8450 60  0000 C CNN
F 1 "NCP3063" H 6300 -9050 60  0000 C CNN
F 2 "~" H 6350 -8750 60  0000 C CNN
F 3 "~" H 6350 -8750 60  0000 C CNN
	1    6350 -8750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 -8700 5700 -8700
Wire Wire Line
	4950 -9000 4950 -8500
Wire Wire Line
	4950 -9600 4950 -9300
Wire Wire Line
	4950 -9600 7050 -9600
Wire Wire Line
	7050 -9600 7050 -8900
Wire Wire Line
	7050 -8900 6850 -8900
Wire Wire Line
	5700 -8800 5600 -8800
Wire Wire Line
	5600 -8800 5600 -9600
Connection ~ 5600 -9600
Connection ~ 4950 -8700
Wire Wire Line
	4950 -8200 4950 -7950
Wire Wire Line
	6850 -8700 7400 -8700
Wire Wire Line
	7400 -8700 7400 -8400
Wire Wire Line
	7400 -8100 7400 -7600
Wire Wire Line
	6850 -8600 7000 -8600
Wire Wire Line
	7000 -8600 7000 -7600
Wire Wire Line
	8000 -8650 8000 -8800
Connection ~ 8000 -8800
Wire Wire Line
	8350 -8800 8350 -8550
Wire Wire Line
	6500 -7600 9300 -7600
Wire Wire Line
	8350 -8250 8350 -7600
Connection ~ 7400 -7600
Wire Wire Line
	5500 -7600 6200 -7600
Wire Wire Line
	5500 -8600 5500 -7350
Wire Wire Line
	5500 -8600 5700 -8600
Connection ~ 5500 -7600
Wire Wire Line
	5500 -7050 5500 -6750
Connection ~ 7000 -7600
Wire Wire Line
	6850 -8800 8350 -8800
Connection ~ 8350 -7600
Wire Wire Line
	8500 -7450 8500 -7600
Connection ~ 8500 -7600
Wire Wire Line
	8500 -7150 8500 -6850
Wire Wire Line
	8800 -7350 8800 -7600
Connection ~ 8800 -7600
Wire Wire Line
	8800 -7050 8800 -6850
Text Label 9200 -7600 0    60   ~ 0
v-
Text Label 4600 -8700 0    60   ~ 0
Vin
Wire Notes Line
	-2050 -10550 2950 -10550
Wire Notes Line
	2950 -10550 2950 -6450
Wire Notes Line
	2950 -6450 -2050 -6400
Wire Notes Line
	-2050 -6400 -2050 -10550
Text Notes -300 -9950 0    276  ~ 0
+12V
Wire Notes Line
	4250 -10550 9750 -10550
Wire Notes Line
	9750 -10550 9750 -6450
Wire Notes Line
	9750 -6450 4300 -6450
Wire Notes Line
	4300 -6450 4250 -10550
Text Notes 5650 -10050 0    276  ~ 0
-12V
Text Label -13250 3250 0    60   ~ 0
txd
Text Label -13250 3450 0    60   ~ 0
rxd
Text Label -13250 1850 0    60   ~ 0
suspend
NoConn ~ -12750 2450
NoConn ~ -12750 2050
NoConn ~ -12750 2650
NoConn ~ -12750 2850
NoConn ~ -12750 3050
Text Label -9300 2550 1    60   ~ 0
Vbus
Wire Wire Line
	-9750 1650 -9100 1650
Wire Wire Line
	-9750 2050 -9100 2050
Wire Wire Line
	-9450 2000 -9450 2150
Wire Wire Line
	-12250 4650 -12250 4850
Wire Wire Line
	-12050 4850 -12050 4650
Wire Wire Line
	-11850 4850 -11850 4650
Connection ~ -12050 4850
Wire Wire Line
	-11650 4850 -11650 4650
Connection ~ -11850 4850
Wire Wire Line
	-11450 4850 -11450 4650
Connection ~ -11650 4850
Wire Wire Line
	-11250 4850 -11250 4650
Connection ~ -11450 4850
Wire Wire Line
	-11050 4850 -11050 4650
Connection ~ -11250 4850
Wire Wire Line
	-10850 4850 -10850 4650
Connection ~ -11050 4850
Wire Wire Line
	-10650 4850 -10650 4650
Connection ~ -10850 4850
Wire Wire Line
	-10450 4850 -10450 4650
Connection ~ -10650 4850
Wire Wire Line
	-9750 4150 -9400 4150
Connection ~ -10450 4850
Wire Wire Line
	-12750 1450 -14250 1450
Wire Wire Line
	-14250 1250 -14250 1650
Connection ~ -14250 1450
Wire Wire Line
	-14250 1950 -14250 2250
Wire Wire Line
	-14250 450  -14250 950 
Wire Wire Line
	-9750 3150 -9300 3150
Wire Wire Line
	-9750 3450 -9300 3450
Wire Wire Line
	-9750 2650 -9050 2650
Wire Wire Line
	-9300 3200 -8550 3200
Wire Wire Line
	-9300 3150 -9300 3200
Wire Wire Line
	-9300 3400 -8550 3400
Wire Wire Line
	-9300 3450 -9300 3400
Wire Wire Line
	-9300 3000 -8550 3000
Wire Wire Line
	-9300 2350 -9300 3000
Wire Wire Line
	-9200 3600 -8550 3600
Wire Wire Line
	-9200 3600 -9200 3900
Wire Wire Line
	-9200 3800 -8550 3800
Connection ~ -9450 1650
Connection ~ -9450 2050
Wire Wire Line
	-9750 1150 -8450 1150
Connection ~ -8800 1150
Wire Wire Line
	-9450 1500 -9450 1700
Wire Wire Line
	-8800 1150 -8800 1000
Wire Wire Line
	-12750 3250 -13300 3250
Wire Wire Line
	-12750 3450 -13300 3450
Wire Wire Line
	-12750 1850 -13300 1850
Connection ~ -9300 2650
Wire Wire Line
	-9050 2650 -9050 2500
$Comp
L PWR_FLAG #FLG?
U 1 1 5597F4E5
P -9050 2500
F 0 "#FLG?" H -9050 2595 30  0001 C CNN
F 1 "PWR_FLAG" H -9050 2680 30  0000 C CNN
F 2 "" H -9050 2500 60  0000 C CNN
F 3 "" H -9050 2500 60  0000 C CNN
	1    -9050 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	-12250 4850 -9550 4850
Wire Wire Line
	-9550 4850 -9550 4750
Wire Wire Line
	-9400 4450 -9400 4650
Wire Notes Line
	-9250 4500 -8950 5000
Text Notes -9450 5150 0    60   ~ 0
Just for compatibility with CP2109 \nand may be not installed for CP2102.
$Comp
L CP2102 U?
U 1 1 5597F502
P -11250 2550
F 0 "U?" H -11250 2200 60  0000 C CNN
F 1 "CP2102" H -11250 2800 60  0000 C CNN
F 2 "~" H -11050 2950 60  0000 C CNN
F 3 "~" H -11050 2950 60  0000 C CNN
	1    -11250 2550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	-9750 2150 -9650 2150
Wire Wire Line
	-9650 2150 -9650 2050
Connection ~ -9650 2050
Text Notes -12400 650  0    276  ~ 0
USB scheme.
Text Label -7000 -7850 0    60   ~ 0
suspend
Text Label -7050 -7450 0    60   ~ 0
txd
Text Label -7050 -7650 0    60   ~ 0
rxd
Wire Wire Line
	-7500 -7650 -6900 -7650
Wire Wire Line
	-7500 -7450 -6900 -7450
Wire Wire Line
	-7500 -7850 -6600 -7850
Wire Wire Line
	-12750 3650 -13300 3650
Text Label -13250 3650 0    60   ~ 0
rts
Wire Wire Line
	-12750 3850 -13300 3850
Text Label -13250 3850 0    60   ~ 0
cts
Wire Wire Line
	-7500 -7250 -6900 -7250
Wire Wire Line
	-7500 -7050 -6900 -7050
Text Label -7050 -7250 0    60   ~ 0
rts
Text Label -7050 -7050 0    60   ~ 0
cts
Wire Wire Line
	-13300 -6650 -13300 -6450
Wire Wire Line
	-15000 -8150 -15000 -8350
Wire Wire Line
	-15000 -8350 -15500 -8350
Wire Wire Line
	-15500 -8350 -15500 -8150
$Comp
L GND #PWR?
U 1 1 55995FE0
P -15500 -8150
F 0 "#PWR?" H -15500 -8400 50  0001 C CNN
F 1 "GND" H -15500 -8300 50  0000 C CNN
F 2 "" H -15500 -8150 60  0000 C CNN
F 3 "" H -15500 -8150 60  0000 C CNN
	1    -15500 -8150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55999FBF
P -13300 -6450
F 0 "#PWR?" H -13300 -6700 50  0001 C CNN
F 1 "GND" H -13300 -6600 50  0000 C CNN
F 2 "" H -13300 -6450 60  0000 C CNN
F 3 "" H -13300 -6450 60  0000 C CNN
	1    -13300 -6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	-12100 -4450 -12500 -4450
Wire Wire Line
	-12500 -4450 -12500 -4250
$Comp
L R R?
U 1 1 5599D40B
P -12500 -4100
F 0 "R?" V -12420 -4100 50  0000 C CNN
F 1 "10k 5%" V -12500 -4100 50  0000 C CNN
F 2 "" V -12570 -4100 30  0000 C CNN
F 3 "" H -12500 -4100 30  0000 C CNN
	1    -12500 -4100
	-1   0    0    1   
$EndComp
Wire Wire Line
	-12500 -3950 -12500 -3750
$Comp
L GND #PWR?
U 1 1 559A262E
P -12500 -3750
F 0 "#PWR?" H -12500 -4000 50  0001 C CNN
F 1 "GND" H -12500 -3900 50  0000 C CNN
F 2 "" H -12500 -3750 60  0000 C CNN
F 3 "" H -12500 -3750 60  0000 C CNN
	1    -12500 -3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	-13150 -4850 -12100 -4850
Wire Wire Line
	-12950 -4850 -12950 -4250
$Comp
L R R?
U 1 1 559A6406
P -12950 -4100
F 0 "R?" V -12870 -4100 50  0000 C CNN
F 1 "10k 5%" V -12950 -4100 50  0000 C CNN
F 2 "" V -13020 -4100 30  0000 C CNN
F 3 "" H -12950 -4100 30  0000 C CNN
	1    -12950 -4100
	-1   0    0    1   
$EndComp
Wire Wire Line
	-12950 -3950 -12950 -3750
$Comp
L GND #PWR?
U 1 1 559A65A7
P -12950 -3750
F 0 "#PWR?" H -12950 -4000 50  0001 C CNN
F 1 "GND" H -12950 -3900 50  0000 C CNN
F 2 "" H -12950 -3750 60  0000 C CNN
F 3 "" H -12950 -3750 60  0000 C CNN
	1    -12950 -3750
	1    0    0    -1  
$EndComp
Connection ~ -12950 -4850
Wire Wire Line
	-13150 -5000 -12950 -5000
Wire Wire Line
	-12950 -5000 -12950 -5100
$Comp
L VDD #PWR?
U 1 1 559A8360
P -12950 -5100
F 0 "#PWR?" H -12950 -5250 50  0001 C CNN
F 1 "VDD" H -12950 -4950 50  0000 C CNN
F 2 "" H -12950 -5100 60  0000 C CNN
F 3 "" H -12950 -5100 60  0000 C CNN
	1    -12950 -5100
	1    0    0    -1  
$EndComp
$Comp
L MCP1703T-3302E/CB U?
U 1 1 559B6239
P -11700 6950
F 0 "U?" H -11700 6900 60  0000 C CNN
F 1 "MCP1703T-3302E/CB" H -11700 7250 60  0000 C CNN
F 2 "" H -11700 6950 60  0000 C CNN
F 3 "" H -11700 6950 60  0000 C CNN
	1    -11700 6950
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 559B73E0
P -10950 7200
F 0 "C?" H -10925 7300 50  0000 L CNN
F 1 "4u7" H -10925 7100 50  0000 L CNN
F 2 "" H -10912 7050 30  0000 C CNN
F 3 "" H -10950 7200 60  0000 C CNN
	1    -10950 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	-10950 6650 -10950 7050
Wire Wire Line
	-11150 6850 -10600 6850
Wire Wire Line
	-10950 7350 -10950 7450
$Comp
L GND #PWR?
U 1 1 559B94F6
P -10950 7450
F 0 "#PWR?" H -10950 7200 50  0001 C CNN
F 1 "GND" H -10950 7300 50  0000 C CNN
F 2 "" H -10950 7450 60  0000 C CNN
F 3 "" H -10950 7450 60  0000 C CNN
	1    -10950 7450
	1    0    0    -1  
$EndComp
Wire Wire Line
	-11700 7600 -11700 7700
$Comp
L GND #PWR?
U 1 1 559B9A09
P -11700 7700
F 0 "#PWR?" H -11700 7450 50  0001 C CNN
F 1 "GND" H -11700 7550 50  0000 C CNN
F 2 "" H -11700 7700 60  0000 C CNN
F 3 "" H -11700 7700 60  0000 C CNN
	1    -11700 7700
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 559BB5FE
P -12450 7200
F 0 "C?" H -12425 7300 50  0000 L CNN
F 1 "4u7" H -12425 7100 50  0000 L CNN
F 2 "" H -12412 7050 30  0000 C CNN
F 3 "" H -12450 7200 60  0000 C CNN
	1    -12450 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	-12450 7050 -12450 6850
Wire Wire Line
	-12850 6850 -12250 6850
Wire Wire Line
	-12450 7350 -12450 7450
$Comp
L GND #PWR?
U 1 1 559BC031
P -12450 7450
F 0 "#PWR?" H -12450 7200 50  0001 C CNN
F 1 "GND" H -12450 7300 50  0000 C CNN
F 2 "" H -12450 7450 60  0000 C CNN
F 3 "" H -12450 7450 60  0000 C CNN
	1    -12450 7450
	1    0    0    -1  
$EndComp
Connection ~ -10950 6850
$Comp
L VDD #PWR?
U 1 1 559BEACB
P -10950 6650
F 0 "#PWR?" H -10950 6500 50  0001 C CNN
F 1 "VDD" H -10950 6800 50  0000 C CNN
F 2 "" H -10950 6650 60  0000 C CNN
F 3 "" H -10950 6650 60  0000 C CNN
	1    -10950 6650
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG?
U 1 1 559C1B3E
P -10600 6650
F 0 "#FLG?" H -10600 6745 50  0001 C CNN
F 1 "PWR_FLAG" H -10600 6830 50  0000 C CNN
F 2 "" H -10600 6650 60  0000 C CNN
F 3 "" H -10600 6650 60  0000 C CNN
	1    -10600 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	-10600 6850 -10600 6650
Connection ~ -12450 6850
$Comp
L GND #PWR?
U 1 1 559D54F6
P -10150 7100
F 0 "#PWR?" H -10150 6850 50  0001 C CNN
F 1 "GND" H -10150 6950 50  0000 C CNN
F 2 "" H -10150 7100 60  0000 C CNN
F 3 "" H -10150 7100 60  0000 C CNN
	1    -10150 7100
	1    0    0    -1  
$EndComp
$Comp
L GNDA #PWR?
U 1 1 559D55D4
P -9750 7100
F 0 "#PWR?" H -9750 6850 50  0001 C CNN
F 1 "GNDA" H -9750 6950 50  0000 C CNN
F 2 "" H -9750 7100 60  0000 C CNN
F 3 "" H -9750 7100 60  0000 C CNN
	1    -9750 7100
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 559D56B2
P -9950 7000
F 0 "R?" V -9870 7000 50  0000 C CNN
F 1 "0" V -9950 7000 50  0000 C CNN
F 2 "" V -10020 7000 30  0000 C CNN
F 3 "" H -9950 7000 30  0000 C CNN
	1    -9950 7000
	0    1    1    0   
$EndComp
Wire Wire Line
	-9750 7100 -9750 7000
Wire Wire Line
	-9750 7000 -9800 7000
Wire Wire Line
	-10150 7100 -10150 7000
Wire Wire Line
	-10150 7000 -10100 7000
$Comp
L MINI-USB-B U?
U 1 1 559DC77C
P -7650 3500
F 0 "U?" H -7650 3100 60  0000 C CNN
F 1 "MINI-USB-B" H -7650 3900 60  0000 C CNN
F 2 "" H -7650 3500 60  0000 C CNN
F 3 "" H -7650 3500 60  0000 C CNN
	1    -7650 3500
	1    0    0    -1  
$EndComp
Connection ~ -9200 3800
$Comp
L GND #PWR?
U 1 1 559DF6DF
P -9200 3900
F 0 "#PWR?" H -9200 3650 50  0001 C CNN
F 1 "GND" H -9200 3750 50  0000 C CNN
F 2 "" H -9200 3900 60  0000 C CNN
F 3 "" H -9200 3900 60  0000 C CNN
	1    -9200 3900
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 559DF7B3
P -9400 4300
F 0 "C?" H -9375 4400 50  0000 L CNN
F 1 "4u7" H -9375 4200 50  0000 L CNN
F 2 "" H -9362 4150 30  0000 C CNN
F 3 "" H -9400 4300 60  0000 C CNN
	1    -9400 4300
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 559E0F8E
P -14250 1800
F 0 "C?" H -14225 1900 50  0000 L CNN
F 1 "100n" H -14225 1700 50  0000 L CNN
F 2 "" H -14212 1650 30  0000 C CNN
F 3 "" H -14250 1800 60  0000 C CNN
	1    -14250 1800
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 559E178B
P -14250 1100
F 0 "R?" V -14170 1100 50  0000 C CNN
F 1 "10k 5%" V -14250 1100 50  0000 C CNN
F 2 "" V -14320 1100 30  0000 C CNN
F 3 "" H -14250 1100 30  0000 C CNN
	1    -14250 1100
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 559E4D48
P -9450 1850
F 0 "C?" H -9425 1950 50  0000 L CNN
F 1 "100n" H -9425 1750 50  0000 L CNN
F 2 "" H -9412 1700 30  0000 C CNN
F 3 "" H -9450 1850 60  0000 C CNN
	1    -9450 1850
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 559EA768
P -9100 1850
F 0 "C?" H -9075 1950 50  0000 L CNN
F 1 "4u7" H -9075 1750 50  0000 L CNN
F 2 "" H -9062 1700 30  0000 C CNN
F 3 "" H -9100 1850 60  0000 C CNN
	1    -9100 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	-9100 1650 -9100 1700
Wire Wire Line
	-9100 2050 -9100 2000
$Comp
L VDD #PWR?
U 1 1 559F6221
P -8800 1000
F 0 "#PWR?" H -8800 850 50  0001 C CNN
F 1 "VDD" H -8800 1150 50  0000 C CNN
F 2 "" H -8800 1000 60  0000 C CNN
F 3 "" H -8800 1000 60  0000 C CNN
	1    -8800 1000
	1    0    0    -1  
$EndComp
$Comp
L VDD #PWR?
U 1 1 559F8E6C
P -14250 450
F 0 "#PWR?" H -14250 300 50  0001 C CNN
F 1 "VDD" H -14250 600 50  0000 C CNN
F 2 "" H -14250 450 60  0000 C CNN
F 3 "" H -14250 450 60  0000 C CNN
	1    -14250 450 
	1    0    0    -1  
$EndComp
$Comp
L VDD #PWR?
U 1 1 559FB0F7
P -9550 4750
F 0 "#PWR?" H -9550 4600 50  0001 C CNN
F 1 "VDD" H -9550 4900 50  0000 C CNN
F 2 "" H -9550 4750 60  0000 C CNN
F 3 "" H -9550 4750 60  0000 C CNN
	1    -9550 4750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55A032C5
P -9400 4650
F 0 "#PWR?" H -9400 4400 50  0001 C CNN
F 1 "GND" H -9400 4500 50  0000 C CNN
F 2 "" H -9400 4650 60  0000 C CNN
F 3 "" H -9400 4650 60  0000 C CNN
	1    -9400 4650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55A04C50
P -9450 2150
F 0 "#PWR?" H -9450 1900 50  0001 C CNN
F 1 "GND" H -9450 2000 50  0000 C CNN
F 2 "" H -9450 2150 60  0000 C CNN
F 3 "" H -9450 2150 60  0000 C CNN
	1    -9450 2150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55A06EDB
P -14250 2250
F 0 "#PWR?" H -14250 2000 50  0001 C CNN
F 1 "GND" H -14250 2100 50  0000 C CNN
F 2 "" H -14250 2250 60  0000 C CNN
F 3 "" H -14250 2250 60  0000 C CNN
	1    -14250 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	-7500 -8050 -7000 -8050
Wire Wire Line
	-7500 -8250 -7000 -8250
Wire Wire Line
	-7500 -8450 -7000 -8450
Text Label -7400 -8250 0    60   ~ 0
adc_miso
Text Label -7400 -8050 0    60   ~ 0
adc_mosi
Text Label -7400 -8450 0    60   ~ 0
adc_sck
Text Label -12800 6850 0    60   ~ 0
Vbus
$Comp
L R R?
U 1 1 55A1CB72
P -1400 -9150
F 0 "R?" V -1320 -9150 50  0000 C CNN
F 1 "0.15 5%" V -1400 -9150 50  0000 C CNN
F 2 "" V -1470 -9150 30  0000 C CNN
F 3 "" H -1400 -9150 30  0000 C CNN
	1    -1400 -9150
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 55A1D789
P -1400 -8350
F 0 "C?" H -1375 -8250 50  0000 L CNN
F 1 "100n" H -1375 -8450 50  0000 L CNN
F 2 "" H -1362 -8500 30  0000 C CNN
F 3 "" H -1400 -8350 60  0000 C CNN
	1    -1400 -8350
	1    0    0    -1  
$EndComp
$Comp
L D_Schottky D?
U 1 1 55A1E75A
P 700 -9550
F 0 "D?" H 700 -9450 50  0000 C CNN
F 1 "1n5819" H 700 -9650 50  0000 C CNN
F 2 "" H 700 -9550 60  0000 C CNN
F 3 "" H 700 -9550 60  0000 C CNN
	1    700  -9550
	-1   0    0    1   
$EndComp
$Comp
L C C?
U 1 1 55A1FAFB
P 500 -8350
F 0 "C?" H 525 -8250 50  0000 L CNN
F 1 "2n2" H 525 -8450 50  0000 L CNN
F 2 "" H 538 -8500 30  0000 C CNN
F 3 "" H 500 -8350 60  0000 C CNN
	1    500  -8350
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 55A21318
P -750 -7500
F 0 "R?" V -670 -7500 50  0000 C CNN
F 1 "10k 5%" V -750 -7500 50  0000 C CNN
F 2 "" V -820 -7500 30  0000 C CNN
F 3 "" H -750 -7500 30  0000 C CNN
	1    -750 -7500
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 55A21F21
P -1150 -7100
F 0 "R?" V -1070 -7100 50  0000 C CNN
F 1 "1k1 1%" V -1150 -7100 50  0000 C CNN
F 2 "" V -1220 -7100 30  0000 C CNN
F 3 "" H -1150 -7100 30  0000 C CNN
	1    -1150 -7100
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG?
U 1 1 55A2627D
P 2500 -10050
F 0 "#FLG?" H 2500 -9955 50  0001 C CNN
F 1 "PWR_FLAG" H 2500 -9870 50  0000 C CNN
F 2 "" H 2500 -10050 60  0000 C CNN
F 3 "" H 2500 -10050 60  0000 C CNN
	1    2500 -10050
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 55A2A9A6
P 4950 -9150
F 0 "R?" V 5030 -9150 50  0000 C CNN
F 1 "0.15 5%" V 4950 -9150 50  0000 C CNN
F 2 "" V 4880 -9150 30  0000 C CNN
F 3 "" H 4950 -9150 30  0000 C CNN
	1    4950 -9150
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 55A2E7DB
P 4950 -8350
F 0 "C?" H 4975 -8250 50  0000 L CNN
F 1 "100n" H 4975 -8450 50  0000 L CNN
F 2 "" H 4988 -8500 30  0000 C CNN
F 3 "" H 4950 -8350 60  0000 C CNN
	1    4950 -8350
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 55A33827
P 6350 -7600
F 0 "R?" V 6430 -7600 50  0000 C CNN
F 1 "1k1 1%" V 6350 -7600 50  0000 C CNN
F 2 "" V 6280 -7600 30  0000 C CNN
F 3 "" H 6350 -7600 30  0000 C CNN
	1    6350 -7600
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 55A37142
P 5500 -7200
F 0 "R?" V 5580 -7200 50  0000 C CNN
F 1 "10k 5%" V 5500 -7200 50  0000 C CNN
F 2 "" V 5430 -7200 30  0000 C CNN
F 3 "" H 5500 -7200 30  0000 C CNN
	1    5500 -7200
	-1   0    0    1   
$EndComp
$Comp
L C C?
U 1 1 55A3B95B
P 1400 -9150
F 0 "C?" H 1425 -9050 50  0000 L CNN
F 1 "100n" H 1425 -9250 50  0000 L CNN
F 2 "" H 1438 -9300 30  0000 C CNN
F 3 "" H 1400 -9150 60  0000 C CNN
	1    1400 -9150
	1    0    0    -1  
$EndComp
$Comp
L CP C?
U 1 1 55A3C55E
P 1800 -9150
F 0 "C?" H 1825 -9050 50  0000 L CNN
F 1 "330u" H 1825 -9250 50  0000 L CNN
F 2 "" H 1838 -9300 30  0000 C CNN
F 3 "" H 1800 -9150 60  0000 C CNN
	1    1800 -9150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55A3F1B3
P -1400 -7950
F 0 "#PWR?" H -1400 -8200 50  0001 C CNN
F 1 "GND" H -1400 -8100 50  0000 C CNN
F 2 "" H -1400 -7950 60  0000 C CNN
F 3 "" H -1400 -7950 60  0000 C CNN
	1    -1400 -7950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55A4120F
P -1150 -6650
F 0 "#PWR?" H -1150 -6900 50  0001 C CNN
F 1 "GND" H -1150 -6800 50  0000 C CNN
F 2 "" H -1150 -6650 60  0000 C CNN
F 3 "" H -1150 -6650 60  0000 C CNN
	1    -1150 -6650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55A4139A
P 200 -7950
F 0 "#PWR?" H 200 -8200 50  0001 C CNN
F 1 "GND" H 200 -8100 50  0000 C CNN
F 2 "" H 200 -7950 60  0000 C CNN
F 3 "" H 200 -7950 60  0000 C CNN
	1    200  -7950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55A41525
P 500 -7950
F 0 "#PWR?" H 500 -8200 50  0001 C CNN
F 1 "GND" H 500 -8100 50  0000 C CNN
F 2 "" H 500 -7950 60  0000 C CNN
F 3 "" H 500 -7950 60  0000 C CNN
	1    500  -7950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55A416B0
P 800 -7950
F 0 "#PWR?" H 800 -8200 50  0001 C CNN
F 1 "GND" H 800 -8100 50  0000 C CNN
F 2 "" H 800 -7950 60  0000 C CNN
F 3 "" H 800 -7950 60  0000 C CNN
	1    800  -7950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55A4183B
P 1400 -8750
F 0 "#PWR?" H 1400 -9000 50  0001 C CNN
F 1 "GND" H 1400 -8900 50  0000 C CNN
F 2 "" H 1400 -8750 60  0000 C CNN
F 3 "" H 1400 -8750 60  0000 C CNN
	1    1400 -8750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55A419C6
P 1800 -8750
F 0 "#PWR?" H 1800 -9000 50  0001 C CNN
F 1 "GND" H 1800 -8900 50  0000 C CNN
F 2 "" H 1800 -8750 60  0000 C CNN
F 3 "" H 1800 -8750 60  0000 C CNN
	1    1800 -8750
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 55A42436
P 7400 -8250
F 0 "C?" H 7425 -8150 50  0000 L CNN
F 1 "2n2" H 7425 -8350 50  0000 L CNN
F 2 "" H 7438 -8400 30  0000 C CNN
F 3 "" H 7400 -8250 60  0000 C CNN
	1    7400 -8250
	1    0    0    -1  
$EndComp
$Comp
L D_Schottky D?
U 1 1 55A442A5
P 8350 -8400
F 0 "D?" H 8350 -8300 50  0000 C CNN
F 1 "1n5819" H 8350 -8500 50  0000 C CNN
F 2 "" H 8350 -8400 60  0000 C CNN
F 3 "" H 8350 -8400 60  0000 C CNN
	1    8350 -8400
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 55A46A02
P 8500 -7300
F 0 "C?" H 8525 -7200 50  0000 L CNN
F 1 "100n" H 8525 -7400 50  0000 L CNN
F 2 "" H 8538 -7450 30  0000 C CNN
F 3 "" H 8500 -7300 60  0000 C CNN
	1    8500 -7300
	1    0    0    -1  
$EndComp
$Comp
L CP C?
U 1 1 55A49CD8
P 8800 -7200
F 0 "C?" H 8825 -7100 50  0000 L CNN
F 1 "330u" H 8825 -7300 50  0000 L CNN
F 2 "" H 8838 -7350 30  0000 C CNN
F 3 "" H 8800 -7200 60  0000 C CNN
	1    8800 -7200
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG?
U 1 1 55A4D097
P 9300 -7600
F 0 "#FLG?" H 9300 -7505 50  0001 C CNN
F 1 "PWR_FLAG" H 9300 -7420 50  0000 C CNN
F 2 "" H 9300 -7600 60  0000 C CNN
F 3 "" H 9300 -7600 60  0000 C CNN
	1    9300 -7600
	0    1    1    0   
$EndComp
$Comp
L INDUCTOR L?
U 1 1 55A50EC9
P 8000 -8350
F 0 "L?" V 7950 -8350 50  0000 C CNN
F 1 "50uH" V 8100 -8350 50  0000 C CNN
F 2 "" H 8000 -8350 60  0000 C CNN
F 3 "" H 8000 -8350 60  0000 C CNN
	1    8000 -8350
	-1   0    0    1   
$EndComp
Wire Wire Line
	8000 -8050 8000 -7950
$Comp
L GND #PWR?
U 1 1 55A54F24
P 8000 -7950
F 0 "#PWR?" H 8000 -8200 50  0001 C CNN
F 1 "GND" H 8000 -8100 50  0000 C CNN
F 2 "" H 8000 -7950 60  0000 C CNN
F 3 "" H 8000 -7950 60  0000 C CNN
	1    8000 -7950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55A57487
P 4950 -7950
F 0 "#PWR?" H 4950 -8200 50  0001 C CNN
F 1 "GND" H 4950 -8100 50  0000 C CNN
F 2 "" H 4950 -7950 60  0000 C CNN
F 3 "" H 4950 -7950 60  0000 C CNN
	1    4950 -7950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55A57610
P 5500 -6750
F 0 "#PWR?" H 5500 -7000 50  0001 C CNN
F 1 "GND" H 5500 -6900 50  0000 C CNN
F 2 "" H 5500 -6750 60  0000 C CNN
F 3 "" H 5500 -6750 60  0000 C CNN
	1    5500 -6750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55A57799
P 8500 -6850
F 0 "#PWR?" H 8500 -7100 50  0001 C CNN
F 1 "GND" H 8500 -7000 50  0000 C CNN
F 2 "" H 8500 -6850 60  0000 C CNN
F 3 "" H 8500 -6850 60  0000 C CNN
	1    8500 -6850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55A57922
P 8800 -6850
F 0 "#PWR?" H 8800 -7100 50  0001 C CNN
F 1 "GND" H 8800 -7000 50  0000 C CNN
F 2 "" H 8800 -6850 60  0000 C CNN
F 3 "" H 8800 -6850 60  0000 C CNN
	1    8800 -6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	-7500 -2950 -6700 -2950
Wire Wire Line
	-7500 -2750 -6700 -2750
Wire Wire Line
	-7500 -2550 -6700 -2550
Text Label -7100 -2950 0    60   ~ 0
dac_sck
Text Label -7100 -2750 0    60   ~ 0
dac_miso
Text Label -7100 -2550 0    60   ~ 0
dac_mosi
$Comp
L INDUCTOR L?
U 1 1 55A5DB3C
P -450 -9550
F 0 "L?" V -500 -9550 50  0000 C CNN
F 1 "50uH" V -350 -9550 50  0000 C CNN
F 2 "" H -450 -9550 60  0000 C CNN
F 3 "" H -450 -9550 60  0000 C CNN
	1    -450 -9550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	-150 -9550 550  -9550
Wire Wire Line
	-1400 -9550 -750 -9550
$EndSCHEMATC
