#ifndef __HW_SPBA_H__
#define __HW_SPBA_H__

#define SPBA_SDHC1		0x04
#define SPBA_SDHC2		0x08
#define SPBA_UART3		0x0C
#define SPBA_CSPI2		0x10
#define SPBA_SSI2		0x14
#define SPBA_SIM		0x18
#define SPBA_IIM		0x1C
#define SPBA_CTI_SDMA		0x20
#define SPBA_USBOTG_CTRL_REGS	0x24
#define SPBA_USBOTG_DATA_REGS	0x28
#define SPBA_CSPI1		0x30
#define SPBA_MQSPI		0x34
#define SPBA_EL1T		0x38
#define SPBA_IOMUX		0x40
#define SPBA_CRM_COM		0x44
#define SPBA_CRM_AP		0x48
#define SPBA_PLL0		0x4C
#define SPBA_PLL1		0x50
#define SPBA_PLL2		0x54
#define SPBA_GPIO4		0x58
#define SPBA_SAHARA		0x5C

#define SPBA_MAX_MODULE         SPBA_SAHARA

#define SPBA_GRANT_A            (1 << 0)
#define SPBA_GRANT_B            (1 << 1)
#define SPBA_GRANT_C            (1 << 2)

int spba_change_rights(unsigned int module, unsigned int rights);

#endif // __HW_SPBA_H__
