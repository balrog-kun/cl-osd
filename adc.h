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

#define ADC_ENABLE_RAW

// adc vars
typedef struct {
  uint8_t low;
  uint8_t high;
} TAnalogValue;

static TAnalogValue gAnalogInputs[ANALOG_IN_NUMBER] = {};
#ifdef ADC_ENABLE_RAW
static uint16_t gAnalogInputsRaw[ANALOG_IN_NUMBER] = {};
#endif //ADC_ENABLE_RAW
static const uint8_t gAnalogMult[ANALOG_IN_NUMBER] = {ADC_MULT};
static const uint8_t gAnalogDiv[ANALOG_IN_NUMBER] = {ADC_DIV};

#ifndef ADC_ENABLED

static uint8_t calcBatteryLevel(uint8_t adcInput) { DUMMY_FUNC }
static uint8_t calcRssiLevel(uint8_t adcInput) { DUMMY_FUNC }

#else

static void setupAdc() {
  // ADC setup
	DIDR0 = 0x00;
	ADMUX |= (1<<REFS0); // Ref is AVCC
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1); // | (1<<ADPS0); 
	ADCSRB = 0; // Free running
}

static void measureAnalog() {
  uint32_t temp = 0;
	uint8_t adcLow = 0; 
  uint8_t adcHigh = 0;
  for (uint8_t i = 0; i < ANALOG_IN_NUMBER; ++i) {
	  ADMUX &= 0xF0; // Clear mux
    ADMUX |= (i + ADC_OFFSET);	 //Setup adc mux
	  ADCSRA |= (1<<ADEN) | (1<<ADATE); // ADC enable & ADC auto trigger enable
	  ADCSRA |= (1<<ADSC); // Start measure
	  while ((ADCSRA & (1<<ADIF)) == 0); // Wait to finish
	  ADCSRA |= (1<<ADIF); // Clear ADC interrupt flag with a 1
	  ADCSRA &= ~(1<<ADEN) & ~(1<<ADATE); // ADC disabled & ADC auto trigger disabled
	  temp = ADCW;
#ifdef ADC_ENABLE_RAW	  
    gAnalogInputsRaw[i] = temp;
#endif
	  temp = (temp * 5 * 62) / 10;
	  temp *= gAnalogMult[i];
	  temp /= gAnalogDiv[i];
	  adcHigh = temp / 1024;
	  temp -= (uint16_t)(adcHigh) * 1024;
	  adcLow = (temp * 100) / 1024;
	
	  gAnalogInputs[i].low = adcLow;
    gAnalogInputs[i].high = adcHigh;
  }    
}

static uint8_t calcGenericLevel(uint8_t adcInput, uint16_t minLevel, uint16_t maxLevel) {
	uint16_t level = ((gAnalogInputs[adcInput].high * 100) + gAnalogInputs[adcInput].low);
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