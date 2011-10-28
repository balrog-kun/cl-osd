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

#ifndef GPS_H_
#define GPS_H_

#ifdef GPS_ENABLED

//#include <string.h>

#include "config.h"
#include "gpsutils.h"
#include "statistics.h"
#include "global.h"
#include "gps-common.h"

#ifdef GPS_DIYD
#include "gps-diyd.h"
#else
#include "gps-nmea.h"
#endif

#endif //GPS_ENABLED

#endif /* GPS_H_ */