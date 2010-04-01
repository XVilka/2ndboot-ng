/*
 * Copyright (C) 2007-2008 Texas Instruments, Inc.
 *
 * USB
 * Imported from omap3-dev drivers/usb/twl4030_usb.c
 * This is unique part of the copyright
 *
 * twl4030_usb - TWL4030 USB transceiver, talking to OMAP OTG controller
 *
 * (C) Copyright 2009 Atin Malaviya (atin.malaviya@gmail.com)
 *
 * Based on: twl4030_usb.c in linux 2.6 (drivers/i2c/chips/twl4030_usb.c)
 * Copyright (C) 2004-2007 Texas Instruments
 * Copyright (C) 2008 Nokia Corporation
 * Contact: Felipe Balbi <felipe.balbi@nokia.com>
 *
 * Author: Atin Malaviya (atin.malaviya@gmail.com)
 *
 *
 * Keypad
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
 *
 */
#include <twl4030.h>
#include <asm/arch/led.h>

/*
 * Battery
 */
#define mdelay(n) ({ unsigned long msec = (n); while (msec--) udelay(1000); })

/* Functions to read and write from TWL4030 */
static inline int twl4030_i2c_write_u8(u8 chip_no, u8 val, u8 reg)
{
	return i2c_write(chip_no, reg, 1, &val, 1);
}

static inline int twl4030_i2c_read_u8(u8 chip_no, u8 *val, u8 reg)
{
	return i2c_read(chip_no, reg, 1, val, 1);
}

/*
 * Sets and clears bits on an given register on a given module
 */
static inline int clear_n_set(u8 chip_no, u8 clear, u8 set, u8 reg)
{
	int ret;
	u8 val = 0;

	/* Gets the initial register value */
	ret = twl4030_i2c_read_u8(chip_no, &val, reg);
	if (ret) {
		printf("a\n");
		return ret;
	}

	/* Clearing all those bits to clear */
	val &= ~(clear);

	/* Setting all those bits to set */
	val |= set;

	/* Update the register */
	ret = twl4030_i2c_write_u8(chip_no, val, reg);
	if (ret) {
		printf("b\n");
		return ret;
	}
	return 0;
}

/*
 * Disable/Enable AC Charge funtionality.
 */
static int twl4030_ac_charger_enable(int enable)
{
	int ret;

	if (enable) {
		/* forcing the field BCIAUTOAC (BOOT_BCI[0]) to 1 */
		ret = clear_n_set(TWL4030_CHIP_PM_MASTER, 0,
			       (CONFIG_DONE | BCIAUTOWEN | BCIAUTOAC),
			       REG_BOOT_BCI);
		if (ret)
			return ret;
	} else {
		/* forcing the field BCIAUTOAC (BOOT_BCI[0]) to 0 */
		ret = clear_n_set(TWL4030_CHIP_PM_MASTER, BCIAUTOAC,
			       (CONFIG_DONE | BCIAUTOWEN),
			       REG_BOOT_BCI);
		if (ret)
			return ret;
	}
	return 0;
}

/*
 * Disable/Enable USB Charge funtionality.
 */
