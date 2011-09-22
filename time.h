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

typedef struct {
  uint8_t sec;
  uint8_t min;
  uint8_t hour;
} TTime;

// Time vars
static volatile uint8_t gTimeTick = 0;
static volatile TTime gTime = {};

#ifdef TIME_ENABLED

static void updateTime() {
	++gTimeTick;
	if (gTimeTick >= 50) {
		gTimeTick = 0;
		gTime.sec++;
	}
	if (gTime.sec >= 60) {
		gTime.sec = 0;
		gTime.min++;
	}
	if (gTime.min >= 60) {
		gTime.min = 0;
		gTime.hour++;
	}
	if (gTime.hour >= 99) {
		gTime.hour = 0;
	}
}

#endif //TIME_ENABLED

#endif /* TIME_H_ */