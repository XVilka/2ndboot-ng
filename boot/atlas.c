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
