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

#ifndef STATISTICS_H_
#define STATISTICS_H_

#ifdef STATISTICS_ENABLED
static volatile uint16_t statDistTraveled = 0; // TODO!
static volatile uint16_t statMaxSpeed = 0;
static volatile uint16_t statMaxAltitude = 0;
static volatile uint16_t statMaxDistance = 0;
#endif //STATISTICS_ENABLED

#endif /* STATISTICS_H_ */