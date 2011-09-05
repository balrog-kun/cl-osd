/*cl-osd - A simple open source osd for e-osd and g-osd
Copyright (C) 2011 Carl Ljungström & Ernani Reis

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

#ifndef GPSUTILS_H_
#define GPSUTILS_H_

#include "trigonometry.h"
#include "commonutils.h"

// Convert nmea WGS84 to seconds * 100
// wgs84 = DDMM[.]MMMM
static int32_t wgs84ToSec100(int32_t wgs84) { 
  int8_t mult = 1;
  
  if (wgs84 < 0) {
    mult = -1;
    wgs84 = -wgs84;
  }
  
  uint8_t hour = wgs84 / 1000000; //Get hour part
  uint8_t min = (wgs84 - (hour * 1000000)) / 10000; //Get minute part
  uint32_t minDecimal = wgs84 % 10000; //Get minute decimal part
  
  int32_t sec100 = 0;
  sec100 += hour; // Add hour part
  sec100 *= 60;
  sec100 += min; // Add minute part
  sec100 *= 60;
  sec100 *= 100;
  minDecimal *= 60;
  minDecimal /= 100;
  sec100 += minDecimal; // Add minute decimal part
  
  if (mult == -1) {
    return -sec100;
  } else {
    return sec100;
  }
}

#ifdef IMPERIAL_SYSTEM
static void meterToFeet(int16_t* var) {
	int32_t tmp = *var;
	tmp *= 3281;
	tmp /= 1000;
	*var = tmp;
}

static void knotToMph(uint16_t* var) {
	int32_t tmp = *var;
	tmp *= 1151;
	tmp /= 1000;
	*var = tmp;
}
#endif //IMPERIAL_SYSTEM

static int32_t parseInt(const char* text, uint8_t maxLength) {
	uint8_t i = 0;
	uint32_t decimal = 0;
	uint8_t neg = 0;
	if (text[0] == '-') {
		++i;
		neg = 1;
	}
	while (isDigit(text[i]) && i < maxLength) {
		decimal *= 10;
		decimal += (text[i]) - '0';
		++i;		
	}
	return decimal * (neg ? -1 : 1);
}

static int32_t parseFloat(const char* text, uint8_t maxLength) {
	
	uint32_t val = 0;
	uint8_t neg = 0;
	uint8_t i = 0;
	if (text[0] == '-') {
		++i;
		neg = 1;
	}
	for (; i < maxLength; ++i) {
		if (isDigit(text[i])) {
			val *= 10;
			val += (text[i]) - '0';
		}
		else if (text[i] != '.') {
			break;
		}			
	}
	return val * (neg ? -1 : 1);
}

static uint8_t parseHex(const char* text, uint8_t maxLength) {
	uint8_t i = 0;
	uint32_t val = 0;
	while (text[i] != 0 && i < maxLength) {
		val *= 16;
		if (text[i] >= 'A' && text[i] <= 'F') {
			val += 10 + text[i] - 'A';
		}
		else if (text[i] >= 'a' && text[i] <= 'f') {
			val += 10 + text[i] - 'a';
		}
		else if (text[i] >= '0' && text[i] <= '9') {
			val += text[i] - '0';
		}
		++i;		
	}
	return val;
}

#endif /* GPSUTILS_H_ */