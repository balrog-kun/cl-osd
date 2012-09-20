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

#ifndef LINE_H_
#define LINE_H_

#include "config.h"
#include "text.h"
#include "graphics.h"

#include <util/delay.h>

#define LINE_TYPE_UNKNOWN 0
#define LINE_TYPE_TEXT 1
#define LINE_TYPE_GRAPHICS 2

static volatile uint8_t gUpdateScreenData = 0;
static uint8_t gActiveTextId = 0;
static uint16_t gActiveTextLine = 0;
static uint16_t gActiveTextLastLine = 2;
static uint8_t gLineType = LINE_TYPE_UNKNOWN;

static void setupLine() {
  // Line trigger
	EICRA = (1<<ISC00) | (1<<ISC01); //set INT0 as rising edge trigger
	EIMSK = (1<<INT0); //enable INTO in global interrupt mask
	ACSR = (1<<ACD); //Comparator disabled
	ADCSRB = 0x00;

	// SPI setup
	SPDR = 0x00; // Clear spi reg or thrash will show on video
#ifdef TEXT_SMALL_ENABLED
	SPSR |= (1<<SPI2X); // Set dual speed
#else
	SPSR &= ~(1<<SPI2X); // Clear dual speed
#endif //TEXT_SMALL_ENABLED
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<CPHA);
	
#ifdef TEXT_ENABLED
	gActiveTextLine = gTextLines[gActiveTextId];
#endif //TEXT_ENABLED
}  

static void updateLine() {
  _delay_us(5); // wait 5us to see if H or V sync

	if(!(PIND & LTRIG)) { // H sync
		if (gActiveLine != 0) {
			switch(gLineType) {
				case LINE_TYPE_TEXT:
#ifdef TEXT_ENABLED				
					drawTextLine(gActiveTextId);
#endif //TEXTENABLED			
					break;
				case LINE_TYPE_GRAPHICS:
#ifdef GRAPICSENABLED
					drawGrapicsLine();
#endif //GRAPICSENABLED
					break;
			}
		}
		
		// We save some time in beginning of line by pre-calculating next type.
		gLineType = LINE_TYPE_UNKNOWN; // Default case
		gActiveLine++;
		if (gActiveLine == UPDATE_LINE) {
			gUpdateScreenData = 1;
		}
		else if (gActiveLine == UPDATE_LINE + 1) {
			gUpdateScreenData = 2;
		}

#ifdef TEXT_ENABLED	
		if (gActiveLine >= gActiveTextLine && gActiveLine < gActiveTextLastLine) {
	    gLineType = LINE_TYPE_TEXT;
		  gActivePixmapLine = (gActiveLine - gActiveTextLine);
		  if (gActiveTextSize == TEXT_SIZE_LARGE_MULT) {
		    gActivePixmapLine /= TEXT_SIZE_LARGE_MULT;
		  }
		}
		else if (gActiveLine == gActiveTextLastLine) {
		  gUpdateScreenData = 2;
			gActiveTextId = (gActiveTextId+1) % TEXT_LINES;
			gActiveTextLine = gTextLines[gActiveTextId];
			gActiveTextSize = gTextLineSizes[gActiveTextId];
			gActiveTextLastLine = gActiveTextLine + TEXT_CHAR_HEIGHT * gActiveTextSize;
			gActivePixmapLine = 1;
			return;
		}
#endif //TEXT_ENABLED		
#ifdef GRAPICSENABLED		
		else if (gActiveLine >= GRAPHICS_LINE && gActiveLine < (GRAPHICS_LINE + GRAPHICS_HEIGHT)) {
			gLineType = LINE_TYPE_GRAPHICS;
			gActivePixmapLine = (gActiveLine - GRAPHICS_LINE);
		}
#endif //GRAPICSENABLED
	}
	else { // V sync
		if(gActiveLine > 200) {
			gActiveLine = 0;
		}
	}
}

#endif /* LINE_H_ */