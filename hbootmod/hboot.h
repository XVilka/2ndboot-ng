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

#ifndef HBOOT_H
#define HBOOT_H

#include "buffers.h"

#ifdef __KERNEL__
#include <linux/types.h>

uint32_t get_high_mem(size_t size);
void free_high_mem(void *p);
uint32_t get_high_pages(unsigned int order);
void free_high_pages(void *p, unsigned int order);
uint32_t get_high_page(void);
void free_high_page(void *p);
int allocate_buffer(uint8_t tag, uint8_t type, uint8_t attrs, uint32_t size, uint32_t rest);
int free_buffer(int handle);
int select_buffer(int handle);
int buffer_append_userdata(const char __user *data, size_t len, loff_t *ppos);
int buffers_init(void);
void buffers_destroy(void);

typedef void (*bootfunc_t)(void *, uint32_t);
bootfunc_t get_bootentry(uint32_t *bootsize, int handle);
void *get_bootlist(uint32_t *listsize, int handle);

#endif

struct hboot_buffer_req {
	uint8_t tag;
	uint8_t type;
	uint8_t attrs;
	uint32_t size;
	uint32_t rest;
};

#define MAX_BUFFERS_COUNT 			8

#define INVALID_BUFFER_HANDLE ((int)-1)
#define HBOOT_ALLOCATE_BUFFER		0
#define HBOOT_FREE_BUFFER			1
#define HBOOT_SELECT_BUFFER			2
#define HBOOT_BOOT					3

#define L2CC_BASE_ADDR              0x0000
#define AIPS1_BASE_ADDR				0x0000
#define AIPS2_BASE_ADDR				0x0000
#define SPBA0_BASE_ADDR				0x0000
#define SPBA1_BASE_ADDR				0x0000
#define X_MEMC_BASE_ADDR			0x0000
#define FB_RAM_BASE_ADDR			0x0000

#endif
	
