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

// For debugging. (All combinations might not work) :-)
#ifdef G_OSD
#define GRAPICSENABLED
#endif
#define TEXT_ENABLED
#define TIME_ENABLED
#define ADC_ENABLED
#ifdef G_OSD
#define GPS_ENABLED
#endif //G_OSD
#ifdef TEXT_ENABLED
//#define TEXT_INVERTED_ENABLED
#endif //TEXT_ENABLED

// Battery
#define CELL_LOW_VOLTAGE 3.8
#define CELL_HIGH_VOLTAGE 4.2
#define CELL_COUNT 3

// RSSI convertion
#define RSSI_MAX_VOLTAGE 4.7
#define RSSI_MIN_VOLTAGE 1.2

//#define IMPERIAL_SYSTEM
#define METRIC_SYSTEM

// ----------------- INTERNAL CONFIGS ------------------

// Check sanity
#if (defined(E_OSD) && defined(G_OSD)) || (!defined(E_OSD) && !defined(G_OSD))
#error "Select one and only one target!"
#endif

#if (defined(IMPERIAL_SYSTEM) && defined(METRIC_SYSTEM)) || (!defined(IMPERIAL_SYSTEM) && !defined(METRIC_SYSTEM))
#error "Select one and only one system!"
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
#define TEXT_LINE_MAX_CHARS 32
#define TEXT_CHAR_HEIGHT 8
#define TEXT_LINES 2
#define TEXT_TRIG_LINES_LIST 50, 256
#define TEXT_INVERTED_OFF 0
#define TEXT_INVERTED_ON 1
#define TEXT_INVERTED_FLIP 2

#ifdef METRIC_SYSTEM
#define TEXT_LENGTH_UNIT "M"
#else // IMPERIAL_SYSTEM
#define TEXT_LENGTH_UNIT "F"
#endif

#ifdef METRIC_SYSTEM
#define TEXT_SPEED_UNIT "KM/H"
#else // IMPERIAL_SYSTEM
#define TEXT_SPEED_UNIT "MPH"
#endif

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
#define GRAPHICS_SIZE 16
#define GRAPHICS_WIDTH_REAL GRAPHICS_SIZE
#define GRAPHICS_WIDTH (GRAPHICS_SIZE/8)
#define GRAPHICS_HEIGHT GRAPHICS_SIZE
//#define GRAPHICS_DATA_SIZE GRAPHICS_WIDTH*GRAPHICS_HEIGHT
#define GRAPHICS_LINE 135
#define GRAPHICS_OFFSET 26

// Line triggering
#define LAST_LINE 0
#if TEXT_1_TRIG_LINE + TEXT_CHAR_HEIGHT * 2 > LAST_LINE
#undef LAST_LINE
#define LAST_LINE TEXT_1_TRIG_LINE + TEXT_CHAR_HEIGHT * 2
#endif
#if TEXT_2_TRIG_LINE + TEXT_CHAR_HEIGHT * 2 > LAST_LINE
#undef LAST_LINE
#define LAST_LINE TEXT_2_TRIG_LINE + TEXT_CHAR_HEIGHT * 2
#endif
#if GRAPHICS_LINE + GRAPHICS_HEIGHT * 2 > LAST_LINE
#undef LAST_LINE
#define LAST_LINE GRAPHICS_LINE + GRAPHICS_HEIGHT * 2
#endif

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

// Global vars
static volatile uint16_t line = 0;

// Debug vars!
static volatile uint8_t u8 = 0;
static volatile int8_t i8 = 0;
static volatile uint16_t u16 = 0;
static volatile int16_t i16 = 0;
static volatile uint32_t u32 = 0;
static volatile int32_t i32 = 0;
static volatile char c8 = 0;

#endif /* SETUP_H_ */