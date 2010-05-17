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
 *  along with 2ndboot-ng.  If not, see <http://www.gnu.org/licenses/>.
 * 
*/

#ifndef HBOOT_H
#define HBOOT_H

#include <linux/types.h>
#include "memory.h"

#define MAX_BUFFERS_COUNT 			8

#define INVALID_BUFFER_HANDLE                   ((int)-1)
#define HBOOT_ALLOCATE_BUFFER                   0
#define HBOOT_FREE_BUFFER			1
#define HBOOT_SELECT_BUFFER			2
#define HBOOT_BOOT				3

#ifdef __PLAT_TI_OMAP3430__
#define L2CC_BASE_ADDR                          0x80000000
#endif

typedef void (*bootfunc_t)(void *, uint32_t);

struct hboot_buffer_req {
	uint8_t tag;
	uint8_t type;
	uint8_t attrs;
	uint32_t size;
	uint32_t rest;
};

/*

ATAG_POWERUP_REASON             /proc/bootinfo
ATAG_IPU_BUFFER_ADDRESS         0x83700000        // Framebuffer address
ATAG_IS_IPU_INITIALIZED         1
ATAG_GPU_CONTEXT                0
ATAG_USB_FIRMWARE_ADDRESS       ?
ATAG_MBM_VERSION                /proc/bootinfo
ATAG_MBM_LOADER_VERSION         /proc/bootinfo
ATAG_BOARDID                    ?
ATAG_FLAT_DEV_TREE_ADDRESS      /proc/bootinfo
ATAG_INITRD2                    0x8F211000         // Initrd address
ATAG_FLASHING_COMPLETED         1
ATAG_LOGO_VERSION               ?
ATAG_MEMORY_TYPE                2
ATAG_BATTERY_STATUS_AT_BOOT     /proc/bootinfo
ATAG_BOOT_FREQUENCY             ?
ATAG_MEDL_INFO                  ?
ATAG_MBM_BOOTUP_TIME            ?
ATAG_BP_LOADER_VERSION          ?
ATAG_CLI_LOGO_VERSION           ?

*/

struct {
    struct {
        char *name;
        uint8_t value;
    } atag[];
} atags;

uint32_t get_high_pages(unsigned int order);
void free_high_pages(void *p, unsigned int order);
uint32_t get_high_page(void);
void free_high_page(void *p);
int allocate_buffer(uint8_t tag, uint8_t type, uint8_t attrs, uint32_t size, uint32_t rest);
int free_buffer(int handle);
int select_buffer(int handle);
int buffer_append_userdata(const char __user *data, size_t len, loff_t *ppos);


bootfunc_t get_bootentry(uint32_t *bootsize, int handle);
void *get_bootlist(uint32_t *listsize, int handle);

#endif