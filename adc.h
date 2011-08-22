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


#ifndef ADC_H_
#define ADC_H_

#include "config.h"

// adc vars
typedef struct {
  uint8_t low;
  uint8_t high;
} TAnalogValue;

static TAnalogValue analogInputs[ANALOG_IN_NUMBER] = {};
static uint16_t analogInputsRaw[ANALOG_IN_NUMBER] = {};

#ifndef ADC_ENABLED

static uint8_t calcBatteryLevel(uint8_t adcInput) { DUMMY_FUNC }
static uint8_t calcRssiLevel(uint8_t adcInput) { DUMMY_FUNC }

#else

static void setupAdc() {
  // ADC setup
	DIDR0 = 0x00;
	ADMUX |= (1<<REFS0) | (1<<MUX0); // Ref is AVCC & mux is set to adc1
	ADCSRA |= (1<<ADEN) | (1<<ADATE) | (1<<ADPS2) | (1<<ADPS1); // | (1<<ADPS0); 
	ADCSRB = 0; // Free running
}

static void measureAnalog() {
  uint32_t temp = 0;
	uint8_t adcLow = 0; 
  uint8_t adcHigh = 0;
  for (uint8_t i = 0; i < ANALOG_IN_NUMBER; ++i) {
	  ADMUX &= 0xF0;
    ADMUX |= (i + ADC_OFFSET);	
	  ADCSRA |= (1<<ADSC); // start measure
	  while ((ADCSRA & (1<<ADIF)) == 0); // Wait to finish
	  ADCSRA |= (1<<ADIF);
	  temp = ADCW;
    analogInputsRaw[i] = temp;
	  temp = (temp * 5 * 62) / 10;
	  adcHigh = temp / 1024;
	  temp -= (uint16_t)(adcHigh) * 1024;
	  adcLow = (temp * 100) / 1024;
	
	  analogInputs[i].low = adcLow;
    analogInputs[i].high = adcHigh;
  }    
}

static uint8_t calcGenericLevel(uint8_t adcInput, uint16_t minLevel, uint16_t maxLevel) {
	uint16_t level = ((analogInputs[adcInput].high * 100) + analogInputs[adcInput].low);
	if (level > maxLevel) {
		level = 100;
	}		
	else if (level > minLevel) {
		level -= minLevel;
		level *= 100;
		level /= maxLevel - minLevel;
	}
	else {
		level = 0;
	}
	return level;
}

static uint8_t calcBatteryLevel(uint8_t adcInput) {
  return calcGenericLevel(adcInput, BATT_MIN_VOLTAGE_INT, BATT_MAX_VOLTAGE_INT);
}

static uint8_t calcRssiLevel(uint8_t adcInput) {
  return calcGenericLevel(adcInput, RSSI_MIN_VOLTAGE_INT, RSSI_MAX_VOLTAGE_INT);
}

#endif //ADC_ENABLED 

#endif /* ADC_H_ */