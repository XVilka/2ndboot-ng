#include "gpt.h"
#include "mxc91231.h"
#include "common.h"
#include "time_priv.h"

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
