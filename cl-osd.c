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
#include "layout.h"

#ifdef DEBUG
#include "test.h"
#endif //DEBUG

#include <avr/io.h>
#include <avr/interrupt.h>

static uint8_t gKeyPressed = 0;
static uint8_t gKeyPressTime = 0;

static void setup(void)
{
	// Port setup
	PORTD = KEY; //key pullup & led off
	DDRD  = LED; // led output
	PORTB &= ~OUT1;
	DDRB  = OUT2 | SS;
	
	TIMSK1 |= (1 << TOIE1); // Enable overflow interrupt
	TCCR1B |= (1 << CS11); // Start timer at Fcpu/8

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
  if (gGpsLastData.fix != 0) {
		PORTD |= LED;
	}
	else {
		PORTD ^= LED;
	}
	
#ifdef DEBUG
  //testCalcHome();
#endif // DEBUG
  
	if (gHomePosSet) {
	  calcHome(gGpsLastValidData.pos.latitude,
	           gGpsLastValidData.pos.longitude,
			       gHomePos.latitude,
				     gHomePos.longitude,
					   &gHomeDistance,
					   &gHomeBearing);
#ifdef STATISTICS_ENABLED
		if (gHomeDistance > gStatMaxDistance) {
      gStatMaxDistance = gHomeDistance;
    }
#endif //STATISTICS_ENABLED
	}
#endif //GPS_ENABLED
  
#ifdef ADC_ENABLED 
  measureAnalog();
#endif //ADCENABLED

#ifdef ALARM_ENABLED
  updateAlarms();
#endif // ALARM_ENABLED
}
  

static void updateOnceEveryFrame() {
  if (gKeyPressed) {
		++gKeyPressTime;
	}

	if (gTimeTick == 0) {
    updateOnceEverySec();
	}

#ifdef TIME_ENABLED
	updateTime();
#endif //TIMEENABLED

#ifdef TEXT_ENABLED
  for (uint8_t i = 0; i < TEXT_LINES; ++i) {
	  clearText(i);
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

	while(1) {
    
#ifdef GPS_ENABLED
    if(UCSR0A & (1<<RXC0)) {
      decodeGpsData(UDR0);
    }
#endif //GPS_ENABLED
    
#ifndef DEBUG
		if((PIND & KEY) != KEY) {
			gKeyPressed = 1;
			if(gKeyPressTime > 50) {
				PORTD |= LED; // long press!
#ifdef GPS_ENABLED
#ifdef HOME_SET_WITH_BUTTON
      if (gGpsLastData.checksumValid != 0 && gGpsLastData.fix != 0) {
				setHomePos();
			}
#endif //HOME_SET_WITH_BUTTON
#endif //GPS_ENABLED
			}
		}
		else {
			if (gKeyPressed) {
				PORTD &= ~LED;  // led off
			}				
			gKeyPressed = 0;
			gKeyPressTime = 0;
		}
#endif //!DEBUG

#ifdef DEBUG
		gUpdateScreenData = 2;
#endif //DEBUG

#ifdef TEXT_ENABLED
    if (gUpdateScreenData == 2) {
		  gUpdateScreenData = 0;
#ifdef TEXT_INVERTED_ENABLED
	    clearTextInverted();
#endif //TEXT_INVERTED_ENABLED
      //DDRB |= OUT1;
      updateTextCharStartPos(gActiveTextId);
	    updateTextPixmap(gActiveTextId);
		  //DDRB &= ~OUT1;
	  }
	  else if (gUpdateScreenData == 1) {
#else
    if (gUpdateScreenData == 1) {
#endif //TEXTENABLED
			gUpdateScreenData = 0;
			//DDRB |= OUT1;
			updateOnceEveryFrame();
			//DDRB &= ~OUT1;
		}
	}
}

ISR(INT0_vect) {
	updateLine();
}

ISR(TIMER1_OVF_vect)
{
	static uint8_t lastLine = 0;
	if (lastLine == gActiveLine) {
    PORTD ^= LED; // Toggle the LED
	}
	lastLine = gActiveLine;
}

