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

#include "graphics.h"
#include "text.h"
#include "time.h"
#include "adc.h"
#include "gps.h"

#ifdef DEBUG
#include "test.h"
#endif //DEBUG

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
#ifndef GPS_ENABLED
  PORTD |= LED;
#else //GPS_ENABLED
  if (gpsLastData.fix != 0) {
		PORTD |= LED;
	}
	else {
		PORTD ^= LED;
	}
	
#ifdef DEBUG
  testCalcHome();
#endif // DEBUG
  
	if (gpsHomePosSet) {
	  calcHome(gpsLastValidData.pos.latitude * 10,
	           gpsLastValidData.pos.longitude * 10,
			       gpsHomePos.latitude * 10,
				     gpsHomePos.longitude * 10);
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

	if (timeTick == 0) {
    updateOnceEverySec();
	}

#ifdef TIME_ENABLED
	updateTime();
#endif //TIMEENABLED

#ifdef TEXT_ENABLED
  clearText();
  for (uint8_t i = 0; i < TEXT_LINES; ++i) {
	  updateText(i);
  }
#endif //TEXT_ENABLED

#ifdef GRAPICSENABLED
	clearGraphics();
	updateGrapics();
#endif //GRAPICSENABLED  
}

void main(void) __attribute__ ((noreturn)); // Main never returns, saves a few bytes

void main(void) {
	//_delay_ms(1000);
	setup();
  
#ifdef TEXT_ENABLED
	clearText();
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
#ifdef GPS_ENABLED
#ifdef HOME_SET_WITH_BUTTON
      if (gpsLastData.checksumValid != 0) {
				setHomePos();
			}
#endif //HOME_SET_WITH_BUTTON
#endif //GPS_ENABLED
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
#endif //DEBUG

#ifdef TEXT_ENABLED
    if (update == 2) {
		  update = 0;
#ifdef TEXT_INVERTED_ENABLED
	    clearTextInverted();
#endif //TEXT_INVERTED_ENABLED
	    updateTextPixmap(activeTextId);
	  }
	  else if (update == 1) {
#else
    if (update == 1) {
#endif //TEXTENABLED
			update = 0;
			//DDRB |= OUT1;
			updateOnceEveryFrame();
			//DDRB &= ~OUT1;
		}
	}
}

ISR(INT0_vect) {
	updateLine();
}
