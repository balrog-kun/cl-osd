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

#ifndef TEST_H_
#define TEST_H_

static void testCalcHome() {
	
#ifdef DEBUG
  return;
#endif //DEBUG
	
#if 1
	// center around 58234380, 15353780. 1000m out
	calcHome(58234380, 15353780, 58239767, 15353780, &gHomeDistance, &gHomeBearing); // 0 deg
	calcHome(58234380, 15353780, 58239442, 15357289, &gHomeDistance, &gHomeBearing); // 20 deg 
	calcHome(58234380, 15353780, 58238189, 15361035, &gHomeDistance, &gHomeBearing); // 45 deg
	calcHome(58234380, 15353780, 58237073, 15362665, &gHomeDistance, &gHomeBearing); //60 deg
	calcHome(58234380, 15353780, 58233444, 15363882, &gHomeDistance, &gHomeBearing); //100
	calcHome(58234380, 15353780, 58229075, 15355561, &gHomeDistance, &gHomeBearing); //170
	calcHome(58234380, 15353780, 58229318, 15350272, &gHomeDistance, &gHomeBearing); //200
	calcHome(58234380, 15353780, 58236222, 15344139, &gHomeDistance, &gHomeBearing); //290
	calcHome(58234380, 15353780, 58239685, 15351998, &gHomeDistance, &gHomeBearing); //350
#endif
	
#if 1
	// center around -23010000, -46010000. 500m out
	calcHome(-23010000, -46010000, -23007291, -46010000, &gHomeDistance, &gHomeBearing); // 0 deg
	calcHome(-23010000, -46010000, -23007454, -46008999, &gHomeDistance, &gHomeBearing); // 20 deg 
	calcHome(-23010000, -46010000, -23008084, -46007931, &gHomeDistance, &gHomeBearing); // 45 deg
	calcHome(-23010000, -46010000, -23008645, -46007466, &gHomeDistance, &gHomeBearing); //60 deg
	calcHome(-23010000, -46010000, -23010470, -46007118, &gHomeDistance, &gHomeBearing); //100
	calcHome(-23010000, -46010000, -23012668, -46009492, &gHomeDistance, &gHomeBearing); //170
	calcHome(-23010000, -46010000, -23012546, -46011001, &gHomeDistance, &gHomeBearing); //200
	calcHome(-23010000, -46010000, -23009073, -46012750, &gHomeDistance, &gHomeBearing); //290
	calcHome(-23010000, -46010000, -23007332, -46010508, &gHomeDistance, &gHomeBearing); //350
#endif
	
#if 1
	// center around 46399970, -97259920, 150m out
	calcHome(46399970, -97259920, 46400780, -97259920, &gHomeDistance, &gHomeBearing); // 0 deg
	calcHome(46399970, -97259920, 46400731, -97259518, &gHomeDistance, &gHomeBearing); // 20 deg
	calcHome(46399970, -97259920, 46400542, -97259088, &gHomeDistance, &gHomeBearing); // 45 deg
	calcHome(46399970, -97259920, 46400375, -97258902, &gHomeDistance, &gHomeBearing); // 60 deg
	calcHome(46399970, -97259920, 46399829, -97258762, &gHomeDistance, &gHomeBearing); // 100 deg
	calcHome(46399970, -97259920, 46399173, -97259716, &gHomeDistance, &gHomeBearing); // 170 deg
	calcHome(46399970, -97259920, 46399209, -97260322, &gHomeDistance, &gHomeBearing); // 200 deg
	calcHome(46399970, -97259920, 46400247, -97261025, &gHomeDistance, &gHomeBearing); // 290 deg
	calcHome(46399970, -97259920, 46400767, -97260124, &gHomeDistance, &gHomeBearing); // 350 deg
#endif

#if 1
	// center around 58234380, 15353780. 45 deg out
	calcHome(58234380, 15353780, 58234418, 15353853, &gHomeDistance, &gHomeBearing); // 10m
	calcHome(58234380, 15353780, 58234570, 15354143, &gHomeDistance, &gHomeBearing); // 50m
	calcHome(58234380, 15353780, 58234761, 15354505, &gHomeDistance, &gHomeBearing); // 100m
	calcHome(58234380, 15353780, 58235332, 15355594, &gHomeDistance, &gHomeBearing); // 250m
	calcHome(58234380, 15353780, 58236284, 15357407, &gHomeDistance, &gHomeBearing); // 500m
	calcHome(58234380, 15353780, 58240093, 15364664, &gHomeDistance, &gHomeBearing); // 1500m
	calcHome(58234380, 15353780, 58243900, 15371923, &gHomeDistance, &gHomeBearing); // 2500m
	calcHome(58234380, 15353780, 58253416, 15390082, &gHomeDistance, &gHomeBearing); // 5000m
	calcHome(58234380, 15353780, 58272436, 15426450, &gHomeDistance, &gHomeBearing); // 10km
	calcHome(58234380, 15353780, 58291437, 15462883, &gHomeDistance, &gHomeBearing); // 15km
	calcHome(58234380, 15353780, 58310422, 15499381, &gHomeDistance, &gHomeBearing); // 20km
	calcHome(58234380, 15353780, 58329389, 15535946, &gHomeDistance, &gHomeBearing); // 25km
	calcHome(58234380, 15353780, 58423961, 16119758, &gHomeDistance, &gHomeBearing); // 50km
	calcHome(58234380, 15353780, 59011769, 16492394, &gHomeDistance, &gHomeBearing); // 100km
#endif

#if 0
	calcHome(58234380, 15353780, 58237300, 15355660, &gHomeDistance, &gHomeBearing); //572m 19 deg (!)
	calcHome(58244360, 15377910, 58357150, 16112030, &gHomeDistance, &gHomeBearing); //38633m 57 deg
	calcHome(58357150, 16112030, 58244360, 15377910, &gHomeDistance, &gHomeBearing); //38633m 237 deg
	calcHome(-23010000, -46010000, -23020000, -46010000, &gHomeDistance, &gHomeBearing); // 1853m 180 deg
	calcHome(-23020000, -46010000, -23020000, -46020000, &gHomeDistance, &gHomeBearing); // 1706m	270 deg
	calcHome(-23020000, -46020000, -23020000, -46010000, &gHomeDistance, &gHomeBearing); // 1706m	90 deg
	calcHome(-23020000, -46010000, -23010000, -46010000, &gHomeDistance, &gHomeBearing); // 1853m	360 deg
#endif
}

#endif /* TEST_H_ */