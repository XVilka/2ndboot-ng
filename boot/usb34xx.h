/*
 * (C) Copyright 2008 
 * Windriver <www.windriver.com>
 * Tom Rix <Tom.Rix@windriver.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR /PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
  */
#ifndef _OMAP34XX_USB_H_
#define _OMAP34XX_USB_H_

#include "omap3430.h"
#include "musb_regs.h"

#define OMAP34XX_USB_BASE	   (OMAP34XX_CORE_L4_IO_BASE + 0xAB000)
#define OMAP34XX_OTG_BASE	   (OMAP34XX_CORE_L4_IO_BASE + 0xAB400)

/* Remap musb */

#define OMAP34XX_USB_FADDR         (OMAP34XX_USB_BASE + MUSB_FADDR)
#define OMAP34XX_USB_POWER         (OMAP34XX_USB_BASE + MUSB_POWER)
#define OMAP34XX_USB_INTRTX        (OMAP34XX_USB_BASE + MUSB_INTRTX)
#define OMAP34XX_USB_INTRRX        (OMAP34XX_USB_BASE + MUSB_INTRRX)
#define OMAP34XX_USB_INTRTXE       (OMAP34XX_USB_BASE + MUSB_INTRTXE)
#define OMAP34XX_USB_INTRRXE       (OMAP34XX_USB_BASE + MUSB_INTRRXE)
#define OMAP34XX_USB_INTRUSB       (OMAP34XX_USB_BASE + MUSB_INTRUSB)
#define OMAP34XX_USB_INTRUSBE      (OMAP34XX_USB_BASE + MUSB_INTRUSBE)
#define OMAP34XX_USB_FRAME         (OMAP34XX_USB_BASE + MUSB_FRAME)
#define OMAP34XX_USB_INDEX         (OMAP34XX_USB_BASE + MUSB_INDEX)
#define OMAP34XX_USB_TESTMODE      (OMAP34XX_USB_BASE + MUSB_TESTMODE)

#define OMAP34XX_USB_EP(n)         (OMAP34XX_USB_BASE + 0x100 + 0x10*(n))
#define OMAP34XX_USB_TXMAXP(n)     (OMAP34XX_USB_EP(n) + MUSB_TXMAXP)
#define OMAP34XX_USB_TXCSR(n)      (OMAP34XX_USB_EP(n) + MUSB_TXCSR)
#define OMAP34XX_USB_RXMAXP(n)     (OMAP34XX_USB_EP(n) + MUSB_RXMAXP)
#define OMAP34XX_USB_RXCSR(n)      (OMAP34XX_USB_EP(n) + MUSB_RXCSR)
#define OMAP34XX_USB_RXCOUNT(n)    (OMAP34XX_USB_EP(n) + MUSB_RXCOUNT)
#define OMAP34XX_USB_TXTYPE(n)     (OMAP34XX_USB_EP(n) + MUSB_TXTYPE)
#define OMAP34XX_USB_TXINTERVAL(n) (OMAP34XX_USB_EP(n) + MUSB_TXINTERVAL)
#define OMAP34XX_USB_RXTYPE(n)     (OMAP34XX_USB_EP(n) + MUSB_RXTYPE)
#define OMAP34XX_USB_RXINTERVAL(n) (OMAP34XX_USB_EP(n) + MUSB_RXINTERVAL)
#define OMAP34XX_USB_FIFOSIZE(n)   (OMAP34XX_USB_EP(n) + MUSB_FIFOSIZE)

#define OMAP34XX_USB_TXFIFOSZ      (OMAP34XX_USB_BASE + MUSB_TXFIFOSZ)
#define OMAP34XX_USB_RXFIFOSZ      (OMAP34XX_USB_BASE + MUSB_RXFIFOSZ)
#define OMAP34XX_USB_TXFIFOADD     (OMAP34XX_USB_BASE + MUSB_TXFIFOADD)
#define OMAP34XX_USB_RXFIFOADD     (OMAP34XX_USB_BASE + MUSB_RXFIFOADD)

#define OMAP34XX_USB_CSR0          (OMAP34XX_USB_TXCSR(0))
#define OMAP34XX_USB_COUNT0        (OMAP34XX_USB_RXCOUNT(0))
#define OMAP34XX_USB_TYPE0         (OMAP34XX_USB_TXTYPE(0))
#define OMAP34XX_USB_NAKLIMIT0     (OMAP34XX_USB_TXINTERVAL(0))
#define OMAP34XX_USB_CONFIGDATA    (OMAP34XX_USB_FIFOSIZE(0))

#define OMAP34XX_USB_FIFO(n)       (OMAP34XX_USB_BASE + 0x20 + ((n) * 4))
#define OMAP34XX_USB_FIFO_0        (OMAP34XX_USB_FIFO(0))

#define OMAP34XX_USB_DEVCTL        (OMAP34XX_USB_BASE + MUSB_DEVCTL)


#define OMAP34XX_USB_INTRUSB_VBUSERR      (1 << 7)
#define OMAP34XX_USB_INTRUSB_SESSREQ      (1 << 6)
#define OMAP34XX_USB_INTRUSB_DISCON       (1 << 5)
#define OMAP34XX_USB_INTRUSB_CONN         (1 << 4)
#define OMAP34XX_USB_INTRUSB_SOF          (1 << 3)
#define OMAP34XX_USB_INTRUSB_RESET_BABBLE (1 << 2)
#define OMAP34XX_USB_INTRUSB_RESUME       (1 << 1)
#define OMAP34XX_USB_INTRUSB_SUSPEND      (1 << 0)
#define OMAP34XX_USB_INTRUSB_ALL           0xff

