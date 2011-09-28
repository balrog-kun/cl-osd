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
//#define E_OSD_GPS //Note: Also enable E_OSD!

// Feature setup. (All combinations might not work) :-)
#if defined(G_OSD) || defined(E_OSD_GPS)
#define GPS_ENABLED
#endif //defined(G_OSD) || defined(E_OSD_GPS)
#ifdef GPS_ENABLED
#define GRAPICSENABLED
#endif //GPS_ENABLED
#define TEXT_ENABLED
#define TIME_ENABLED
#define ADC_ENABLED
//#define STATISTICS_ENABLED

#ifdef TEXT_ENABLED
//#define TEXT_INVERTED_ENABLED
//#define TEXT_SMALL_ENABLED
#endif //TEXT_ENABLED

// Battery
#define CELL_LOW_VOLTAGE 3.8 //(Max two digits after the dot)
#define CELL_HIGH_VOLTAGE 4.2
#define CELL_COUNT 3

// RSSI convertion 
#define RSSI_MAX_VOLTAGE 4.7 //(Max two digits after the dot)
#define RSSI_MIN_VOLTAGE 1.2

// Unit system
//#define IMPERIAL_SYSTEM
#define METRIC_SYSTEM

// Home set (Use at least one of the first two)
//#define HOME_SET_AT_FIX
#define HOME_AUTO_SET
#define HOME_SET_WITH_BUTTON
#define HOME_SET_FIX_COUNT 10
#define HOME_FIX_MIN_SPEED 10
#define HOME_SET_MIN_SATS 4

//GPS
//#define GPS_GOOGLE_FORMAT

//User data
#define TEXT_CALLSIGN "CLJ"

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
#else
#define KEY (1<<PD4)
#define LED (1<<PD3)
#endif //GOSD

#define OUT1 (1<<PB1)
#define OUT2 (1<<PB3)

#define LTRIG (1<<PD2) //INT0
#define SS (1<<PB2)

// Text
#ifndef TEXT_SMALL_ENABLED
#define TEXT_LINE_MAX_CHARS 33
#else
#define TEXT_LINE_MAX_CHARS 30
#endif //TEXT_SMALL_ENABLED
#define TEXT_CHAR_HEIGHT 8

#define TEXT_LINES 6
#define TEXT_TRIG_LINES_LIST 50, 50+16, 160, 185, 255, 280 // Must be rising line numbers!

#define TEXT_INVERTED_OFF 0
#define TEXT_INVERTED_ON 1
#define TEXT_INVERTED_FLIP 2

#ifndef TEXT_SMALL_ENABLED
#define TEXT_SIZE_LARGE_MULT 2
#define TEXT_SIZE_SMALL_MULT 1
#define TEXT_LINE_TEXT_SIZES TEXT_SIZE_SMALL_MULT, TEXT_SIZE_LARGE_MULT, TEXT_SIZE_SMALL_MULT, TEXT_SIZE_SMALL_MULT, TEXT_SIZE_LARGE_MULT, TEXT_SIZE_SMALL_MULT
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
#ifdef E_OSD
#define ANALOG_IN_1 1 // Voltage 1 = ADC1
#define ANALOG_IN_2 0 // Voltage 2 = ADC0
#else
#define ANALOG_IN_1 0 // Voltage 1 = ADC0
#define ANALOG_IN_2 1 // Voltage 2 = ADC1
#endif
#define ANALOG_IN_3 2 // RSSI = ADC2 (only 0-5 Volt!)
#define ANALOG_IN_4 3 // No input, just a pot = ADC3

#define ADC_OFFSET 0

#ifdef E_OSD
#define ADC_MULT 1,1
#define ADC_DIV 1,1
#else
#define ADC_MULT 1,1,1,1
#define ADC_DIV 1,1,1,1
#endif //E_OSD

#ifdef E_OSD
#define ANALOG_IN_NUMBER 2
#else
#define ANALOG_IN_NUMBER 4
#endif //E_OSD

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
#define GRAPHICS_OFFSET 46

// Line triggering
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define UPDATE_LINE 120 //240+25+16+2

// GPS
#define GPS_BAUD 4800
#define GPS_UBRR (F_CPU/16/GPS_BAUD-1)
#define GPS_MAX_CHARS 11

// CPU speed
#define F_CPU 24000000UL

//Other
#define NULL 0


#endif /* SETUP_H_ */