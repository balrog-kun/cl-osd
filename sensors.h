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

#ifndef SENSORS_H_
#define SENSORS_H_

#include "adc.h"

static uint8_t gSensorBatteryPercentage = 0;
static uint8_t gSensorRssi = 0;
static uint8_t gSensorCompassDirection = 0;
static uint8_t gSensorCurrent = 0;
static uint32_t gSensorPowerUsage = 0;
static TAnalogValue gSensorVoltage1 = {};
static TAnalogValue gSensorVoltage2 = {};

#ifdef SENSORS_ENABLED
#ifndef ADC_ENABLED

#define DUMMY_FUNC return 0;
static void updateSensors() { DUMMY_FUNC }

#else //ADC_ENABLED

static uint8_t calcGenericVoltageLevel(uint8_t adcInput, uint16_t inMin, uint16_t inMax, uint16_t outMin, uint16_t outMax) {
	uint16_t level = ((gAnalogInputs[adcInput].high * 100) + gAnalogInputs[adcInput].low);
	if (level > inMax) {
		level = outMax;
	}		
	else if (level > inMin) {
		level -= inMin;
		level *= outMax - outMin;
		level /= inMax - inMin;
		level += outMin;
	}
	else {
		level = outMin;
	}
	return level;
}

static void updateSensors() {
#ifdef SENSOR_VOLTAGE_1_ENABLED
    gSensorVoltage1 = gAnalogInputs[ANALOG_IN_1];
#endif
#ifdef SENSOR_VOLTAGE_2_ENABLED
    gSensorVoltage2 = gAnalogInputs[ANALOG_IN_2];
#endif
#ifdef SENSOR_BATTERY_PERCENTAGE_ENABLED
   gSensorBatteryPercentage = calcGenericVoltageLevel(SENSOR_BATTERY_PERCENTAGE_INPUT, BATT_MIN_VOLTAGE_INT, BATT_MAX_VOLTAGE_INT, 0, 100);
#endif 
#ifdef SENSOR_RSSI_ENABLED
   gSensorRssi = calcGenericVoltageLevel(SENSOR_RSSI_INPUT, RSSI_MIN_VOLTAGE_INT, RSSI_MAX_VOLTAGE_INT, 0, 100);
#endif
#ifdef SENSOR_COMPASS_ENABLED
   gSensorCompassDirection = calcGenericVoltageLevel(SENSOR_COMPASS_INPUT, COMPASS_MIN_VOLTAGE_INT, COMPASS_MAX_VOLTAGE_INT, 0, 359);
#endif
#ifdef SENSOR_CURRENT_ENABLED
   gSensorCurrent = calcGenericVoltageLevel(SENSOR_CURRENT_INPUT, CURRENT_MIN_VOLTAGE_INT, CURRENT_MAX_VOLTAGE_INT, 0, SENSOR_CURRENT_MAX_AMPS);
   gSensorPowerUsage += ((uint32_t)(gSensorCurrent) * 1000 * 1000) / (3600 / SCREEN_AND_SENSOR_UPDATES_PER_SEC);
#endif
}  

#endif //ADC_ENABLED
#endif //SENSORS_ENABLED

#endif /* SENSORS_H_ */