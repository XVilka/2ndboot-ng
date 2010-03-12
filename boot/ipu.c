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

#include "types.h"
#include "mxc91231.h"
#include "ipu_priv.h"
#include "ipu_regs.h"
#include "common.h"
static inline uint32_t ipu_channel_2_dma(ipu_channel_t ch, ipu_buffer_t type) {
  return ((type == IPU_INPUT_BUFFER) ? ((uint32_t)ch & 0xff) :
	  ((type == IPU_OUTPUT_BUFFER) ? (((uint32_t)ch >> 8) & 0xff) :
	   ((uint32_t)ch >> 16) & 0xff));
}

inline static uint32_t DMAParamAddr(uint32_t dma_ch) {
  return (0x10000 | (dma_ch << 4));
}

int ipu_get_channel_params(uint32_t addr, uint32_t *buf, size_t cnt) {
  while (cnt-- > 0) {
    write32(addr, IPU_IMA_ADDR);
    *buf++ = read32(IPU_IMA_DATA);
    addr++;
    if ((addr & 0x7) == 0x5) {
      addr &= ~0x7;
      addr += 8;
    }
  }
  return 0;
}
int ipu_get_sdc_channel_params(uint32_t *buf, size_t cnt) {
  uint32_t dma_chan;
  dma_chan = ipu_channel_2_dma(MEM_SDC_BG, IPU_INPUT_BUFFER);
  if (dma_chan == IDMA_CHAN_INVALID) {
    return -1;
  }
  return ipu_get_channel_params(DMAParamAddr(dma_chan), buf, cnt);
}
