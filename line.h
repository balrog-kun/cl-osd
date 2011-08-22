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
static volatile uint16_t textNumber = 0;
static volatile uint16_t lineType = LINE_TYPE_UNKNOWN;

static void setupLine() {
  // Line trigger
	EICRA = (1<<ISC00) | (1<<ISC01); //set INT0 as rising edge trigger
	EIMSK = (1<<INT0); //enable INTO in global interrupt mask
	ACSR = (1<<ACD); //Comparator disabled
	ADCSRB = 0x00;

	// SPI setup
	SPDR = 0x00; // Clear spi reg or thrash will show on video
	SPSR &= ~(1<<SPI2X); // Clear dual speed
	//SPSR |= (1<<SPI2X); // Set dual speed
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<CPHA);
}  

static void updateLine() {
  _delay_us(5); // wait 5us to see if H or V sync

	if(!(PIND & LTRIG)) { // H sync
		if (line != 0) {
			switch(lineType) {
				case LINE_TYPE_TEXT:
#ifdef TEXT_ENABLED				
					drawTextLine(textNumber);
#endif //TEXTENABLED			
					break;
				case LINE_TYPE_GRAPHICS:
#ifdef GRAPICSENABLED
					drawGrapicsLine();
#endif //GRAPICSENABLED
					break;
			}
		}
		
		// We save some time by pre-calculating next type.
		line++;
		if (line >= TEXT_1_TRIG_LINE && line < (TEXT_1_TRIG_LINE + TEXT_CHAR_HEIGHT * 2)) {
			lineType = LINE_TYPE_TEXT;
			textNumber = 0;
		}
		else if (line >= TEXT_2_TRIG_LINE && line < (TEXT_2_TRIG_LINE + TEXT_CHAR_HEIGHT * 2)) {
			lineType = LINE_TYPE_TEXT;
			textNumber = 1;
		}
		else if (line >= GRAPHICS_LINE && line < (GRAPHICS_LINE + GRAPHICS_HEIGHT)) {
			lineType = LINE_TYPE_GRAPHICS;
		}
		else if (line == LAST_LINE) {
			// We want to start update as fast as possible so we don't use a type here.
			lineType = LINE_TYPE_UNKNOWN;
			update = 1;
		}
		else {
			lineType = LINE_TYPE_UNKNOWN;
		}
	}
	else { // V sync
		if(line > 200) {
			line = 0;
		}
	}
}

#endif /* LINE_H_ */