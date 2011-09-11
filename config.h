/*cl-osd - A simple open source osd for e-osd and g-osd
Copyright (C) 2011 Carl Ljungström

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.*/


#ifndef SETUP_H_
#define SETUP_H_

#include <avr/io.h>

// ------------------ MAIN CONFIGS -----------------------

// Select one of these depending on your board (Wrong type might damage the board!)
// This can also be set using symbol in configuration.
//#define E_OSD
//#define G_OSD
//#define E_OSD_GPS //Non supported! Note: Also enable E_OSD!

// For debugging. (All combinations might not work) :-)
#if defined(G_OSD) || defined(E_OSD_GPS)
#define GPS_ENABLED
#endif //defined(G_OSD) || defined(E_OSD_GPS)
#ifdef GPS_ENABLED
#define GRAPICSENABLED
#endif //GPS_ENABLED
#define TEXT_ENABLED
#define TIME_ENABLED
#define ADC_ENABLED
#ifdef TEXT_ENABLED
//#define TEXT_INVERTED_ENABLED
//#define TEXT_SMALL_ENABLED
#endif //TEXT_ENABLED

// Battery
#define CELL_LOW_VOLTAGE 3.8
#define CELL_HIGH_VOLTAGE 4.2
#define CELL_COUNT 3

// RSSI convertion
#define RSSI_MAX_VOLTAGE 4.7
#define RSSI_MIN_VOLTAGE 1.2

// Unit system
//#define IMPERIAL_SYSTEM
#define METRIC_SYSTEM

// Home set (Use at least one of the first two)
//#define HOME_SET_AT_FIRST_FIX
#define HOME_AUTO_SET
#define HOME_SET_WITH_BUTTON

// ----------------- INTERNAL CONFIGS ------------------

// Check sanity
#if (defined(E_OSD_GPS) && !defined(E_OSD)) 
#error "Make sure to define E_OSD as well when using E_OSD_GPS"
#endif

#if (defined(E_OSD) && defined(G_OSD)) || (!defined(E_OSD) && !defined(G_OSD))
#error "Select one and only one target!"
#endif

#if (defined(IMPERIAL_SYSTEM) && defined(METRIC_SYSTEM)) || (!defined(IMPERIAL_SYSTEM) && !defined(METRIC_SYSTEM))
#error "Select one and only one unit system!"
#endif

// in / out
#ifdef E_OSD
#define KEY (1<<PD5)
#define LED (1<<PD6)
#endif // EOSD
#ifdef G_OSD
#define KEY (1<<PD4)
#define LED (1<<PD3)
#endif //GOSD

#define OUT1 (1<<PB1)
#define OUT2 (1<<PB3)

#ifdef E_OSD
#define ADC1 (1<<PC0) //ADC0
#define ADC2 (1<<PC1) //ADC1
#endif // EOSD
#ifdef G_OSD
#define ADC1 (1<<PC1) //ADC1
#define ADC2 (1<<PC2) //ADC2
#define ADC3 (1<<PC3) //ADC3
#endif //GOSD

#define LTRIG (1<<PD2) //INT0
#define SS (1<<PB2)

// Text
#ifndef TEXT_SMALL_ENABLED
#define TEXT_LINE_MAX_CHARS 33
#else
#define TEXT_LINE_MAX_CHARS 58
#endif //TEXT_SMALL_ENABLED
#define TEXT_CHAR_HEIGHT 8
/*#define TEXT_LINES 9
#define START 43
#define DIFF 29
#define TEXT_TRIG_LINES_LIST START, START+(DIFF*1), START+(DIFF*2), START+(DIFF*3), START+(DIFF*4), START+(DIFF*5), START+(DIFF*6), START+(DIFF*7), START+(DIFF*8)*/
#define TEXT_LINES 4
#define TEXT_TRIG_LINES_LIST 51, 51+25, 240, 240+25
#define TEXT_INVERTED_OFF 0
#define TEXT_INVERTED_ON 1
#define TEXT_INVERTED_FLIP 2
#ifndef TEXT_SMALL_ENABLED
#define TEXT_SIZE_MULT 2
#else
#define TEXT_SIZE_MULT 1
#endif //TEXT_SMALL_ENABLED

#ifdef METRIC_SYSTEM
#define TEXT_LENGTH_UNIT "M"
#else
#define TEXT_LENGTH_UNIT "F"
#endif //METRIC_SYSTEM

#ifdef METRIC_SYSTEM
#define TEXT_SPEED_UNIT "KM/H"
#else 
#define TEXT_SPEED_UNIT "MPH"
#endif //METRIC_SYSTEM

