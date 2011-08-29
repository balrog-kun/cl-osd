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

// Text vars
static uint16_t const textLines[TEXT_LINES] = {TEXT_TRIG_LINES_LIST};
static char text[TEXT_LINE_MAX_CHARS];
static uint8_t textData[TEXT_LINES][TEXT_LINE_MAX_CHARS*TEXT_CHAR_HEIGHT];
#ifdef TEXT_INVERTED_ENABLED
static uint8_t textInverted[TEXT_LINES][TEXT_LINE_MAX_CHARS/8];
#endif // TEXT_INVERTED_ENABLED

// Functions
static void clearText() {
	for (uint8_t j = 0; j < TEXT_LINE_MAX_CHARS; ++j) {
		text[j] = 0;
	}
}

static void clearTextData() {
	for (uint8_t i = 0; i < TEXT_LINES; ++i) {
		for (uint16_t j = 0; j < TEXT_LINE_MAX_CHARS*TEXT_CHAR_HEIGHT; ++j) {
			textData[i][j] = 0;
		}		
	}
}

#ifdef TEXT_INVERTED_ENABLED
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
#endif // TEXT_INVERTED_ENABLED

static uint8_t getCharData(uint16_t charPos) {
	if (charPos >= CHAR_ARRAY_OFFSET && charPos < CHAR_ARRAY_MAX) {
	  return eeprom_read_byte(&(oem6x8[charPos - CHAR_ARRAY_OFFSET]));
	}	
	else {
		return 0x00;
	}	  
}

static void drawText(uint8_t textId) {
	for (uint8_t i = 0; i < TEXT_CHAR_HEIGHT; i++) {
		for (uint8_t j = 0; j < TEXT_LINE_MAX_CHARS; j++) {
			uint16_t charPos = (text[j]*TEXT_CHAR_HEIGHT) + i;
			uint8_t val = getCharData(charPos);
#ifdef TEXT_INVERTED_ENABLED
			if (charInverted(textId, j)) {
				val = ~val;
			}
#endif // TEXT_INVERTED_ENABLED
			uint16_t bytePos = i*TEXT_LINE_MAX_CHARS + j;
			textData[textId][bytePos] = val;
		}			
	}
}

static void myReverse(char s[], uint8_t size) {
  uint8_t i;
  char c;
  size -= 1;
  for (i = 0; i < size; i++) {
    c = s[i];
    s[i] = s[size - i];
    s[size - i] = c;
  }
}

static void myItoa(int32_t n, char s[])
{
  int8_t i;
  int8_t sign = 0;
 
  if (n < 0) {  
	  sign = -1; /* record sign */
    n = -n;          /* make n positive */
  }	
  i = 0;
  do {       /* generate digits in reverse order */
    s[i++] = n % 10 + '0';   /* get next digit */
  } while ((n /= 10) > 0);     /* delete it */
  if (sign < 0) {
    s[i++] = '-';
  }	
  s[i] = '\0';
  myReverse(s, i);
}

static uint8_t printText(uint8_t pos, char* str) {
	uint8_t length = strlen(str);
	if (pos + length >= TEXT_LINE_MAX_CHARS) {
    length = TEXT_LINE_MAX_CHARS;
	}
	strncpy(&text[pos], str, length);
	return length+pos;
}

static uint8_t printNumber(uint8_t pos, int32_t number) {
	uint8_t length = 1;
	int32_t tmp = number;
	while (tmp > 9) {
		tmp /= 10;
		++length;
	}
	if (pos + length >= TEXT_LINE_MAX_CHARS) {
    return TEXT_LINE_MAX_CHARS;
	}	
	myItoa(number, &text[pos]);
	return pos+length;
}

static uint8_t printNumberWithUnit(uint8_t pos, int32_t number, char* unit) {
	pos = printNumber(pos, number);
	return printText(pos, unit);
}

static uint8_t printTime(uint8_t pos) {
	if (timeHour < 10) {
		text[pos++] = '0';
	}
	pos = printNumberWithUnit(pos, timeHour, ":");
	if (timeMin < 10) {
		text[pos++] = '0';
	}	
	pos = printNumberWithUnit(pos, timeMin, ":");
	if (timeSec < 10) {
		text[pos++] = '0';
	}	
	return printNumber(pos, timeSec);
}

static uint8_t printAdc(uint8_t pos, const uint8_t adcInput) {
	uint8_t low = analogInputs[adcInput].low;
	uint8_t high = analogInputs[adcInput].high;
	pos = printNumber(pos, high);
	text[pos++] = '.';
	if(low < 10) {
		text[pos++] = '0';
	}
	return printNumberWithUnit(pos, low, "V");		
}

static uint8_t printRssiLevel(uint8_t pos, const uint8_t adcInput) {
	uint8_t rssiLevel = calcRssiLevel(ANALOG_IN_1);
	return printNumberWithUnit(pos, rssiLevel, "%");
}

