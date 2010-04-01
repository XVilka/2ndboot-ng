/*
 * (C) Copyright 2006
 * Texas Instruments, <www.ti.com>
 * Richard Woodruff <r-woodruff2@ti.com>
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
#include <asm/arch/cpu.h>
#include <asm/io.h>
#include <asm/arch/bits.h>
#include <asm/arch/clocks.h>
#include <asm/arch/mem.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/sys_info.h>
#include <asm/arch/rev.h>
#include <environment.h>
#include <command.h>

/* Used to index into DPLL parameter tables */
struct dpll_param {
        unsigned int m;
        unsigned int n;
        unsigned int fsel;
        unsigned int m2;
};

#ifdef CONFIG_OMAP36XX

struct dpll_per_param {
        unsigned int sys_clk;
        unsigned int m;
        unsigned int n;
        unsigned int clkin;
        unsigned int sd;
        unsigned int dco;
        unsigned int m2;
        unsigned int m3;
        unsigned int m4;
        unsigned int m5;
        unsigned int m6;
        unsigned int m2div;
};
typedef struct dpll_per_param dpll_per_param;
#define MAX_SIL_INDEX	1

#else

typedef struct dpll_param dpll_per_param;
#define MAX_SIL_INDEX	3

#endif /* CONFIG_OMAP36XX */

typedef struct dpll_param dpll_param;

/* Following functions are exported from lowlevel_init.S */

extern dpll_param * get_mpu_dpll_param(void);
extern dpll_param * get_iva_dpll_param(void);
extern dpll_param * get_core_dpll_param(void);
extern dpll_param * get_per_dpll_param(void);

/*************************************************************
 * get_sys_clk_speed - determine reference oscillator speed
 *  based on known 32kHz clock and gptimer.
 *************************************************************/
u32 get_osc_clk_speed(void)
{
	u32 start, cstart, cend, cdiff, val;
	
	val = __raw_readl(PRM_CLKSRC_CTRL);
	/* If SYS_CLK is being divided by 2, remove for now */
	val = (val & (~BIT7)) | BIT6;
	__raw_writel(val, PRM_CLKSRC_CTRL);

	/* enable timer2 */
	val = __raw_readl(CM_CLKSEL_WKUP) | BIT0;
	__raw_writel(val, CM_CLKSEL_WKUP);	/* select sys_clk for GPT1 */

	/* Enable I and F Clocks for GPT1 */
	val = __raw_readl(CM_ICLKEN_WKUP) | BIT0 | BIT2;
	__raw_writel(val, CM_ICLKEN_WKUP);
	val = __raw_readl(CM_FCLKEN_WKUP) | BIT0;
	__raw_writel(val, CM_FCLKEN_WKUP);

	__raw_writel(0, OMAP34XX_GPT1 + TLDR);	/* start counting at 0 */
	__raw_writel(GPT_EN, OMAP34XX_GPT1 + TCLR);     /* enable clock */
	/* enable 32kHz source *//* enabled out of reset */
	/* determine sys_clk via gauging */

	start = 20 + __raw_readl(S32K_CR);	/* start time in 20 cycles */
	while (__raw_readl(S32K_CR) < start);	/* dead loop till start time */
	cstart = __raw_readl(OMAP34XX_GPT1 + TCRR);	/* get start sys_clk count */
	while (__raw_readl(S32K_CR) < (start + 20));	/* wait for 40 cycles */
	cend = __raw_readl(OMAP34XX_GPT1 + TCRR);	/* get end sys_clk count */
	cdiff = cend - cstart;				/* get elapsed ticks */

	/* based on number of ticks assign speed */
	if (cdiff > 19000)
		return (S38_4M);
	else if (cdiff > 15200)
		return (S26M);
	else if (cdiff > 13000)
		return (S24M);
	else if (cdiff > 9000)
		return (S19_2M);
	else if (cdiff > 7600)
		return (S13M);
	else
		return (S12M);
}

/******************************************************************************
 * get_sys_clkin_sel() - returns the sys_clkin_sel field value based on 
 *   -- input oscillator clock frequency.
 *   
 *****************************************************************************/
void get_sys_clkin_sel(u32 osc_clk, u32 *sys_clkin_sel)
{
	if(osc_clk == S38_4M)
		*sys_clkin_sel=  4;
	else if(osc_clk == S26M)
		*sys_clkin_sel = 3;
	else if(osc_clk == S19_2M)
		*sys_clkin_sel = 2;
	else if(osc_clk == S13M)
		*sys_clkin_sel = 1;
	else if(osc_clk == S12M)
		*sys_clkin_sel = 0;
}

