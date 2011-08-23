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

#ifdef TEXT_ENABLED

#include "xconvert.h"
#include "oem6x8.h"
#include "time.h"
#include "adc.h"
#include "delay.h"
#include "gps.h"
#include "trigonometry.h"

// Text vars
static uint16_t const textLines[TEXT_LINES] = {TEXT_TRIG_LINES_LIST};
static char text[TEXT_LINES][TEXT_LINE_MAX_CHARS];
static uint8_t textData[TEXT_LINES][TEXT_LINE_MAX_CHARS*TEXT_CHAR_HEIGHT];
static uint8_t textInverted[TEXT_LINES][TEXT_LINE_MAX_CHARS/8];

// Functions
static void clearText() {
	for (uint8_t i = 0; i < TEXT_LINES; ++i) {
		for (uint8_t j = 0; j < TEXT_LINE_MAX_CHARS; ++j) {
			text[i][j] = 0;
		}
	}
}

static void clearTextData() {
	for (uint8_t i = 0; i < TEXT_LINES; ++i) {
		for (uint16_t j = 0; j < TEXT_LINE_MAX_CHARS*TEXT_CHAR_HEIGHT; ++j) {
			textData[i][j] = 0;
		}		
	}
}

static void clearTextInverted() {
	for (uint8_t i = 0; i < TEXT_LINES; ++i) {
	  for (uint8_t j = 0; j < TEXT_LINE_MAX_CHARS/8; ++j) {
	    textInverted[i][j] = 0;
	  }
	}	  
}

static void setCharInverted(uint8_t line, uint8_t pos, uint8_t bitValue) {
	uint8_t bytePos = pos/8;
	uint8_t bitPos = pos - (bytePos*8);
	if (bitValue == TEXT_INVERTED_OFF) {
	  textInverted[line][bytePos] ^= ~(1<<bitPos);
	}
	else if (bitValue == TEXT_INVERTED_ON) {
	  textInverted[line][bytePos] |= (1<<bitPos);
	}
	else { //TEXT_INVERTED_FLIP
	  textInverted[line][bytePos] ^= (1<<bitPos);
	}
}

static uint8_t charInverted(uint8_t line, uint8_t pos) {
	uint8_t bytePos = pos/8;
	uint8_t bitPos = pos - (bytePos*8);
	if (textInverted[line][bytePos] & (1<<bitPos)) {
		return 1;
	}
	return 0;
}

static void drawText() {
	for (uint8_t k = 0; k < TEXT_LINES; ++k) {
		for (uint8_t i = 0; i < TEXT_CHAR_HEIGHT; i++) {
			for (uint8_t j = 0; j < TEXT_LINE_MAX_CHARS; j++) {
				uint16_t oem6x8Pos = (text[k][j]*TEXT_CHAR_HEIGHT) + i;
				uint8_t bytePos = i*TEXT_LINE_MAX_CHARS + j;
				uint8_t val = pgm_read_byte(&(oem6x8[oem6x8Pos]));
				if (charInverted(k, j)) {
					val = ~val;
				}
				textData[k][bytePos] = val;
			}			
		}
	}
}

static void printDebugInfo() {
	// ---- TODO: Cleanup here! ----
	
  //snprintf(text[0], TEXT_MAX_CHARS, "%02d:%02d:%02d:%02d", hour, min, sec, tick);
	//snprintf(text[1], TEXT_MAX_CHARS, "%02d:%02d:%02d %d.%02dV %d.%02dV %d%%", hour, min, sec, adc0High, adc0Low, adc1High, adc1Low, batt1);
	//if (gpsTextType != GPS_TYPE_GPGGA ) {
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
	//}
	//snprintf(text[1], TEXT_MAX_CHARS, "%dV %dV %dV", analogInputsRaw[ANALOG_IN_1], analogInputsRaw[ANALOG_IN_2], analogInputsRaw[ANALOG_IN_3]);
}

static void updateText() {
	uint8_t batterLevel = calcBatteryLevel(ANALOG_IN_1);
	uint8_t rssiLevel = calcBatteryLevel(ANALOG_IN_1);
	
	/*if (timeSec % 2 == 0) {
	  setCharInverted(TEXT_1_LINE, 0, TEXT_INVERTED_FLIP);
	  setCharInverted(TEXT_1_LINE, 1, TEXT_INVERTED_FLIP);
	}*/
	
	//printDebugInfo();

	snprintf(text[0], TEXT_LINE_MAX_CHARS, "%02d:%02d:%02d %d.%02dV %d.%02dV %d.%02dV %d%%", 
    timeHour, timeMin, timeSec,
    analogInputs[ANALOG_IN_1].high, analogInputs[ANALOG_IN_1].low, 
    analogInputs[ANALOG_IN_2].high, analogInputs[ANALOG_IN_2].low, 
#ifdef G_OSD
    analogInputs[ANALOG_IN_3].high, analogInputs[ANALOG_IN_3].low, 
#else
    0, 0,
#endif
    rssiLevel);

#ifdef GPS_ENABLED

	if (timeSec%6 < 2)	{
	  snprintf(text[1], TEXT_LINE_MAX_CHARS, "GPS1: %ld, %ld %dm %ds", gpsLastData.pos.latitude, gpsLastData.pos.longitude, gpsLastData.pos.altitude, gpsLastData.sats);
	}	  
	else if (timeSec%6 < 4)	{
	  snprintf(text[1], TEXT_LINE_MAX_CHARS, "GPS2: %lum, %udeg Home %s", gpsDistToHome, gpsBearingToHome, gpsHomePosSet ? "VALID" : "BAD");
	} 
	else {
		char tmp[3];
		if (gpsLastData.checksumValid) {
		  strncpy(tmp, "FIX", 4);
		}
		else {
			strncpy(tmp, "BAD", 4);
		}
	  snprintf(text[1], TEXT_LINE_MAX_CHARS, "GPS3: %dkm/h %d deg %ld %s", gpsLastData.speed, gpsLastData.angle, gpsLastData.date, tmp);
	}
#endif
}

static void drawTextLine(uint8_t textNumber)
{
	_delay_us(4);
	uint8_t currLine = (line - textLines[textNumber]) / 2;
	DDRB |= OUT1;
	for (uint8_t i = 0; i < TEXT_LINE_MAX_CHARS; ++i) {
		SPDR = textData[textNumber][currLine*TEXT_LINE_MAX_CHARS + i];
		DELAY_9_NOP();
		DELAY_9_NOP();
		DELAY_9_NOP();
	}
	DELAY_4_NOP();
	SPDR = 0x00;
	DDRB &= ~OUT1;
}

#endif // TEXT_ENABLED

#endif /* TEXT_H_ */