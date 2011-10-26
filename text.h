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
#include "global.h"
#include "commonutils.h"

#define TEXT_ALIGN_LEFT 0
#define TEXT_ALIGN_RIGHT 1

// Text vars
static uint16_t const gTextLines[TEXT_LINES] = {TEXT_TRIG_LINES_LIST};
static uint8_t const gTextLineSizes[TEXT_LINES] = {TEXT_LINE_TEXT_SIZES};
static char gText[TEXT_LINES][TEXT_LINE_MAX_CHARS];
static int16_t gTextCharEepromPos[TEXT_LINE_MAX_CHARS];
static uint8_t gTextPixmap[TEXT_LINE_MAX_CHARS*TEXT_CHAR_HEIGHT];
static uint8_t gTextLastCharBuffer[TEXT_CHAR_HEIGHT];
static int8_t gTextLastCharEepromPos = -1;
#ifdef TEXT_INVERTED_ENABLED
static uint8_t gTextInverted[TEXT_LINES][TEXT_LINE_MAX_CHARS/8];
#endif // TEXT_INVERTED_ENABLED

// Functions
static void clearText(uint8_t textId) {
	for (uint8_t j = 0; j < TEXT_LINE_MAX_CHARS; ++j) {
		gText[textId][j] = 0;
	}
}

static void clearTextPixmap() {
	for (uint16_t j = 0; j < TEXT_LINE_MAX_CHARS*TEXT_CHAR_HEIGHT; ++j) {
		gTextPixmap[j] = 0;
	}
}

#ifdef TEXT_INVERTED_ENABLED
static void clearTextInverted() {
	for (uint8_t i = 0; i < TEXT_LINES; ++i) {
	  for (uint8_t j = 0; j < TEXT_LINE_MAX_CHARS/8; ++j) {
	    gTextInverted[i][j] = 0;
	  }
	}	  
}

static void setCharInverted(uint8_t line, uint8_t pos, uint8_t bitValue) {
	uint8_t bytePos = pos/8;
	uint8_t bitPos = pos - (bytePos*8);
	if (bitValue == TEXT_INVERTED_OFF) {
	  gTextInverted[line][bytePos] ^= ~(1<<bitPos);
	}
	else if (bitValue == TEXT_INVERTED_ON) {
	  gTextInverted[line][bytePos] |= (1<<bitPos);
	}
	else { //TEXT_INVERTED_FLIP
	  gTextInverted[line][bytePos] ^= (1<<bitPos);
	}
}

static uint8_t charInverted(uint8_t line, uint8_t pos) {
	uint8_t bytePos = pos/8;
	uint8_t bitPos = pos - (bytePos*8);
	if (gTextInverted[line][bytePos] & (1<<bitPos)) {
		return 1;
	}
	return 0;
}
#endif // TEXT_INVERTED_ENABLED

static void updateTextCharStartPos(uint8_t textId) {
	for (uint8_t j = 0; j < TEXT_LINE_MAX_CHARS; ++j) {
		uint8_t character = gText[textId][j];
		int16_t eepromPos;
		if (character == ' ' || character == 0) {
			eepromPos = -1;
		}
		else if (character < CHAR_OFFSET + CHAR_LENGTH) {
			uint16_t charPos = ((character - CHAR_OFFSET) * TEXT_CHAR_HEIGHT);
			eepromPos = (int16_t)&(oem6x8[charPos]);
		}
#ifdef TEXT_USE_SPECIAL_CHARS
		else if (character > CHAR_SPECIAL_OFFSET && character < CHAR_SPECIAL_OFFSET+CHAR_SPECIAL_LENGTH) {
			uint16_t charPos = ((character - CHAR_SPECIAL_OFFSET) * TEXT_CHAR_HEIGHT);
			eepromPos = (int16_t)&(specialChars[charPos]);
		}
#endif //TEXT_USE_SPECIAL_CHARS
		gTextCharEepromPos[j] = eepromPos;
	}		
}

static void updateTextPixmap(uint8_t textId) {
	for (uint8_t j = 0; j < TEXT_LINE_MAX_CHARS; ++j) {
		int16_t eepromPos = gTextCharEepromPos[j];
		if (gTextLastCharEepromPos != eepromPos) {
		  if (eepromPos != -1) {
		  eeprom_read_block((void*)gTextLastCharBuffer, (const void*)eepromPos, 8);
			  gTextLastCharEepromPos = eepromPos;
		  }
		  else {
			  memset(gTextLastCharBuffer, 0, 8);
			  gTextLastCharEepromPos = -1;
		  }
		}		  

	  for (uint8_t i = 0; i < TEXT_CHAR_HEIGHT; i++) {

#ifdef TEXT_INVERTED_ENABLED
		if (charInverted(textId, j)) {
		  gTextLastCharBuffer[i] = ~gTextLastCharBuffer[i];
		}
#endif // TEXT_INVERTED_ENABLED

      gTextPixmap[j + (i*TEXT_LINE_MAX_CHARS)] = gTextLastCharBuffer[i];
	  }		  
	}
}