static int get_silindex(void)
{
	int sil_index = 0;

	/*
	 * The DPLL tables are defined according to sysclk value and
	 * silicon revision. The clk_index value will be used to get
	 * the values for that input sysclk from the DPLL param table
	 * and sil_index will get the values for that SysClk for the
	 * appropriate silicon rev.
	 */
#ifdef CONFIG_OMAP36XX
	sil_index = 0;
#else
	if (cpu_is_3410()) {
		sil_index = 2;
	} else {
		if (get_cpu_rev() == CPU_3XX_ES10)
			sil_index = 0;
		else if (get_cpu_rev() >= CPU_3XX_ES20)
			sil_index = 1;
	}
#endif
	return sil_index;
}

static dpll_param *_get_core_dpll(int clk_index, int sil_index)
{
	dpll_param *ret = (dpll_param *)get_core_dpll_param();
	ret += (MAX_SIL_INDEX * clk_index) + sil_index;
	return ret;
}

static dpll_param *_get_mpu_dpll(int clk_index, int sil_index)
{
	dpll_param *ret = (dpll_param *)get_mpu_dpll_param();
	ret += (MAX_SIL_INDEX * clk_index) + sil_index;
	return ret;
}

static dpll_per_param *_get_per_dpll(int clk_index)
{
	dpll_per_param *ret = (dpll_per_param *)get_per_dpll_param();
	ret += clk_index;
	return ret;
}

static dpll_param *_get_iva_dpll(int clk_index, int sil_index)
{
	dpll_param *ret = (dpll_param *)get_iva_dpll_param();
	ret += (MAX_SIL_INDEX * clk_index) + sil_index;
	return ret;
}

#ifdef CONFIG_OMAP36XX

#define PER_M_BITS 12
#define PER_M2_BITS 5
#define PER_M3_BITS 6
#define PER_M4_BITS 6
#define PER_M5_BITS 6
#define PER_M6_BITS 6

static void per_dpll_init_36XX(int clk_index)
{
	dpll_per_param *per;

	per = _get_per_dpll(clk_index);

	sr32(CM_CLKEN_PLL, 16, 3, PLL_STOP);
	wait_on_value(BIT1, 0, CM_IDLEST_CKGEN, LDELAY);

	sr32(CM_CLKSEL2_PLL,  8, PER_M_BITS, per->m);
	sr32(CM_CLKSEL2_PLL,  0, 7, per->n);
	sr32(PRM_CLKSRC_CTRL, 8, 1, per->clkin);
	sr32(CM_CLKSEL2_PLL, 24, 7, per->sd);
	sr32(CM_CLKSEL2_PLL, 21, 3, per->dco);
	sr32(CM_CLKSEL3_PLL,  0, PER_M2_BITS, per->m2);
	sr32(CM_CLKSEL_DSS,   8, PER_M3_BITS, per->m3);
	sr32(CM_CLKSEL_DSS,   0, PER_M4_BITS, per->m4);
	sr32(CM_CLKSEL_CAM,   0, PER_M5_BITS, per->m5);
	sr32(CM_CLKSEL1_EMU, 24, PER_M6_BITS, per->m6);
	sr32(CM_CLKSEL_CORE, 12, 2, per->m2div);

	sr32(CM_CLKEN_PLL, 16, 3, PLL_LOCK);	/* lock mode */
	wait_on_value(BIT1, 2, CM_IDLEST_CKGEN, LDELAY);
}

static void iva_dpll_init_36XX(int clk_index, int sil_index)
{
	dpll_param *iva;

	iva = _get_iva_dpll(clk_index, sil_index);

	sr32(CM_CLKEN_PLL_IVA2, 0, 3, PLL_STOP);
	wait_on_value(BIT0, 0, CM_IDLEST_PLL_IVA2, LDELAY);

	/* IVA bypass clock set to CORECLK/2=(100) at OPP1 */
	sr32(CM_CLKSEL1_PLL_IVA2, 19, 3, 2);	/* set CLK_SRC */
	sr32(CM_CLKSEL1_PLL_IVA2, 8, 11, iva->m);
	sr32(CM_CLKSEL1_PLL_IVA2, 0,  7, iva->n);
	sr32(CM_CLKSEL2_PLL_IVA2, 0,  5, iva->m2);

	sr32(CM_CLKEN_PLL_IVA2, 0, 3, PLL_LOCK);	/* lock mode */
	wait_on_value(BIT0, 1, CM_IDLEST_PLL_IVA2, LDELAY);
}

