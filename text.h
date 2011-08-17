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


#ifndef TEXT_H_
#define TEXT_H_

#include <stdio.h>
#include <util/delay.h>

#include "config.h"
#include "xconvert.h"
#include "oem6x8.h"
#include "time.h"
#include "adc.h"
#include "delay.h"
#include "gps.h"

// Text vars
static uint16_t const textLines[TEXT_LINES] = {TEXT_LINES_LIST};
static char text[TEXT_LINES][TEXT_MAX_CHARS];
static uint8_t textData[TEXT_LINES][TEXT_MAX_CHARS*TEXT_CHAR_HEIGHT];

// Functions
static void clearText() {
	for (uint8_t i = 0; i < TEXT_LINES; ++i) {
		for (uint8_t j = 0; j < TEXT_MAX_CHARS; ++j) {
			text[i][j] = 0;
		}
	}
}

static void clearTextData() {
	for (uint8_t i = 0; i < TEXT_LINES; ++i) {
		for (uint8_t j = 0; j < TEXT_MAX_CHARS*TEXT_CHAR_HEIGHT; ++j) {
			textData[i][j] = 0;
		}		
	}
}

static void drawText() {
	for (uint8_t k = 0; k < TEXT_LINES; ++k) {
		for (uint8_t i = 0; i < TEXT_CHAR_HEIGHT; i++) {
			for (uint8_t j = 0; j < TEXT_MAX_CHARS; j++) {
				uint16_t oem6x8Pos = (text[k][j]*TEXT_CHAR_HEIGHT) + i;
				uint8_t bytePos = i*TEXT_MAX_CHARS + j;
				uint8_t val = pgm_read_byte(&(oem6x8[oem6x8Pos]));
				textData[k][bytePos] = val;
			}			
		}
	}
}

inline void printDebugInfo() {
	// ---- TODO: Cleanup here! ----
	
  //snprintf(text[0], TEXT_MAX_CHARS, "%02d:%02d:%02d:%02d", hour, min, sec, tick);
	//snprintf(text[1], TEXT_MAX_CHARS, "%02d:%02d:%02d %d.%02dV %d.%02dV %d%%", hour, min, sec, adc0High, adc0Low, adc1High, adc1Low, batt1);
	if (gpsTextType != GPS_TYPE_GPGGA ) {
		//snprintf(text[0], TEXT_MAX_CHARS, "%.32s", gpsFullText);
		//snprintf(text[0], TEXT_MAX_CHARS, "Part (%d): %s", gpsTextPartLength, gpsTextPart); //part
		//snprintf(text[1], TEXT_MAX_CHARS, "%s == %d", gpsTextPart, gpsTextType);
		//snprintf(text[0], TEXT_MAX_CHARS, "%s == %06ld", gpsTextPart, gpsTime); //time
		//snprintf(text[0], TEXT_MAX_CHARS, "%s == %ld", gpsTextPart, gpsLat); //Lat
		//snprintf(text[0], TEXT_MAX_CHARS, "%s == %ld", gpsTextPart, gpsLong); //Long
		//snprintf(text[0], TEXT_MAX_CHARS, "%s == %d", gpsTextPart, gpsFix); //fix?
		//snprintf(text[0], TEXT_MAX_CHARS, "%s == %d", gpsTextPart, gpsSats); //sats
		//snprintf(text[0], TEXT_MAX_CHARS, "%s == %d", gpsTextPart, gpsAltitude); //altitude
		//snprintf(text[0], TEXT_MAX_CHARS, "(%s == %d)? => %d", gpsTextPart, gpsChecksum, gpsChecksumValid); //checksum
		//snprintf(text[1], TEXT_MAX_CHARS, "%.32s", &gpsFullText[30]);		
	}
	//snprintf(text[1], TEXT_MAX_CHARS, "%dV %dV %dV", analogInputsRaw[ANALOG_IN_1], analogInputsRaw[ANALOG_IN_2], analogInputsRaw[ANALOG_IN_3]);
}

static uint8_t calcBatteryLevel(uint8_t adcInput) {
	uint16_t batteryLevel = ((analogInputs[adcInput].high*100) + analogInputs[adcInput].low);
	if (batteryLevel > BATT_MIN_VOLTAGE_INT) {
		batteryLevel -= BATT_MIN_VOLTAGE_INT;
		batteryLevel *= 100;
		batteryLevel /= BATT_MAX_VOLTAGE_INT - BATT_MIN_VOLTAGE_INT;
	}
	else {
		batteryLevel = 0;
	}
	return batteryLevel;
}

static void updateText() {
	uint8_t batterLevel = calcBatteryLevel(ANALOG_IN_1);
	
	clearText();
	//printDebugInfo();
	snprintf(text[0], TEXT_MAX_CHARS, "%02d:%02d:%02d %d.%02dV %d.%02dV %d.%02dV %d%%", 
    hour, min, sec, 
    analogInputs[ANALOG_IN_1].high, analogInputs[ANALOG_IN_1].low, 
    analogInputs[ANALOG_IN_2].high, analogInputs[ANALOG_IN_2].low, 
    analogInputs[ANALOG_IN_3].high, analogInputs[ANALOG_IN_3].low, 
    batterLevel);
	snprintf(text[1], TEXT_MAX_CHARS, "GPS: %ld, %ld %dm %ds", gpsLat, gpsLong, gpsAltitude, gpsSats);	  
	//snprintf(text[1], TEXT_MAX_CHARS, "GPS: %dkm/h %d deg %ld %s", gpsSpeed, gpsAngle, gpsDate, gpsChecksumValid ? "OK" : "BAD");
}

static void drawTextLine(uint8_t textNumber)
{
	_delay_us(4);
	uint8_t currLine = (line - textLines[textNumber]) / 2;
	DDRB |= OUT1;
	for (uint8_t i = 0; i < TEXT_MAX_CHARS; ++i) {
		SPDR = textData[textNumber][currLine*TEXT_MAX_CHARS + i];
		DELAY_9_NOP();
		DELAY_9_NOP();
		DELAY_9_NOP();
	}
	DELAY_4_NOP();
	SPDR = 0x00;
	DDRB &= ~OUT1;
}

#endif /* TEXT_H_ */