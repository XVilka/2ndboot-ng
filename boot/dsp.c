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

#include "mu.h"
#include "dsp.h"
#include "common.h"
#include "images.h"
#include "memory.h"

int dsp_mem_read_word(addr_t addr, uint32_t *dest) {
  uint32_t tmp;

  mu_write(0, 0x2);
  mu_write(1, addr);
  mu_read(0, &tmp);
  if (tmp != 0) {
    printf("%08x failed 1st read\n", addr);
    return -1;
  }
  mu_read(1, dest);
  mu_read(0, &tmp);
  if (tmp != 0x2) {
    printf("%08x failed 2nd read\n", addr);
    return -1;
  }
  *dest = ((*dest & 0xffff) << 16) | ((*dest & 0xffff0000) >> 16);
  return 0;
}

int dsp_mem_write_word(addr_t addr, uint32_t *word) {
  uint32_t tmp;

  mu_write(0, 0x1);
  mu_write(1, addr);
  mu_write(2, ((*word & 0xffff) << 16) | ((*word & 0xffff0000) >> 16));
  mu_read(0, &tmp);
  if (tmp != 0x1) {
    printf("%08x failed write\n", addr);
    return -1;
  }
  return 0;
}

int dsp_mem_read(addr_t src, uint32_t *dest, unsigned int words) {
  uint32_t *sdest = dest;
  uint32_t rd;

  for (rd = 0; rd < words; ++rd) {
    if (dsp_mem_read_word(src, dest)) {
      printf("read failed on %08x\n", rd);
      return -1;
    }
    src+=4;
    dest++;
  }
  return 0;
}

int dsp_mem_write(addr_t dest, uint32_t *src, unsigned int words) {
  unsigned int wr;

  for (wr = 0; wr < words; ++wr) {
    if (dsp_mem_write_word(dest, src)) {
      printf("write failed on %08x\n", wr);
      return -1;
    }
    dest+=4;
    src++;
  }
  return 0;
}

static inline int dsp_startapp(addr_t paddr) {
  mu_write(0, 4);
  mu_write(1, paddr);
}

int dsp_start_loader(addr_t paddr) {
  uint32_t foo;

  dsp_startapp(paddr);
  mu_read(0, &foo);
  if ((foo & 0xff00) != 0xf000) {
    printf("start_ldr ret: %08x\n", foo);
    return -1;
  }
  mu_read(1, &foo);
  mu_read(2, &foo);
  mu_dump_mrrs();
  return 0;
}

int dsp_start_image(addr_t paddr) {
  uint32_t foo;

  dsp_startapp(paddr);
  mu_read(0, &foo);
  mu_read(3, &foo);
  mu_dump_mrrs();
  return 0;
}

int dsp_reboot() {
  struct memory_image image;

  if (image_find(IMG_BPLOADER, &image) == NULL) {
    printf("no bploader, won't reset DSP\n");
    return -1;
  }
  mu_dsp_reset();
  if (dsp_mem_write(DSP_BPLOADER_ADDR, image.data, image.size >> 2) < 0) {
    printf("failed to write bploader\n");
    return -1;
  }
  if (dsp_start_loader(DSP_BPLOADER_ADDR) < 0) {
    printf("failed to start bploader\n");
    return -1;
  }
  image.data = (void*)DSP_BPFW_ADDR;
  image.size = (size_t)(DSP_BPLOADER_ADDR - DSP_BPFW_ADDR);

  if (image_unpack(IMG_BPFW, &image) == NULL) {
    printf("failed to unpack bpfw\n");
    return -1;
  }
  caches_flush_lines(DSP_BPFW_ADDR, DSP_BPFW_ADDR + image.size);
  printf("starting dsp fw..\n");
  if (dsp_start_image(DSP_BPFW_ADDR) == 0) {
    printf("ok\n");
  } else {
    printf("failed\n");
  }
}