static void mpu_dpll_init_36XX(int clk_index, int sil_index)
{
	dpll_param *mpu;

	mpu = _get_mpu_dpll(clk_index, sil_index);

	/* MPU DPLL (unlocked already) */
	sr32(CM_CLKSEL1_PLL_MPU, 8, 11, mpu->m);
	sr32(CM_CLKSEL1_PLL_MPU, 0,  7, mpu->n);
	sr32(CM_CLKSEL2_PLL_MPU, 0,  5, mpu->m2);

	sr32(CM_CLKEN_PLL_MPU, 0, 3, PLL_LOCK); /* lock mode */
	wait_on_value(BIT0, 1, CM_IDLEST_PLL_MPU, LDELAY);
}

#else /* 34xx */

#define PER_M_BITS 11
#define PER_M2_BITS 5
#define PER_M3_BITS 5
#define PER_M4_BITS 5
#define PER_M5_BITS 5
#define PER_M6_BITS 5

static void per_dpll_init_34XX(int clk_index)
{
	dpll_per_param *dpll_param_p;

	sr32(CM_CLKEN_PLL, 16, 3, PLL_STOP);
	wait_on_value(BIT1, 0, CM_IDLEST_CKGEN, LDELAY);

	/* Getting the base address to PER  DPLL param table*/
		/* Set N */
	dpll_param_p = (dpll_param *)get_per_dpll_param();
	/* Moving it to the right sysclk base */
	dpll_param_p = dpll_param_p + clk_index;
	/* Errata 1.50 Workaround for 3430 ES1.0 only */
	/* If using default divisors, write default divisor + 1
	   and then the actual divisor value */
	/* Need to change it to silicon and revisino check */
	if(1) {
		sr32(CM_CLKSEL1_EMU, 24, 5, PER_M6X2 + 1);	/* set M6 */
		sr32(CM_CLKSEL1_EMU, 24, 5, PER_M6X2);		/* set M6 */
		sr32(CM_CLKSEL_CAM, 0, 5, PER_M5X2 + 1);	/* set M5 */
		sr32(CM_CLKSEL_CAM, 0, 5, PER_M5X2);		/* set M5 */
		sr32(CM_CLKSEL_DSS, 0, 5, PER_M4X2 + 1);	/* set M4 */
		sr32(CM_CLKSEL_DSS, 0, 5, PER_M4X2);		/* set M4 */
		sr32(CM_CLKSEL_DSS, 8, 5, PER_M3X2 + 1);	/* set M3 */
		sr32(CM_CLKSEL_DSS, 8, 5, PER_M3X2);		/* set M3 */
		sr32(CM_CLKSEL3_PLL, 0, 5, dpll_param_p->m2 + 1);/* set M2 */
		sr32(CM_CLKSEL3_PLL, 0, 5, dpll_param_p->m2);	/* set M2 */
	}
	else {
		sr32(CM_CLKSEL1_EMU, 24, 5, PER_M6X2);	/* set M6 */
		sr32(CM_CLKSEL_CAM, 0, 5, PER_M5X2);	/* set M5 */
		sr32(CM_CLKSEL_DSS, 0, 5, PER_M4X2);	/* set M4 */
		sr32(CM_CLKSEL_DSS, 8, 5, PER_M3X2);	/* set M3 */
		sr32(CM_CLKSEL3_PLL, 0, 5, dpll_param_p->m2);	/* set M2 */
	}
	sr32(CM_CLKSEL2_PLL, 8, 11, dpll_param_p->m);	/* set m */
	sr32(CM_CLKSEL2_PLL, 0, 7, dpll_param_p->n);	/* set n */
	sr32(CM_CLKEN_PLL, 20, 4, dpll_param_p->fsel);/* FREQSEL */
	sr32(CM_CLKEN_PLL, 16, 3, PLL_LOCK);	/* lock mode */
	wait_on_value(BIT1, 2, CM_IDLEST_CKGEN, LDELAY);
}

