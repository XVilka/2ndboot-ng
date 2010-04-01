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
#include "spba.h"

#ifdef __PLAT_TI_OMAP3430__
#include "plat/omap3430.h"
#endif

#ifdef __PLAT_FREESCALE_IMX31__
#include "plat/mxc91231.h"
#endif

int spba_change_rights(unsigned int module, unsigned int rights) {
  uint32_t v;

  if (module > SPBA_MAX_MODULE) {
    return -1;
  }
  rights &= 0x7;
  modify_register32(SPBA_CTRL_BASE_ADDR+module, 0x7, rights);
  if ((read32(SPBA_CTRL_BASE_ADDR+module) & 0x7) != rights) {
    return -1;
  }
  return 0;
}
