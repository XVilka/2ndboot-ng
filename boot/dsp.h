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

#ifndef __HW_DSP_H__
#define __HW_DSP_H__

#include "types.h"

#define DSP_BPLOADER_ADDR (0x93ffc000)
#define DSP_BPFW_ADDR (0x93c00000)

int dsp_mem_read_word(addr_t src, uint32_t *dest);
int dsp_mem_write_word(addr_t dest, uint32_t *word);
int dsp_mem_read(addr_t src, uint32_t *dest, unsigned int words);
int dsp_mem_write(addr_t dest, uint32_t *src, unsigned int words);

int dsp_start_loader(addr_t paddr);
int dsp_start_image(addr_t paddr);

int dsp_reboot();

#endif // __HW_DSP_H__