static void iva_dpll_init_34XX(int clk_index, int sil_index)
{
	dpll_param *dpll_param_p;

	/* Getting the base address to IVA DPLL param table*/
	dpll_param_p = (dpll_param *)get_iva_dpll_param();
	/* Moving it to the right sysclk and ES rev base */
	dpll_param_p = dpll_param_p + MAX_SIL_INDEX*clk_index + sil_index;
	/* IVA DPLL (set to 12*20=240MHz) */
	sr32(CM_CLKEN_PLL_IVA2, 0, 3, PLL_STOP);
	wait_on_value(BIT0, 0, CM_IDLEST_PLL_IVA2, LDELAY);
	sr32(CM_CLKSEL2_PLL_IVA2, 0, 5, dpll_param_p->m2);	/* set M2 */

	/* IVA bypass clock set to CORECLK/4=(83Mhz) at OPP1 */
	sr32(CM_CLKSEL1_PLL_IVA2, 19, 3, 4);		/* set CLK_SRC */
	sr32(CM_CLKSEL1_PLL_IVA2, 8, 11, dpll_param_p->m);	/* set M */
	sr32(CM_CLKSEL1_PLL_IVA2, 0, 7, dpll_param_p->n);	/* set N */
	sr32(CM_CLKEN_PLL_IVA2, 4, 4, dpll_param_p->fsel);	/* FREQSEL */
	sr32(CM_CLKEN_PLL_IVA2, 0, 3, PLL_LOCK);	/* lock mode */
	wait_on_value(BIT0, 1, CM_IDLEST_PLL_IVA2, LDELAY);
}

static void mpu_dpll_init_34XX(int clk_index, int sil_index)
{
	dpll_param *dpll_param_p;

	/* Getting the base address to MPU DPLL param table*/
	dpll_param_p = (dpll_param *)get_mpu_dpll_param();
	/* Moving it to the right sysclk and ES rev base */
	dpll_param_p = dpll_param_p + MAX_SIL_INDEX*clk_index + sil_index;
	/* MPU DPLL (unlocked already) */
	sr32(CM_CLKSEL2_PLL_MPU, 0, 5, dpll_param_p->m2);	/* Set M2 */
	sr32(CM_CLKSEL1_PLL_MPU, 8, 11, dpll_param_p->m);	/* Set M */
	sr32(CM_CLKSEL1_PLL_MPU, 0, 7, dpll_param_p->n);	/* Set N */
	sr32(CM_CLKEN_PLL_MPU, 4, 4, dpll_param_p->fsel);	/* FREQSEL */
	sr32(CM_CLKEN_PLL_MPU, 0, 3, PLL_LOCK); /* lock mode */
	wait_on_value(BIT0, 1, CM_IDLEST_PLL_MPU, LDELAY);
}

#endif


/******************************************************************************
 * prcm_init() - inits clocks for PRCM as defined in clocks.h
 *   -- called from SRAM, or Flash (using temp SRAM stack).
 *****************************************************************************/
