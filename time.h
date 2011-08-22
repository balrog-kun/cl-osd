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

// Time vars
static volatile uint8_t timeTick = 0;
static volatile uint8_t timeSec = 0;
static volatile uint8_t timeMin = 0;
static volatile uint8_t timeHour = 0;

#ifdef TIME_ENABLED

static void updateTime() {
	++timeTick;
	if (timeTick >= 50) {
		timeTick = 0;
		timeSec++;
	}
	if (timeSec >= 60) {
		timeSec = 0;
		timeMin++;
	}
	if (timeMin >= 60) {
		timeMin = 0;
		timeHour++;
	}
	if (timeHour >= 99) {
		timeHour = 0;
	}
}

#endif //TIME_ENABLED

#endif /* TIME_H_ */