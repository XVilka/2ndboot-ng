/*
 * Copyright (C) 2007-2009 Texas Instruments, Inc.
 *
 * (C) Copyright 2009
 * Windriver, <www.windriver.com>
 * Tom Rix <Tom.Rix@windriver.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <i2c.h>

/* I2C chip addresses */

/* USB ID */
#define TWL4030_CHIP_USB		0x48
/* AUD ID */
#define TWL4030_CHIP_AUDIO_VOICE	0x49
#define TWL4030_CHIP_GPIO		0x49
#define TWL4030_CHIP_INTBR		0x49
#define TWL4030_CHIP_PIH		0x49
#define TWL4030_CHIP_TEST		0x49
/* AUX ID */
#define TWL4030_CHIP_KEYPAD		0x4a
#define TWL4030_CHIP_MADC		0x4a
#define TWL4030_CHIP_INTERRUPTS		0x4a
#define TWL4030_CHIP_LED		0x4a
#define TWL4030_CHIP_MAIN_CHARGE	0x4a
#define TWL4030_CHIP_PRECHARGE		0x4a
#define TWL4030_CHIP_PWM0		0x4a
#define TWL4030_CHIP_PWM1		0x4a
#define TWL4030_CHIP_PWMA		0x4a
#define TWL4030_CHIP_PWMB		0x4a
/* POWER ID */
#define TWL4030_CHIP_BACKUP		0x4b
#define TWL4030_CHIP_INT		0x4b
#define TWL4030_CHIP_PM_MASTER		0x4b
#define TWL4030_CHIP_PM_RECEIVER	0x4b
#define TWL4030_CHIP_RTC		0x4b
#define TWL4030_CHIP_SECURED_REG	0x4b

/* Register base addresses */

/* USB ID */
#define TWL4030_BASEADD_USB		0x0000
/* AUD ID */
#define TWL4030_BASEADD_AUDIO_VOICE	0x0000
#define TWL4030_BASEADD_GPIO		0x0098
#define TWL4030_BASEADD_INTBR		0x0085
#define TWL4030_BASEADD_PIH		0x0080
#define TWL4030_BASEADD_TEST		0x004C
/* AUX ID */
#define TWL4030_BASEADD_INTERRUPTS	0x00B9
#define TWL4030_BASEADD_LED		0x00EE
#define TWL4030_BASEADD_MADC		0x0000
#define TWL4030_BASEADD_MAIN_CHARGE	0x0074
#define TWL4030_BASEADD_PRECHARGE	0x00AA
#define TWL4030_BASEADD_PWM0		0x00F8
#define TWL4030_BASEADD_PWM1		0x00FB
#define TWL4030_BASEADD_PWMA		0x00EF
#define TWL4030_BASEADD_PWMB		0x00F1
#define TWL4030_BASEADD_KEYPAD		0x00D2
/* POWER ID */
#define TWL4030_BASEADD_BACKUP		0x0014
#define TWL4030_BASEADD_INT		0x002E
#define TWL4030_BASEADD_PM_MASTER	0x0036
#define TWL4030_BASEADD_PM_RECIEVER	0x005B
#define TWL4030_BASEADD_RTC		0x001C
#define TWL4030_BASEADD_SECURED_REG	0x0000

/* Register addresses */

#define REG_STS_HW_CONDITIONS	(TWL4030_BASEADD_PM_MASTER + 0x0F)
#define 	STS_VBUS		0x080
#define	STS_USB			0x04
#define 	STS_CHG			0x02
#define REG_BCICTL1 		(TWL4030_BASEADD_MAIN_CHARGE + 0x023)
#define REG_BCICTL2 		(TWL4030_BASEADD_MAIN_CHARGE + 0x024)
#define 	CGAIN			0x020
#define 	ITHEN			0x010
#define 	ITHSENS			0x007
#define REG_BCIMFTH1		(TWL4030_BASEADD_PM_MASTER + 0x016)
#define REG_BCIMFTH2		(TWL4030_BASEADD_PM_MASTER + 0x017)
#define BCIAUTOWEN		(TWL4030_BASEADD_PM_MASTER + 0x020)
#define 	CONFIG_DONE		0x010
#define 	CVENAC			0x04
#define 	BCIAUTOUSB		0x002
#define 	BCIAUTOAC		0x001
#define 	BCIMSTAT_MASK		0x03F
#define REG_BOOT_BCI		(TWL4030_BASEADD_PM_MASTER + 0x007)

#define REG_GPBR1		(TWL4030_BASEADD_INTBR + 0x0c)
#define 	MADC_HFCLK_EN		0x80
#define 	DEFAULT_MADC_CLK_EN	0x10