void prcm_init(void)
{
	void (*f_lock_pll) (u32, u32, u32, u32);
	int xip_safe, p0, p1, p2, p3;
	u32 osc_clk=0, sys_clkin_sel;
	extern void *_end_vect, *_start;
	u32 clk_index, sil_index=1;
	dpll_param *dpll_param_p;

	f_lock_pll =
	    (void *)((u32) & _end_vect - (u32) & _start + SRAM_VECT_CODE);

	xip_safe = running_in_sram();
#ifdef CONFIG_3430VIRTIO
	xip_safe = 1;
#endif
	/* Gauge the input clock speed and find out the sys_clkin_sel
	 * value corresponding to the input clock.
	 */
	osc_clk = get_osc_clk_speed();
	get_sys_clkin_sel(osc_clk, &sys_clkin_sel);
	
	sr32(PRM_CLKSEL, 0, 3, sys_clkin_sel); /* set input crystal speed */

	sr32(PRM_CLKSRC_CTRL, 6, 2, 1);/* input clock divider */
	clk_index = sys_clkin_sel;

	sr32(PRM_CLKSRC_CTRL, 0, 2, 0);/* Bypass mode: T2 inputs a square clock */

	sil_index = get_silindex();

	/* Unlock MPU DPLL (slows things down, and needed later) */
	sr32(CM_CLKEN_PLL_MPU, 0, 3, PLL_LOW_POWER_BYPASS);
	wait_on_value(BIT0, 0, CM_IDLEST_PLL_MPU, LDELAY);

	/* Getting the base address of Core DPLL param table*/
	dpll_param_p = (dpll_param *)get_core_dpll_param();
	/* Moving it to the right sysclk and ES rev base */
	dpll_param_p = dpll_param_p + MAX_SIL_INDEX*clk_index + sil_index;
	if(xip_safe){
		/* CORE DPLL */
		/* sr32(CM_CLKSEL2_EMU) set override to work when asleep */
		sr32(CM_CLKEN_PLL, 0, 3, PLL_FAST_RELOCK_BYPASS);
		wait_on_value(BIT0, 0, CM_IDLEST_CKGEN, LDELAY);
		/* For 3430 ES1.0 Errata 1.50, default value directly doesnt
		   work. write another value and then default value. */
		sr32(CM_CLKSEL1_EMU, 16, 5, CORE_M3X2 + 1);	/* m3x2 */
		sr32(CM_CLKSEL1_EMU, 16, 5, CORE_M3X2);		/* m3x2 */
		sr32(CM_CLKSEL1_PLL, 27, 2, dpll_param_p->m2);	/* Set M2 */
		sr32(CM_CLKSEL1_PLL, 16, 11, dpll_param_p->m);	/* Set M */
		sr32(CM_CLKSEL1_PLL, 8, 7, dpll_param_p->n);	/* Set N */
		sr32(CM_CLKSEL1_PLL, 6, 1, 0);			/* 96M Src */
		sr32(CM_CLKSEL_CORE, 8, 4, CORE_SSI_DIV);	/* ssi */
		sr32(CM_CLKSEL_CORE, 4, 2, CORE_FUSB_DIV);	/* fsusb ES1 only */
		sr32(CM_CLKSEL_CORE, 2, 2, CORE_L4_DIV);	/* l4 */
		sr32(CM_CLKSEL_CORE, 0, 2, CORE_L3_DIV);	/* l3 */
		sr32(CM_CLKSEL_GFX, 0, 3, GFX_DIV);		/* gfx */
		sr32(CM_CLKSEL_WKUP, 1, 2, WKUP_RSM);		/* reset mgr */
#ifndef CONFIG_OMAP36XX
		sr32(CM_CLKEN_PLL, 4, 4, dpll_param_p->fsel);	/* FREQSEL */
#endif
		sr32(CM_CLKEN_PLL, 0, 3, PLL_LOCK);		/* lock mode */
		wait_on_value(BIT0, 1, CM_IDLEST_CKGEN, LDELAY);
	} else if(running_in_flash()){
		/* if running from flash, jump to small relocated code area in SRAM.*/
		p0 = __raw_readl(CM_CLKEN_PLL);
		sr32((u32)&p0, 0, 3, PLL_FAST_RELOCK_BYPASS);
#ifndef CONFIG_OMAP36XX
		sr32((u32)&p0, 4, 4, dpll_param_p->fsel);	/* FREQSEL */
#endif

		p1 = __raw_readl(CM_CLKSEL1_PLL);
		sr32((u32)&p1, 27, 2, dpll_param_p->m2);	/* Set M2 */
		sr32((u32)&p1, 16, 11, dpll_param_p->m);	/* Set M */
		sr32((u32)&p1, 8, 7, dpll_param_p->n);		/* Set N */
		sr32((u32)&p1, 6, 1, 0);		/* set source for 96M */
		p2 = __raw_readl(CM_CLKSEL_CORE);
		sr32((u32)&p2, 8, 4, CORE_SSI_DIV);	/* ssi */
		sr32((u32)&p2, 4, 2, CORE_FUSB_DIV);	/* fsusb ES1 only*/
		sr32((u32)&p2, 2, 2, CORE_L4_DIV);	/* l4 */
		sr32((u32)&p2, 0, 2, CORE_L3_DIV);	/* l3 */

		p3 = CM_IDLEST_CKGEN;

		(*f_lock_pll) (p0, p1, p2, p3);
	}

#ifdef CONFIG_OMAP36XX
	per_dpll_init_36XX(clk_index);
	iva_dpll_init_36XX(clk_index, sil_index);
	mpu_dpll_init_36XX(clk_index, sil_index);
#else
	per_dpll_init_34XX(clk_index);
	iva_dpll_init_34XX(clk_index, sil_index);
	mpu_dpll_init_34XX(clk_index, sil_index);
#endif

	/* Set up GPTimers to sys_clk source only */
 	sr32(CM_CLKSEL_PER, 0, 8, 0xff);
	sr32(CM_CLKSEL_WKUP, 0, 1, 1);

	sdelay(5000);
}

/*****************************************************************
 * Routine: peripheral_enable
 * Description: Enable the clks & power for perifs (GPT2, UART1,...)
 ******************************************************************/