static int twl4030_usb_charger_enable(int enable)
{
	u8 value;
	int ret;

	if (enable) {
		/* enable access to BCIIREF1 */
		ret = twl4030_i2c_write_u8(TWL4030_CHIP_MAIN_CHARGE, 0xE7,
			REG_BCIMFKEY);
		if (ret)
			return ret;

		/* set charging current = 852mA */
		ret = twl4030_i2c_write_u8(TWL4030_CHIP_MAIN_CHARGE, 0xFF,
			REG_BCIIREF1);
		if (ret)
			return ret;

		/* forcing the field BCIAUTOUSB (BOOT_BCI[1]) to 1 */
		ret = clear_n_set(TWL4030_CHIP_PM_MASTER, 0,
			(CONFIG_DONE | BCIAUTOWEN | BCIAUTOUSB),
			REG_BOOT_BCI);
		if (ret)
			return ret;

		/* Enabling interfacing with usb thru OCP */
		ret = clear_n_set(TWL4030_CHIP_USB, 0, PHY_DPLL_CLK,
			REG_PHY_CLK_CTRL);
		if (ret)
			return ret;

		value = 0;

		while (!(value & PHY_DPLL_CLK)) {
			udelay(10);
			ret = twl4030_i2c_read_u8(TWL4030_CHIP_USB, &value,
				REG_PHY_CLK_CTRL_STS);
			if (ret)
				return ret;
		}

		/* OTG_EN (POWER_CTRL[5]) to 1 */
		ret = clear_n_set(TWL4030_CHIP_USB, 0, OTG_EN,
			REG_POWER_CTRL);
		if (ret)
			return ret;

		mdelay(50);

		/* forcing USBFASTMCHG(BCIMFSTS4[2]) to 1 */
		ret = clear_n_set(TWL4030_CHIP_MAIN_CHARGE, 0,
			USBFASTMCHG, REG_BCIMFSTS4);
		if (ret)
			return ret;
	} else {
		ret = clear_n_set(TWL4030_CHIP_PM_MASTER, BCIAUTOUSB,
			(CONFIG_DONE | BCIAUTOWEN), REG_BOOT_BCI);
		if (ret)
			return ret;
	}

	return 0;
}

/*
 * Setup the twl4030 MADC module to measure the backup
 * battery voltage.
 */
static int twl4030_madc_setup(void)
{
	int ret = 0;

	/* turning MADC clocks on */
	ret = clear_n_set(TWL4030_CHIP_INTBR, 0,
		(MADC_HFCLK_EN | DEFAULT_MADC_CLK_EN), REG_GPBR1);
	if (ret)
		return ret;

	/* turning adc_on */
	ret = twl4030_i2c_write_u8(TWL4030_CHIP_MADC, MADC_ON,
		REG_CTRL1);
	if (ret)
		return ret;

	/* setting MDC channel 9 to trigger by SW1 */
	ret = clear_n_set(TWL4030_CHIP_MADC, 0, SW1_CH9_SEL,
		REG_SW1SELECT_MSB);

	return ret;
}

/*
 * Charge backup battery through main battery
 */
static int twl4030_charge_backup_battery(void)
{
	int ret;

	ret = clear_n_set(TWL4030_CHIP_PM_RECEIVER, 0xff,
			  (BBCHEN | BBSEL_3200mV | BBISEL_150uA), REG_BB_CFG);
	if (ret)
		return ret;

	return 0;
}

/*
 * Helper function to read a 2-byte register on BCI module
 */
static int read_bci_val(u8 reg)
{
	int ret = 0, temp = 0;
	u8 val;

	/* reading MSB */
	ret = twl4030_i2c_read_u8(TWL4030_CHIP_MAIN_CHARGE, &val, reg + 1);
	if (ret)
		return ret;

	temp = ((int)(val & 0x03)) << 8;

	/* reading LSB */
	ret = twl4030_i2c_read_u8(TWL4030_CHIP_MAIN_CHARGE, &val, reg);
	if (ret)
		return ret;

	return temp + val;
}

/*
 * Triggers the sw1 request for the twl4030 module to measure the sw1 selected
 * channels
 */
static int twl4030_madc_sw1_trigger(void)
{
	u8 val;
	int ret;

	/* Triggering SW1 MADC convertion */
	ret = twl4030_i2c_read_u8(TWL4030_CHIP_MADC, &val, REG_CTRL_SW1);
	if (ret)
		return ret;

	val |= SW1_TRIGGER;

	ret = twl4030_i2c_write_u8(TWL4030_CHIP_MADC, val, REG_CTRL_SW1);
	if (ret)
		return ret;

	/* Waiting until the SW1 conversion ends*/
	val =  BUSY;

	while (!((val & EOC_SW1) && (!(val & BUSY)))) {
		ret = twl4030_i2c_read_u8(TWL4030_CHIP_MADC, &val,
					  REG_CTRL_SW1);
		if (ret)
			return ret;
		mdelay(10);
	}

	return 0;
}

/*
 * Return battery voltage
 */
static int twl4030_get_battery_voltage(void)
{
	int volt;

	volt = read_bci_val(T2_BATTERY_VOLT);
	return (volt * 588) / 100;
}

/*
 * Return the battery backup voltage
 */
