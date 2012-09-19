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

#ifndef ALARMS_H_
#define ALARMS_H_

// Alarms
static uint8_t gAlarmBatt1 = 0;
#if ANALOG_IN_NUMBER > 2
static uint8_t gAlarmBatt2 = 0;
#endif
static uint8_t gAlarmRssi = 0;
static uint8_t gAlarmCurrent = 0;
static uint8_t gAlarmPowerUsage = 0;
#ifdef GPS_ENABLED
static uint16_t gAlarmSpeed = 0;
static uint16_t gAlarmDistance = 0;
static uint16_t gAlarmAltitude = 0;
#endif //GPS_ENABLED

#ifdef ALARM_ENABLED
static void updateAlarms() {
#ifdef GPS_ENABLED
#ifdef ALARM_SPEED_HIGH  
  gAlarmSpeed = gGpsLastValidData.speed > ALARM_SPEED_HIGH; 
#endif
#ifdef ALARM_ALTITUDE_HIGH 
	gAlarmAltitude = gGpsLastValidData.pos.altitude - gHomePos.altitude > ALARM_ALTITUDE_HIGH || gGpsLastValidData.pos.altitude - gHomePos.altitude < ALARM_ALTITUDE_LOW;
#endif
#ifdef ALARM_DISTANCE_HIGH
	gAlarmDistance = (gHomeDistance > ALARM_DISTANCE_HIGH);
#endif
#endif //GPS_ENABLED

#ifdef ALARM_BATT1_LOW_INT
	gAlarmBatt1 = ((gSensorVoltage1.high * 100) + gSensorVoltage1.low) < ALARM_BATT1_LOW_INT;
#endif
#if ANALOG_IN_NUMBER == 2
#ifdef ALARM_RSSI_LOW
  gAlarmRssi = gSensorRssi < ALARM_RSSI_LOW;
#endif
#ifdef ALARM_CURRENT_HIGH
  gAlarmCurrent = gSensorCurrent < ALARM_CURRENT_HIGH;
#endif
#ifdef ALARM_POWER_USAGE_HIGH
  gAlarmPowerUsage = (gSensorPowerUsage/1000) < ALARM_POWER_USAGE_HIGH;
#endif
#else // ANALOG_IN_NUMBER > 2
#ifdef ALARM_BATT2_LOW_INT
	gAlarmBatt2 = ((gSensorVoltage1.high * 100) + gSensorVoltage1.low) < ALARM_BATT2_LOW_INT;
#endif
#ifdef ALARM_RSSI_LOW
	gAlarmRssi = gSensorRssi < ALARM_RSSI_LOW;
#endif
#endif //ANALOG_IN_NUMBER == 2
}
#endif // ALARM_ENABLED

#endif /* ALARMS_H_ */