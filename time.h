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


#ifndef TIME_H_
#define TIME_H_

#include "config.h"

#ifdef COLORSYSTEM_NTSC
#define TIME_FRAMES_PER_SEC 60
#else //PAL
#define TIME_FRAMES_PER_SEC 50
#endif

typedef struct {
  uint8_t sec;
  uint8_t min;
#ifdef TIME_HOUR_ENABLED
  uint8_t hour;
#endif //TIME_HOUR_ENABLED
} TTime;

// Time vars
static volatile uint8_t gTimeTick = 0;
static volatile TTime gTime = {};
static volatile uint8_t gBlink1Hz = 0;

#ifdef TIME_ENABLED

static void updateTime() {
	++gTimeTick;
	if (gTimeTick >= TIME_FRAMES_PER_SEC) {
		gTimeTick = 0;
		gTime.sec++;
		gBlink1Hz = (gBlink1Hz+1)%2;
	}
	if (gTime.sec >= 60) {
		gTime.sec = 0;
		gTime.min++;
	}
	if (gTime.min >= 60) {
		gTime.min = 0;
#ifdef TIME_HOUR_ENABLED
		gTime.hour++;
#endif //TIME_HOUR_ENABLED
	}
#ifdef TIME_HOUR_ENABLED
	if (gTime.hour >= 99) {
		gTime.hour = 0;
	}
#endif //TIME_HOUR_ENABLED
}

#endif //TIME_ENABLED

#endif /* TIME_H_ */