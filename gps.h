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

#include "config.h"

#include <string.h>

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
#define GPS_PART_GPRMC_FIX GPS_GPRMC_PART_OFFSET + 2
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
static volatile char gpsFullText[GPS_FULL_LENGTH];
static volatile uint16_t gpsFullTextPos = 0;
#endif // GPS_FULL_TEXT

static volatile char gpsText[GPS_MAX_CHARS];
static volatile uint8_t gpsTextPos = 0;
static volatile uint8_t gpsTextPartStep = GPS_PART_FINISHED; // Try to start on a $
static volatile uint8_t gpsTextType = GPS_TYPE_NONE;
static volatile uint8_t gpsChecksum = 0;
static volatile uint8_t gpsChecksum1 = 0;

// Text data
static volatile uint32_t gpsTime = 0;
static volatile int32_t gpsLat = 0;
static volatile int32_t gpsLong = 0;
static volatile uint8_t gpsFix = 0;
static volatile uint8_t gpsSats = 0;
static volatile uint16_t gpsAltitude = 0;
static volatile uint16_t gpsSpeed = 0;
static volatile uint16_t gpsAngle = 0;
static volatile uint32_t gpsDate = 0;
static volatile uint16_t gpsChecksumValid = 0;

// For debugging
#ifdef GPS_PART_TEXT
static volatile char gpsTextPart[GPS_MAX_CHARS];
static volatile uint8_t gpsTextPartLength = 0;
#endif //GPS_PART_TEXT

static void setupGpsInterrupt() {
  // USART setup
	UCSR0B = (1<<RXEN0) | (1<<RXCIE0); // Enable RX and RX interrupt
	UCSR0B |= (1<<UCSZ02); // 8 bits
	UBRR0H = (uint8_t)(GPS_UBRR>>8); // set baud
	UBRR0L = (uint8_t)GPS_UBRR;
}

static void setupGps() {
  // USART setup
	UCSR0B = (1<<RXEN0); // Enable RX
	UCSR0B |= (1<<UCSZ02); // 8 bits
	UBRR0H = (uint8_t)(GPS_UBRR>>8); // set baud
	UBRR0L = (uint8_t)GPS_UBRR;
}

static void clearGpsText() {
	for (uint8_t i = 0; i < GPS_MAX_CHARS; ++i) {
		gpsText[i] = 0;
	}
}

#ifdef GPS_FULL_TEXT
static void clearFullGpsText() {
	for (uint8_t i = 0; i < GPS_FULL_LENGTH; ++i) {
		gpsFullText[i] = 0;
	}
}
#endif // GPS_FULL_TEXT

inline uint8_t isDigit(char c) {
	if (c >= '0' && c <= '9') {
		return 1;
	}
	return 0;
}

static int32_t parseInt() {
	uint8_t i = 0;
	uint32_t decimal = 0;
	uint8_t neg = 0;
	if (gpsText[0] == '-') {
		++i;
		neg = 1;
	}
	while (isDigit(gpsText[i]) && i <= GPS_MAX_CHARS) {
		decimal *= 10;
		decimal += (gpsText[i]) - '0';
		++i;		
	}
	return decimal * (neg ? -1 : 1);
}

static int32_t parseFloat() {
	
	uint32_t val = 0;
	uint8_t neg = 0;
	uint8_t i = 0;
	if (gpsText[0] == '-') {
		++i;
		neg = 1;
	}
	for (; i <= GPS_MAX_CHARS; ++i) {
		if (isDigit(gpsText[i])) {
			val *= 10;
			val += (gpsText[i]) - '0';
		}
		else if (gpsText[i] != '.') {
			break;
		}			
	}
	return val * (neg ? -1 : 1);
}

static uint8_t parseHex() {
	uint8_t i = 0;
	uint32_t val = 0;
	while (gpsText[i] != 0 && i <= GPS_MAX_CHARS) {
		val *= 16;
		if (gpsText[i] >= 'A' && gpsText[i] <= 'F') {
			val += 10 + gpsText[i] - 'A';
		}
		else if (gpsText[i] >= 'a' && gpsText[i] <= 'f') {
			val += 10 + gpsText[i] - 'a';
		}
		else if (gpsText[i] >= '0' && gpsText[i] <= '9') {
			val += gpsText[i] - '0';
		}
		++i;		
	}
	return val;
}

static void updateParts() {
#ifdef GPS_PART_TEXT
	strncpy((char*)gpsTextPart, (char*)gpsText, GPS_MAX_CHARS);
#endif //GPS_PART_TEXT
}