// ADC
#define ANALOG_IN_1 0
#define ANALOG_IN_2 1
#define ANALOG_IN_3 2

#ifdef E_OSD
#define ADC_OFFSET 0
#endif // EOSD
#ifdef G_OSD
#define ADC_OFFSET 1
#endif //GOSD

#ifdef E_OSD
#define ANALOG_IN_NUMBER 2
#endif // EOSD
#ifdef G_OSD
#define ANALOG_IN_NUMBER 3
#endif //GOSD

// Battery
#define BATT_MIN_VOLTAGE CELL_LOW_VOLTAGE*CELL_COUNT
#define BATT_MAX_VOLTAGE CELL_HIGH_VOLTAGE*CELL_COUNT
#define BATT_MIN_VOLTAGE_INT (uint16_t)(BATT_MIN_VOLTAGE*100)
#define BATT_MAX_VOLTAGE_INT (uint16_t)(BATT_MAX_VOLTAGE*100)

// RSSI conversion
#define RSSI_MIN_VOLTAGE_INT (uint16_t)(RSSI_MIN_VOLTAGE*100)
#define RSSI_MAX_VOLTAGE_INT (uint16_t)(RSSI_MAX_VOLTAGE*100)

// Graphics
#define GRAPHICS_SIZE 24 // Multiple of 8
#define GRAPHICS_WIDTH_REAL GRAPHICS_SIZE
#define GRAPHICS_WIDTH (GRAPHICS_SIZE/8)
#define GRAPHICS_HEIGHT GRAPHICS_SIZE
#define GRAPHICS_LINE 105
#define GRAPHICS_OFFSET 48

// Line triggering
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define LAST_LINE 100 //240+25+16+2

#define LINE_TYPE_UNKNOWN 0
#define LINE_TYPE_TEXT 1
#define LINE_TYPE_GRAPHICS 2

// CPU speed
#define F_CPU 24000000UL

// GPS
#define GPS_BAUD 4800
#define GPS_UBRR (F_CPU/16/GPS_BAUD-1)
#define GPS_MAX_CHARS 11

#define DUMMY_FUNC return 0;

// TODO: Work more on this...
// Text layout
#define DATA_TYPE_NONE 0
#define DATA_TYPE_VOLT 1
#define DATA_TYPE_RSSI 2
#define DATA_TYPE_BATTERY 3
#define DATA_TYPE_TIME 4
#define DATA_TYPE_GPS_TIME 5
#define DATA_TYPE_GPS_DATE 6
#define DATA_TYPE_GPS_LAT 7
#define DATA_TYPE_GPS_LONG 8
#define DATA_TYPE_GPS_SATS 9
#define DATA_TYPE_GPS_FIX 10
#define DATA_TYPE_GPS_ANGLE 11
#define DATA_TYPE_GPS_SPEED 12
#define DATA_TYPE_HOME_DISTANCE 13
#define DATA_TYPE_HOME_BEARING 14

// Text placement
#define LINE(a) a
#define NO_INDEX 0
#define AUTO_ADJUST -1
#define TEXT_PLACEMENT { \
{{DATA_TYPE_TIME, LINE(0), NO_INDEX, AUTO_ADJUST}, \
 {DATA_TYPE_VOLT, LINE(0), ANALOG_IN_1, AUTO_ADJUST}, \
 {DATA_TYPE_VOLT, LINE(0), ANALOG_IN_2, AUTO_ADJUST}, \
 {DATA_TYPE_RSSI, LINE(0), ANALOG_IN_3, AUTO_ADJUST}}, \
{{DATA_TYPE_GPS_SPEED, LINE(1), NO_INDEX, AUTO_ADJUST}, \
 {DATA_TYPE_GPS_ANGLE, LINE(1), NO_INDEX, AUTO_ADJUST}}, \
{{DATA_TYPE_GPS_LAT, LINE(1), NO_INDEX, AUTO_ADJUST}, \
 {DATA_TYPE_GPS_LONG, LINE(1), NO_INDEX, TEXT_LINE_MAX_CHARS-1-7}}}
	

// Global vars
static volatile uint16_t line = 0;

// Debug vars!
#ifdef DEBUG
static volatile uint8_t u8 = 0;
static volatile int8_t i8 = 0;
static volatile uint16_t u16 = 0;
static volatile int16_t i16 = 0;
static volatile uint32_t u32 = 0;
static volatile int32_t i32 = 0;
static volatile char c8 = 0;
#endif //DEBUG

#endif /* SETUP_H_ */