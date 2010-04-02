/*
 *  A module for creating rebootless custom image boot support.
 *
 *  Copyright (C) 2010 XVilka <xvilka at gmail.com>
 *
 *  Inspired by 2ndboot by dimich: http://hg.ezxdev.org/2ndboot/
 * 
 *  This file is part of 2ndboot-ng.
 *
 *  2ndboot-ng is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Foobar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *   along with 2ndboot-ng.  If not, see <http://www.gnu.org/licenses/>.
 * 
*/

#ifdef __PLAT_TI_OMAP3430__

#include "plat/omap3430.h"

#define MUSB_EP0_FIFOSIZE	64	/* This is non-configurable */

/*
 * Common USB registers
 */

#define MUSB_FADDR		0x00	/* 8-bit */
#define MUSB_POWER		0x01	/* 8-bit */

#define MUSB_INTRTX		0x02	/* 16-bit */
#define MUSB_INTRRX		0x04
#define MUSB_INTRTXE		0x06
#define MUSB_INTRRXE		0x08
#define MUSB_INTRUSB		0x0A	/* 8 bit */
#define MUSB_INTRUSBE		0x0B	/* 8 bit */
#define MUSB_FRAME		0x0C
#define MUSB_INDEX		0x0E	/* 8 bit */
#define MUSB_TESTMODE		0x0F	/* 8 bit */

/*
 * Additional Control Registers
 */

#define MUSB_DEVCTL		0x60	/* 8 bit */

/* These are always controlled through the INDEX register */
#define MUSB_TXFIFOSZ		0x62	/* 8-bit (see masks) */
#define MUSB_RXFIFOSZ		0x63	/* 8-bit (see masks) */
#define MUSB_TXFIFOADD		0x64	/* 16-bit offset shifted right 3 */
#define MUSB_RXFIFOADD		0x66	/* 16-bit offset shifted right 3 */

/* REVISIT: vctrl/vstatus: optional vendor utmi+phy register at 0x68 */
#define MUSB_HWVERS		0x6C	/* 8 bit */

#define MUSB_EPINFO		0x78	/* 8 bit */
#define MUSB_RAMINFO		0x79	/* 8 bit */
#define MUSB_LINKINFO		0x7a	/* 8 bit */
#define MUSB_VPLEN		0x7b	/* 8 bit */
#define MUSB_HS_EOF1		0x7c	/* 8 bit */
#define MUSB_FS_EOF1		0x7d	/* 8 bit */
#define MUSB_LS_EOF1		0x7e	/* 8 bit */

/* Offsets to endpoint registers */
#define MUSB_TXMAXP		0x00
#define MUSB_TXCSR		0x02
#define MUSB_CSR0		MUSB_TXCSR	/* Re-used for EP0 */
#define MUSB_RXMAXP		0x04
#define MUSB_RXCSR		0x06
#define MUSB_RXCOUNT		0x08
#define MUSB_COUNT0		MUSB_RXCOUNT	/* Re-used for EP0 */
#define MUSB_TXTYPE		0x0A
#define MUSB_TYPE0		MUSB_TXTYPE	/* Re-used for EP0 */
#define MUSB_TXINTERVAL		0x0B
#define MUSB_NAKLIMIT0		MUSB_TXINTERVAL	/* Re-used for EP0 */
#define MUSB_RXTYPE		0x0C
#define MUSB_RXINTERVAL		0x0D
#define MUSB_FIFOSIZE		0x0F
#define MUSB_CONFIGDATA		MUSB_FIFOSIZE	/* Re-used for EP0 */

/* Offsets to endpoint registers in indexed model (using INDEX register) */
#define MUSB_INDEXED_OFFSET(_epnum, _offset)	\
	(0x10 + (_offset))

/* Offsets to endpoint registers in flat models */
#define MUSB_FLAT_OFFSET(_epnum, _offset)	\
	(0x100 + (0x10*(_epnum)) + (_offset))

