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

#ifndef __HW_COMMON_H__
#define __HW_COMMON_H__
#include "types.h"
#include "stdio.h"

static inline uint8_t read8(addr_t addr) {
  return *((volatile uint8_t*)addr);
}

static inline void write8(uint8_t v, addr_t addr) {
  *((volatile uint8_t*)addr) = v;
}

static inline uint16_t read16(addr_t addr) {
  return *((volatile uint16_t*)addr);
}

static inline void write16(uint32_t v, addr_t addr) {
  *((volatile uint16_t*)addr) = v;
}

static inline uint32_t read32(addr_t addr) {
  return *((volatile uint32_t*)addr);
}

static inline void write32(uint32_t v, addr_t addr) {
  *((volatile uint32_t*)addr) = v;
}

static inline void modify_register16(addr_t addr, uint16_t mask, uint16_t val) {
  write16(read16(addr) & (~mask) | val, addr);
}

static inline void modify_register32(addr_t addr, uint32_t mask, uint32_t val) {
  write32(read32(addr) & (~mask) | val, addr);
}

#endif // __HW_COMMON_H__
