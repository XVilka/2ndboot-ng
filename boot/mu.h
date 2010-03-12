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

#ifndef __HW_MU_H__
#define __HW_MU_H__
#include "types.h"

int mu_reset(void);
int mu_dsp_reset(void);

int mu_write_noblock(int reg, uint32_t data);
int mu_read(int reg, uint32_t *data);
int mu_write_noblock(int reg, uint32_t data);
int mu_read(int reg, uint32_t *data);

void mu_dump_mrrs();
void mu_dump_mtrs();

#endif // __HW_MU_H__