/* "bus control"/target registers, for host side multipoint (external hubs) */
#define MUSB_TXFUNCADDR		0x00
#define MUSB_TXHUBADDR		0x02
#define MUSB_TXHUBPORT		0x03

#define MUSB_RXFUNCADDR		0x04
#define MUSB_RXHUBADDR		0x06
#define MUSB_RXHUBPORT		0x07

#define MUSB_BUSCTL_OFFSET(_epnum, _offset) \
	(0x80 + (8*(_epnum)) + (_offset))

/* DMA Control Registers */
#define MUSB_DMA_INTR			0x200
#define MUSB_DMA_CNTL_CH(n)		(0x204 + ((n-1)*0x10))
#define MUSB_DMA_ADDR_CH(n)		(0x208 + ((n-1)*0x10))
#define MUSB_DMA_COUNT_CH(n)		(0x20C + ((n-1)*0x10))

/*
 * MUSB Register bits
 */

/* POWER */
#define MUSB_POWER_ISOUPDATE	0x80
#define MUSB_POWER_SOFTCONN	0x40
#define MUSB_POWER_HSENAB	0x20
#define MUSB_POWER_HSMODE	0x10
#define MUSB_POWER_RESET	0x08
#define MUSB_POWER_RESUME	0x04
#define MUSB_POWER_SUSPENDM	0x02
#define MUSB_POWER_ENSUSPEND	0x01

/* INTRUSB */
#define MUSB_INTR_SUSPEND	0x01
#define MUSB_INTR_RESUME	0x02
#define MUSB_INTR_RESET		0x04
#define MUSB_INTR_BABBLE	0x04
#define MUSB_INTR_SOF		0x08
#define MUSB_INTR_CONNECT	0x10
#define MUSB_INTR_DISCONNECT	0x20
#define MUSB_INTR_SESSREQ	0x40
#define MUSB_INTR_VBUSERROR	0x80	/* For SESSION end */

/* DEVCTL */
#define MUSB_DEVCTL_BDEVICE	0x80
#define MUSB_DEVCTL_FSDEV	0x40
#define MUSB_DEVCTL_LSDEV	0x20
#define MUSB_DEVCTL_VBUS	0x18
#define MUSB_DEVCTL_VBUS_SHIFT	3
#define MUSB_DEVCTL_HM		0x04
#define MUSB_DEVCTL_HR		0x02
#define MUSB_DEVCTL_SESSION	0x01

/* TESTMODE */
#define MUSB_TEST_FORCE_HOST	0x80
#define MUSB_TEST_FIFO_ACCESS	0x40
#define MUSB_TEST_FORCE_FS	0x20
#define MUSB_TEST_FORCE_HS	0x10
#define MUSB_TEST_PACKET	0x08
#define MUSB_TEST_K		0x04
#define MUSB_TEST_J		0x02
#define MUSB_TEST_SE0_NAK	0x01

/* Allocate for double-packet buffering (effectively doubles assigned _SIZE) */
#define MUSB_FIFOSZ_DPB	0x10
/* Allocation size (8, 16, 32, ... 4096) */
#define MUSB_FIFOSZ_SIZE	0x0f

/* CSR0 */
#define MUSB_CSR0_FLUSHFIFO	0x0100
#define MUSB_CSR0_TXPKTRDY	0x0002
#define MUSB_CSR0_RXPKTRDY	0x0001

/* CSR0 in Peripheral mode */
#define MUSB_CSR0_P_SVDSETUPEND	0x0080
#define MUSB_CSR0_P_SVDRXPKTRDY	0x0040
#define MUSB_CSR0_P_SENDSTALL	0x0020
#define MUSB_CSR0_P_SETUPEND	0x0010
#define MUSB_CSR0_P_DATAEND	0x0008
#define MUSB_CSR0_P_SENTSTALL	0x0004