static int twl4030_get_backup_battery_voltage(void)
{
	int ret, temp;
	u8 volt;

	/* trigger MADC convertion */
	twl4030_madc_sw1_trigger();

	ret = twl4030_i2c_read_u8(TWL4030_CHIP_MADC, &volt, REG_GPCH9 + 1);
	if (ret)
		return ret;

	temp = ((int) volt) << 2;

	ret = twl4030_i2c_read_u8(TWL4030_CHIP_MADC, &volt, REG_GPCH9);
	if (ret)
		return ret;

	temp = temp + ((int) ((volt & MADC_LSB_MASK) >> 6));

	return  (temp * 441) / 100;
}

#if 0 /* Maybe used in future */
/*
 * Return the AC power supply voltage
 */
static int twl4030_get_ac_charger_voltage(void)
{
	int volt = read_bci_val(T2_BATTERY_ACVOLT);
	return (volt * 735) / 100;
}

/*
 * Return the USB power supply voltage
 */
static int twl4030_get_usb_charger_voltage(void)
{
	int volt = read_bci_val(T2_BATTERY_USBVOLT);
	return (volt * 2058) / 300;
}
#endif

/*
 * Battery charging main function called from board-specific file
 */

int twl4030_init_battery_charging(void)
{
	u8 hwsts;
	int battery_volt = 0, charger_present = 0;
	int ret = 0, ac_t2_enabled = 0, charger_tries = 0;

#ifdef CONFIG_3430ZOOM2
	/* For Zoom2 enable Main charge Automatic mode:
	 * by enabling MADC clocks
	 */

	OMAP3_LED_ERROR_ON();

	/* Disable USB, enable AC: 0x35 defalut */
	ret = clear_n_set(TWL4030_CHIP_PM_MASTER, BCIAUTOUSB,
			       BCIAUTOAC,
			       REG_BOOT_BCI);

	/* Enable AC charging : ROM code is shutting down MADC CLK */
	ret = clear_n_set(TWL4030_CHIP_INTBR, 0,
		(MADC_HFCLK_EN | DEFAULT_MADC_CLK_EN), REG_GPBR1);

	udelay(100);

	ret = clear_n_set(TWL4030_CHIP_PM_MASTER,
				BCIAUTOAC | CVENAC,
				0,
				REG_BOOT_BCI);

	/* Change charging current */
	ret = twl4030_i2c_write_u8(TWL4030_CHIP_MAIN_CHARGE, 0xE7,
			REG_BCIMFKEY);
	/* set 1 Amp charging */
	ret = twl4030_i2c_write_u8(TWL4030_CHIP_MAIN_CHARGE, 0x58,
			REG_BCIIREF1);
	ret = twl4030_i2c_write_u8(TWL4030_CHIP_MAIN_CHARGE, 0xE7,
			REG_BCIMFKEY);
	ret = twl4030_i2c_write_u8(TWL4030_CHIP_MAIN_CHARGE, 0x03,
			REG_BCIIREF2);

	/* Set CGAIN=1 */
	ret = clear_n_set(TWL4030_CHIP_MAIN_CHARGE,
				0,
				CGAIN,
				REG_BCICTL1);

	ret = clear_n_set(TWL4030_CHIP_PM_MASTER,
				0,
				BCIAUTOAC | CVENAC,
				REG_BOOT_BCI);


#endif

#ifdef CFG_BATTERY_CHARGING
	/* Read the sts_hw_conditions register */
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_MASTER, &hwsts,
			  REG_STS_HW_CONDITIONS);

	/* AC T2 charger present */
	if (hwsts & STS_CHG) {
		OMAP3_LED_OK_ON(); /* Blue LED - on */
		ret = twl4030_ac_charger_enable(1);
		if (ret)
			return ret;
		udelay(10000); /* 0.01 sec */
		charger_present = 1;
		ac_t2_enabled = 1;
		OMAP3_LED_OK_OFF(); /* Blue LED - off */
	}

	/* USB charger present */
	if ((hwsts & STS_VBUS) | (hwsts & STS_USB)) {
		OMAP3_LED_OK_ON(); /* Blue LED - on */
		charger_present = 1;
	}

	ret = twl4030_madc_setup();
	if (ret) {
		printf("twl4030 madc setup error %d\n", ret);
		return ret;
	}

	/* usb charging is enabled regardless of the whether the
	* charger is attached, otherwise we will not be able to enable
	* usb charging at a later stage
	*/
	ret = twl4030_usb_charger_enable(1);
	if (ret)
		return ret;
	udelay(10000); /* 0.01 sec */
	OMAP3_LED_OK_OFF(); /* Blue LED - off */

	/* AC charging is enabled regardless of the whether the
	* charger is attached
	*/
	if (!ac_t2_enabled) {
		ret = twl4030_ac_charger_enable(1);
		if (ret)
			return ret;
	}

	/* backup battery charges through main battery */
	ret = twl4030_charge_backup_battery();
	if (ret) {
		printf("backup battery charging error\n");
		return ret;
	}

	battery_volt = twl4030_get_battery_voltage();
	printf("Battery levels: main %d mV, backup %d mV\n",
	battery_volt, twl4030_get_backup_battery_voltage());
	if (battery_volt < CFG_LOW_BAT) {
		printf("Main battery charge too low!\n");
		printf("Please connect USB or AC charger to continue.\n");
		/*
		* Main charging loop
		* If the battery volage is below CFG_LOW_BAT, attempt is made
		* to recharge the battery to CFG_BAT_CHG.  The main led will
		* blink red until either the ac or the usb charger is connected.
		*/
		do {
			/* Read the sts_hw_conditions register */
			twl4030_i2c_read_u8(TWL4030_CHIP_PM_MASTER, &hwsts,
			REG_STS_HW_CONDITIONS);

			if ((hwsts & STS_CHG) |
			((hwsts & STS_VBUS) | (hwsts & STS_USB))) {
				OMAP3_LED_OK_OFF(); /* Blue LED - off */
				charger_present = 1;
			}	else {
				OMAP3_LED_ERROR_OFF();
				udelay(10000); /* 0.01 sec */
			}

			if (charger_present)	{
				OMAP3_LED_OK_ON(); /* Blue LED - on */
				udelay(10000); /* 0.01 sec */
			}	else
				OMAP3_LED_ERROR_ON();

			charger_tries++;

			if (ctrlc()) {
				printf("Battery charging terminated by user\n");
				printf("Battery charged to %d\n", battery_volt);
				break;
			}	else if (charger_tries >
							CFG_CHARGER_TRIES_MAX) {
					printf("No charger connected, \
					       giving up on charging.\n");
					break;
			}

		} while (((battery_volt = twl4030_get_battery_voltage())
					< CFG_BAT_CHG) && (!charger_present));
		/* If debug board charger is connected,
		*  battery_volt is approximately 4100mV
		*/
	}

	OMAP3_LED_OK_OFF(); /* Blue LED - off */
	OMAP3_LED_ERROR_OFF();

