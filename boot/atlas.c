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

#include "cspi.h"
#include "types.h"
#include "atlas.h"

#define ATLAS_CSPI_MODULE 1

static unsigned int g_timeout;

int atlas_init() {
  struct cspi_config cfg;
  g_timeout = 1000;

  cfg.chip_select = 2;
  cfg.data_rate = 4200000;
  cfg.bit_count = 32;
  cfg.active_high_ss_polarity = 1;
  cfg.ss_waveform = 1;
  cfg.phase = 0;
  cfg.active_high_clk_polarity = 0;
  return cspi_init(ATLAS_CSPI_MODULE, &cfg);
}  

int atlas_reg_io(atlas_register reg, uint32_t *p, int write) {
  uint32_t outdata, indata;
  unsigned int timeout = g_timeout;

  outdata = (reg & 0x3f) << 0x19;
  if (write) {
    outdata |= *p & 0xffffff;
    outdata |= 1 << 31;
  }
  if (cspi_send(ATLAS_CSPI_MODULE, &outdata, 1, &timeout) != 1) {
    return -1;
  }
  if (cspi_recv(ATLAS_CSPI_MODULE, &indata, 1, &timeout) != 1) {
    return -2;
  }
  if ((indata >> 0x18) != 0) {
    return -3;
  }
  if (!write) {
    *p = indata & 0xffffff;
  }
  return 0;
}

void atlas_set_timeout(unsigned int timeout) {
  g_timeout = timeout;
}

int atlas_reg_read(atlas_register reg, uint32_t *v) {
  return atlas_reg_io(reg, v, 0);
}

int atlas_reg_modify(atlas_register reg, uint32_t mask, uint32_t v) {
  uint32_t foo;
  int ret;

  mask &= 0xffffff;
  v &= 0xffffff;

  ret = atlas_reg_read(reg, &foo); // force read;
  if (ret != 0) {
    return ret;
  }
  foo = (foo & (~mask)) | v;
  return atlas_reg_io(reg, &foo, 1);
}

int atlas_reg_write(atlas_register reg, uint32_t v) {
  return atlas_reg_modify(reg, 0xffffff, v);
}

int atlas_test_io() {
  int ret;
  uint32_t orig, v;

  ret = atlas_reg_read(ATLAS_REG_MEMORY_B, &orig);
  if (ret != 0) {
    return -1;
  }
  atlas_reg_write(ATLAS_REG_MEMORY_B, 0x01dead);
  atlas_reg_read(ATLAS_REG_MEMORY_B, &v);
  if ((v & 0xffffff) != 0x01dead) {
    ret = -2;
  }
  atlas_reg_write(ATLAS_REG_MEMORY_B, orig);
  return ret;
}
