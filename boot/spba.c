#include "common.h"
#include "mxc91231.h"
#include "spba.h"

int spba_change_rights(unsigned int module, unsigned int rights) {
  uint32_t v;

  if (module > SPBA_MAX_MODULE) {
    return -1;
  }
  rights &= 0x7;
  modify_register32(SPBA_CTRL_BASE_ADDR+module, 0x7, rights);
  if ((read32(SPBA_CTRL_BASE_ADDR+module) & 0x7) != rights) {
    return -1;
  }
  return 0;
}