#define REG_CTRL1		(TWL4030_BASEADD_MADC + 0x00)
#define 	MADC_ON			0x01
#define 	REG_SW1SELECT_MSB	0x07
#define 	SW1_CH9_SEL		0x02
#define REG_CTRL_SW1		(TWL4030_BASEADD_MADC + 0x012)
#define 	SW1_TRIGGER		0x020
#define 	EOC_SW1			0x002
#define 	BUSY			0x001
#define REG_GPCH9		(TWL4030_BASEADD_MADC + 0x049)

#define REG_BCIMSTATEC		(TWL4030_BASEADD_MAIN_CHARGE + 0x002)
#define REG_BCIMFSTS2		(TWL4030_BASEADD_MAIN_CHARGE + 0x00E)
#define REG_BCIMFSTS3 		(TWL4030_BASEADD_MAIN_CHARGE + 0x00F)
#define REG_BCIMFSTS4		(TWL4030_BASEADD_MAIN_CHARGE + 0x010)
#define REG_BCIMFKEY		(TWL4030_BASEADD_MAIN_CHARGE + 0x011)
#define REG_BCIIREF1		(TWL4030_BASEADD_MAIN_CHARGE + 0x027)
#define REG_BCIIREF2		(TWL4030_BASEADD_MAIN_CHARGE + 0x028)

#define REG_BCIMFSTS1		(TWL4030_BASEADD_PRECHARGE + 0x001)
#define 	USBFASTMCHG		0x004
#define 	BATSTSPCHG		0x004
#define 	BATSTSMCHG		0x040
#define 	VBATOV4			0x020
#define 	VBATOV3			0x010
#define 	VBATOV2			0x008
#define 	VBATOV1			0x004
#define 	MADC_LSB_MASK		0xC0
#define REG_BB_CFG		(TWL4030_BASEADD_PM_RECIEVER + 0x12)
#define 	BBCHEN			0x10
#define		BBSEL_2500mV		0x00
#define		BBSEL_3000mV		0x04
#define		BBSEL_3100mV		0x08
#define		BBSEL_3200mV		0x0C
#define		BBISEL_25uA		0x00
#define		BBISEL_150uA		0x01
#define		BBISEL_500uA		0x02
#define		BBISEL_1000uA		0x03

#define REG_POWER_CTRL		(TWL4030_BASEADD_USB + 0x0AC)
#define REG_POWER_CTRL_SET 	(TWL4030_BASEADD_USB + 0x0AD)
#define REG_POWER_CTRL_CLR	(TWL4030_BASEADD_USB + 0x0AE)
#define 	OTG_EN			0x020
#define REG_PHY_CLK_CTRL	(TWL4030_BASEADD_USB + 0x0FE)
#define REG_PHY_CLK_CTRL_STS 	(TWL4030_BASEADD_USB + 0x0FF)
#define 	PHY_DPLL_CLK		0x01

/*  TWL4030 battery measuring parameters */
#define T2_BATTERY_VOLT		(TWL4030_BASEADD_MAIN_CHARGE + 0x04)
#define T2_BATTERY_TEMP		(TWL4030_BASEADD_MAIN_CHARGE + 0x06)
#define T2_BATTERY_CUR		(TWL4030_BASEADD_MAIN_CHARGE + 0x08)
#define T2_BATTERY_ACVOLT	(TWL4030_BASEADD_MAIN_CHARGE + 0x0A)
#define T2_BATTERY_USBVOLT	(TWL4030_BASEADD_MAIN_CHARGE + 0x0C)

/*
 * Power Management Master
 */
