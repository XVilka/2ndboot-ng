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

#include "gpt.h"
#include "common.h"
#include "time_priv.h"

#ifdef __PLAT_TI_OMAP3430__
#include "plat/omap3430.h"
#endif

#ifdef __PLAT_FREESCALE_IMX31__
#include "plat/mxc91231.h"
#endif

#define GPT1_GPTCR (GPT1_BASE_ADDR + 0x00)
#define GPT1_GPTPR (GPT1_BASE_ADDR + 0x04)
#define GPT1_GPTSR (GPT1_BASE_ADDR + 0x08)
#define GPT1_GPTIR (GPT1_BASE_ADDR + 0x0c)
#define GPT1_GPTOCR1 (GPT1_BASE_ADDR + 0x10)
#define GPT1_GPTOCR2 (GPT1_BASE_ADDR + 0x14)
#define GPT1_GPTOCR3 (GPT1_BASE_ADDR + 0x18)
#define GPT1_GPTICR1 (GPT1_BASE_ADDR + 0x1c)
#define GPT1_GPTICR2 (GPT1_BASE_ADDR + 0x20)
#define GPT1_GPTCNT (GPT1_BASE_ADDR + 0x24)

static void gpt_disable() {
  modify_register32(GPT1_GPTCR, GPTCR_ENABLE, 0);
}

static void gpt_enable() {
  modify_register32(GPT1_GPTCR, 0, GPTCR_ENABLE);
}

void gpt_reset() {
  modify_register32(GPT1_GPTCR, 0, GPTCR_SWR);
  while (read32(GPT1_GPTCR) & GPTCR_SWR);
}

static void gpt_wait(uint32_t ticks) {
  gpt_enable();
  while (read32(GPT1_GPTCNT) < ticks);
  gpt_disable();
}

void gpt_init() {
  gpt_disable();
  gpt_reset();
  modify_register32(GPT1_GPTCR, GPTCR_DBGEN, 0);
  modify_register32(GPT1_GPTCR, GPTCR_WAITEN, 0);
  modify_register32(GPT1_GPTCR, GPTCR_DOZEN, 0);
  modify_register32(GPT1_GPTCR, GPTCR_STOPEN, 0);
  modify_register32(GPT1_GPTCR, 0, GPTCR_ENMOD);
  modify_register32(GPT1_GPTCR, GPTCR_CLKSRC_MASK, GPTCR_CLKSRC_CLK32K);
  modify_register32(GPT1_GPTCR, 0, GPTCR_FRR);
  write32(0, GPT1_GPTPR);
}

/* absolutely imprecise */
void msleep(unsigned int msecs) {
  uint32_t ticks;

  ticks = (msecs << 5) + (msecs >> 1) + (msecs >> 2) + (msecs >> 6) + (msecs >> 9);
  gpt_wait(ticks);
}
