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

#ifndef HARDWARE_H_
#define HARDWARE_H_

// ----------- PINOUT --------------
#ifdef E_OSD
#define KEY (1<<PD5)
#define LED (1<<PD6)
#else
#define KEY (1<<PD4)
#define LED (1<<PD3)
#endif //GOSD

#define OUT1 (1<<PB1)
#define OUT2 (1<<PB3)

#define LTRIG (1<<PD2) //INT0
#define SS (1<<PB2)

// ----------- ADC --------------
#ifdef E_OSD
#define ANALOG_IN_1 1 // Voltage 1 = ADC1
#define ANALOG_IN_2 0 // Voltage 2 = ADC0
#else
#define ANALOG_IN_1 0 // Voltage 1 = ADC0
#define ANALOG_IN_2 1 // Voltage 2 = ADC1
#define ANALOG_IN_3 2 // RSSI = ADC2 (only 0-5 Volt!)
#define ANALOG_IN_4 3 // No input, just a pot = ADC3
#endif


#define ADC_OFFSET 0

#ifdef E_OSD
#define ADC_MULT 1,1
#define ADC_DIV 1,1
#else
#define ADC_MULT 1,1,1,1
#define ADC_DIV 1,1,6,1
#endif //E_OSD

#ifdef E_OSD
#define ANALOG_IN_NUMBER 2
#else
#define ANALOG_IN_NUMBER 4
#endif //E_OSD



#endif /* HARDWARE_H_ */