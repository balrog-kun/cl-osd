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

//NOTE: Most options can be disabled by commenting them out.
//This is done by inserting // at the start of the line.
//Some options depend on others so you might get an error if you mess around too much. :-)

// ------------------ MAIN CONFIGS -------------------------------------------------

// ----------- MAIN FEATURES --------------
//(Comment to disable)
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
#define ALARM_BATT1_LOW 5.00 //Warn if below this level (in volt)
#define ALARM_BATT2_LOW 0.00 //Warn if below this level (in volt)
//#define ALARM_RSSI_LOW 50 //Warn if below this level (in %)
#define ALARM_SPEED_HIGH 200 //Warn if above this level (in kmph/mph)
#define ALARM_ALTITUDE_LOW 0 //Warn if below this level (in meters/feet)
#define ALARM_ALTITUDE_HIGH 5000 //Warn if above this level (in meters/feet)
#define ALARM_DISTANCE_HIGH 10000 //Warn if below this level (in meters/feet)
#define ALARM_CURRENT_HIGH 50 //Warn if above this level (in Ampere)
#define ALARM_POWER_USAGE_HIGH 5000 //Warn if above this level (in mAh)

// ----------- SENSORS --------------
// Enabled sensors
//#define SENSOR_BATTERY_PERCENTAGE_ENABLED
#define SENSOR_RSSI_ENABLED
//#define SENSOR_COMPASS_ENABLED
//#define SENSOR_CURRENT_ENABLED

// Battery voltage sensor
#define SENSOR_VOLTAGE_1_ENABLED
//#define SENSOR_VOLTAGE_2_ENABLED

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
//#define SENSOR_RSSI_REVERSED // If a low voltage value means high signal strength and vice versa, then enable this. (Thanks BlueAngel2000)
#endif //SENSOR_RSSI_ENABLED

// Compass sensor
#ifdef SENSOR_COMPASS_ENABLED
#define SENSOR_COMPASS_INPUT ANALOG_IN_2
#define SENSOR_COMPASS_MIN_VOLTAGE 0.00 //(Max two digits after the dot)
#define SENSOR_COMPASS_MAX_VOLTAGE 5.00
#endif

// Current sensor (Settings for ACS758LCB-100U-PFF-T, might need to tweak a bit.)
#ifdef SENSOR_CURRENT_ENABLED
// Peacefullhill says to use analog 2 since RSSI is less stable. Not tested by me.
#ifdef ANALOG_IN_3
#define SENSOR_CURRENT_INPUT ANALOG_IN_3
#else
#define SENSOR_CURRENT_INPUT ANALOG_IN_2
#endif

// If your sensor is defined in mV/A and a 0A value us this calculation: (Thanks to kai)
// Max amp = Amp at max level.
// Min voltage = 0A voltage.
// Max voltage = (0A voltage) + (max amp) * ((mV/A) / 1000) = Max voltage
// For example we have a sensor with 40mV/A and 0.6V at 0A with max 100 Amps:
// Max amp = 100, Min voltage = 0.60, Max value = 0.6 + 100 * (40 / 1000) = 4.60
#define SENSOR_CURRENT_MIN_VOLTAGE 0.50 //(Max two digits after the dot)
#define SENSOR_CURRENT_MAX_VOLTAGE 4.00
#define SENSOR_CURRENT_MAX_AMPS 100 //AMP
#endif

// ----------- OTHER --------------

#ifdef TEXT_ENABLED
//#define TEXT_INVERTED_ENABLED //Make text transparent and background white.
//#define TEXT_SMALL_ENABLED //Enable small text. Might be buggy!
#define TEXT_USE_SPECIAL_CHARS //Use chars with non-ascii symbols.
//#define TEXT_COMPASS_ENABLED //Enable compass when no statistics is shown.
#endif //TEXT_ENABLED

// Unit system
//#define IMPERIAL_SYSTEM //Feet, mph etc...
#define METRIC_SYSTEM //Meter, hm/h etc...

// Home pos set
// Note: Use at least one or you will never get a home pos!
// Comment out unwanted to disable
//#define HOME_SET_AT_FIX // Home position is set when GPS gets satellited fix.
#define HOME_AUTO_SET //Home position is set when a certain speed is exceeded.
#define HOME_SET_WITH_BUTTON //Home position is set when the little button on OSD is long pressed.
#define HOME_SET_FIX_COUNT 10 //Config for _SET_AT_FIX: After 10 successfully fixes, home is set.
#define HOME_FIX_MIN_SPEED 10 //Config for _AUTO_SET: More than 10 km/h sets home.
#define HOME_SET_MIN_SATS 4 //Config for _SET_AT_FIX: Set home only when more than 4 satellites. (Thanks to Yury Smirnov)

//GPS
//#define GPS_GOOGLE_FORMAT
//#define GPS_DIYD //Use DIYD/APM binary protocol (Thanks to David Collett)

//Pre and post flight info
#define STATISTICS_MIN_SPEED_SHOW 3 //Under this speed the statistics will show.
#define STATISTICS_DELAY_SHOW 10 //It takes ten secs before the statistics is shown. (Higher is better for multicopters!)

//User data
#define TEXT_CALL_SIGN "" //Set this to your call sign.

//Time
//#define TIME_HOUR_ENABLED //Enable time to show and count hours.

// Color system (Thanks to chatch15117)
//#define COLORSYSTEM_NTSC

//BUG fix for GOSDII GPS (sends km/h instead of knots)
//#define GOSDII_GPS_SPEED_BUGFIX

//Screen and sensor refresh rate (Don't set too high!)
#define SCREEN_AND_SENSOR_UPDATES_PER_SEC 1

// ----------------- INTERNAL CONFIGS ---------------------------------------------

// ----------- CHECK SANITY --------------
#if (defined(IMPERIAL_SYSTEM) && defined(METRIC_SYSTEM)) || (!defined(IMPERIAL_SYSTEM) && !defined(METRIC_SYSTEM))
#error "Select one and only one unit system!"
#endif

#if (defined(SENSOR_VOLTAGE_2_ENABLED) && (ANALOG_IN_NUMBER <= 2) && defined(SENSOR_RSSI_ENABLED))
#error "Can't use both RSSI and voltage 2 at the same time on this board."
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

// Compass
#define COMPASS_MIN_VOLTAGE_INT (uint16_t)(SENSOR_COMPASS_MIN_VOLTAGE*100)
#define COMPASS_MAX_VOLTAGE_INT (uint16_t)(SENSOR_COMPASS_MAX_VOLTAGE*100)

// Current sensor
#define CURRENT_MIN_VOLTAGE_INT (uint16_t)(SENSOR_CURRENT_MIN_VOLTAGE*100)
#define CURRENT_MAX_VOLTAGE_INT (uint16_t)(SENSOR_CURRENT_MAX_VOLTAGE*100)

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