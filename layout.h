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

#ifndef LAYOUT_H_
#define LAYOUT_H_

static void updateText(uint8_t textId) {
  //testPrintDebugInfo();
  uint8_t pos = 0;

  if (textId == 0) {
	  pos = printTime(gText[textId], pos);
	  
	  pos = printAdc(gText[textId], pos+1, ANALOG_IN_1);
	  
#if ANALOG_IN_NUMBER == 2
    pos = printRssiLevel(gText[textId], pos+1, ANALOG_IN_2);
#else // ANALOG_IN_NUMBER == 3
    pos = printAdc(gText[textId], pos+1, ANALOG_IN_2);
	  pos = printRssiLevel(gText[textId], pos+1, ANALOG_IN_3);
#endif //ANALOG_IN_NUMBER == 2
  }
  else if (textId == 1) {
#ifdef GPS_ENABLED
		pos = printNumberWithUnit(gText[textId], pos, gHomeDistance, TEXT_LENGTH_UNIT);
		pos = printNumberWithUnit(gText[textId], pos+1, gHomeBearing, "DEG");
		pos = printText(gText[textId], pos+1, gHomePosSet ? "H-SET" : "");
#endif //GPS_ENABLED
	}
	else if (textId == 2) {
#ifdef GPS_ENABLED
		pos = printGpsNumber(gText[textId], pos, gGpsLastValidData.pos.latitude, 1);
		char tmp[13];
		uint8_t length = printGpsNumber(tmp, 0, gGpsLastValidData.pos.longitude, 0);
		printText(gText[textId], TEXT_LINE_MAX_CHARS - length, tmp);
#endif //GPS_ENABLED
	}
	else if (textId == 3) {
#ifdef GPS_ENABLED
		pos = printNumberWithUnit(gText[textId], pos, gGpsLastValidData.pos.altitude - gHomePos.altitude, TEXT_LENGTH_UNIT);
		pos = printNumberWithUnit(gText[textId], pos+1, gGpsLastValidData.speed, TEXT_SPEED_UNIT);
		pos = printNumberWithUnit(gText[textId], pos+1, gGpsLastValidData.angle, "DEG");
		pos = printNumberWithUnit(gText[textId], pos+1, gGpsLastValidData.sats, "S");
		pos = printText(gText[textId], pos+1, gGpsLastValidData.fix ? "FIX" : "BAD");
#endif //GPS_ENABLED
	}
	else {		
		pos = printText(gText[textId], pos, "T:");
		pos = printText(gText[textId], TEXT_LINE_MAX_CHARS-1-4, "V:");
		pos = printNumber(gText[textId], pos+1, textId + 1);
	}
}

static void updateGrapics() {
#ifdef GPS_ENABLED
  drawArrow(gHomeBearing - gGpsLastValidData.angle);
#endif //GPS_ENABLED
}

#endif /* LAYOUT_H_ */