#define OMAP34XX_USB_INTRTX_EP_15         (1 << 15)
#define OMAP34XX_USB_INTRTX_EP_14         (1 << 14)
#define OMAP34XX_USB_INTRTX_EP_13         (1 << 13)
#define OMAP34XX_USB_INTRTX_EP_12         (1 << 12)
#define OMAP34XX_USB_INTRTX_EP_11         (1 << 11)
#define OMAP34XX_USB_INTRTX_EP_10         (1 << 10)
#define OMAP34XX_USB_INTRTX_EP_9          (1 << 9)
#define OMAP34XX_USB_INTRTX_EP_8          (1 << 8)
#define OMAP34XX_USB_INTRTX_EP_7          (1 << 7)
#define OMAP34XX_USB_INTRTX_EP_6          (1 << 6)
#define OMAP34XX_USB_INTRTX_EP_5          (1 << 5)
#define OMAP34XX_USB_INTRTX_EP_4          (1 << 4)
#define OMAP34XX_USB_INTRTX_EP_3          (1 << 3)
#define OMAP34XX_USB_INTRTX_EP_2          (1 << 2)
#define OMAP34XX_USB_INTRTX_EP_1          (1 << 1)
#define OMAP34XX_USB_INTRTX_EP_0          (1 << 0)
#define OMAP34XX_USB_INTRTX_EP_ALL         0xffff

#define OMAP34XX_USB_INTRRX_EP_15         (1 << 15)
#define OMAP34XX_USB_INTRRX_EP_14         (1 << 14)
#define OMAP34XX_USB_INTRRX_EP_13         (1 << 13)
#define OMAP34XX_USB_INTRRX_EP_12         (1 << 12)
#define OMAP34XX_USB_INTRRX_EP_11         (1 << 11)
#define OMAP34XX_USB_INTRRX_EP_10         (1 << 10)
#define OMAP34XX_USB_INTRRX_EP_9          (1 << 9)
#define OMAP34XX_USB_INTRRX_EP_8          (1 << 8)
#define OMAP34XX_USB_INTRRX_EP_7          (1 << 7)
#define OMAP34XX_USB_INTRRX_EP_6          (1 << 6)
#define OMAP34XX_USB_INTRRX_EP_5          (1 << 5)
#define OMAP34XX_USB_INTRRX_EP_4          (1 << 4)
#define OMAP34XX_USB_INTRRX_EP_3          (1 << 3)
#define OMAP34XX_USB_INTRRX_EP_2          (1 << 2)
#define OMAP34XX_USB_INTRRX_EP_1          (1 << 1)
/* 0 is reserved */
#define OMAP34XX_USB_INTRRX_EP_ALL         0xfffe

/* DMA Control Registers */
#define OMAP34XX_USB_DMA_INTR		(OMAP34XX_USB_BASE + MUSB_DMA_INTR)
#define OMAP34XX_USB_DMA_CNTL_CH(n)	(OMAP34XX_USB_BASE + MUSB_DMA_CNTL_CH(n))
#define OMAP34XX_USB_DMA_ADDR_CH(n)	(OMAP34XX_USB_BASE + MUSB_DMA_ADDR_CH(n))
#define OMAP34XX_USB_DMA_COUNT_CH(n)	(OMAP34XX_USB_BASE + MUSB_DMA_COUNT_CH(n))

/* OTG */
#define OMAP34XX_OTG_REVISION		(OMAP34XX_OTG_BASE + 0x00)
#define OMAP34XX_OTG_SYSCONFIG		(OMAP34XX_OTG_BASE + 0x04)
#define OMAP34XX_OTG_SYSSTATUS		(OMAP34XX_OTG_BASE + 0x08)
#define OMAP34XX_OTG_INTERFSEL		(OMAP34XX_OTG_BASE + 0x0C)
#define OMAP34XX_OTG_FORCESTDBY		(OMAP34XX_OTG_BASE + 0x14)

#define OMAP34XX_OTG_SYSCONFIG_SMART_STANDBY_MODE	0x2000
#define OMAP34XX_OTG_SYSCONFIG_NO_STANDBY_MODE		0x1000
#define OMAP34XX_OTG_SYSCONFIG_SMART_IDLE_MODE		0x0010
#define OMAP34XX_OTG_SYSCONFIG_NO_IDLE_MODE		0x0008
#define OMAP34XX_OTG_SYSCONFIG_ENABLEWAKEUP		0x0004
#define OMAP34XX_OTG_SYSCONFIG_SOFTRESET		0x0002
#define OMAP34XX_OTG_SYSCONFIG_AUTOIDLE			0x0001

#define OMAP34XX_OTG_SYSSTATUS_RESETDONE		0x0001

#define OMAP34XX_OTG_INTERFSEL_OMAP			0x0001

#define OMAP34XX_OTG_FORCESTDBY_STANDBY			0x0001

#endif /* _OMAP34XX_USB_H_ */
