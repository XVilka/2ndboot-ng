#include "hw_misc.h"
#include "mxc91231.h"
#include "common.h"

int hw_preboot() {
/* Enable USBOTG clocks */
  modify_register32(CRM_AP_BASE_ADDR+0xc, 0, 1 << 12);
  modify_register32(CRM_AP_BASE_ADDR+0x28, 0, 0x3 << 22);
  modify_register32(CRM_AP_BASE_ADDR+0x34, 0, 0x3 << 16);
/* Reset USBOTG */
  write32(0x3f, USBOTG_CTRL_BASE_ADDR+0x10);
  while (read32(USBOTG_CTRL_BASE_ADDR+0x10)) {
  }
/* Enable main USBOTG clock */
  write32(0x1, USBOTG_CTRL_BASE_ADDR+0xc);
/* Disable SDHC1/2 clocks */
  modify_register32(CRM_AP_BASE_ADDR+0x60, 0, 1 << 0);
  modify_register32(CRM_AP_BASE_ADDR+0x60, 0, 1 << 8);
/* Reset EPIT */
  modify_register32(EPIT1_AP_BASE_ADDR+0x0, 0x1, 0);
  modify_register32(EPIT1_AP_BASE_ADDR+0x0, 0, 0x10000);
  while (read32(EPIT1_AP_BASE_ADDR+0x0) & 0x10000) {
  }
/* Disable and clear KPP */
  write16(0xf, KPP_BASE_ADDR+0x2);
/* Stop SDMA */
  write32(0xffffffff, SDMA_BASE_ADDR+0x8);
  write32(0xffffffff, SDMA_BASE_ADDR+0x4);
/* Reset SDMA */
  write32(0x1, SDMA_BASE_ADDR+0x24);
  while (read32(SDMA_BASE_ADDR+0x28) & 0x1) {
  }
/* Enable UART3 clocks */
  modify_register32(CRM_AP_BASE_ADDR+0x5c, 0, 1 << 16);  
}
