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

#include "hardware.h"
#include <avr/io.h>

// ------------------ MAIN CONFIGS -------------------------------------------------

// ----------- MAIN FEATURES --------------
#ifdef HARDWARE_SUPPORT_GPS
#define GPS_ENABLED
#endif //HARDWARE_SUPPORT_GPSE
#ifdef GPS_ENABLED
#define GRAPICSENABLED
#endif //GPS_ENABLED
#define TEXT_ENABLED
#define TIME_ENABLED
#define ADC_ENABLED
#define STATISTICS_ENABLED
#define ALARM_ENABLED
#define SENSORS_ENABLED

// ----------- ALARMS --------------
//Alarms (Comment to disable)
#define ALARM_BATT1_LOW 5.00
#define ALARM_BATT2_LOW 0.00
//#define ALARM_RSSI_LOW 50
#define ALARM_SPEED_HIGH 200
#define ALARM_ALTITUDE_LOW 0
#define ALARM_ALTITUDE_HIGH 5000
#define ALARM_DISTANCE_HIGH 10000

// ----------- SENSORS --------------
// Enabled sensors
//#define SENSOR_BATTERY_PERCENTAGE_ENABLED
#define SENSOR_RSSI_ENABLED
//#define SENSOR_COMPASS_ENABLED
//#define SENSOR_CURRENT_ENABLED

// Battery percentage sensor
#ifdef SENSOR_BATTERY_PERCENTAGE_ENABLED
#define SENSOR_BATTERY_PERCENTAGE_INPUT ANALOG_IN_1
#define SENSOR_CELL_LOW_VOLTAGE 3.8 //(Max two digits after the dot)
#define SENSOR_CELL_HIGH_VOLTAGE 4.2
#define SENSOR_CELL_COUNT 3
#endif //SENSOR_BATTERY_PERCENTAGE_ENABLED

// RSSI sensor
#ifdef SENSOR_RSSI_ENABLED
#ifdef ANALOG_IN_3
#define SENSOR_RSSI_INPUT ANALOG_IN_3
#else
#define SENSOR_RSSI_INPUT ANALOG_IN_2
#endif
#define SENSOR_RSSI_MAX_VOLTAGE 5.0 //(Max two digits after the dot)
#define SENSOR_RSSI_MIN_VOLTAGE 1.2
#endif //SENSOR_RSSI_ENABLED

// Compass sensor
#ifdef SENSOR_COMPASS_ENABLED
#define SENSOR_COMPASS_INPUT ANALOG_IN_2
#define SENSOR_COMPASS_MIN_VOLTAGE 0.00 //(Max two digits after the dot)
#define SENSOR_COMPASS_MAX_VOLTAGE 5.00
#endif

// Current sensor
#define SENSOR_CURRENT_INPUT ANALOG_IN_2
#define SENSOR_CURRENT_MIN_VOLTAGE 0.00 //(Max two digits after the dot)
#define SENSOR_CURRENT_MAX_VOLTAGE 5.00
#define SENSOR_CURRENT_MAX_AMPS 100 //AMP

// ----------- OTHER --------------

#ifdef TEXT_ENABLED
//#define TEXT_INVERTED_ENABLED
//#define TEXT_SMALL_ENABLED
#define TEXT_USE_SPECIAL_CHARS
#endif //TEXT_ENABLED

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
//#define GPS_DIYD //Use DIYD/APM binary protocol (Thanks to David Collett)

//Pre and post flight info
#define INFO_MIN_SPEED_SHOW 3

//User data
#define TEXT_CALLSIGN ""

//Time
//#define TIME_HOUR_ENABLED

// Color system (Thanks to chatch15117)
//#define COLORSYSTEM_NTSC

// ----------------- INTERNAL CONFIGS ---------------------------------------------

// ----------- CHECK SANITY --------------
#if (defined(IMPERIAL_SYSTEM) && defined(METRIC_SYSTEM)) || (!defined(IMPERIAL_SYSTEM) && !defined(METRIC_SYSTEM))
#error "Select one and only one unit system!"
#endif

// ----------- TEXT --------------
#ifndef TEXT_SMALL_ENABLED
#ifdef COLORSYSTEM_NTSC
#define TEXT_LINE_MAX_CHARS 32
#else //PAL
#define TEXT_LINE_MAX_CHARS 33
#endif //COLORSYSTEM_NTSC
#else
#define TEXT_LINE_MAX_CHARS 30
#endif //TEXT_SMALL_ENABLED
#define TEXT_CHAR_HEIGHT 8

