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


#include "config.h"
#include "line.h"

#ifdef GRAPICSENABLED
#include "graphics.h"
#endif // GRAPICSENABLED

#ifdef TEXT_ENABLED
#include "text.h"
#endif //TEXTENABLED

#ifdef TIME_ENABLED
#include "time.h"
#endif //TIMEENABLED

#ifdef ADC_ENABLED
#include "adc.h"
#endif //ADCENABLED

#ifdef GPS_ENABLED
#include "gps.h"
#endif //GPS_ENABLED

#include <avr/io.h>
#include <avr/interrupt.h>

static uint8_t keyPressed = 0;
static uint8_t keyTime = 0;

static void setup(void)
{
	// Port setup
	PORTD = KEY; //key pullup & led off
	DDRD  = LED; // led output
	PORTB &= ~OUT1;
	DDRB  = OUT2 | SS;

	setupLine();
  	
#ifdef ADC_ENABLED
	setupAdc();
#endif //ADCENABLED

#ifdef GPS_ENABLED
	setupGps();
#endif //GPS_ENABLED
	
	sei();
}

static void updateOnceEverySec() {
#ifdef GPS_ENABLED
  if (gpsFix) {
		PORTD |= LED;
	}
	else {
		PORTD ^= LED;
	}
#endif //GPS_ENABLED
  
#ifdef ADC_ENABLED  
  measureAnalog();
#endif //ADCENABLED
}
  

static void updateOnceEveryFrame() {
  if (keyPressed) {
		++keyTime;
	}

	if (tick == 0) {
    updateOnceEverySec();
	}

#ifdef TIME_ENABLED
	updateTime();
#endif //TIMEENABLED

#ifdef TEXT_ENABLED
	//clearText();
	updateText();
	drawText();
#endif //TEXTENABLED

#ifdef GRAPICSENABLED
	clearGraphics();
	updateGrapics();
#endif //GRAPICSENABLED  
}

int main(void)
{
	setup();
  
#ifdef TEXT_ENABLED
	drawText();
#endif //TEXTENABLED

#ifdef GRAPICSENABLED
	clearGraphics();
#endif //GRAPICSENABLED

	while(1) {
    
#ifdef GPS_ENABLED    
    if(UCSR0A & (1<<RXC0)) {
      decodeGpsData(UDR0);
    }
#endif //GPS_ENABLED
    
#ifndef DEBUG
		if((PIND & KEY) != KEY) {
			keyPressed = 1;
			if(keyTime > 10) {
				PORTD |= LED; // long press!
			}
		}
		else {
			if (keyPressed) {
				PORTD &= ~LED;  // led off
			}				
			keyPressed = 0;
			keyTime = 0;
		}
#endif //!DEBUG

#ifdef DEBUG
		update = 1;
#endif // DEBUG

		if (update == 1) {
			update = 0;
			updateOnceEveryFrame();
		}      
	}
}

ISR(INT0_vect) {
	updateLine();
}