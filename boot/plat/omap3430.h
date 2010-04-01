/*
 *  Copyright 2004-2006 Freescale Semiconductor, Inc. All Rights Reserved.
 *    - Platform specific register memory map
 *
 *  Copyright (C) 2006-2007 Motorola, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __ASM_ARM_ARCH_MXC91231_H_
#define __ASM_ARM_ARCH_MXC91231_H_

/*
 * L2CC
 */
#define     L2CC_BASE_ADDR          0x30000000

/*
 * AIPS 1
 */
#define     AIPS1_BASE_ADDR         0x43F00000

#define     AIPS1_CTRL_BASE_ADDR    AIPS1_BASE_ADDR
#define     MAX_BASE_ADDR           (AIPS1_BASE_ADDR + 0x04000)
#define     EVTMON_BASE_ADDR        (AIPS1_BASE_ADDR + 0x08000)
#define     CLKCTL_BASE_ADDR        (AIPS1_BASE_ADDR + 0x0C000)
#define     ETB_SLOT4_BASE_ADDR     (AIPS1_BASE_ADDR + 0x10000)
#define     ETB_SLOT5_BASE_ADDR     (AIPS1_BASE_ADDR + 0x14000)
#define     ECT_CTIO_BASE_ADDR      (AIPS1_BASE_ADDR + 0x18000)
#define     I2C_BASE_ADDR           (AIPS1_BASE_ADDR + 0x80000)
#define     MU_BASE_ADDR            (AIPS1_BASE_ADDR + 0x88000)
#define     UART1_BASE_ADDR         (AIPS1_BASE_ADDR + 0x90000)
#define     UART2_BASE_ADDR         (AIPS1_BASE_ADDR + 0x94000)
#define     DSM_BASE_ADDR           (AIPS1_BASE_ADDR + 0x98000)
#define     OWIRE_BASE_ADDR         (AIPS1_BASE_ADDR + 0x9C000)
#define     SSI1_BASE_ADDR          (AIPS1_BASE_ADDR + 0xA0000)
#define     KPP_BASE_ADDR           (AIPS1_BASE_ADDR + 0xA8000)
#define     IOMUX_AP_BASE_ADDR      (AIPS1_BASE_ADDR + 0xAC000)
#define     CTI_AP_BASE_ADDR        (AIPS1_BASE_ADDR + 0xB8000)

/*
 * SPBA global module enabled #0
 */
#define     SPBA0_BASE_ADDR         0x50000000

#define     MMC_SDHC1_BASE_ADDR     (SPBA0_BASE_ADDR + 0x04000)
#define     MMC_SDHC2_BASE_ADDR     (SPBA0_BASE_ADDR + 0x08000)
#define     UART3_BASE_ADDR         (SPBA0_BASE_ADDR + 0x0C000)
#define     CSPI2_BASE_ADDR         (SPBA0_BASE_ADDR + 0x10000)
#define     SSI2_BASE_ADDR          (SPBA0_BASE_ADDR + 0x14000)
#define     SIM_BASE_ADDR           (SPBA0_BASE_ADDR + 0x18000)
#define     IIM_BASE_ADDR           (SPBA0_BASE_ADDR + 0x1C000)
#define     CTI_SDMA_BASE_ADDR      (SPBA0_BASE_ADDR + 0x20000)
#define     USBOTG_CTRL_BASE_ADDR   (SPBA0_BASE_ADDR + 0x24000)
#define     USBOTG_DATA_BASE_ADDR   (SPBA0_BASE_ADDR + 0x28000)
#define     CSPI1_BASE_ADDR         (SPBA0_BASE_ADDR + 0x30000)
#define     SPBA_CTRL_BASE_ADDR     (SPBA0_BASE_ADDR + 0x3C000)
#define     IOMUX_COM_BASE_ADDR     (SPBA0_BASE_ADDR + 0x40000)
#define     CRM_COM_BASE_ADDR       (SPBA0_BASE_ADDR + 0x44000)
#define     CRM_AP_BASE_ADDR        (SPBA0_BASE_ADDR + 0x48000)
#define     PLL0_BASE_ADDR          (SPBA0_BASE_ADDR + 0x4C000)
#define     PLL1_BASE_ADDR          (SPBA0_BASE_ADDR + 0x50000)
#define     PLL2_BASE_ADDR          (SPBA0_BASE_ADDR + 0x54000)
#define     GPIO4_SH_BASE_ADDR      (SPBA0_BASE_ADDR + 0x58000)
#define     HAC_BASE_ADDR           (SPBA0_BASE_ADDR + 0x5C000)
#define     SAHARA_BASE_ADDR        (SPBA0_BASE_ADDR + 0x5C000)
#define     PLL3_BASE_ADDR          (SPBA0_BASE_ADDR + 0x60000)

