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


#ifndef COMMONUTILS_H_
#define COMMONUTILS_H_

// abs seems to only support 16 bits so I had to make my own.
static uint32_t absi32(int32_t a) {
	return (a > 0) ? a : -a;
}

static uint32_t absu32(uint32_t a) {
	return (a > 0) ? a : -a;
}

static uint32_t calcSqrt(uint32_t input)
{
  int32_t nv;
  int32_t v = input>>1;
  int32_t c = 0;
  if (!v) {
    return input;
  }	
  do {
    nv = (v + input / v)>>1;
    if (absu32(v - nv) <= 1) {
      return nv;
	  }	  
    v = nv;
  } while (c++ < 25);
  return nv;
}

inline uint8_t isDigit(char c) {
	if (c >= '0' && c <= '9') {
		return 1;
	}
	return 0;
}

static void myReverse(char s[], uint8_t size) {
  uint8_t i;
  char c;
  size -= 1;
  for (i = 0; i <= size/2; i++) {
    c = s[i];
    s[i] = s[size - i];
    s[size - i] = c;
  }
}

static void myItoa(int32_t n, char s[])
{
  int8_t i;
  int8_t sign = 0;
 
  if (n < 0) {  
	  sign = -1; /* record sign */
    n = -n;          /* make n positive */
  }	
  i = 0;
  do {       /* generate digits in reverse order */
    s[i++] = n % 10 + '0';   /* get next digit */
  } while ((n /= 10) > 0);     /* delete it */
  if (sign < 0) {
    s[i++] = '-';
  }	
  s[i] = '\0';
  myReverse(s, i);
}

#endif /* COMMONUTILS_H_ */