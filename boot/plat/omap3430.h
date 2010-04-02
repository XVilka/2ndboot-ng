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
#ifndef __TI_OMAP3430_H_
#define __TI_OMAP3430_H_

/*
 * 3430 specific Section
 */

/* Stuff on L3 Interconnect */
#define SMX_APE_BASE			0x68000000

/* L3 Firewall */
#define A_REQINFOPERM0		(SMX_APE_BASE + 0x05048)
#define A_READPERM0		(SMX_APE_BASE + 0x05050)
#define A_WRITEPERM0		(SMX_APE_BASE + 0x05058)

/* GPMC */
#define OMAP34XX_GPMC_BASE		(0x6E000000)

/* SMS */
#define OMAP34XX_SMS_BASE		0x6C000000

/* SDRC */
#define OMAP34XX_SDRC_BASE		0x6D000000

/*
 * L4 Peripherals - L4 Wakeup and L4 Core now
 */
#define OMAP34XX_CORE_L4_IO_BASE	0x48000000

#define OMAP34XX_WAKEUP_L4_IO_BASE	0x48300000

#define OMAP34XX_L4_PER			0x49000000

#define OMAP34XX_L4_IO_BASE		OMAP34XX_CORE_L4_IO_BASE

/* CONTROL */
#define OMAP34XX_CTRL_BASE		(OMAP34XX_L4_IO_BASE+0x2000)

/* TAP information  dont know for 3430*/
#define OMAP34XX_TAP_BASE		(0x49000000) /*giving some junk for virtio */

/* UART */
#define OMAP34XX_UART1			(OMAP34XX_L4_IO_BASE+0x6a000)
#define OMAP34XX_UART2			(OMAP34XX_L4_IO_BASE+0x6c000)
#define OMAP34XX_UART3			(OMAP34XX_L4_PER+0x20000)

/* General Purpose Timers */ 
#define OMAP34XX_GPT1			0x48318000
#define OMAP34XX_GPT2			0x49032000
#define OMAP34XX_GPT3			0x49034000 
#define OMAP34XX_GPT4			0x49036000
#define OMAP34XX_GPT5			0x49038000 
#define OMAP34XX_GPT6			0x4903A000 
#define OMAP34XX_GPT7			0x4903C000 
#define OMAP34XX_GPT8			0x4903E000 
#define OMAP34XX_GPT9			0x49040000 
#define OMAP34XX_GPT10			0x48086000
#define OMAP34XX_GPT11			0x48088000
#define OMAP34XX_GPT12			0x48304000

/* WatchDog Timers (1 secure, 3 GP) */
#define WD1_BASE			(0x4830C000)
#define WD2_BASE			(0x48314000)
#define WD3_BASE			(0x49030000)

/* 32KTIMER */
#define SYNC_32KTIMER_BASE		(0x48320000)
#define S32K_CR				(SYNC_32KTIMER_BASE+0x10)

/* OMAP3 GPIO registers */
#define OMAP34XX_GPIO1_BASE		0x48310000
#define OMAP34XX_GPIO2_BASE		0x49050000
#define OMAP34XX_GPIO3_BASE		0x49052000
#define OMAP34XX_GPIO4_BASE		0x49054000
#define OMAP34XX_GPIO5_BASE		0x49056000
#define OMAP34XX_GPIO6_BASE		0x49058000

#ifndef __ASSEMBLY__
typedef struct gpio {
	unsigned char res1[0x34];
	unsigned int oe;		/* 0x34 */
	unsigned int datain;            /* 0x38 */
	unsigned char res2[0x54];
	unsigned int cleardataout;	/* 0x90 */
	unsigned int setdataout;	/* 0x94 */
} gpio_t;
#endif

