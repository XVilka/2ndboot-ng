#include "common.h"
#include "types.h"
#include "mxc91231.h"
#include "iomux.h"

struct iomux {
  addr_t base;
  unsigned int pins;
};

static struct iomux iomuxes[] = {
  [IOMUX_AP] = {
    .base = IOMUX_AP_BASE_ADDR,
    .pins = 44,
  },
  [IOMUX_COM] = {
    .base = IOMUX_COM_BASE_ADDR,
    .pins = 41,
  },
};

int iomux_set_function(int iomux, unsigned int pin, unsigned int infunc, unsigned int outfunc) {
  addr_t base;
  uint32_t v;
  unsigned int shift;

  if ((iomux < 0) || (iomux > IOMUX_MAX_NUM) || (pin > iomuxes[iomux].pins)) {
    return -1;
  }
  if ((outfunc > 7) || (infunc > 3)) {
    return -1;
  }
  base = iomuxes[iomux].base + (pin & ~0x3);
  shift = (pin & 3) << 3;
  v = read32(base);
  v &= ~(0xff << shift);
  v |= ((outfunc & 7) << 4) + (1 << infunc) << shift;
  write32(v, base);
  return 0;
}