/* CSR0 in Host mode */
#define MUSB_CSR0_H_DIS_PING		0x0800
#define MUSB_CSR0_H_WR_DATATOGGLE	0x0400	/* Set to allow setting: */
#define MUSB_CSR0_H_DATATOGGLE		0x0200	/* Data toggle control */
#define MUSB_CSR0_H_NAKTIMEOUT		0x0080
#define MUSB_CSR0_H_STATUSPKT		0x0040
#define MUSB_CSR0_H_REQPKT		0x0020
#define MUSB_CSR0_H_ERROR		0x0010
#define MUSB_CSR0_H_SETUPPKT		0x0008
#define MUSB_CSR0_H_RXSTALL		0x0004

/* CSR0 bits to avoid zeroing (write zero clears, write 1 ignored) */
#define MUSB_CSR0_P_WZC_BITS	\
	(MUSB_CSR0_P_SENTSTALL)
#define MUSB_CSR0_H_WZC_BITS	\
	(MUSB_CSR0_H_NAKTIMEOUT | MUSB_CSR0_H_RXSTALL \
	| MUSB_CSR0_RXPKTRDY)

/* TxType/RxType */
#define MUSB_TYPE_SPEED		0xc0
#define MUSB_TYPE_SPEED_SHIFT	6
#define MUSB_TYPE_PROTO		0x30	/* Implicitly zero for ep0 */
#define MUSB_TYPE_PROTO_SHIFT	4
#define MUSB_TYPE_REMOTE_END	0xf	/* Implicitly zero for ep0 */

/* CONFIGDATA */
#define MUSB_CONFIGDATA_MPRXE		0x80	/* Auto bulk pkt combining */
#define MUSB_CONFIGDATA_MPTXE		0x40	/* Auto bulk pkt splitting */
#define MUSB_CONFIGDATA_BIGENDIAN	0x20
#define MUSB_CONFIGDATA_HBRXE		0x10	/* HB-ISO for RX */
#define MUSB_CONFIGDATA_HBTXE		0x08	/* HB-ISO for TX */
#define MUSB_CONFIGDATA_DYNFIFO		0x04	/* Dynamic FIFO sizing */
#define MUSB_CONFIGDATA_SOFTCONE	0x02	/* SoftConnect */
#define MUSB_CONFIGDATA_UTMIDW		0x01	/* Data width 0/1 => 8/16bits */

/* TXCSR in Peripheral and Host mode */
#define MUSB_TXCSR_AUTOSET		0x8000
#define MUSB_TXCSR_MODE			0x2000
#define MUSB_TXCSR_DMAENAB		0x1000
#define MUSB_TXCSR_FRCDATATOG		0x0800
#define MUSB_TXCSR_DMAMODE		0x0400
#define MUSB_TXCSR_CLRDATATOG		0x0040
#define MUSB_TXCSR_FLUSHFIFO		0x0008
#define MUSB_TXCSR_FIFONOTEMPTY		0x0002
#define MUSB_TXCSR_TXPKTRDY		0x0001

/* TXCSR in Peripheral mode */
#define MUSB_TXCSR_P_ISO		0x4000
#define MUSB_TXCSR_P_INCOMPTX		0x0080
#define MUSB_TXCSR_P_SENTSTALL		0x0020
#define MUSB_TXCSR_P_SENDSTALL		0x0010
#define MUSB_TXCSR_P_UNDERRUN		0x0004

/* TXCSR in Host mode */
#define MUSB_TXCSR_H_WR_DATATOGGLE	0x0200
#define MUSB_TXCSR_H_DATATOGGLE		0x0100
#define MUSB_TXCSR_H_NAKTIMEOUT		0x0080
#define MUSB_TXCSR_H_RXSTALL		0x0020
#define MUSB_TXCSR_H_ERROR		0x0004

/* TXCSR bits to avoid zeroing (write zero clears, write 1 ignored) */
#define MUSB_TXCSR_P_WZC_BITS	\
	(MUSB_TXCSR_P_INCOMPTX | MUSB_TXCSR_P_SENTSTALL \
	| MUSB_TXCSR_P_UNDERRUN | MUSB_TXCSR_FIFONOTEMPTY)
