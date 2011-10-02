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


#ifndef GPS_H_
#define GPS_H_


#include <string.h>

#include "config.h"
#include "gpsutils.h"
#include "statistics.h"
#include "home.h"

#ifdef GPS_ENABLED

#define GPS_START_CHAR '$'
#define GPS_SEPARATOR_CHAR ','
#define GPS_CHECKSUM_CHAT '*'
#define GPS_TYPE_NONE 0
#define GPS_TYPE_GPGGA 1
#define GPS_TYPE_GPRMC 2

#define GPS_PART_NONE 0
#define GPS_PART_CHECKSUM 200
#define GPS_PART_FINISHED 255
// GPGGA parts
#define GPS_PART_GPGGA_TIME 1
#define GPS_PART_GPGGA_LAT 2
#define GPS_PART_GPGGA_LAT_UNIT 3
#define GPS_PART_GPGGA_LONG 4
#define GPS_PART_GPGGA_LONG_UNIT 5
#define GPS_PART_GPGGA_FIX 6
#define GPS_PART_GPGGA_SATS 7
#define GPS_PART_GPGGA_DILUTION 8
#define GPS_PART_GPGGA_ALTITUDE 9
#define GPS_PART_GPGGA_ALTITUDE_UNIT 10
#define GPS_PART_GPGGA_GEOID_HEIGHT 11
#define GPS_PART_GPGGA_GEOID_HEIGHT_UNIT 12
// GPRMC parts
#define GPS_GPRMC_PART_OFFSET 100
#define GPS_PART_GPRMC_TIME GPS_GPRMC_PART_OFFSET + 1
#define GPS_PART_GPRMC_STATUS GPS_GPRMC_PART_OFFSET + 2
#define GPS_PART_GPRMC_LAT GPS_GPRMC_PART_OFFSET + 3
#define GPS_PART_GPRMC_LAT_UNIT GPS_GPRMC_PART_OFFSET + 4
#define GPS_PART_GPRMC_LONG GPS_GPRMC_PART_OFFSET + 5
#define GPS_PART_GPRMC_LONG_UNIT GPS_GPRMC_PART_OFFSET + 6
#define GPS_PART_GPRMC_SPEED GPS_GPRMC_PART_OFFSET + 7
#define GPS_PART_GPRMC_ANGLE GPS_GPRMC_PART_OFFSET + 8
#define GPS_PART_GPRMC_DATE GPS_GPRMC_PART_OFFSET + 9
#define GPS_PART_GPRMC_MAG_VARIATION GPS_GPRMC_PART_OFFSET + 10
#define GPS_PART_GPRMC_MAG_VARIATION_UNIT GPS_GPRMC_PART_OFFSET + 11

// For debugging GPS. Might need to turn off graphics to use this.
//#define GPS_FULL_TEXT
//#define GPS_PART_TEXT
#define GPS_FULL_LENGTH 10*GPS_MAX_CHARS

// Text parsing
#ifdef GPS_FULL_TEXT
static char gGpsFullText[GPS_FULL_LENGTH];
static uint16_t gGpsFullTextPos = 0;
#endif // GPS_FULL_TEXT

static char gGpsText[GPS_MAX_CHARS];
static uint8_t gGpsTextPos = 0;
static uint8_t gGpsTextPartStep = GPS_PART_FINISHED; // Try to start on a $
static uint8_t gGpsTextType = GPS_TYPE_NONE;
static uint8_t gGpsChecksum = 0;

static TGpsData gGpsLastValidData = {};
static uint8_t gGpsValidData = 0;
static TGpsData gGpsLastData = {};
static TTime gLastFix = {};

// For debugging
#ifdef GPS_PART_TEXT
static char gGpsTextPart[GPS_MAX_CHARS];
static uint8_t gGpsTextPartLength = 0;
#endif //GPS_PART_TEXT

static void setupGps() {
  // USART setup
	UCSR0B = (1<<RXEN0); // Enable RX
	UCSR0B |= (1<<UCSZ02); // 8 bits
	UBRR0H = (uint8_t)(GPS_UBRR>>8); // set baud
	UBRR0L = (uint8_t)GPS_UBRR;
}

static void clearGpsText() {
	for (uint8_t i = 0; i < GPS_MAX_CHARS; ++i) {
		gGpsText[i] = 0;
	}
}

