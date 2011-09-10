/*cl-osd - A simple open source osd for e-osd and g-osd
Copyright (C) 2011 Ernani Reis & Carl Ljungström

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

#ifndef HOME_H_
#define HOME_H_

#include "gpsutils.h"

static int32_t gpsHomeDistance = 0; // Distance to home in meters
static uint16_t gpsHomeBearing = 0; // Direction to home

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
  int8_t c;
  // Delta lat & lon
  int32_t deltaLat; 
  int32_t deltaLong;
      
  c = myCos(absi32(homeLat + currLat) / 2000000); // c ~ cos(lat) -> long to fix the delta

  int32_t currLatSec100 = wgs84ToSec100(currLat); // Convert wgs84 to sec * 100
  int32_t homeLatSec100 = wgs84ToSec100(homeLat);
  int32_t currLongSec100 = wgs84ToSec100(currLong);
  int32_t homeLongSec100 = wgs84ToSec100(homeLong);

  deltaLong = homeLongSec100 - currLongSec100; // Calculate deltas
  deltaLat = homeLatSec100 - currLatSec100; // delta long ainda com erro (Delta still long with error?)

  deltaLong = deltaLong * c;   // corrige dla p/ lat da região
  deltaLong = deltaLong / 100; // cosseno volta * 100

  uint8_t mult = 1;
  while ((absi32(deltaLong) >= 0xFFFF) 
         || (absi32(deltaLat) >= 0xFFFF)) {
	  deltaLong /= 2;
	  deltaLat /= 2;
	  mult *= 2;
  }
  
	gpsHomeDistance = calcSqrt((deltaLong * deltaLong) + (deltaLat * deltaLat));
	gpsHomeDistance *= mult;

  gpsHomeBearing = 0;
  if (gpsHomeDistance > 0) { // over home -> do not compute; home forced to 0
    if (absi32(deltaLong) >= absi32(deltaLat)) { // low angle, sine is better
      c = (absi32(deltaLat) * 100) / gpsHomeDistance; // computes sine -> dist > 0
      while((mySin(gpsHomeBearing) <= c) && (gpsHomeBearing < 90)) {
        ++gpsHomeBearing;
      }
    } 
	  else { // high angle, cosine is better
      c = (absi32(deltaLong) * 100) / gpsHomeDistance; // computes cosine
      while((mySin(gpsHomeBearing) <= c) && (gpsHomeBearing < 90)) {
        ++gpsHomeBearing;
      }
      gpsHomeBearing = 90 - gpsHomeBearing;
    }
    if (deltaLat == 0) {
      if (deltaLong >= 0) { // home = East
        gpsHomeBearing = 90; 
      } 
	    else { // home = West
        gpsHomeBearing = 270;
      }
    } 
    else if (deltaLat > 0) {
      if (deltaLong >= 0) { // NE quadrant
        gpsHomeBearing = 90 - gpsHomeBearing; // home positivo
      } 
	    else { // NW quadrant
        gpsHomeBearing = 270 + gpsHomeBearing;
      }
    } 
	  else {
      if (deltaLong >= 0) { // SE quadrant
        gpsHomeBearing = 90 + gpsHomeBearing;
      } 
	    else { // SW quadrant
        gpsHomeBearing = 270 - gpsHomeBearing;
      }
    }
  }
  gpsHomeDistance = gpsHomeDistance * 309; // ate aqui em segundo * 100
  gpsHomeDistance = gpsHomeDistance / 1000; // metros, agora
#ifdef IMPERIAL_SYSTEM  
  gpsHomeDistance *= 3281;
	gpsHomeDistance /= 1000;
#endif //IMPERIAL_SYSTEM
}

#endif /* HOME_H_ */