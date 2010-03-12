#include "mxc91231.h"
#include "error.h"
#include "common.h"
#include "gpt.h"

#define MU_MTR_BASE_ADDR (MU_BASE_ADDR+0x00)
#define MU_MRR_BASE_ADDR (MU_BASE_ADDR+0x10)
#define MU_MSR_ADDR (MU_BASE_ADDR+0x20)
#define MU_MCR_ADDR (MU_BASE_ADDR+0x24)

#define DSP_RUN_TRIES 100

static void mu_nmi(void) {
  modify_register32(MU_MCR_ADDR, 0, 1 << 3);
}

static int mu_dsp_power_state(void) {
  return (read32(MU_MSR_ADDR) >> 5) & 0x3;
}

static void mu_dsp_reset_wait() {
  while (read32(MU_MSR_ADDR) & (1 << 7)) {
  }
}

static void mu_assert_dsp_running() {
  int cnt = DSP_RUN_TRIES;
  if (mu_dsp_power_state() != 0) {
    mu_nmi();
  }
  while (cnt--) {
    if (mu_dsp_power_state() == 0) {
      return;
    }
    msleep(1);
  }
  critical_error(DSP_IS_NOT_RUNNING);
}

static inline int mu_mrr_ready(int reg) {
  return (read32(MU_MSR_ADDR) >> (27 - reg)) & 1;
}

static inline int mu_mtr_ready(int reg) {
  return (read32(MU_MSR_ADDR) >> (23 - reg)) & 1;
}

int mu_reset(void) {
  mu_assert_dsp_running();
  modify_register32(MU_MCR_ADDR, 0, 1 << 5);
  mu_dsp_reset_wait();
  return 0;
}

int mu_dsp_reset(void) {
  mu_assert_dsp_running();
  modify_register32(MU_MCR_ADDR, 0, 1 << 4);
  modify_register32(MU_MCR_ADDR, 1 << 4, 0);
  mu_dsp_reset_wait();
  printf("DSP reset completed\n");
  return 0;
}

int mu_write_noblock(int reg, uint32_t val) {
  addr_t mtr;

  if (reg < 0 || reg > 3) {
    return -1;
  }
  if (!mu_mtr_ready(reg)) {
    return -2;
  }
  mtr = MU_MTR_BASE_ADDR + (reg << 2);
  write32(val, mtr);
  return 0;
}

int mu_write(int reg, uint32_t val) {
  if (reg < 0 || reg > 3) {
    return -1;
  }
  while (!mu_mtr_ready(reg)) {
  }
  return mu_write_noblock(reg, val);
}

int mu_read_noblock(int reg, uint32_t *val) {
  addr_t mrr;

  if (reg < 0 || reg > 3) {
    return -1;
  }
  if (!mu_mrr_ready(reg)) {
    return -2;
  }
  mrr = MU_MRR_BASE_ADDR + (reg << 2);
  *val = read32(mrr);
  return 0;
}

int mu_read(int reg, uint32_t *val) {
  if (reg < 0 || reg > 3) {
    return -1;
  }
  while (!mu_mrr_ready(reg)) {
  }
  return mu_read_noblock(reg, val);
}

static inline void mu_dump_set(int rset) {
  int reg = 3;
  int wr = 0;

  printf("{");
  while (rset != 0) {
    if (rset & 1) {
      if (wr++ != 0) {
	printf(" ");
      }
      printf("%d", reg);
    }
    reg--;
    rset >>= 1;
  }
  printf("}");
}

void mu_dump_mrrs() {
  printf("MRRS: ");
  mu_dump_set((read32(MU_MSR_ADDR) >> 24) & 0xf);
  printf("\n");
}

void mu_dump_mtrs() {
  printf("MTRS: ");
  mu_dump_set((read32(MU_MSR_ADDR) >> 20) & 0xf);
  printf("\n");
}