#define PM_MASTER_CFG_P1_TRANSITION	0x36
#define PM_MASTER_CFG_P2_TRANSITION	0x37
#define PM_MASTER_CFG_P3_TRANSITION	0x38
#define PM_MASTER_CFG_P123_TRANSITION	0x39
#define PM_MASTER_STS_BOOT		0x3A
#define PM_MASTER_CFG_BOOT		0x3B
#define PM_MASTER_SHUNDAN		0x3C
#define PM_MASTER_BOOT_BCI		0x3D
#define PM_MASTER_CFG_PWRANA1		0x3E
#define PM_MASTER_CFG_PWRANA2		0x3F
#define PM_MASTER_BGAP_TRIM		0x40
#define PM_MASTER_BACKUP_MISC_STS	0x41
#define PM_MASTER_BACKUP_MISC_CFG	0x42
#define PM_MASTER_BACKUP_MISC_TST	0x43
#define PM_MASTER_PROTECT_KEY		0x44
#define PM_MASTER_STS_HW_CONDITIONS	0x45
#define PM_MASTER_P1_SW_EVENTS		0x46
#define PM_MASTER_P2_SW_EVENTS		0x47
#define PM_MASTER_P3_SW_EVENTS		0x48
#define PM_MASTER_STS_P123_STATE	0x49
#define PM_MASTER_PB_CFG		0x4A
#define PM_MASTER_PB_WORD_MSB		0x4B
#define PM_MASTER_PB_WORD_LSB		0x4C
#define PM_MASTER_SEQ_ADD_W2P		0x52
#define PM_MASTER_SEQ_ADD_P2A		0x53
#define PM_MASTER_SEQ_ADD_A2W		0x54
#define PM_MASTER_SEQ_ADD_A2S		0x55
#define PM_MASTER_SEQ_ADD_S2A12		0x56
#define PM_MASTER_SEQ_ADD_S2A3		0x57
#define PM_MASTER_SEQ_ADD_WARM		0x58
#define PM_MASTER_MEMORY_ADDRESS	0x59
#define PM_MASTER_MEMORY_DATA		0x5A
#define PM_MASTER_SC_CONFIG		0x5B
#define PM_MASTER_SC_DETECT1		0x5C
#define PM_MASTER_SC_DETECT2		0x5D
#define PM_MASTER_WATCHDOG_CFG		0x5E
#define PM_MASTER_IT_CHECK_CFG		0x5F
#define PM_MASTER_VIBRATOR_CFG		0x60
#define PM_MASTER_DCDC_GLOBAL_CFG	0x61
#define PM_MASTER_VDD1_TRIM1		0x62
#define PM_MASTER_VDD1_TRIM2		0x63
#define PM_MASTER_VDD2_TRIM1		0x64
#define PM_MASTER_VDD2_TRIM2		0x65
#define PM_MASTER_VIO_TRIM1		0x66
#define PM_MASTER_VIO_TRIM2		0x67
#define PM_MASTER_MISC_CFG		0x68
#define PM_MASTER_LS_TST_A		0x69
#define PM_MASTER_LS_TST_B		0x6A
#define PM_MASTER_LS_TST_C		0x6B
#define PM_MASTER_LS_TST_D		0x6C
#define PM_MASTER_BB_CFG		0x6D
#define PM_MASTER_MISC_TST		0x6E
#define PM_MASTER_TRIM1			0x6F
/* P[1-3]_SW_EVENTS */
#define SW_EVENTS_STOPON_PWRON		(1 << 6)
#define SW_EVENTS_STOPON_SYSEN		(1 << 5)
#define SW_EVENTS_ENABLE_WARMRESET	(1 << 4)
#define SW_EVENTS_LVL_WAKEUP		(1 << 3)
#define SW_EVENTS_DEVACT		(1 << 2)
#define SW_EVENTS_DEVSLP		(1 << 1)
#define SW_EVENTS_DEVOFF		(1 << 0)

/* Power Managment Receiver */
#define PM_RECEIVER_VDD1_DEV_GRP		0xB0
#define PM_RECEIVER_VDD1_TYPE			0xB1
#define PM_RECEIVER_VDD1_REMAP			0xB2
#define PM_RECEIVER_VDD1_CFG			0xB3
#define PM_RECEIVER_VDD1_MISC_CFG		0xB4
#define PM_RECEIVER_VDD1_TEST1			0xB5
#define PM_RECEIVER_VDD1_TEST2			0xB6
#define PM_RECEIVER_VDD1_OSC			0xB7
#define PM_RECEIVER_VDD1_RESERVED		0xB8
#define PM_RECEIVER_VDD1_VSEL			0xB9
#define PM_RECEIVER_VDD1_VMODE_CFG		0xBA
#define PM_RECEIVER_VDD1_VFLOOR			0xBB
#define PM_RECEIVER_VDD1_VROOF			0xBC
#define PM_RECEIVER_VDD1_STEP			0xBD
#define PM_RECEIVER_VDD2_DEV_GRP		0xBE
#define PM_RECEIVER_VDD2_TYPE			0xBF
#define PM_RECEIVER_VDD2_REMAP			0xC0
#define PM_RECEIVER_VDD2_CFG			0xC1
#define PM_RECEIVER_VDD2_MISC_CFG		0xC2
#define PM_RECEIVER_VDD2_TEST1			0xC3
#define PM_RECEIVER_VDD2_TEST2			0xC4
#define PM_RECEIVER_VDD2_OSC			0xC5
#define PM_RECEIVER_VDD2_RESERVED		0xC6
#define PM_RECEIVER_VDD2_VSEL			0xC7
#define PM_RECEIVER_VDD2_VMODE_CFG		0xC8
#define PM_RECEIVER_VDD2_VFLOOR			0xC9
#define PM_RECEIVER_VDD2_VROOF			0xCA
#define PM_RECEIVER_VDD2_STEP			0xCB
#define PM_RECEIVER_VUSB1V5_DEV_GRP		0xCC
#define PM_RECEIVER_VUSB1V5_TYPE		0xCD
#define PM_RECEIVER_VUSB1V5_REMAP		0xCE
#define PM_RECEIVER_VUSB1V8_DEV_GRP		0xCF
#define PM_RECEIVER_VUSB1V8_TYPE		0xD0
#define PM_RECEIVER_VUSB1V8_REMAP		0xD1
#define PM_RECEIVER_VUSB3V1_DEV_GRP		0xD2
#define PM_RECEIVER_VUSB3V1_TYPE		0xD3
#define PM_RECEIVER_VUSB3V1_REMAP		0xD4
#define PM_RECEIVER_VUSBCP_DEV_GRP		0xD5
#define PM_RECEIVER_VUSBCP_DEV_TYPE		0xD6
#define PM_RECEIVER_VUSBCP_DEV_REMAP		0xD7
#define PM_RECEIVER_VUSB_DEDICATED1		0xD8
#define PM_RECEIVER_VUSB_DEDICATED2		0xD9