#define MUSB_TXCSR_H_WZC_BITS	\
	(MUSB_TXCSR_H_NAKTIMEOUT | MUSB_TXCSR_H_RXSTALL \
	| MUSB_TXCSR_H_ERROR | MUSB_TXCSR_FIFONOTEMPTY)

/* RXCSR in Peripheral and Host mode */
#define MUSB_RXCSR_AUTOCLEAR		0x8000
#define MUSB_RXCSR_DMAENAB		0x2000
#define MUSB_RXCSR_DISNYET		0x1000
#define MUSB_RXCSR_PID_ERR		0x1000
#define MUSB_RXCSR_DMAMODE		0x0800
#define MUSB_RXCSR_INCOMPRX		0x0100
#define MUSB_RXCSR_CLRDATATOG		0x0080
#define MUSB_RXCSR_FLUSHFIFO		0x0010
#define MUSB_RXCSR_DATAERROR		0x0008
#define MUSB_RXCSR_FIFOFULL		0x0002
#define MUSB_RXCSR_RXPKTRDY		0x0001

/* RXCSR in Peripheral mode */
#define MUSB_RXCSR_P_ISO		0x4000
#define MUSB_RXCSR_P_SENTSTALL		0x0040
#define MUSB_RXCSR_P_SENDSTALL		0x0020
#define MUSB_RXCSR_P_OVERRUN		0x0004

/* RXCSR in Host mode */
#define MUSB_RXCSR_H_AUTOREQ		0x4000
#define MUSB_RXCSR_H_WR_DATATOGGLE	0x0400
#define MUSB_RXCSR_H_DATATOGGLE		0x0200
#define MUSB_RXCSR_H_RXSTALL		0x0040
#define MUSB_RXCSR_H_REQPKT		0x0020
#define MUSB_RXCSR_H_ERROR		0x0004

/* RXCSR bits to avoid zeroing (write zero clears, write 1 ignored) */
#define MUSB_RXCSR_P_WZC_BITS	\
	(MUSB_RXCSR_P_SENTSTALL | MUSB_RXCSR_P_OVERRUN \
	| MUSB_RXCSR_RXPKTRDY)
#define MUSB_RXCSR_H_WZC_BITS	\
	(MUSB_RXCSR_H_RXSTALL | MUSB_RXCSR_H_ERROR \
	| MUSB_RXCSR_DATAERROR | MUSB_RXCSR_RXPKTRDY)

/* HUBADDR */
#define MUSB_HUBADDR_MULTI_TT		0x80

/* DMA Control */
#define MUSB_DMA_CNTL_BUSRT_MODE_0      0x0000
#define MUSB_DMA_CNTL_BUSRT_MODE_1      0x0200
#define MUSB_DMA_CNTL_BUSRT_MODE_2      0x0400
#define MUSB_DMA_CNTL_BUSRT_MODE_3      0x0600
#define MUSB_DMA_CNTL_ERR		0x0100
#define MUSB_DMA_CNTL_END_POINT(n)	((n)<<4)
#define MUSB_DMA_CNTL_INTERRUPT_ENABLE	0x0008
#define MUSB_DMA_CNTL_MODE_0		0x0000
#define MUSB_DMA_CNTL_MODE_1		0x0004
#define MUSB_DMA_CNTL_WRITE		0x0000
#define MUSB_DMA_CNTL_READ		0x0002
#define MUSB_DMA_CNTL_ENABLE		0x0001

/* Tx/Rx fifo size */
#define MUSB_TXFIFOSZ_DPB		0x0010
#define MUSB_RXFIFOSZ_DPB		0x0010

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

#endif /* _OMAP3430_USB_H_ */

#ifdef __PLAT_FREESCALE_IMX31__

#include "plat/mxc91231.h"


#endif
