/*
 *  A module for creating rebootless custom image boot support.
 *
 *  Copyright (C) 2010 XVilka <xvilka at gmail.com>
 *
 *  Inspired by 2ndboot by dimich: http://hg.ezxdev.org/2ndboot/
 * 
 *  This file is part of 2ndboot-ng.
 *
 *  2ndboot-ng is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Foobar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *   along with 2ndboot-ng.  If not, see <http://www.gnu.org/licenses/>.
 * 
*/

#include "string.h"

void *memcpy(void *dest, const void *src, size_t n) {
  char *d = (char*)dest;
  const char *s = (const char*)src;

  while (n-- > 0) {
    *d++ = *s++;
  }
  return dest;
}

void *memset(void *dest, int fill, size_t n) {
  unsigned char *d = (unsigned char*)dest;

  while (n-- > 0) {
    *d++ = (unsigned char)fill;
  }
  return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  unsigned char *ss1 = (unsigned char*)s1;
  unsigned char *ss2 = (unsigned char*)s2;

  while (n-- > 0) {
    if (*ss1 != *ss2) {
      return *ss1 - *ss2;
    }
    ++ss1;
    ++ss2;
  }
  return 0;
}

size_t strlen(const char *s) {
  size_t n = 0;

  while (*s++) n++;
  return n;
}

char *strcpy(char *d, const char *s) {
  char *dest = d;

  while (*d++ = *s++);
  return dest;
}

int strcmp(const char *s1, const char *s2) {
  size_t l1, l2;
  l1 = strlen(s1);
  l2 = strlen(s2);
  if (l1 < l2) {
    return -1;
  }
  if (l1 > l2) {
    return 1;
  }
  while (*s1) {
    if (*s1 != *s2) {
      return *s1 - *s2;
    }
    ++s1;
    ++s2;
  }
  return 0;
}