#define GPIO0				(0x1 << 0)
#define GPIO1				(0x1 << 1)
#define GPIO2				(0x1 << 2)
#define GPIO3				(0x1 << 3)
#define GPIO4				(0x1 << 4)
#define GPIO5				(0x1 << 5)
#define GPIO6				(0x1 << 6)
#define GPIO7				(0x1 << 7)
#define GPIO8				(0x1 << 8)
#define GPIO9				(0x1 << 9)
#define GPIO10				(0x1 << 10)
#define GPIO11				(0x1 << 11)
#define GPIO12				(0x1 << 12)
#define GPIO13				(0x1 << 13)
#define GPIO14				(0x1 << 14)
#define GPIO15				(0x1 << 15)
#define GPIO16				(0x1 << 16)
#define GPIO17				(0x1 << 17)
#define GPIO18				(0x1 << 18)
#define GPIO19				(0x1 << 19)
#define GPIO20				(0x1 << 20)
#define GPIO21				(0x1 << 21)
#define GPIO22				(0x1 << 22)
#define GPIO23				(0x1 << 23)
#define GPIO24				(0x1 << 24)
#define GPIO25				(0x1 << 25)
#define GPIO26				(0x1 << 26)
#define GPIO27				(0x1 << 27)
#define GPIO28				(0x1 << 28)
#define GPIO29				(0x1 << 29)
#define GPIO30				(0x1 << 30)
#define GPIO31				(0x1 << 31)

/* CM_FCLKEN_PER and CM_ICLKEN_PER */
#define CLKEN_PER_EN_GPIO6_BIT           17
#define CLKEN_PER_EN_GPIO5_BIT           16
#define CLKEN_PER_EN_GPIO4_BIT           15
#define CLKEN_PER_EN_GPIO3_BIT           14
#define CLKEN_PER_EN_GPIO2_BIT           13
#define CLKEN_PER_EN_WDT3_BIT            12
#define CLKEN_PER_EN_UART3_BIT           11
#define CLKEN_PER_EN_GPT9_BIT            10
#define CLKEN_PER_EN_GPT8_BIT             9
#define CLKEN_PER_EN_GPT7_BIT             8
#define CLKEN_PER_EN_GPT6_BIT             7
#define CLKEN_PER_EN_GPT5_BIT             6
#define CLKEN_PER_EN_GPT4_BIT             5
#define CLKEN_PER_EN_GPT3_BIT             4
#define CLKEN_PER_EN_GPT2_BIT             3
#define CLKEN_PER_EN_MCBSP4_BIT           2
#define CLKEN_PER_EN_MCBSP3_BIT           1
#define CLKEN_PER_EN_MCBSP2_BIT           0

/*
 * SDP3430 specific Section
 */

/*
 *  The 343x's chip selects are programmable.  The mask ROM
 *  does configure CS0 to 0x08000000 before dispatch.  So, if
 *  you want your code to live below that address, you have to
 *  be prepared to jump though hoops, to reset the base address.
 *  Same as in SDP3430
 */
#ifdef CONFIG_OMAP34XX
/* base address for indirect vectors (internal boot mode) */
#define SRAM_OFFSET0			0x40000000
#define SRAM_OFFSET1			0x00200000
#define SRAM_OFFSET2			0x0000F800
#define SRAM_VECT_CODE			(SRAM_OFFSET0|SRAM_OFFSET1|SRAM_OFFSET2)
#define LOW_LEVEL_SRAM_STACK		0x4020FFFC
#endif

#if defined(CONFIG_3430SDP) || defined(CONFIG_3630SDP)
/* FPGA on Debug board.*/
# define ETH_CONTROL_REG			(DEBUG_BASE+0x30b)
# define LAN_RESET_REGISTER		(DEBUG_BASE+0x1c)

# define DIP_SWITCH_INPUT_REG2		(DEBUG_BASE+0x60)
# define LED_REGISTER			(DEBUG_BASE+0x40)
# define FPGA_REV_REGISTER		(DEBUG_BASE+0x10)
# define EEPROM_MAIN_BRD			(DEBUG_BASE+0x10000+0x1800)
# define EEPROM_CONN_BRD			(DEBUG_BASE+0x10000+0x1900)
# define EEPROM_UI_BRD			(DEBUG_BASE+0x10000+0x1A00)
# define EEPROM_MCAM_BRD			(DEBUG_BASE+0x10000+0x1B00)
# define ENHANCED_UI_EE_NAME		"750-2075"
#endif

/* Some needing for cleaning code from FREESCALE */

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

#endif				/*  __TI_OMAP3430_H_ */