void per_clocks_enable(void)
{
	/* Enable GP2 timer. */
	sr32(CM_CLKSEL_PER, 0, 1, 0x1); /* GPT2 = sys clk */
	sr32(CM_ICLKEN_PER, 3, 1, 0x1); /* ICKen GPT2 */
	sr32(CM_FCLKEN_PER, 3, 1, 0x1); /* FCKen GPT2 */

#ifdef CFG_NS16550
	/* Enable UART1 clocks */
	sr32(CM_FCLKEN1_CORE, 13, 1, 0x1);
	sr32(CM_ICLKEN1_CORE, 13, 1, 0x1);
	/* Enable UART3 clocks */
	sr32(CM_FCLKEN_PER, 11, 1, 0x1);
	sr32(CM_ICLKEN_PER, 11, 1, 0x1);
#endif

#ifdef CONFIG_3430ZOOM2
	/* Zoom2 uses GPIO to control LED's and to detect if
	   the debug board is present */
	/* GPIO2 */
	sr32(CM_FCLKEN_PER, CLKEN_PER_EN_GPIO2_BIT, 1, 1);
	sr32(CM_ICLKEN_PER, CLKEN_PER_EN_GPIO2_BIT, 1, 1);
	/* GPIO3 */
	sr32(CM_FCLKEN_PER, CLKEN_PER_EN_GPIO3_BIT, 1, 1);
	sr32(CM_ICLKEN_PER, CLKEN_PER_EN_GPIO3_BIT, 1, 1);
	/* GPIO5 */
	sr32(CM_FCLKEN_PER, CLKEN_PER_EN_GPIO5_BIT, 1, 1);
	sr32(CM_ICLKEN_PER, CLKEN_PER_EN_GPIO5_BIT, 1, 1);
	/* GPIO6 */
	sr32(CM_FCLKEN_PER, CLKEN_PER_EN_GPIO6_BIT, 1, 1);
	sr32(CM_ICLKEN_PER, CLKEN_PER_EN_GPIO6_BIT, 1, 1);
#endif

#ifdef CONFIG_FASTBOOT
	/* USB : EN_HSOTGUSB */
	sr32(CM_ICLKEN1_CORE, 4, 1, 1);
#endif

#ifdef CONFIG_DRIVER_OMAP34XX_I2C
	/* Turn on all 3 I2C clocks*/
	sr32(CM_FCLKEN1_CORE, 15, 3, 0x7);
	sr32(CM_ICLKEN1_CORE, 15, 3, 0x7); /* I2C1,2,3 = on */
#endif

#ifdef CONFIG_MMC
	sr32(CM_FCLKEN1_CORE, 24, 1, 1);
	sr32(CM_ICLKEN1_CORE, 24, 1, 1);
#endif /* CONFIG_MMC */

	/* Enable the ICLK for 32K Sync Timer as its used in udelay */
	sr32(CM_ICLKEN_WKUP,2, 1, 0x1);

//#define CLOCKS_ALL_ON	1
#ifdef CLOCKS_ALL_ON
	#define FCK_IVA2_ON	0x00000001
	#define FCK_CORE1_ON	0x03fffe29
	#define ICK_CORE1_ON	0x3ffffffb
	#define ICK_CORE2_ON	0x0000001f
	#define	FCK_WKUP_ON	0x000000e9
	#define ICK_WKUP_ON	0x0000003f
	#define FCK_DSS_ON	0x00000005 /* tv+dss1 (not dss2) */
	#define ICK_DSS_ON	0x00000001
	#define FCK_CAM_ON	0x00000001
	#define ICK_CAM_ON	0x00000001
	#define FCK_PER_ON	0x0003ffff
	#define ICK_PER_ON	0x0003ffff
	sr32(CM_FCLKEN_IVA2, 0, 32, FCK_IVA2_ON);
	sr32(CM_FCLKEN1_CORE, 0, 32, FCK_CORE1_ON);
	sr32(CM_ICLKEN1_CORE, 0, 32, ICK_CORE1_ON);
	sr32(CM_ICLKEN2_CORE, 0, 32, ICK_CORE2_ON);
	sr32(CM_FCLKEN_WKUP, 0, 32, FCK_WKUP_ON);
	sr32(CM_ICLKEN_WKUP, 0, 32, ICK_WKUP_ON);
	sr32(CM_FCLKEN_DSS, 0, 32, FCK_DSS_ON);
	sr32(CM_ICLKEN_DSS, 0, 32, ICK_DSS_ON);
	sr32(CM_FCLKEN_CAM, 0, 32, FCK_CAM_ON);
	sr32(CM_ICLKEN_CAM, 0, 32, ICK_CAM_ON);
	sr32(CM_FCLKEN_PER, 0, 32, FCK_PER_ON);
	sr32(CM_ICLKEN_PER, 0, 32, ICK_PER_ON);
#endif
	sdelay(1000);
}

/*
 * Clock Info
 */
#if defined(CONFIG_CMD_CLOCK) && defined(CONFIG_CMD_CLOCK_INFO_CPU)

static void print_dpll_param(dpll_param *r, char *s)
{
	printf("DPLL %s ", s);
	printf("m %d n %d fsel %d m2 %d\n", r->m, r->n, r->fsel, r->m2);
}

