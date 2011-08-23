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


#ifndef TRIGOMETRY_H_
#define TRIGOMETRY_H_

#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>

static int32_t gpsDistToHome = 0; // Distance to home in meters
static uint16_t gpsBearingToHome = 0; // Direction to home

// Created using excel with: "=SIN(A1*PI()/180 )*100"
const static int8_t sinData[90] PROGMEM = {
  0, 2, 3, 5, 7, 9, 10, 12, 14, 16, 17, 19, 21, 22, 24, 26, 28, 29, 31, 33,
  34, 36, 37, 39, 41, 42, 44, 45, 47, 48, 50, 52, 53, 54, 56, 57, 59, 60, 62,
  63, 64, 66, 67, 68, 69, 71, 72, 73, 74, 75, 77, 78, 79, 80, 81, 82, 83, 84,
  85, 86, 87, 87, 88, 89, 90, 91, 91, 92, 93, 93, 94, 95, 95, 96, 96, 97, 97,
  97, 98, 98, 98, 99, 99, 99, 99, 100, 100, 100, 100, 100};
  
//static uint16_t pos = 0;
static int8_t mySin(uint16_t angle) {
	uint16_t pos = 0;
	pos = angle % 360;
	int8_t mult = 1;
	// 180-359 is same as 0-179 but negative.
	if (pos >= 180) { 
		pos = pos - 180;
		mult = -1;
	}
	// 0-89 is equal to 90-179 except backwards.
	if (pos >= 90) { 
		pos = 180 - pos;
	}
	return mult * (int8_t)(pgm_read_byte(&sinData[pos]));
}

static int8_t myCos(uint16_t angle) {
	return mySin(angle + 90);
}

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
  int16_t minDecimal = (wgs84 % 10000); //Get minute decimal part
  
  int32_t sec100 = 0;
  sec100 += hour * 60; // Add hour part
  sec100 = (sec100 + min) * 60; // Add minute part
  sec100 = sec100 * 100 + (minDecimal * 60) / 100; // Add minute decimal part
  
  if (mult == -1) {
    return -sec100;
  } else {
    return sec100;
  }
}

static uint32_t calcSqrt(uint32_t input)
{
  int32_t nv;
  int32_t v = input>>1;
  int32_t c = 0;
  if (!v) {
    return input;
  }	
  do {
    nv = (v + input / v)>>1;
    if (abs(v - nv) <= 1) {
      return nv;
	  }	  
    v = nv;
  } while (c++ < 25);
  return nv;
}

// TODO: Some translations left
static void calcHome(int32_t currLat, int32_t currLong, int32_t homeLat, int32_t homeLong) {
  // calculates bearing and distance to reference point
  // vehicle in currLat, currLong
  // reference point on homeLat, homeLong

  // returns nothing
  // updates global variables dist, and home
  // gpsDistToHome = distance to reference in meters
  // gpsBearingToHome = bearing to reference, in degrees

  // computes using integers, only.
  // presents errors ~ 5m, and 5° for short distances (few km)
  // should present considerable errors for longer distances

  // considers flat earth around vehicle and reference

  // Ernani Reis, Aug 2011


  // cosseno de lat recicla p/ seno de home
  int32_t c;
  // Delta lat & lon
  int32_t deltaLat; 
  int32_t deltaLong;
      
  c = myCos(abs(currLat) / 1000000); // c ~ cos(lat) -> long to fix the delta

  int32_t currLatSec100 = wgs84ToSec100(currLat); // Convert wgs84 to sec * 100
  int32_t homeLatSec100 = wgs84ToSec100(homeLat);
  int32_t currLongSec100 = wgs84ToSec100(currLong);
  int32_t homeLongSec100 = wgs84ToSec100(homeLong);

  deltaLong = homeLongSec100 - currLongSec100; // Calculate deltas
  deltaLat = homeLatSec100 - currLatSec100; // delta long ainda com erro (Delta still long with error?)

  deltaLong = deltaLong * c;   // corrige dla p/ lat da região
  deltaLong = deltaLong / 100; // cosseno volta * 100

  int32_t div = 1;
  while (deltaLong >= 0xFFFF || deltaLat >= 0xFFFF) {
	  deltaLong /= 2;
	  deltaLat /= 2;
	  div *= 2;
  }
    
	gpsDistToHome = calcSqrt((deltaLong * deltaLong) + (deltaLat * deltaLat));
	gpsDistToHome *= div;

  gpsBearingToHome = 0;
  if (gpsDistToHome > 0) { // sobre home -> só erros; home continua 0
    if (abs(deltaLong) >= abs(deltaLat)) { // angulo é baixo, seno é melhor
      c = (abs(deltaLat) * 100) / gpsDistToHome; // calcula seno -> dist > 0
      while((mySin(gpsBearingToHome) <= c) && (gpsBearingToHome < 90)) {
        ++gpsBearingToHome;
      }
    } 
	  else { // angulo é alto, cosseno é melhor
      c = (abs(deltaLong) * 100) / gpsDistToHome; // calcula cosseno
      while((mySin(gpsBearingToHome) <= c) && (gpsBearingToHome < 90)) {
        ++gpsBearingToHome;
      }
      gpsBearingToHome = 90 - gpsBearingToHome;
    }
    if (deltaLat == 0) {
      if (deltaLong >= 0) { // home = leste
        gpsBearingToHome = 90; 
      } 
	    else { // home = oeste
        gpsBearingToHome = 270;
      }
    } 
    else if (deltaLat > 0) {
      if (deltaLong >= 0) { // NE quadrant
        gpsBearingToHome = 90 - gpsBearingToHome; // home positivo
      } 
	    else { // NW quadrant
        gpsBearingToHome = 270 + gpsBearingToHome;
      }
    } 
	  else {
      if (deltaLong >= 0) { // SE quadrant
        gpsBearingToHome = 90 + gpsBearingToHome;
      } 
	    else { // SW quadrant
        gpsBearingToHome = 270 - gpsBearingToHome;
      }
    }
  }
  gpsDistToHome = gpsDistToHome * 309; // ate aqui em segundo * 100
  gpsDistToHome = gpsDistToHome / 1000; // metros, agora
}

#endif /* TRIGOMETRY_H_ */