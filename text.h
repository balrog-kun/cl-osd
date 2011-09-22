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
#include "home.h"

#define TEXT_ALIGN_LEFT 0
#define TEXT_ALIGN_RIGHT 1

// Text vars
static uint16_t const textLines[TEXT_LINES] = {TEXT_TRIG_LINES_LIST};
static char text[TEXT_LINES][TEXT_LINE_MAX_CHARS];
static uint8_t textPixmap[TEXT_LINE_MAX_CHARS*TEXT_CHAR_HEIGHT];
#ifdef TEXT_INVERTED_ENABLED
static uint8_t textInverted[TEXT_LINES][TEXT_LINE_MAX_CHARS/8];
#endif // TEXT_INVERTED_ENABLED

// Functions
static void clearText() {
	for (uint8_t i = 0; i < TEXT_LINES; ++i) {
	  for (uint8_t j = 0; j < TEXT_LINE_MAX_CHARS; ++j) {
		  text[i][j] = 0;
	  }		  
	}
}

static void clearTextPixmap() {
	for (uint16_t j = 0; j < TEXT_LINE_MAX_CHARS*TEXT_CHAR_HEIGHT; ++j) {
		textPixmap[j] = 0;
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

static void updateTextPixmapLine(uint8_t textId, uint8_t line) {
	for (uint8_t j = 0; j < TEXT_LINE_MAX_CHARS; ++j) {
		uint8_t val;
		if (text[textId][j] == ' ' || text[textId][j] == 0) {
			val = 0;
		}
		else {			
		  uint16_t charPos = (text[textId][j] * TEXT_CHAR_HEIGHT) + line;
		  val = getCharData(charPos);
#ifdef TEXT_INVERTED_ENABLED
		  if (charInverted(textId, j)) {
		    val = ~val;
		  }
#endif // TEXT_INVERTED_ENABLED
		}
		uint16_t bytePos = line*TEXT_LINE_MAX_CHARS + j; 
		textPixmap[bytePos] = val;			
	}
}

static void updateTextPixmap(uint8_t textId) {
	for (uint8_t i = 0; i < TEXT_CHAR_HEIGHT; i++) {
	  updateTextPixmapLine(textId, i);
	}
}

static uint8_t printText(char* str, uint8_t pos, const char* str2) {
	uint8_t length = strlen(str2);
	if (pos + length >= TEXT_LINE_MAX_CHARS) {
    length = TEXT_LINE_MAX_CHARS;
	}
	strncpy(&str[pos], str2, length);
	return length+pos;
}

static uint8_t printNumber(char* str, uint8_t pos, int32_t number) {
	uint8_t length = 1;
	int32_t tmp = absi32(number);
	while (tmp > 9) {
		tmp /= 10;
		++length;
	}
	if (number < 0) {
		++length;
	}
	if (pos + length >= TEXT_LINE_MAX_CHARS) {
    return TEXT_LINE_MAX_CHARS;
	}
	myItoa(number, &str[pos]);
	return pos+length;
}

static uint8_t printNumberWithUnit(char* str, uint8_t pos, int32_t number, const char* unit) {
	pos = printNumber(str, pos, number);
	return printText(str, pos, unit);
}

static uint8_t printTime(char* str, uint8_t pos) {
	if (time.hour < 10) {
		str[pos++] = '0';
	}
	pos = printNumberWithUnit(str, pos, time.hour, ":");
	if (time.min < 10) {
		str[pos++] = '0';
	}	
	pos = printNumberWithUnit(str, pos, time.min, ":");
	if (time.sec < 10) {
		str[pos++] = '0';
	}	
	return printNumber(str, pos, time.sec);
}

static uint8_t printAdc(char* str, uint8_t pos, const uint8_t adcInput) {
	uint8_t low = analogInputs[adcInput].low;
	uint8_t high = analogInputs[adcInput].high;
	pos = printNumber(str, pos, high);
	str[pos++] = '.';
	if(low < 10) {
		str[pos++] = '0';
	}
	return printNumberWithUnit(str, pos, low, "V");		
}

static uint8_t printRssiLevel(char* str, uint8_t pos, const uint8_t adcInput) {
	uint8_t rssiLevel = calcRssiLevel(adcInput);
	return printNumberWithUnit(str, pos, rssiLevel, "%");
}

static uint8_t printBatterLevel(char* str, uint8_t pos, const uint8_t adcInput) {
	uint8_t batterLevel = calcBatteryLevel(adcInput);
	return printNumberWithUnit(str, pos, batterLevel, "%");
}

static uint8_t printGpsNumber(char* str, uint8_t pos, int32_t number, uint8_t numberLat) {
	if (number == 0) {
	  pos = printText(str, pos, "--:--.----?"); 
	  return pos;
  }
	
	uint8_t hour = number / 1000000;
	uint8_t min = (number - (hour * 1000000)) / 10000; //Get minute part
  uint32_t minDecimal = number % 10000; //Get minute decimal part
  
  const char* str2;
  if (numberLat) {
	  str2 = number > 0 ? "N" : "S";
  }
  else {
	  str2 = number > 0 ? "E" : "W";
  }
  
  pos = printNumberWithUnit(str, pos, hour, ":");
  pos = printNumberWithUnit(str, pos, min, ".");
  return printNumberWithUnit(str, pos, minDecimal, str2);
}

static void updateText(uint8_t textId) {
  //testPrintDebugInfo();
  uint8_t pos = 0;

  if (textId == 0) {
	  pos = printTime(text[textId], pos);
	  
	  pos = printAdc(text[textId], pos+1, ANALOG_IN_1);
	  
#if ANALOG_IN_NUMBER == 2
    pos = printRssiLevel(text[textId], pos+1, ANALOG_IN_2);
#else // ANALOG_IN_NUMBER == 3
    pos = printAdc(text[textId], pos+1, ANALOG_IN_2);
	  pos = printRssiLevel(text[textId], pos+1, ANALOG_IN_3);
#endif //ANALOG_IN_NUMBER == 2
  }
  else if (textId == 1) {
#ifdef GPS_ENABLED
		pos = printNumberWithUnit(text[textId], pos, homeDistance, TEXT_LENGTH_UNIT);
		pos = printNumberWithUnit(text[textId], pos+1, homeBearing, "DEG");
		pos = printText(text[textId], pos+1, homePosSet ? "H-SET" : "");
#endif //GPS_ENABLED
	}
	else if (textId == 2) {
#ifdef GPS_ENABLED
		pos = printGpsNumber(text[textId], pos, gpsLastValidData.pos.latitude, 1);
		char tmp[13];
		uint8_t length = printGpsNumber(tmp, 0, gpsLastValidData.pos.longitude, 0);
		printText(text[textId], TEXT_LINE_MAX_CHARS - length, tmp);
#endif //GPS_ENABLED
	}
	else if (textId == 3) {
#ifdef GPS_ENABLED
		pos = printNumberWithUnit(text[textId], pos, gpsLastValidData.pos.altitude - homePos.altitude, TEXT_LENGTH_UNIT);
		pos = printNumberWithUnit(text[textId], pos+1, gpsLastValidData.speed, TEXT_SPEED_UNIT);
		pos = printNumberWithUnit(text[textId], pos+1, gpsLastValidData.angle, "DEG");
		pos = printNumberWithUnit(text[textId], pos+1, gpsLastValidData.sats, "S");
		pos = printText(text[textId], pos+1, gpsLastValidData.fix ? "FIX" : "BAD");
#endif //GPS_ENABLED
	}
	else {		
		pos = printText(text[textId], pos, "T:");
		pos = printText(text[textId], TEXT_LINE_MAX_CHARS-1-4, "V:");
		pos = printNumber(text[textId], pos+1, textId + 1);
	}
}

static void drawTextLine(uint8_t textId)
{
	_delay_us(3);
	uint8_t currLine = ((uint16_t)(line) - textLines[textId]) / TEXT_SIZE_MULT;
	for (uint8_t i = 0; i < TEXT_LINE_MAX_CHARS; ++i) {
		if (text[textId][i] != ' ' && text[textId][i] != 0) {
			DDRB |= OUT1;
		}
		else {
			DDRB &= ~OUT1;
		}
		SPDR = textPixmap[(uint16_t)(currLine)*TEXT_LINE_MAX_CHARS + i];
		DELAY_4_NOP();
#ifndef TEXT_SMALL_ENABLED
		DELAY_6_NOP();
		DELAY_9_NOP();
#endif //TEXT_SMALL_ENABLED	
	}
	DELAY_10_NOP();
	SPDR = 0x00;
	DDRB &= ~OUT1;
}

#endif // TEXT_ENABLED

#endif /* TEXT_H_ */