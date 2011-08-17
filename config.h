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

// CPU speed
#define F_CPU 24000000UL

// Select one of these depending on your board. 
// Selecting the wrong type might damage the board.
//#define E_OSD
#define G_OSD

// Check sanity
#ifdef E_OSD
#ifdef G_OSD
#error "Cannot use both e-osd and g-osd at the same time!"
#endif
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

// For debugging. (All combinations might not work) :-)
#define GRAPICSENABLED
#define TEXT_ENABLED
#define TIME_ENABLED
#define ADC_ENABLED
#ifdef G_OSD
#define GPS_ENABLED
#endif //G_OSD

// Text
#define TEXT_MAX_CHARS 32
#define TEXT_CHAR_HEIGHT 8
#define TEXT_1_LINE 50
#define TEXT_2_LINE 265
#define TEXT_LINES 2
#define TEXT_LINES_LIST TEXT_1_LINE, TEXT_2_LINE

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
#define CELL_LOW_VOLTAGE 3.8
#define CELL_HIGH_VOLTAGE 4.2
#define CELL_COUNT 3
#define BATT_MIN_VOLTAGE CELL_LOW_VOLTAGE*CELL_COUNT
#define BATT_MAX_VOLTAGE CELL_HIGH_VOLTAGE*CELL_COUNT
#define BATT_MIN_VOLTAGE_INT (uint16_t)(BATT_MIN_VOLTAGE*100)
#define BATT_MAX_VOLTAGE_INT (uint16_t)(BATT_MAX_VOLTAGE*100)

// Graphics
#define GRAPHICS_SIZE 32
#define GRAPHICS_WIDTH_REAL GRAPHICS_SIZE
#define GRAPHICS_WIDTH (GRAPHICS_SIZE/8)
#define GRAPHICS_HEIGHT GRAPHICS_SIZE
#define GRAPHICS_DATA_SIZE GRAPHICS_WIDTH*GRAPHICS_HEIGHT
#define GRAPICS_LINE 135
#define GRAPICS_OFFSET 26

// Line triggering
#define LAST_LINE 0
#if TEXT_1_LINE + TEXT_CHAR_HEIGHT * 2 > LAST_LINE
#undef LAST_LINE
#define LAST_LINE TEXT_1_LINE + TEXT_CHAR_HEIGHT * 2
#endif
#if TEXT_2_LINE + TEXT_CHAR_HEIGHT * 2 > LAST_LINE
#undef LAST_LINE
#define LAST_LINE TEXT_2_LINE + TEXT_CHAR_HEIGHT * 2
#endif
#if GRAPICS_LINE + GRAPHICS_HEIGHT * 2 > LAST_LINE
#undef LAST_LINE
#define LAST_LINE GRAPICS_LINE + GRAPHICS_HEIGHT * 2
#endif

#define LINE_TYPE_UNKNOWN 0
#define LINE_TYPE_TEXT 1
#define LINE_TYPE_GRAPICS 2

// GPS
#define GPS_BAUD 4800
#define GPS_UBRR (F_CPU/16/GPS_BAUD-1)
#define GPS_MAX_CHARS 15

// Global vars
static volatile uint16_t line = 0;

#endif /* SETUP_H_ */