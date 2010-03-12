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

#ifndef __BUFFERS_H__
#define __BUFFERS_H__

#if defined(__KERNEL__) || defined(__LOADER__)

#ifndef __ASSEMBLER__

#if defined(__KERNEL__)
#include <linux/types.h>
#elif defined(__LOADER__)
#include "types.h"
#endif

#pragma pack(push, 1)
struct abstract_buffer {
  uint8_t tag;
  uint8_t state;
  uint8_t type;
  uint8_t attrs;
  uint32_t size;
  uint32_t checksum;
};
#pragma pack(pop)
#endif // __ASSEMBLER__

#define B_STAT_NONE      (0)
#define B_STAT_CREATED   (1)
#define B_STAT_COMPLETED (2)
#define B_STAT_CRCERROR  (3)
#define B_STAT_OVERFLOW  (4)
#define B_STAT_ERROR     (5)

#endif // defined(__KERNEL__) || defined(__LOADER__)

#define B_TYPE_PLAIN     (0)
#define B_TYPE_SCATTERED (1)
#define B_TYPE_NAND      (2)

#define B_ATTR_VERIFY    (1 << 0)

#define IMG_ATTR_LAZY    (1 << 0)

#define IMG_LINUX        (1)
#define IMG_INITRAMFS    (2)
#define IMG_MOTFLATTREE  (3)
#define IMG_CMDLINE      (4)
#define IMG_USBFW        (5)
#define IMG_BPLOADER     (6)
#define IMG_BPFW         (7)

#define IMG_LAST_TAG     IMG_BPFW

#endif // __BUFFERS_H__