/*
 * SPBA global module enabled #1
 */
#define     SPBA1_BASE_ADDR         0x52000000

#define     MQSPI_BASE_ADDR         (SPBA1_BASE_ADDR + 0x34000)
#define     EL1T_BASE_ADDR          (SPBA1_BASE_ADDR + 0x38000)

/*
 * AIPS 2
 */
#define     AIPS2_BASE_ADDR         0x53F00000

#define     GEMK_BASE_ADDR          (AIPS2_BASE_ADDR + 0x8C000)
#define     GPT1_BASE_ADDR          (AIPS2_BASE_ADDR + 0x90000)
#define     EPIT1_AP_BASE_ADDR      (AIPS2_BASE_ADDR + 0x94000)
#define     SCC_BASE_ADDR           (AIPS2_BASE_ADDR + 0xAC000)
#define     RNGA_BASE_ADDR          (AIPS2_BASE_ADDR + 0xB0000)
#define     IPU_CTRL_BASE_ADDR      (AIPS2_BASE_ADDR + 0xC0000)
#define     AUDMUX_BASE_ADDR        (AIPS2_BASE_ADDR + 0xC4000)
#define     EDIO_BASE_ADDR          (AIPS2_BASE_ADDR + 0xC8000)
#define     GPIO1_AP_BASE_ADDR      (AIPS2_BASE_ADDR + 0xCC000)
#define     GPIO2_AP_BASE_ADDR      (AIPS2_BASE_ADDR + 0xD0000)
#define     SDMA_BASE_ADDR          (AIPS2_BASE_ADDR + 0xD4000)
#define     RTC_BASE_ADDR           (AIPS2_BASE_ADDR + 0xD8000)
#define     WDOG1_BASE_ADDR         (AIPS2_BASE_ADDR + 0xDC000)
#define     PWM_BASE_ADDR           (AIPS2_BASE_ADDR + 0xE0000)
#define     GPIO3_AP_BASE_ADDR      (AIPS2_BASE_ADDR + 0xE4000)
#define     WDOG2_BASE_ADDR         (AIPS2_BASE_ADDR + 0xE8000)
#define     RTIC_BASE_ADDR          (AIPS2_BASE_ADDR + 0xEC000)
#define     LPMC_BASE_ADDR          (AIPS2_BASE_ADDR + 0xF0000)

/*
 * ROMP and AVIC
 */
#define     ROMP_BASE_ADDR          0x60000000

#define     AVIC_BASE_ADDR          0x68000000

/*
 * NAND, SDRAM, WEIM, M3IF, EMI controllers
 */
#define     X_MEMC_BASE_ADDR        0xB8000000

#define     NFC_BASE_ADDR           (X_MEMC_BASE_ADDR + 0x0000)
#define     ESDCTL_BASE_ADDR        (X_MEMC_BASE_ADDR + 0x1000)
#define     WEIM_BASE_ADDR          (X_MEMC_BASE_ADDR + 0x2000)
#define     M3IF_BASE_ADDR          (X_MEMC_BASE_ADDR + 0x3000)
#define     EMI_CTL_BASE_ADDR       (X_MEMC_BASE_ADDR + 0x4000)

#define     FB_RAM_BASE_ADDR        0x78000000

#define PROD_SIGNATURE          0x8	/* For MXC91231 */
#define CHIP_REV_1_0            0x10
#define CHIP_REV_2_0            0x20
#define CHIP_REV_2_1            0x21

#define SYSTEM_REV_MIN          CHIP_REV_1_0
#define SYSTEM_REV_NUM          3

#define L1_LINE_LENGTH          (32)
#define L2_LINE_LENGTH          (32)

#endif				/*  __ASM_ARM_ARCH_MXC91231_H_ */