#ifdef GPS_FULL_TEXT
static void clearFullGpsText() {
	for (uint8_t i = 0; i < GPS_FULL_LENGTH; ++i) {
		gGpsFullText[i] = 0;
	}
}
#endif // GPS_FULL_TEXT

#ifdef GPS_PART_TEXT
static void updateParts() {
	strncpy((char*)gGpsTextPart, (char*)gGpsText, GPS_MAX_CHARS);
	gGpsTextPartLength = strlen((const char*)gGpsText);
}
#endif //GPS_PART_TEXT

static void parseGpsPart() {
	if (gGpsTextType == GPS_TYPE_NONE) {
		if (!strncmp((const char*)gGpsText, "GPGGA", 5)) {
			gGpsTextType = GPS_TYPE_GPGGA;
			gGpsTextPartStep = GPS_PART_NONE;
			//updateParts();
		}
		else if (!strncmp((const char*)gGpsText, "GPRMC", 5)) {
			gGpsTextType = GPS_TYPE_GPRMC;
			gGpsTextPartStep = GPS_GPRMC_PART_OFFSET;
			//updateParts();
		}
	}
	else if (gGpsTextType != GPS_TYPE_NONE) {
		if (gGpsTextPos != 0) {
			switch (gGpsTextPartStep) {
			case GPS_PART_GPGGA_TIME:
			case GPS_PART_GPRMC_TIME:
				gGpsLastData.gTime = parseInt(gGpsText, GPS_MAX_CHARS);
				//updateParts();
				break;
			case GPS_PART_GPGGA_LAT:
			case GPS_PART_GPRMC_LAT:
				gGpsLastData.pos.latitude = parseFloat(gGpsText, GPS_MAX_CHARS);
				//updateParts();
				break;
			case GPS_PART_GPGGA_LAT_UNIT:
			case GPS_PART_GPRMC_LAT_UNIT:
				if (gGpsText[0] == 'S') {
					gGpsLastData.pos.latitude = -gGpsLastData.pos.latitude;
				}
				break;
			case GPS_PART_GPGGA_LONG:
			case GPS_PART_GPRMC_LONG:
				gGpsLastData.pos.longitude = parseFloat(gGpsText, GPS_MAX_CHARS);
				//updateParts();
				break;
			case GPS_PART_GPGGA_LONG_UNIT:
			case GPS_PART_GPRMC_LONG_UNIT:
				if (gGpsText[0] == 'W') {
					gGpsLastData.pos.longitude = -gGpsLastData.pos.longitude;
				}
				//updateParts();
				break;
			case GPS_PART_GPGGA_FIX:
				gGpsLastData.fix = parseInt(gGpsText, GPS_MAX_CHARS);
				//updateParts();
				break;
			case GPS_PART_GPRMC_STATUS:
				// Status
				break;
			case GPS_PART_GPGGA_SATS:
				gGpsLastData.sats = parseInt(gGpsText, GPS_MAX_CHARS);
				//updateParts();
				break;
			case GPS_PART_GPGGA_DILUTION:
				// Horizontal dilution of position
				break;
			case GPS_PART_GPGGA_ALTITUDE:
				gGpsLastData.pos.altitude = parseInt(gGpsText, GPS_MAX_CHARS);
#ifdef IMPERIAL_SYSTEM
        meterToFeet(&gGpsLastData.pos.altitude);
#endif // IMPERIAL_SYSTEM
				//updateParts();
				break;
			case GPS_PART_GPGGA_ALTITUDE_UNIT:
				// Altitude unit TODO!
				break;
			case GPS_PART_GPGGA_GEOID_HEIGHT:
				// If missing altitude wrong?
				break;
			case GPS_PART_GPGGA_GEOID_HEIGHT_UNIT:
				// Geoid unit
				break;
			case GPS_PART_GPRMC_SPEED:
				gGpsLastData.speed = parseInt(gGpsText, GPS_MAX_CHARS); // Only use int part
#ifdef METRIC_SYSTEM
				// Convert to km/h. 1 knot = 1.852 km/h = 463/250
				gGpsLastData.speed *= 463; // Might need bigger var if you go really fast! :-)
				gGpsLastData.speed /= 250;
#else // IMPERIAL_SYSTEM
        knotToMph(&gGpsLastData.speed);
#endif // METRIC_SYSTEM
				break;
			case GPS_PART_GPRMC_ANGLE:
				gGpsLastData.angle = parseInt(gGpsText, GPS_MAX_CHARS); // Only use int part
				break;
			case GPS_PART_GPRMC_DATE:
			  gGpsLastData.date = parseInt(gGpsText, GPS_MAX_CHARS);
			  break;
			case GPS_PART_CHECKSUM:
				//updateParts();
				{
				uint8_t val = parseHex(gGpsText, GPS_MAX_CHARS);
				gGpsLastData.checksumValid = (val == gGpsChecksum);
				}				
				break;
			}				
		}			
	}
}