static uint8_t printBatterLevel(uint8_t pos, const uint8_t adcInput) {
	uint8_t batterLevel = calcBatteryLevel(ANALOG_IN_1);
	return printNumberWithUnit(pos, batterLevel, "%");
}

static void printDebugInfo() {
	// ---- TODO: Cleanup here! ----
	
  //snprintf(text[0], TEXT_LINE_MAX_CHARS, "%02d:%02d:%02d:%02d", hour, min, sec, tick);
	//snprintf(text[1], TEXT_LINE_MAX_CHARS, "%02d:%02d:%02d %d.%02dV %d.%02dV %d%%", hour, min, sec, adc0High, adc0Low, adc1High, adc1Low, batt1);
	//if (gpsTextType != GPS_TYPE_GPGGA ) {
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, " %s", gpsFullText);
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, "Part (%d): %s", gpsTextPartLength, gpsTextPart); //part
		//snprintf(text[1], TEXT_LINE_MAX_CHARS, "%s == %d", gpsTextPart, gpsTextType);
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, "%s == %06ld", gpsTextPart, gpsTime); //time
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, "%s == %ld", gpsTextPart, gpsLat); //Lat
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, "%s == %ld", gpsTextPart, gpsLong); //Long
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, "%s == %d", gpsTextPart, gpsFix); //fix?
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, "%s == %d", gpsTextPart, gpsSats); //sats
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, "%s == %d", gpsTextPart, gpsAltitude); //altitude
		//snprintf(text[0], TEXT_LINE_MAX_CHARS, "(%s == %d)? => %d", gpsTextPart, gpsChecksum, gpsChecksumValid); //checksum
		//snprintf(text[1], TEXT_LINE_MAX_CHARS, "%.32s", &gpsFullText[30]);		
	//}
	//snprintf(text[1], TEXT_LINE_MAX_CHARS, "%dV %dV %dV", analogInputsRaw[ANALOG_IN_1], analogInputsRaw[ANALOG_IN_2], analogInputsRaw[ANALOG_IN_3]);
}

static void updateText(uint8_t textId) {
  //printDebugInfo();
  uint8_t pos = 0;

  if (textId == 0) {
	  pos = printTime(pos);
	  pos = printAdc(pos+1, ANALOG_IN_1);
	  pos = printAdc(pos+1, ANALOG_IN_2);
#ifdef G_OSD	  
	  //pos = printAdc(pos+1, ANALOG_IN_3);
	  pos = printRssiLevel(pos+1, ANALOG_IN_3);
#endif
	  
  }
  else if(textId == 1) {
#ifdef GPS_ENABLED
	  if (timeSec%6 < 2)	{
		  pos = printText(pos, "GPS1:");
		  pos = printNumber(pos+1, gpsLastData.pos.latitude);
		  pos = printNumber(pos+1, gpsLastData.pos.longitude);
		  pos = printNumberWithUnit(pos+1, gpsLastData.pos.altitude, TEXT_LENGTH_UNIT);
		  pos = printNumber(pos+1, gpsLastData.sats);
		  pos = printText(pos, "S");
	  }
	  else if (timeSec%6 < 4)	{
		  pos = printText(pos, "GPS2:");
		  pos = printNumberWithUnit(pos+1, gpsDistToHome, TEXT_LENGTH_UNIT);
		  pos = printNumberWithUnit(pos+1, gpsBearingToHome, "DEG");
		  pos = printText(pos+1, gpsHomePosSet ? "HOME" : "");
		} 
	  else {
		  pos = printText(pos, "GPS2:");
		  pos = printNumberWithUnit(pos+1, gpsLastData.speed, TEXT_SPEED_UNIT);
		  pos = printNumberWithUnit(pos+1, gpsLastData.angle, "DEG");
		  pos = printNumber(pos+1, gpsLastData.date);
		  pos = printText(pos+1, gpsLastData.checksumValid ? "FIX" : "BAD");
	  }
#endif
	}
}

static void drawTextLine(uint8_t textNumber)
{
	_delay_us(4);
	uint8_t currLine = (line - textLines[textNumber]) / TEXT_SIZE_MULT;
	DDRB |= OUT1;
	for (uint8_t i = 0; i < TEXT_LINE_MAX_CHARS; ++i) {
		SPDR = textData[textNumber][currLine*TEXT_LINE_MAX_CHARS + i];
		DELAY_9_NOP();
		DELAY_3_NOP();
#ifndef TEXT_SMALL_ENABLED
		DELAY_6_NOP();
		DELAY_9_NOP();
#endif //TEXT_SMALL_ENABLED	
	}
	DELAY_4_NOP();
	SPDR = 0x00;
	DDRB &= ~OUT1;
}

#endif // TEXT_ENABLED

#endif /* TEXT_H_ */