static void parseGpsPart() {
	if (gpsTextType == GPS_TYPE_NONE) {
		if (!strncmp((const char*)gpsText, "GPGGA", 5)) {
			gpsTextType = GPS_TYPE_GPGGA;
			gpsTextPartStep = GPS_PART_NONE;
			//updateParts();
		}
		else if (!strncmp((const char*)gpsText, "GPRMC", 5)) {
			gpsTextType = GPS_TYPE_GPRMC;
			gpsTextPartStep = GPS_GPRMC_PART_OFFSET;
			//updateParts();
		}
	}
	else if (gpsTextType != GPS_TYPE_NONE) {
		if (gpsTextPos != 0) {
			switch (gpsTextPartStep) {
			case GPS_PART_GPGGA_TIME:
			case GPS_PART_GPRMC_TIME:
				//updateParts();
				gpsTime = parseInt();
				break;
			case GPS_PART_GPGGA_LAT:
			case GPS_PART_GPRMC_LAT:
				gpsLat = parseFloat();
				//updateParts();
				break;
			case GPS_PART_GPGGA_LAT_UNIT:
			case GPS_PART_GPRMC_LAT_UNIT:
				if (gpsText[0] == 'S') {
					gpsLat = -gpsLat;
				}
				break;
			case GPS_PART_GPGGA_LONG:
			case GPS_PART_GPRMC_LONG:
				gpsLong = parseFloat();
				//updateParts();
				break;
			case GPS_PART_GPGGA_LONG_UNIT:
			case GPS_PART_GPRMC_LONG_UNIT:
				if (gpsText[0] == 'W') {
					gpsLat = -gpsLat;
				}
				//updateParts();
				break;
			case GPS_PART_GPGGA_FIX:
				gpsFix = parseInt();
				//updateParts();
				break;
			case GPS_PART_GPRMC_FIX:
				gpsFix = (gpsText[0] == 'A');
				break;
			case GPS_PART_GPGGA_SATS:
				gpsSats = parseInt();
				//updateParts();
				break;
			case GPS_PART_GPGGA_DILUTION:
				// Horizontal dilution of position
				break;
			case GPS_PART_GPGGA_ALTITUDE:
				gpsAltitude = parseInt();
				//updateParts();
				break;
			case GPS_PART_GPGGA_ALTITUDE_UNIT:
				// Altitude unit
				break;
			case GPS_PART_GPGGA_GEOID_HEIGHT:
				// If missing altitude wrong?
				break;
			case GPS_PART_GPGGA_GEOID_HEIGHT_UNIT:
				// Geoid unit
				break;
			case GPS_PART_GPRMC_SPEED:
				gpsSpeed = parseInt(); // Only use int part
				// Convert to km/h. 1 knot = 1.852 km/h = 463/250
				gpsSpeed *= 463; // Might need bigger var if you go really fast! :-)
				gpsSpeed /= 250;
				break;
			case GPS_PART_GPRMC_ANGLE:
				gpsAngle = parseInt(); // Only use int part
				break;
			case GPS_PART_GPRMC_DATE:
			  gpsDate = parseInt();
			  break;
			case GPS_PART_CHECKSUM:
				updateParts();
				uint8_t val = parseHex();
				gpsChecksum1 = val;
				gpsChecksumValid = (val == gpsChecksum);
				break;
			}				
		}			
	}
}

static void decodeGpsData(char data) {
	if (gpsTextPartStep == GPS_PART_FINISHED && data != '$') {
		return;
	}
	
	switch (data) {
	case '$':
		gpsTextPos = 0;
		gpsTextType = GPS_TYPE_NONE;
		gpsChecksum = 0;
		gpsTextPartStep = 0;
		clearGpsText();
#ifdef GPS_FULL_TEXT
		clearFullGpsText();
		gpsFullTextPos = 0;
#endif // FULLGPSTEXT
		break;
	case ',':
		gpsChecksum ^= data;
	case '*':
		parseGpsPart();
		clearGpsText();		
		gpsTextPos = 0;
		gpsTextPartStep++;
		if (data =='*') {
			gpsTextPartStep = GPS_PART_CHECKSUM;
		}			
		break;
	case '\r':
	case '\n':
	  parseGpsPart();
	  gpsTextPartStep = GPS_PART_FINISHED;
		return;
		break;
	default:
		gpsText[gpsTextPos] = data;
		if (gpsTextPartStep != GPS_PART_CHECKSUM) {
		  gpsChecksum ^= data;
		}		  
		++gpsTextPos;
		break;
	}
	
#ifdef GPS_FULL_TEXT
	gpsFullText[gpsFullTextPos] = data;
	if (gpsFullTextPos + 1 != GPS_FULL_LENGTH) {
	  gpsFullTextPos++;
	}
#endif // FULLGPSTEXT
}

#endif /* GPS_H_ */