#define TEXT_LINES 6
#ifdef COLORSYSTEM_NTSC
#define TEXT_TRIG_LINES_LIST 25, 50, 135, 180, 210, 235 // Must be rising line numbers!
#else //PAL
#define TEXT_TRIG_LINES_LIST 50, 66, 170, 205, 255, 280 // Must be rising line numbers!
#endif //COLORSYSTEM_NTSC

#define TEXT_INVERTED_OFF 0
#define TEXT_INVERTED_ON 1
#define TEXT_INVERTED_FLIP 2

#ifndef TEXT_SMALL_ENABLED
#define TEXT_SIZE_LARGE_MULT 2
#define TEXT_SIZE_SMALL_MULT 1
#define TEXT_LINE_TEXT_SIZES TEXT_SIZE_SMALL_MULT, TEXT_SIZE_LARGE_MULT, TEXT_SIZE_SMALL_MULT, TEXT_SIZE_SMALL_MULT, TEXT_SIZE_LARGE_MULT, TEXT_SIZE_SMALL_MULT
#endif //TEXT_SMALL_ENABLED

// ----------- GRAPHICS --------------
#define GRAPHICS_SIZE 24 // Multiple of 8
#define GRAPHICS_WIDTH_REAL GRAPHICS_SIZE
#define GRAPHICS_WIDTH (GRAPHICS_SIZE/8)
#define GRAPHICS_HEIGHT GRAPHICS_SIZE
#ifdef COLORSYSTEM_NTSC
#define GRAPHICS_LINE 80
#define GRAPHICS_OFFSET 45
#else //PAL
#define GRAPHICS_LINE 105
#define GRAPHICS_OFFSET 46
#endif //COLORSYSTEM_NTSC
#define GRAPHICS_MID (GRAPHICS_SIZE/2)-1

// ----------- UPDATE LINE --------------
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#ifdef COLORSYSTEM_NTSC
#define UPDATE_LINE 85
#else //PAL
#define UPDATE_LINE 110
#endif //COLORSYSTEM_NTSC

// ----------- TEXT UNITS --------------
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

// ----------- SENSORS --------------

// Battery
#define BATT_MIN_VOLTAGE SENSOR_CELL_LOW_VOLTAGE*SENSOR_CELL_COUNT
#define BATT_MAX_VOLTAGE SENSOR_CELL_HIGH_VOLTAGE*SENSOR_CELL_COUNT
#define BATT_MIN_VOLTAGE_INT (uint16_t)(BATT_MIN_VOLTAGE*100)
#define BATT_MAX_VOLTAGE_INT (uint16_t)(BATT_MAX_VOLTAGE*100)

// RSSI conversion
#define RSSI_MIN_VOLTAGE_INT (uint16_t)(SENSOR_RSSI_MIN_VOLTAGE*100)
#define RSSI_MAX_VOLTAGE_INT (uint16_t)(SENSOR_RSSI_MAX_VOLTAGE*100)

#define COMPASS_MIN_VOLTAGE_INT (uint16_t)(SENSOR_COMPASS_MIN_VOLTAGE*100)
#define COMPASS_MAX_VOLTAGE_INT (uint16_t)(SENSOR_COMPASS_MAX_VOLTAGE*100)

#define CURRENT_MIN_VOLTAGE_INT (uint16_t)(SENSOR_CURRENT_MIN_VOLTAGE*100)
#define CURRENT_MAX_VOLTAGE_INT (uint16_t)(SENSOR_CURRENT_MAX_VOLTAGE*100)
#define SENSOR_CURRENT_MAX_AMPS 100 //AMP

// ----------- GPS --------------
#define GPS_BAUD 4800
#define GPS_UBRR (F_CPU/16/GPS_BAUD-1)
#define GPS_MAX_CHARS 11

// ----------- OTHER --------------

// Alarm conversion
#define ALARM_BATT1_LOW_INT (uint16_t)(ALARM_BATT1_LOW*100)
#define ALARM_BATT2_LOW_INT (uint16_t)(ALARM_BATT2_LOW*100)

// CPU speed
#define F_CPU 24000000UL

//Other
#define NULL 0


#endif /* SETUP_H_ */