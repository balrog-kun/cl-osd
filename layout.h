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

#include "statistics.h"
#include "alarms.h"

#ifdef TEXT_ENABLED
static void updateText(uint8_t textId) {
  //testPrintDebugInfo();
  uint8_t pos = 0;

  // --------------- TEXT LINE 1 (From top) -----------------------
	if (textId == 0) {
#ifdef SENSOR_VOLTAGE_1_ENABLED
		printText(gText[textId], pos, "\1521"); //Battery symbol + '1'
		pos += 7;
#endif
#ifdef SENSOR_VOLTAGE_2_ENABLED
    printText(gText[textId], pos, "\1522"); //Battery symbol + '2'
	  pos += 7;
#endif
#ifdef SENSOR_RSSI_ENABLED
    printText(gText[textId], pos, "\151"); //Antenna symbol (RSSI)
	  pos += 7;
#endif
#ifdef SENSOR_CURRENT_ENABLED
    printText(gText[textId], pos, "A"); //Ampere
    printText(gText[textId], pos+7, "MAH"); //Milli ampere hours
#endif
#ifdef TIME_HOUR_ENABLED
	  pos = printTime(gText[textId], TEXT_LINE_MAX_CHARS-9); // Time with hours
#else
    pos = printTime(gText[textId], TEXT_LINE_MAX_CHARS-6); // Time without hours
#endif //TIME_HOUR_ENABLED
  }
  // --------------- TEXT LINE 2 (From top) -----------------------
  else if (textId == 1) {
	  
	  printText(gText[textId], TEXT_LINE_MAX_CHARS-1-strlen(TEXT_CALL_SIGN), TEXT_CALL_SIGN); // Call sign
	  
#ifdef SENSOR_VOLTAGE_1_ENABLED
	  if (!gAlarmBatt1 || gBlink1Hz) {
	    printAdc(gText[textId], pos, ANALOG_IN_1); // Voltage 1
	  }
	  pos += 7;
#endif
#ifdef SENSOR_VOLTAGE_2_ENABLED
    if (!gAlarmBatt2 || gBlink1Hz) {
      printAdc(gText[textId], pos, ANALOG_IN_2); // Voltage 2
	  }
	  pos += 7;
#endif
#ifdef SENSOR_RSSI_ENABLED
	  if (!gAlarmRssi || gBlink1Hz) {
	    printRssiLevel(gText[textId], pos, SENSOR_RSSI_INPUT); // RSSI
	  }
	  pos += 7;
#endif
#ifdef SENSOR_CURRENT_ENABLED
	  if (!gAlarmCurrent || gBlink1Hz) {
	    printNumber(gText[textId], pos, gSensorCurrent); //Ampere
	  }
	  if (!gAlarmPowerUsage || gBlink1Hz) {		
      printNumber(gText[textId], pos+7, gSensorPowerUsage/1000); //Milli ampere hours
	  }
	  pos += 7;
#endif
  }
  // --------------- TEXT LINE 3 (From top) -----------------------
  else if (textId == 2) {
#ifdef GPS_ENABLED
	  if (gStatisticsShow) {
	    if (gHomePosSet) {
#ifdef STATISTICS_ENABLED
		    pos = printText(gText[textId], 7, "DST  SPD TRIP  ALT"); // Statistics (titles)
#endif //STATISTICS_ENABLED
	    }
	    else if (gBlink1Hz) {
	      pos = printText(gText[textId], 10, "NO HOME POS");
	    }
	  }
	  else {
#ifdef TEXT_COMPASS_ENABLED
		  printCompassArrow(gText[textId], 7, gGpsLastValidData.angle, 19); // Upper compass part
#endif //TEXT_COMPASS_ENABLED
#ifdef GPS_POS_ALWAYS_SHOWN_ENABLED 
      pos = printGpsNumber(gText[textId], pos+4, gGpsLastValidData.pos.latitude, 1); // GPS position in place of text compas (Truglodite)
      pos = printGpsNumber(gText[textId], pos+2, gGpsLastValidData.pos.longitude, 0);
#endif //GPS_POS_ALWAYS_SHOWN_ENABLED 
	  }
#endif //GPS_ENABLED
  }
  // --------------- TEXT LINE 4 (From top) -----------------------
  else if (textId == 3) {
#ifdef GPS_ENABLED
	  if (gStatisticsShow) {
	    if (gHomePosSet) {
#ifdef STATISTICS_ENABLED
		    pos = 7;
		    printNumber(gText[textId], pos, gStatMaxDistance); // Statistics (numbers)
		    printNumber(gText[textId], pos+5, gStatMaxSpeed);
		    printNumber(gText[textId], pos+9, gStatDistTraveled);
		    printNumber(gText[textId], pos+15, gStatMaxAltitude);
#endif //STATISTICS_ENABLED
	    }
	    else {
        pos = printGpsNumber(gText[textId], pos+4, gGpsLastValidData.pos.latitude, 1); // GPS position
        pos = printGpsNumber(gText[textId], pos+2, gGpsLastValidData.pos.longitude, 0);
	    }
	  }
	  else {
#ifdef TEXT_COMPASS_ENABLED		  
		  printCompass(gText[textId], 7, gGpsLastValidData.angle, 19); // Lower compass part
#endif //TEXT_COMPASS_ENABLED
	  }
#endif //GPS_ENABLED
	}
	// --------------- TEXT LINE 5 (From top) -----------------------
	else if (textId == 4) {
#ifdef GPS_ENABLED
    if (!gAlarmSpeed || gBlink1Hz) {
	    pos = printNumber(gText[textId], 0, gGpsLastValidData.speed); // Speed
	  }		
    if (!gHomePosSet) {
		  pos = printText(gText[textId], 5, "-");
	  }
	  else {
		  if (!gAlarmDistance || gBlink1Hz) {
		    pos = printNumber(gText[textId], 5, gHomeDistance); // Distance
		  }			
	  }		  
		if (gGpsLastValidData.fix) {
		  pos = printNumber(gText[textId], TEXT_LINE_MAX_CHARS-10, gGpsLastValidData.sats); // Satellites
		}
		else {			
		  pos = printText(gText[textId], TEXT_LINE_MAX_CHARS-10, "-");
	  }
	  if (!gAlarmAltitude || gBlink1Hz) {
		  pos = printNumber(gText[textId], TEXT_LINE_MAX_CHARS-5, gGpsLastValidData.pos.altitude - gHomePos.altitude); // Altitude
	  }		  
#endif //GPS_ENABLED
	}
	// --------------- TEXT LINE 6 (From top) -----------------------
	else if (textId == 5) {
#ifdef GPS_ENABLED
		pos = printText(gText[textId], 0, "SPD");
		pos = printText(gText[textId], 5, "LOS");
		
		if (gHomePosSet) {
		  pos = printText(gText[textId], pos+3, "\146"); // Home set symbol
		}
		else {
			pos = printText(gText[textId], pos+3, "\147"); // No home set symbol
		}
		
		// Blink satellite at checksum errors! Thanks to after.burner
		if (gGpsLastData.checksumValid> 0 || gBlink1Hz) {
  		  pos = printText(gText[textId], TEXT_LINE_MAX_CHARS-10, "\150"); // Satellites symbol
		}
		pos = printText(gText[textId], TEXT_LINE_MAX_CHARS-5, "ALT");
		//pos = printText(gText[textId], pos-2, "\144-\145");
#endif //GPS_ENABLED
	}
	// --------------- For testing... -----------------------
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