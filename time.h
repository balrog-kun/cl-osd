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
static uint8_t tick = 0;
static uint8_t sec = 0;
static uint8_t min = 0;
static uint8_t hour = 0;

static void updateTime() {
	++tick;
	if (tick == 50) {
		tick = 0;
		sec++;
	}
	if (sec == 60) {
		sec = 0;
		min++;
	}
	if (min == 60) {
		min = 0;
		hour++;
	}
	if (hour == 99) {
		hour = 0;
	}
}

#endif /* TIME_H_ */