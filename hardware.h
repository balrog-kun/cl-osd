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

// ----------- HARDWARE SETUP --------------

// NOTE: This should be set with configuration from the GUI in AVR Studio. Not here!
// For debug: Select one of these depending on your board (Wrong type might damage the board!)
//#define E_OSD
//#define G_OSD
//#define E_OSD_GPS

// ----------- CHECK SANITY --------------
#if (defined(E_OSD_GPS) && defined(E_OSD)) 
#error "Make sure to NOT to define E_OSD as well when using E_OSD_GPS (THIS HAS BEEN CHANGED)"
#endif


#if (defined(E_OSD) && defined(E_OSD_GPS)) \
 || (defined(E_OSD) && defined(G_OSD)) \
 || (defined(E_OSD_GPS) && defined(G_OSD)) \
 || (defined(E_OSD) && defined(E_OSD_GPS) && defined(G_OSD)) \
 || (!defined(E_OSD) && !defined(E_OSD_GPS) && !defined(G_OSD))
#error "Select one and only one target!"
#endif

// ----------- PINOUT --------------
#if defined(E_OSD) || defined(E_OSD_GPS)
#define KEY (1<<PD5)
#define LED (1<<PD6)
#else //G_OSD
#define KEY (1<<PD4)
#define LED (1<<PD3)
#endif //E_OSD

#define OUT1 (1<<PB1)
#define OUT2 (1<<PB3)

#define LTRIG (1<<PD2) //INT0
#define SS (1<<PB2)

// ----------- ADC --------------
#if defined(E_OSD) || defined(E_OSD_GPS)
#define ANALOG_IN_1 1 // Voltage 1 = ADC1
#define ANALOG_IN_2 0 // Voltage 2 = ADC0 (can be RSSI)
#else //G_OSD
#define ANALOG_IN_1 0 // Voltage 1 = ADC0
#define ANALOG_IN_2 1 // Voltage 2 = ADC1
#define ANALOG_IN_3 2 // RSSI = ADC2 (only 0-5 Volt!)
#define ANALOG_IN_4 3 // No input, just a pot = ADC3
#endif

#define ADC_OFFSET 0

#if defined(E_OSD) || defined(E_OSD_GPS)
#define ADC_MULT 1,1
#define ADC_DIV 1,1
#else //G_OSD
#define ADC_MULT 1,1,1,1
#define ADC_DIV 1,1,6,1
#endif

#if defined(E_OSD) || defined(E_OSD_GPS)
#define ANALOG_IN_NUMBER 2
#else //G_OSD
#define ANALOG_IN_NUMBER 4
#endif

// ----------- FEATURES --------------

#if defined(G_OSD) || defined(E_OSD_GPS)
#define HARDWARE_SUPPORT_GPS
#endif


#endif /* HARDWARE_H_ */