static uint8_t printText(char* const str, uint8_t pos, const char* const str2) {
	uint8_t length = strlen(str2);
	if (pos + length >= TEXT_LINE_MAX_CHARS) {
    length = TEXT_LINE_MAX_CHARS;
	}
	strncpy(&str[pos], str2, length);
	return length+pos;
}

static uint8_t printNumber(char* const str, uint8_t pos, int32_t number) {
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

static uint8_t printNumberWithUnit(char* const str, uint8_t pos, int32_t number, const char* unit) {
	pos = printNumber(str, pos, number);
	return printText(str, pos, unit);
}

static uint8_t printTime(char* const str, uint8_t pos) {
#ifdef TIME_HOUR_ENABLED
	if (gTime.hour < 10) {
		str[pos++] = '0';
	}
	pos = printNumberWithUnit(str, pos, gTime.hour, ":");
#endif //TIME_HOUR_ENABLED
	if (gTime.min < 10) {
		str[pos++] = '0';
	}	
	pos = printNumberWithUnit(str, pos, gTime.min, ":");
	if (gTime.sec < 10) {
		str[pos++] = '0';
	}	
	return printNumber(str, pos, gTime.sec);
}

static uint8_t printAdc(char* const str, uint8_t pos, const uint8_t adcInput) {
	uint8_t low = gAnalogInputs[adcInput].low;
	uint8_t high = gAnalogInputs[adcInput].high;
	pos = printNumber(str, pos, high);
	str[pos++] = '.';
	if(low < 10) {
		str[pos++] = '0';
	}
	return printNumberWithUnit(str, pos, low, "V");		
}

static uint8_t printRssiLevel(char* const str, uint8_t pos, const uint8_t adcInput) {
	uint8_t rssiLevel = calcRssiLevel(adcInput);
	return printNumberWithUnit(str, pos, rssiLevel, "%");
}

static uint8_t printBatterLevel(char* const str, uint8_t pos, const uint8_t adcInput) {
	uint8_t batterLevel = calcBatteryLevel(adcInput);
	return printNumberWithUnit(str, pos, batterLevel, "%");
}

static uint8_t printGpsNumber(char* const str, uint8_t pos, int32_t number, uint8_t numberLat) {
	if (number == 0) {
#ifdef GPS_GOOGLE_FORMAT
    pos = printText(str, pos, "--.-------?");
#else
	  pos = printText(str, pos, "--:--.----?");
#endif
	  return pos;
  }
	
	uint8_t hour = number / 1000000;
#ifdef GPS_GOOGLE_FORMAT
  uint32_t min = number - (hour * 1000000);
  min = (min * 100)/60;
#else
	uint8_t min = (number - (hour * 1000000)) / 10000; //Get minute part
  uint32_t minDecimal = number % 10000; //Get minute decimal part
#endif
  
  const char* str2;
  if (numberLat) {
	  str2 = number > 0 ? "N" : "S";
  }
  else {
	  str2 = number > 0 ? "E" : "W";
  }

#ifdef GPS_GOOGLE_FORMAT
  pos = printNumberWithUnit(str, pos, hour, ".");
  return printNumberWithUnit(str, pos, min, str2);
#else
  pos = printNumberWithUnit(str, pos, hour, ":");
  pos = printNumberWithUnit(str, pos, min, ".");
  return printNumberWithUnit(str, pos, minDecimal, str2);
#endif
}

static uint8_t printCompassArrow(char* const str, uint8_t pos, uint16_t angle, uint8_t length) {
  printText(str, pos + ((length*10)+5)/20, "\155");
  return pos + length;
}

static uint8_t printCompass(char* const str, uint8_t pos, uint16_t angle, uint8_t length) {
  // Made by superjelli - Changed a bit by me
  if(angle % 10 < 5) {
    angle -= (angle % 10);
  }
  else {
    angle += (10 - (angle % 10));
  }
  angle += 260;
				
  for (uint8_t i = pos; i < pos+length; i++) {
	  angle = ((angle + 10) % 360);
    switch (angle) {
      case (0):
        str[i] = 'N';
      break;
      case (90):
        str[i] = 'E';
      break;
      case (180):
        str[i] = 'S';
      break;
      case (270):
        str[i] = 'W';
      break;
      default:
        str[i] = '\154';
    }
  }
  return pos + length;
}  

static void drawTextLine(uint8_t textId)
{
	_delay_us(3);
	uint8_t currLine = gActivePixmapLine;
	for (uint8_t i = 0; i < TEXT_LINE_MAX_CHARS; ++i) {
		if (gText[textId][i] != ' ' && gText[textId][i] != 0) {
			DDRB |= OUT1;
		}
		else {
			DDRB &= ~OUT1;
			DELAY_1_NOP();
		}
		SPDR = gTextPixmap[(uint16_t)(currLine)*TEXT_LINE_MAX_CHARS + i];
		DELAY_4_NOP();
#ifndef TEXT_SMALL_ENABLED
		DELAY_6_NOP();
		DELAY_7_NOP();
#endif //TEXT_SMALL_ENABLED	
	}
	DELAY_10_NOP();
	SPDR = 0x00;
	DDRB &= ~OUT1;
}

#endif // TEXT_ENABLED

#endif /* TEXT_H_ */