/* Keypad */
#define KEYPAD_KEYP_CTRL_REG         0xD2
#define KEYPAD_KEY_DEB_REG           0xD3
#define KEYPAD_LONG_KEY_REG1         0xD4
#define KEYPAD_LK_PTV_REG            0xD5
#define KEYPAD_TIME_OUT_REG1         0xD6
#define KEYPAD_TIME_OUT_REG2         0xD7
#define KEYPAD_KBC_REG               0xD8
#define KEYPAD_KBR_REG               0xD9
#define KEYPAD_KEYP_SMS              0xDA
#define KEYPAD_FULL_CODE_7_0         0xDB
#define KEYPAD_FULL_CODE_15_8        0xDC
#define KEYPAD_FULL_CODE_23_16       0xDD
#define KEYPAD_FULL_CODE_31_24       0xDE
#define KEYPAD_FULL_CODE_39_32       0xDF
#define KEYPAD_FULL_CODE_47_40       0xE0
#define KEYPAD_FULL_CODE_55_48       0xE1
#define KEYPAD_FULL_CODE_63_56       0xE2
#define KEYPAD_KEYP_ISR1             0xE3
#define KEYPAD_KEYP_IMR1             0xE4
#define KEYPAD_KEYP_ISR2             0xE5
#define KEYPAD_KEYP_IMR2             0xE6
#define KEYPAD_KEYP_SIR              0xE7
#define KEYPAD_KEYP_EDR              0xE8
#define KEYPAD_KEYP_SIH_CTRL         0xE9

#define KEYPAD_CTRL_KBD_ON           (1 << 6)
#define KEYPAD_CTRL_RP_EN            (1 << 5)
#define KEYPAD_CTRL_TOLE_EN          (1 << 4)
#define KEYPAD_CTRL_TOE_EN           (1 << 3)
#define KEYPAD_CTRL_LK_EN            (1 << 2)
#define KEYPAD_CTRL_SOFTMODEN        (1 << 1)
#define KEYPAD_CTRL_SOFT_NRST        (1 << 0)

/* Declarations for users of the keypad, stubs for everyone else. */
#if (defined(CONFIG_TWL4030_KEYPAD) && (CONFIG_TWL4030_KEYPAD))
int twl4030_keypad_init(void);
int twl4030_keypad_reset(void);
int twl4030_keypad_keys_pressed(unsigned char *key1, unsigned char *key2);
#else
#define twl4030_keypad_init() 0
#define twl4030_keypad_reset() 0
#define twl4030_keypad_keys_pressed(a, b) 0
#endif

/* USB */
#define	USB_FUNC_CTRL		(0x04)
#define	USB_OPMODE_MASK		(3 << 3) /* bits 3 and 4 */
#define	USB_XCVRSELECT_MASK	(3 << 0) /* bits 0 and 1 */
#define	USB_IFC_CTRL		(0x07)
#define	USB_CARKITMODE		(1 << 2)
#define	USB_POWER_CTRL		(0xAC)
#define	USB_OTG_ENAB		(1 << 5)
#define	USB_PHY_PWR_CTRL	(0xFD)
#define	USB_PHYPWD		(1 << 0)
#define	USB_PHY_CLK_CTRL	(0xFE)
#define	USB_CLOCKGATING_EN	(1 << 2)
#define	USB_CLK32K_EN		(1 << 1)
#define	USB_REQ_PHY_DPLL_CLK	(1 << 0)
#define	USB_PHY_CLK_CTRL_STS	(0xFF)
#define	USB_PHY_DPLL_CLK	(1 << 0)

/* Declarations for users of usb, stubs for everyone else. */
#if (defined(CONFIG_TWL4030_USB) && (CONFIG_TWL4030_USB))
int twl4030_usb_init(void);
#else
#define twl4030_usb_init() 0
#endif

/* Power Reset, use to initialize system wide resetting */
void twl4030_power_reset_init(void);