static void print_dpll_per_param(dpll_per_param *r, char *s)
{
	printf("DPLL %s ", s);
#ifdef CONFIG_OMAP36XX
	printf("sys clk %d m %d n %d clkin_div %d sd_div %d dco_sel %d"
	       "\n\tm2 %d m3 %d m4 %d m5 %d m6 %d m2div %d\n",
	       r->sys_clk, r->m, r->n, r->clkin, r->sd, r->dco,
	       r->m2, r->m3, r->m4, r->m5, r->m6, r->m2div);
#else
	printf("m %d n %d fsel %d m2 %d\n", r->m, r->n, r->fsel, r->m2);
#endif

}

void cpu_clock_info(void)
{
	u32 osc_clk, clk_index;
	int sil_index;
	dpll_param *core, *mpu, *iva;
	dpll_per_param *per;

	osc_clk = get_osc_clk_speed();
	get_sys_clkin_sel(osc_clk, &clk_index);
	sil_index = get_silindex();

	printf("OSC CLK %d\n", osc_clk);
	printf("Clock index %d Silicon Index %d\n", clk_index, sil_index);

	core = _get_core_dpll(clk_index, sil_index);
	print_dpll_param(core, "core params");

	per = _get_per_dpll(clk_index);
	print_dpll_per_param(per, "per params ");

	mpu = _get_mpu_dpll(clk_index, sil_index);
	print_dpll_param(mpu, "mpu params ");

	iva = _get_iva_dpll(clk_index, sil_index);
	print_dpll_param(iva, "iva params ");

	/* Now verify */
	{
		/* System clk */
		u32 sys_clk;
		u32 sys_clk_div;
		u32 sys_clk_calc;

		/* PER / DPLL 4 clk */
		u32 per_m, per_n;
		u32 per_m2;
		u32 clk_96m_calc, per_clk_calc;
		u32 per_m3;
		u32 per_m4;
		u32 per_m5;
		u32 per_m6;
#ifdef CONFIG_OMAP36XX
		u32 per_clk_div;
		u32 per_dco_sel;
		u32 per_sd_div;
		u32 per_m2_div;
#else
		u32 per_fsel;
#endif
		/* MPU clk */
		u32 mpu_m, mpu_n, mpu_m2;
#ifndef CONFIG_OMAP36XX
		u32 mpu_fsel;
#endif

		/* IVA clk */
		u32 iva_m, iva_n, iva_m2;
#ifndef CONFIG_OMAP36XX
		u32 iva_fsel;
#endif

		printf("Verifying from hardware registers..\n");

		/* Sys clk */
		sys_clk = readl(PRM_CLKSEL);
		sys_clk >>= 0;

		sys_clk &= ((1 << 3) - 1);

		sys_clk_calc = 0;

		sys_clk_div = readl(PRM_CLKSRC_CTRL);
		sys_clk_div >>= 6;
		sys_clk_div &= ((1 << 2) - 1);

		if (4 == sys_clk)
			sys_clk_calc = 38400000;
		else if (3 == sys_clk)
			sys_clk_calc = 26000000;
		else if (2 == sys_clk)
			sys_clk_calc = 19200000;
		else if (1 == sys_clk)
			sys_clk_calc = 13000000;
		else if (0 == sys_clk)
			sys_clk_calc = 12000000;

		if (sys_clk_div)
			sys_clk_calc /= sys_clk_div;
		else
			sys_clk_calc = 0;

		printf("sys_clk %d sys_clk_div %d\n", sys_clk, sys_clk_div);
		printf("calculated system clock %d\n", sys_clk_calc);

		per_clk_calc = sys_clk_calc;

		/* Per clk */
		per_m = readl(CM_CLKSEL2_PLL);
		per_m >>= 8;
		per_m &= ((1 << PER_M_BITS) - 1);

		per_n = readl(CM_CLKSEL2_PLL);
		per_n >>= 0;
		per_n &= ((1 << 7) - 1);

		printf("per m %d n %d ", per_m, per_n);

#ifndef CONFIG_OMAP36XX
		per_fsel = readl(CM_CLKEN_PLL);
		per_fsel >>= 20;
		per_fsel &= ((1 << 4) - 1);

		printf("fsel %d ", per_fsel);
#endif

		per_m2 = readl(CM_CLKSEL3_PLL);
		per_m2 >>= 0;
		per_m2 &= ((1 << PER_M2_BITS) - 1);

		per_m3 = readl(CM_CLKSEL_DSS);
		per_m3 >>= 8;
		per_m3 &= ((1 << PER_M3_BITS) - 1);

		per_m4 = readl(CM_CLKSEL_DSS);
		per_m4 >>= 0;
		per_m4 &= ((1 << PER_M4_BITS) - 1);

		per_m5 = readl(CM_CLKSEL_CAM);
		per_m5 >>= 0;
		per_m5 &= ((1 << PER_M5_BITS) - 1);

		per_m6 = readl(CM_CLKSEL1_EMU);
		per_m6 >>= 24;
		per_m6 &= ((1 << PER_M6_BITS) - 1);

#ifdef CONFIG_OMAP36XX
		per_clk_div = readl(PRM_CLKSRC_CTRL);
		per_clk_div >>= 8;
		per_clk_div &= 1;

		per_dco_sel = readl(CM_CLKSEL2_PLL);
		per_dco_sel >>= 21;
		per_dco_sel &= ((1 << 3) - 1);

		per_sd_div = readl(CM_CLKSEL2_PLL);
		per_sd_div >>= 24;
		per_sd_div &= ((1 << 7) - 1);

		/* Div by 6.5 */
		if (per_clk_div) {
			per_clk_calc *= 2;
			per_clk_calc /= ((per_n + 1) * 13);
		} else {
			per_clk_calc /= (per_n + 1);
		}

		/* M2 div */
		per_m2_div = readl(CM_CLKSEL_CORE);
		per_m2_div >>= 12;
		per_m2_div &= 3;

		per_m2 *= per_m2_div;

		printf("clkdiv %d dco_sel %d sd_div %d m2_div %d ",
		       per_clk_div, per_dco_sel, per_sd_div, per_m2_div);

#else
		per_clk_calc /= (per_n + 1);
#endif

		printf("m2 %d m3 %d m4 %d m5 %d m6 %d",
		       per_m2, per_m3, per_m4, per_m5, per_m6);

		per_clk_calc *= per_m;

		if (per_m2)
			clk_96m_calc = per_clk_calc / per_m2;
		else
			clk_96m_calc = 0;

		printf("\n");
		if (!(per_clk_calc % 1000000))
			printf("dpll4 base clk %d MHz\n",
			       per_clk_calc / 1000000);
		else if (!(per_clk_calc % 1000))
			printf("dpll4 base clk %d KHz\n", per_clk_calc / 1000);
		else
			printf("dpll4 base clk %d Hz\n", per_clk_calc);

		if (!(clk_96m_calc % 1000000))
			printf("\t96M clk %d MHz\n", clk_96m_calc / 1000000);
		else if (!(clk_96m_calc % 1000))
			printf("\t96M clk %d KHz\n", clk_96m_calc / 1000);
		else
			printf("\t96M clk %d Hz\n", clk_96m_calc);

		/* MPU */
		mpu_m = readl(CM_CLKSEL1_PLL_MPU);
		mpu_m >>= 8;
		mpu_m &= ((1 << 11) - 1);

		mpu_n = readl(CM_CLKSEL1_PLL_MPU);
		mpu_n >>= 0;
		mpu_n &= ((1 << 5) - 1);

		printf("mpu m %d n %d ", mpu_m, mpu_n);

#ifndef CONFIG_OMAP36XX
		mpu_fsel = readl(CM_CLKEN_PLL_MPU);
		mpu_fsel >>= 4;
		mpu_fsel &= ((1 << 4) - 1);

		printf("fsel %d ", mpu_fsel);
#endif

		mpu_m2 = readl(CM_CLKSEL2_PLL_MPU);
		mpu_m2 >>= 0;
		mpu_m2 &= ((1 << 5) - 1);

		printf("m2 %d\n", mpu_m2);

		/* IVA */
		iva_m = readl(CM_CLKSEL1_PLL_IVA2);
		iva_m >>= 8;
		iva_m &= ((1 << 11) - 1);

		iva_n = readl(CM_CLKSEL1_PLL_IVA2);
		iva_n >>= 0;
		iva_n &= ((1 << 7) - 1);

		printf("iva m %d n %d ", iva_m, iva_n);

#ifndef CONFIG_OMAP36XX
		iva_fsel = readl(CM_CLKEN_PLL_IVA2);
		iva_fsel >>= 4;
		iva_fsel &= ((1 << 4) - 1);

		printf("fsel %d ", iva_fsel);
#endif

		iva_m2 = readl(CM_CLKSEL2_PLL_IVA2);
		iva_m2 >>= 0;
		iva_m2 &= ((1 << 5) - 1);

		printf("m2 %d\n", iva_m2);
	}
}
#endif
