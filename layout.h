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

#ifdef TEXT_ENABLED
static void updateText(uint8_t textId) {
  //testPrintDebugInfo();
  uint8_t pos = 0;

	if (textId == 0) {
		pos = printText(gText[textId], 0, "BATT1 ");
#if ANALOG_IN_NUMBER == 2
    pos = printText(gText[textId], 7, "RSSI");
#else // ANALOG_IN_NUMBER == 3
    pos = printText(gText[textId], 7, "BATT2 ");
	  pos = printText(gText[textId], 14, "RSSI");    
#endif //ANALOG_IN_NUMBER == 2	  
	  pos = printTime(gText[textId], TEXT_LINE_MAX_CHARS-9);
  }
  else if (textId == 1) {
	  printText(gText[textId], TEXT_LINE_MAX_CHARS-1-strlen(TEXT_CALLSIGN), TEXT_CALLSIGN);
	  pos = printAdc(gText[textId], 0, ANALOG_IN_1);
#if ANALOG_IN_NUMBER == 2
    pos = printRssiLevel(gText[textId], 7, ANALOG_IN_2);
#else // ANALOG_IN_NUMBER == 3
    pos = printAdc(gText[textId], 7, ANALOG_IN_2);
	  pos = printRssiLevel(gText[textId], 14, ANALOG_IN_3);
#endif //ANALOG_IN_NUMBER == 2
  }
  else if (textId == 2) {
#ifdef GPS_ENABLED
	  if (gHomePosSet || gGpsLastValidData.speed >= 10) {
		  return;
	  }
	  pos = printText(gText[textId], 4, "ACQUIRING HOME POS..."); 
#endif //GPS_ENABLED
  }	  
  else if (textId == 3) {
#ifdef GPS_ENABLED
	  if (gHomePosSet || gGpsLastValidData.speed >= 10) {
		  return;
	  }
    pos = printGpsNumber(gText[textId], pos+3, gGpsLastValidData.pos.latitude, 1);
    pos = printGpsNumber(gText[textId], pos+1, gGpsLastValidData.pos.longitude, 0); 
#endif //GPS_ENABLED
	}
	else if (textId == 4) {
#ifdef GPS_ENABLED
	  pos = printNumber(gText[textId], 0, gGpsLastValidData.speed);
    if (!gHomePosSet) {
		  pos = printText(gText[textId], 4, "-");
	  }
	  else {
		  pos = printNumber(gText[textId], 4, gHomeDistance);
	  }		  
		if (gGpsLastValidData.fix) {
		  pos = printNumber(gText[textId], TEXT_LINE_MAX_CHARS-10, gGpsLastValidData.sats);
		}
		else {			
		  pos = printText(gText[textId], TEXT_LINE_MAX_CHARS-10, "-");
	  }
		pos = printNumber(gText[textId], TEXT_LINE_MAX_CHARS-5, gGpsLastValidData.pos.altitude - gHomePos.altitude);
#endif //GPS_ENABLED
	}
	else if (textId == 5) {
#ifdef GPS_ENABLED
		pos = printText(gText[textId], pos, "SPD");
		pos = printText(gText[textId], pos+1, "LOS   ");
		
		pos = printText(gText[textId], TEXT_LINE_MAX_CHARS-10, "SAT");
		pos = printText(gText[textId], TEXT_LINE_MAX_CHARS-5, "ALT ");
#endif //GPS_ENABLED
	}
	else {		
		pos = printText(gText[textId], pos, "T:");
		pos = printText(gText[textId], TEXT_LINE_MAX_CHARS-1-4, "V:");
		pos = printNumber(gText[textId], pos+1, textId + 1);
	}
}
#endif //TEXT_ENABLED

static void updateGrapics() {
#ifdef GPS_ENABLED
  drawArrow(gHomeBearing - gGpsLastValidData.angle);
#endif //GPS_ENABLED
}

#endif /* LAYOUT_H_ */