#endif

	return ret;

}

#if (defined(CONFIG_TWL4030_KEYPAD) && (CONFIG_TWL4030_KEYPAD))
/*
 * Keypad
 */
int twl4030_keypad_init(void)
{
	int ret = 0;
	u8 ctrl;
	ret = twl4030_i2c_read_u8(TWL4030_CHIP_KEYPAD, &ctrl, KEYPAD_KEYP_CTRL_REG);
	if (!ret) {
		ctrl |= KEYPAD_CTRL_KBD_ON | KEYPAD_CTRL_SOFT_NRST;
		ctrl &= ~KEYPAD_CTRL_SOFTMODEN;
		ret = twl4030_i2c_write_u8(TWL4030_CHIP_KEYPAD, ctrl, KEYPAD_KEYP_CTRL_REG);
	}
	return ret;
}

int twl4030_keypad_reset(void)
{
	int ret = 0;
	u8 ctrl;
	ret = twl4030_i2c_read_u8(TWL4030_CHIP_KEYPAD, &ctrl, KEYPAD_KEYP_CTRL_REG);
	if (!ret) {
		ctrl &= ~KEYPAD_CTRL_SOFT_NRST;
		ret = twl4030_i2c_write_u8(TWL4030_CHIP_KEYPAD, ctrl, KEYPAD_KEYP_CTRL_REG);
	}
	return ret;
}

