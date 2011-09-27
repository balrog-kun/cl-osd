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

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "config.h"

// Global vars
static uint16_t gActiveLine = 0;
static uint8_t gActiveTextSize = 2;
static uint8_t gActivePixmapLine = 0;

// Debug vars!
#ifdef DEBUG
static volatile uint8_t u8 = 0;
static volatile int8_t i8 = 0;
static volatile uint16_t u16 = 0;
static volatile int16_t i16 = 0;
static volatile uint32_t u32 = 0;
static volatile int32_t i32 = 0;
static volatile char c8 = 0;
#endif //DEBUG

#endif /* GLOBAL_H_ */