static void setHomePos() {
#ifdef STATISTICS_ENABLED	
	resetStatistics();
#endif //STATISTICS_ENABLED
	gHomePos = gGpsLastValidData.pos;
	gHomePosSet = 1;
}

#ifdef STATISTICS_ENABLED
static void updateDistanceTraveled() {
	TGpsPos last = gGpsLastValidData.pos;
	TGpsPos current = gGpsLastData.pos;
	if (last.latitude != current.latitude || last.longitude != current.longitude) {
	  uint32_t distance;
    calcHome(last.latitude, last.longitude, current.latitude, current.longitude, &distance, NULL);
	  gStatDistTraveled += distance;
	}	  
}
#endif //STATISTICS_ENABLED

static void finishGpsDecoding() {
	if (gGpsLastData.checksumValid != 0) {
#ifdef STATISTICS_ENABLED
		updateDistanceTraveled();
#endif //STATISTICS_ENABLED
		gGpsLastValidData = gGpsLastData;
		gGpsValidData = 1;
		gLastFix = gTime;

		if (gHomePosSet == 0) {
			if (gGpsLastValidData.fix != 0) {
#ifdef HOME_SET_AT_FIX
        if (gHomeFixCount >= HOME_SET_FIX_COUNT) {
			    if (gGpsLastValidData.sats >= HOME_SET_MIN_SATS) {
			      setHomePos();
				  }				  
	      }
		    else {
			    ++gHomeFixCount;
		    }
#endif //HOME_FIRST_FIX
#ifdef HOME_AUTO_SET
        if (gGpsLastValidData.speed >= HOME_FIX_MIN_SPEED) {
			    setHomePos();
		    }
#endif //HOME_AUTO_SET
      }
		}	  
#ifdef STATISTICS_ENABLED		
		else {
			if (gGpsLastValidData.speed > gStatMaxSpeed) {
          gStatMaxSpeed = gGpsLastValidData.speed;
			}
			if (gGpsLastValidData.pos.altitude - gHomePos.altitude > gStatMaxAltitude) {
          gStatMaxAltitude = gGpsLastValidData.pos.altitude - gHomePos.altitude;
			}
		}
#endif //STATISTICS_ENABLED
	}		  
}

static void decodeGpsData(char data) {
	if (gGpsTextPartStep == GPS_PART_FINISHED && data != '$') {
		return;
	}
	
	switch (data) {
	case '$':
		gGpsTextPos = 0;
		gGpsTextType = GPS_TYPE_NONE;
		gGpsChecksum = 0;
		gGpsTextPartStep = 0;
		clearGpsText();
#ifdef GPS_FULL_TEXT
		clearFullGpsText();
		gGpsFullTextPos = 0;
#endif // FULLGPSTEXT
		break;
	case ',':
		gGpsChecksum ^= data;
	case '*':
		parseGpsPart();
		clearGpsText();		
		gGpsTextPos = 0;
		gGpsTextPartStep++;
		if (data =='*') {
			gGpsTextPartStep = GPS_PART_CHECKSUM;
		}			
		break;
	case '\r':
	case '\n':
	  parseGpsPart();
	  gGpsTextPartStep = GPS_PART_FINISHED;
	  finishGpsDecoding();
		return;
		break;
	default:
		gGpsText[gGpsTextPos] = data;
		if (gGpsTextPartStep != GPS_PART_CHECKSUM) {
		  gGpsChecksum ^= data;
		}
		if (gGpsTextPos < GPS_MAX_CHARS-1) {
		  ++gGpsTextPos;
		}
		break;
	}
	
#ifdef GPS_FULL_TEXT
	gGpsFullText[gGpsFullTextPos] = data;
	if (gGpsFullTextPos + 1 != GPS_FULL_LENGTH) {
	  gGpsFullTextPos++;
	}
#endif // FULLGPSTEXT
}

#endif //GPS_ENABLED

#endif /* GPS_H_ */