int twl4030_keypad_keys_pressed(unsigned char *key1, unsigned char *key2)
{
	int ret = 0;
	u8 cb, c, rb, r;
	for (cb = 0; cb < 8; cb++) {
		c = 0xff & ~(1 << cb);
		twl4030_i2c_write_u8(TWL4030_CHIP_KEYPAD, c, KEYPAD_KBC_REG);
		twl4030_i2c_read_u8(TWL4030_CHIP_KEYPAD, &r, KEYPAD_KBR_REG);
		for (rb = 0; rb < 8; rb++) {
			if (!(r & (1 << rb))) {
				if (!ret)
					*key1 = cb << 3 | rb;
				else if (1 == ret)
					*key2 = cb << 3 | rb;
				ret++;
			}
		}
	}
	return ret;
}

#endif

/* USB */

#if (defined(CONFIG_TWL4030_USB) && (CONFIG_TWL4030_USB))

static int twl4030_usb_write(u8 address, u8 data)
{
	int ret = 0;
	ret = twl4030_i2c_write_u8(TWL4030_CHIP_USB, data, address);
	if (ret != 0)
		printf("TWL4030:USB:Write[0x%x] Error %d\n", address, ret);

	return ret;
}

static int twl4030_usb_read(u8 address)
{
	u8 data;
	int ret = 0;
	ret = twl4030_i2c_read_u8(TWL4030_CHIP_USB, &data, address);
	if (ret == 0)
		ret = data;
	else
		printf("TWL4030:USB:Read[0x%x] Error %d\n", address, ret);

	return ret;
}

static int twl4030_usb_set_bits(u8 reg, u8 bits)
{
	return twl4030_usb_write(reg + 1, bits);
}

static int twl4030_usb_clear_bits(u8 reg, u8 bits)
{
	return twl4030_usb_write(reg + 2, bits);
}

static void twl4030_usb_ldo_init(void)
{
	/* Enable writing to power configuration registers */
	twl4030_i2c_write_u8(TWL4030_CHIP_PM_MASTER, 0xC0, PM_MASTER_PROTECT_KEY);
	twl4030_i2c_write_u8(TWL4030_CHIP_PM_MASTER, 0x0C, PM_MASTER_PROTECT_KEY);

	/* put VUSB3V1 LDO in active state */
	twl4030_i2c_write_u8(TWL4030_CHIP_PM_RECEIVER, 0x00, PM_RECEIVER_VUSB_DEDICATED2);

	/* input to VUSB3V1 LDO is from VBAT, not VBUS */
	twl4030_i2c_write_u8(TWL4030_CHIP_PM_RECEIVER, 0x14, PM_RECEIVER_VUSB_DEDICATED1);

	/* turn on 3.1V regulator */
	twl4030_i2c_write_u8(TWL4030_CHIP_PM_RECEIVER, 0x20, PM_RECEIVER_VUSB3V1_DEV_GRP);
	twl4030_i2c_write_u8(TWL4030_CHIP_PM_RECEIVER, 0x00, PM_RECEIVER_VUSB3V1_TYPE);

	/* turn on 1.5V regulator */
	twl4030_i2c_write_u8(TWL4030_CHIP_PM_RECEIVER, 0x20, PM_RECEIVER_VUSB1V5_DEV_GRP);
	twl4030_i2c_write_u8(TWL4030_CHIP_PM_RECEIVER, 0x00, PM_RECEIVER_VUSB1V5_TYPE);

	/* turn on 1.8V regulator */
	twl4030_i2c_write_u8(TWL4030_CHIP_PM_RECEIVER, 0x20, PM_RECEIVER_VUSB1V8_DEV_GRP);
	twl4030_i2c_write_u8(TWL4030_CHIP_PM_RECEIVER, 0x00, PM_RECEIVER_VUSB1V8_TYPE);

	/* disable access to power configuration registers */
	twl4030_i2c_write_u8(TWL4030_CHIP_PM_MASTER, 0x00, PM_MASTER_PROTECT_KEY);
}

