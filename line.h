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

static volatile uint8_t update = 0;
static volatile uint8_t activeTextId = 0;
static volatile uint8_t activeTextLine = 0;
static volatile uint8_t lineType = LINE_TYPE_UNKNOWN;

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
	
	activeTextLine = textLines[activeTextId];
}  

static void updateLine() {
  _delay_us(5); // wait 5us to see if H or V sync

	if(!(PIND & LTRIG)) { // H sync
		if (line != 0) {
			switch(lineType) {
				case LINE_TYPE_TEXT:
#ifdef TEXT_ENABLED				
					drawTextLine(activeTextId);
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
		lineType = LINE_TYPE_UNKNOWN; // Default case
		line++;
		if (line == LAST_LINE) {
			update = 1;
			return;
		}
	
		if (line >= activeTextLine && line < (activeTextLine + TEXT_CHAR_HEIGHT * TEXT_SIZE_MULT)) {
	    lineType = LINE_TYPE_TEXT;
		}
		else if (line == (activeTextLine + TEXT_CHAR_HEIGHT * TEXT_SIZE_MULT)) {
		  update = 2;
			activeTextId = (activeTextId+1) % TEXT_LINES;
			activeTextLine = textLines[activeTextId];
			return;
		}
		#ifdef GRAPICSENABLED		
		else if (line >= GRAPHICS_LINE && line < (GRAPHICS_LINE + GRAPHICS_HEIGHT)) {
			lineType = LINE_TYPE_GRAPHICS;
		}
    #endif //GRAPICSENABLED
	}
	else { // V sync
		if(line > 200) {
			line = 0;
		}
	}
}

#endif /* LINE_H_ */