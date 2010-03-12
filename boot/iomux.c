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

#include "common.h"
#include "types.h"
#include "mxc91231.h"
#include "iomux.h"

struct iomux {
  addr_t base;
  unsigned int pins;
};

static struct iomux iomuxes[] = {
  [IOMUX_AP] = {
    .base = IOMUX_AP_BASE_ADDR,
    .pins = 44,
  },
  [IOMUX_COM] = {
    .base = IOMUX_COM_BASE_ADDR,
    .pins = 41,
  },
};

int iomux_set_function(int iomux, unsigned int pin, unsigned int infunc, unsigned int outfunc) {
  addr_t base;
  uint32_t v;
  unsigned int shift;

  if ((iomux < 0) || (iomux > IOMUX_MAX_NUM) || (pin > iomuxes[iomux].pins)) {
    return -1;
  }
  if ((outfunc > 7) || (infunc > 3)) {
    return -1;
  }
  base = iomuxes[iomux].base + (pin & ~0x3);
  shift = (pin & 3) << 3;
  v = read32(base);
  v &= ~(0xff << shift);
  v |= ((outfunc & 7) << 4) + (1 << infunc) << shift;
  write32(v, base);
  return 0;
}