static void twl4030_phy_power(void)
{
	u8 pwr;

	pwr = twl4030_usb_read(USB_PHY_PWR_CTRL);
	pwr &= ~USB_PHYPWD;
	twl4030_usb_write(USB_PHY_PWR_CTRL, pwr);
	twl4030_usb_write(USB_PHY_CLK_CTRL, twl4030_usb_read(USB_PHY_CLK_CTRL) |
				(USB_CLOCKGATING_EN | USB_CLK32K_EN));
}

int twl4030_usb_init(void)
{
	unsigned long timeout;

	/* twl4030 ldo init */
	twl4030_usb_ldo_init();

	/* Enable the twl4030 phy */
	twl4030_phy_power();

	/* enable DPLL to access PHY registers over I2C */
	twl4030_usb_write(USB_PHY_CLK_CTRL, twl4030_usb_read(USB_PHY_CLK_CTRL) |
			  USB_REQ_PHY_DPLL_CLK);
	timeout = 1000 * 1000; /* 1 sec */
	while (!(twl4030_usb_read(USB_PHY_CLK_CTRL_STS) & USB_PHY_DPLL_CLK) &&
		0 < timeout) {
		udelay(10);
		timeout -= 10;
	}
	if (!(twl4030_usb_read(USB_PHY_CLK_CTRL_STS) & USB_PHY_DPLL_CLK)) {
		printf("Timeout setting T2 HSUSB PHY DPLL clock\n");
		return -1;
	}

	/* Enable ULPI mode */
	twl4030_usb_clear_bits(USB_IFC_CTRL, USB_CARKITMODE);
	twl4030_usb_set_bits(USB_POWER_CTRL, USB_OTG_ENAB);
	twl4030_usb_clear_bits(USB_FUNC_CTRL, USB_XCVRSELECT_MASK | USB_OPMODE_MASK);
	/* let ULPI control the DPLL clock */
	twl4030_usb_write(USB_PHY_CLK_CTRL, twl4030_usb_read(USB_PHY_CLK_CTRL) &
				~USB_REQ_PHY_DPLL_CLK);
	return 0;
}

#endif

/*
 * Power Reset
 */
void twl4030_power_reset_init(void)
{
#ifdef CONFIG_3430ZOOM2
	clear_n_set(TWL4030_CHIP_PM_MASTER, 0, SW_EVENTS_STOPON_PWRON,
		    PM_MASTER_P1_SW_EVENTS);
#endif
}

#ifdef CONFIG_CMD_VOLTAGE

/* Override the weakly defined voltage_info function */
void voltage_info (void)
{
	u8 vdd1_dev_grp, vdd1_type, vdd1_remap, vdd1_cfg, vdd1_misc_cfg;
	u8 vdd1_test1, vdd1_test2, vdd1_osc, vdd1_vsel, vdd1_vmode_cfg;
	u8 vdd1_vfloor, vdd1_vroof, vdd1_step;
	u8 vdd2_dev_grp, vdd2_type, vdd2_remap, vdd2_cfg, vdd2_misc_cfg;
	u8 vdd2_test1, vdd2_test2, vdd2_osc, vdd2_vsel, vdd2_vmode_cfg;
	u8 vdd2_vfloor, vdd2_vroof, vdd2_step;
	unsigned int vdd1 = 0;
	unsigned int vdd2 = 0;
	/* Units are in micro volts */
	unsigned int base = 600000;
	unsigned int scale = 12500;

	/* VDD1 */
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd1_dev_grp,
			    PM_RECEIVER_VDD1_DEV_GRP);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd1_type,
			    PM_RECEIVER_VDD1_TYPE);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd1_remap,
			    PM_RECEIVER_VDD1_REMAP);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd1_cfg,
			    PM_RECEIVER_VDD1_CFG);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd1_misc_cfg,
			    PM_RECEIVER_VDD1_MISC_CFG);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd1_test1,
			    PM_RECEIVER_VDD1_TEST1);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd1_test2,
			    PM_RECEIVER_VDD1_TEST2);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd1_osc,
			    PM_RECEIVER_VDD1_OSC);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd1_vsel,
			    PM_RECEIVER_VDD1_VSEL);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd1_vmode_cfg,
			    PM_RECEIVER_VDD1_VMODE_CFG);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd1_vfloor,
			    PM_RECEIVER_VDD1_VFLOOR);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd1_vroof,
			    PM_RECEIVER_VDD1_VROOF);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd1_step,
			    PM_RECEIVER_VDD1_STEP);

	printf("VDD1 Regs\n");
	printf("\tDEV_GRP   0x%x\n", vdd1_dev_grp);
	printf("\tTYPE      0x%x\n", vdd1_type);
	printf("\tREMAP     0x%x\n", vdd1_remap);
	printf("\tCFG       0x%x\n", vdd1_cfg);
	printf("\tMISC_CFG  0x%x\n", vdd1_misc_cfg);
	printf("\tTEST1     0x%x\n", vdd1_test1);
	printf("\tTEST2     0x%x\n", vdd1_test2);
	printf("\tOSC       0x%x\n", vdd1_osc);
	printf("\tVSEL      0x%x\n", vdd1_vsel);
	printf("\tVMODE_CFG 0x%x\n", vdd1_vmode_cfg);
	printf("\tVFLOOR    0x%x\n", vdd1_vfloor);
	printf("\tVROOF     0x%x\n", vdd1_vroof);
	printf("\tSTEP      0x%x\n", vdd1_step);

	/* VDD2 */
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd2_dev_grp,
			    PM_RECEIVER_VDD2_DEV_GRP);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd2_type,
			    PM_RECEIVER_VDD2_TYPE);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd2_remap,
			    PM_RECEIVER_VDD2_REMAP);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd2_cfg,
			    PM_RECEIVER_VDD2_CFG);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd2_misc_cfg,
			    PM_RECEIVER_VDD2_MISC_CFG);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd2_test1,
			    PM_RECEIVER_VDD2_TEST1);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd2_test2,
			    PM_RECEIVER_VDD2_TEST2);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd2_osc,
			    PM_RECEIVER_VDD2_OSC);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd2_vsel,
			    PM_RECEIVER_VDD2_VSEL);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd2_vmode_cfg,
			    PM_RECEIVER_VDD2_VMODE_CFG);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd2_vfloor,
			    PM_RECEIVER_VDD2_VFLOOR);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd2_vroof,
			    PM_RECEIVER_VDD2_VROOF);
	twl4030_i2c_read_u8(TWL4030_CHIP_PM_RECEIVER, &vdd2_step,
			    PM_RECEIVER_VDD2_STEP);

	printf("VDD2 Regs\n");
	printf("\tDEV_GRP   0x%x\n", vdd2_dev_grp);
	printf("\tTYPE      0x%x\n", vdd2_type);
	printf("\tREMAP     0x%x\n", vdd2_remap);
	printf("\tCFG       0x%x\n", vdd2_cfg);
	printf("\tMISC_CFG  0x%x\n", vdd2_misc_cfg);
	printf("\tTEST1     0x%x\n", vdd2_test1);
	printf("\tTEST2     0x%x\n", vdd2_test2);
	printf("\tOSC       0x%x\n", vdd2_osc);
	printf("\tVSEL      0x%x\n", vdd2_vsel);
	printf("\tVMODE_CFG 0x%x\n", vdd2_vmode_cfg);
	printf("\tVFLOOR    0x%x\n", vdd2_vfloor);
	printf("\tVROOF     0x%x\n", vdd2_vroof);
	printf("\tSTEP      0x%x\n", vdd2_step);

	/* Calculated the voltages */
	printf("\n");
	if (!(vdd1_cfg & 1))
	{
		/* voltage controled by vsel */
		vdd1 = scale * vdd1_vsel + base;
		printf("VDD1 calculated to be ");
		if (!(vdd1 % 1000)) {
			printf("%d mV\n", vdd1 / 1000);
		} else {
			printf("%d uV\n", vdd1);
		}
	} else {
		printf("VDD1 calculation unsupport for this mode\n");
	}

	if (!(vdd2_cfg & 1))
	{
		/* voltage controled by vsel */
		vdd2 = scale * vdd2_vsel + base;
		printf("VDD2 calculated to be ");
		if (!(vdd2 % 1000)) {
			printf("%d mV\n", vdd2 / 1000);
		} else {
			printf("%d uV\n", vdd2);
		}
	} else {
		printf("VDD2 calculation unsupport for this mode\n");
	}
}


#endif
