/*
 * Copyright 2006 Freescale Semiconductor, Inc.
 * Copyright 2006-2007 Motorola, Inc.
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
 *
 */

/* Date         Author          Comment
 * ===========  ==============  ==============================================
 * 06-Oct-2006  Motorola        Support for power management
 * 20-Oct-2006  Motorola        Added offset and mask for IPU PAD settings.
 * 05-Dec-2006  Motorola        Moved PLL defines from mxc_pm.c
 * 09-Jan-2007  Motorola        Implement MGPER fix for crash after WFI.
 * 12-Jan-2007  Motorola        Set clkctl_gp_ctrl[7] to enable JTAG debugging.
 * 16-Jan-2007  Motorola        Add DSM CONTROL1 definitions.
 */

#ifndef _ARCH_ARM_MACH_MXC91231_CRM_REGS_H_
#define _ARCH_ARM_MACH_MXC91231_CRM_REGS_H_
#include "mxc91231.h"

#define IO_ADDRESS(x) x

#define MXC_CRM_AP_BASE		(IO_ADDRESS(CRM_AP_BASE_ADDR))

#define MXC_CRM_COM_BASE	(IO_ADDRESS(CRM_COM_BASE_ADDR))

#define MXC_DSM_BASE		(IO_ADDRESS(DSM_BASE_ADDR))

#define MXC_PLL0_BASE		(IO_ADDRESS(PLL0_BASE_ADDR))

#define MXC_PLL1_BASE		(IO_ADDRESS(PLL1_BASE_ADDR))

#define MXC_PLL2_BASE		(IO_ADDRESS(PLL2_BASE_ADDR))

/* PLL Register Offsets */
#define MXC_PLL_DP_CTL                  0x00
#define MXC_PLL_DP_CONFIG               0x04
#define MXC_PLL_DP_OP                   0x08
#define MXC_PLL_DP_MFD                  0x0C
#define MXC_PLL_DP_MFN                  0x10
#define MXC_PLL_DP_HFS_OP               0x1C
#define MXC_PLL_DP_HFS_MFD              0x20
#define MXC_PLL_DP_HFS_MFN              0x24

/* PLL Register Bit definitions */
#define MXC_PLL_DP_CTL_DPDCK0_2_EN      0x1000
#define MXC_PLL_DP_CTL_ADE              0x800
#define MXC_PLL_DP_CTL_REF_CLK_DIV      0x400
#define MXC_PLL_DP_CTL_HFSM             0x80
#define MXC_PLL_DP_CTL_PRE              0x40
#define MXC_PLL_DP_CTL_UPEN             0x20
#define MXC_PLL_DP_CTL_RST              0x10
#define MXC_PLL_DP_CTL_RCP              0x8
#define MXC_PLL_DP_CTL_PLM              0x4
#define MXC_PLL_DP_CTL_BRM0             0x2
#define MXC_PLL_DP_CTL_LRF              0x1

#define MXC_PLL_DP_OP_MFI_OFFSET        4
#define MXC_PLL_DP_OP_MFI_MASK          0xF
#define MXC_PLL_DP_OP_PDF_OFFSET        0
#define MXC_PLL_DP_OP_PDF_MASK          0xF

#define MXC_PLL_DP_MFD_OFFSET           0
#define MXC_PLL_DP_MFD_MASK             0x7FFFFFF

#define MXC_PLL_DP_MFN_OFFSET           0
#define MXC_PLL_DP_MFN_MASK             0x7FFFFFF

/* CRM AP Register Offsets */
#define MXC_CRMAP_ASCSR                 IO_ADDRESS(CRM_AP_BASE_ADDR + 0x00)
#define MXC_CRMAP_ACDR                  IO_ADDRESS(CRM_AP_BASE_ADDR + 0x04)
#define MXC_CRMAP_ACDER1                IO_ADDRESS(CRM_AP_BASE_ADDR + 0x08)
#define MXC_CRMAP_ACDER2                IO_ADDRESS(CRM_AP_BASE_ADDR + 0x0C)
#define MXC_CRMAP_ACGCR                 IO_ADDRESS(CRM_AP_BASE_ADDR + 0x10)
#define MXC_CRMAP_ACCGCR                IO_ADDRESS(CRM_AP_BASE_ADDR + 0x14)
#define MXC_CRMAP_AMLPMRA               IO_ADDRESS(CRM_AP_BASE_ADDR + 0x18)
#define MXC_CRMAP_AMLPMRB               IO_ADDRESS(CRM_AP_BASE_ADDR + 0x1C)
#define MXC_CRMAP_AMLPMRC               IO_ADDRESS(CRM_AP_BASE_ADDR + 0x20)
#define MXC_CRMAP_AMLPMRD               IO_ADDRESS(CRM_AP_BASE_ADDR + 0x24)
#define MXC_CRMAP_AMLPMRE1              IO_ADDRESS(CRM_AP_BASE_ADDR + 0x28)
#define MXC_CRMAP_AMLPMRE2              IO_ADDRESS(CRM_AP_BASE_ADDR + 0x2C)
#define MXC_CRMAP_AMLPMRF               IO_ADDRESS(CRM_AP_BASE_ADDR + 0x30)
#define MXC_CRMAP_AMLPMRG               IO_ADDRESS(CRM_AP_BASE_ADDR + 0x34)
#define MXC_CRMAP_APGCR                 IO_ADDRESS(CRM_AP_BASE_ADDR + 0x38)
#define MXC_CRMAP_ACSR                  IO_ADDRESS(CRM_AP_BASE_ADDR + 0x3C)
#define MXC_CRMAP_ADCR                  IO_ADDRESS(CRM_AP_BASE_ADDR + 0x40)
#define MXC_CRMAP_ACR                   IO_ADDRESS(CRM_AP_BASE_ADDR + 0x44)
#define MXC_CRMAP_AMCR                  IO_ADDRESS(CRM_AP_BASE_ADDR + 0x48)
#define MXC_CRMAP_APCR                  IO_ADDRESS(CRM_AP_BASE_ADDR + 0x4C)
#define MXC_CRMAP_AMORA                 IO_ADDRESS(CRM_AP_BASE_ADDR + 0x50)
#define MXC_CRMAP_AMORB                 IO_ADDRESS(CRM_AP_BASE_ADDR + 0x54)
#define MXC_CRMAP_AGPR                  IO_ADDRESS(CRM_AP_BASE_ADDR + 0x58)
#define MXC_CRMAP_APRA                  IO_ADDRESS(CRM_AP_BASE_ADDR + 0x5C)
#define MXC_CRMAP_APRB                  IO_ADDRESS(CRM_AP_BASE_ADDR + 0x60)
#define MXC_CRMAP_APOR                  IO_ADDRESS(CRM_AP_BASE_ADDR + 0x64)
#define MXC_CRMAP_ADFMR                 IO_ADDRESS(CRM_AP_BASE_ADDR + 0x68)

/* CRM AP Register Bit definitions */
#define MXC_CRMAP_ASCSR_CRS                     0x10000
#define MXC_CRMAP_ASCSR_AP_PATDIV2_OFFSET       15
#define MXC_CRMAP_ASCSR_AP_PATREF_DIV2          0x8000
#define MXC_CRMAP_ASCSR_USBSEL_OFFSET           13
#define MXC_CRMAP_ASCSR_USBSEL_MASK             (0x3 << 13)
#define MXC_CRMAP_ASCSR_CSISEL_OFFSET           11
#define MXC_CRMAP_ASCSR_CSISEL_MASK             (0x3 << 11)
#define MXC_CRMAP_ASCSR_SSI2SEL_OFFSET          7
#define MXC_CRMAP_ASCSR_SSI2SEL_MASK            (0x3 << 7)
#define MXC_CRMAP_ASCSR_SSI1SEL_OFFSET          5
#define MXC_CRMAP_ASCSR_SSI1SEL_MASK            (0x3 << 5)
#define MXC_CRMAP_ASCSR_APSEL_OFFSET            3
#define MXC_CRMAP_ASCSR_APSEL_MASK              (0x3 << 3)
#define MXC_CRMAP_ASCSR_AP_PATDIV1_OFFSET       2
#define MXC_CRMAP_ASCSR_AP_PATREF_DIV1          0x4
#define MXC_CRMAP_ASCSR_APISEL                  0x1
#ifdef CONFIG_MOT_FEAT_PM 
#define ASCSR_APSEL_0                           0x00000008
#define ASCSR_APSEL_1                           0x00000010
#endif


#define MXC_CRMAP_ACDR_ARMDIV_OFFSET            8
#define MXC_CRMAP_ACDR_ARMDIV_MASK              (0xF << 8)
#define MXC_CRMAP_ACDR_AHBDIV_OFFSET            4
#define MXC_CRMAP_ACDR_AHBDIV_MASK              (0xF << 4)
#define MXC_CRMAP_ACDR_IPDIV_OFFSET             0
#define MXC_CRMAP_ACDR_IPDIV_MASK               0xF

#define MXC_CRMAP_ACDER1_CSIEN                  0x10000000
#define MXC_CRMAP_ACDER1_CSIEN_PASS2            0x40000000
#define MXC_CRMAP_ACDER1_CSIDIV_OFFSET          24
#define MXC_CRMAP_ACDER1_CSIDIV_MASK            (0xF << 24)
#define MXC_CRMAP_ACDER1_CSIDIV_MASK_PASS2      (0x3F << 24)
#define MXC_CRMAP_ACDER1_SSI2EN                 0x4000
#define MXC_CRMAP_ACDER1_SSI2DIV_OFFSET         8
#define MXC_CRMAP_ACDER1_SSI2DIV_MASK           (0x3F << 8)
#define MXC_CRMAP_ACDER1_SSI1EN                 0x40
#define MXC_CRMAP_ACDER1_SSI1DIV_OFFSET         0
#define MXC_CRMAP_ACDER1_SSI1DIV_MASK           0x3F

#define MXC_CRMAP_ACDER2_CRCT_CLK_DIV_OFF_PASS2 24
#define MXC_CRMAP_ACDER2_CRCT_CLK_DIV_MSK_PASS2 (0x7 << 24)
#define MXC_CRMAP_ACDER2_NFCEN                  0x00100000
#define MXC_CRMAP_ACDER2_NFCDIV_OFFSET          16
#define MXC_CRMAP_ACDER2_NFCDIV_MASK            (0xF << 16)
#define MXC_CRMAP_ACDER2_USBEN                  0x1000
#define MXC_CRMAP_ACDER2_USBDIV_OFFSET          8
#define MXC_CRMAP_ACDER2_USBDIV_MASK            (0xF << 8)
#define MXC_CRMAP_ACDER2_BAUD_ISEL_OFF_PASS2    5
#define MXC_CRMAP_ACDER2_BAUD_ISEL_MASK_PASS2   (0x3 << 5)
#define MXC_CRMAP_ACDER2_BAUDDIV_OFFSET         0
#define MXC_CRMAP_ACDER2_BAUDDIV_MASK           0xF

#define MXC_CRMAP_AMLPMRA_MLPMA7_OFFSET_PASS2   22
#define MXC_CRMAP_AMLPMRA_MLPMA7_MASK_PASS2     (0x7 << 22)
#define MXC_CRMAP_AMLPMRA_MLPMA6_OFFSET         19
#define MXC_CRMAP_AMLPMRA_MLPMA6_MASK           (0x7 << 19)
#define MXC_CRMAP_AMLPMRA_MLPMA4_OFFSET         12
#define MXC_CRMAP_AMLPMRA_MLPMA4_MASK           (0x7 << 12)
#define MXC_CRMAP_AMLPMRA_MLPMA3_OFFSET         9
#define MXC_CRMAP_AMLPMRA_MLPMA3_MASK           (0x7 << 9)
#define MXC_CRMAP_AMLPMRA_MLPMA2_OFFSET         6
#define MXC_CRMAP_AMLPMRA_MLPMA2_MASK           (0x7 << 6)
#define MXC_CRMAP_AMLPMRA_MLPMA1_OFFSET         3
#define MXC_CRMAP_AMLPMRA_MLPMA1_MASK           (0x7 << 3)

#define MXC_CRMAP_AMLPMRB_MLPMB0_OFFSET         0
#define MXC_CRMAP_AMLPMRB_MLPMB0_MASK           0x7

#define MXC_CRMAP_AMLPMRC_MLPMC9_OFFSET         28
#define MXC_CRMAP_AMLPMRC_MLPMC9_MASK           (0x7 << 28)
#define MXC_CRMAP_AMLPMRC_MLPMC7_OFFSET         22
#define MXC_CRMAP_AMLPMRC_MLPMC7_MASK           (0x7 << 22)
#define MXC_CRMAP_AMLPMRC_MLPMC5_OFFSET         16
#define MXC_CRMAP_AMLPMRC_MLPMC5_MASK           (0x7 << 16)
#define MXC_CRMAP_AMLPMRC_MLPMC4_OFFSET         12
#define MXC_CRMAP_AMLPMRC_MLPMC4_MASK           (0x7 << 12)
#define MXC_CRMAP_AMLPMRC_MLPMC3_OFFSET         9
#define MXC_CRMAP_AMLPMRC_MLPMC3_MASK           (0x7 << 9)
#define MXC_CRMAP_AMLPMRC_MLPMC2_OFFSET         6
#define MXC_CRMAP_AMLPMRC_MLPMC2_MASK           (0x7 << 6)
#define MXC_CRMAP_AMLPMRC_MLPMC1_OFFSET         3
#define MXC_CRMAP_AMLPMRC_MLPMC1_MASK           (0x7 << 3)
#define MXC_CRMAP_AMLPMRC_MLPMC0_OFFSET         0
#define MXC_CRMAP_AMLPMRC_MLPMC0_MASK           0x7

#define MXC_CRMAP_AMLPMRD_MLPMD7_OFFSET         22
#define MXC_CRMAP_AMLPMRD_MLPMD7_MASK           (0x7 << 22)
#define MXC_CRMAP_AMLPMRD_MLPMD4_OFFSET         12
#define MXC_CRMAP_AMLPMRD_MLPMD4_MASK           (0x7 << 12)
#define MXC_CRMAP_AMLPMRD_MLPMD3_OFFSET         9
#define MXC_CRMAP_AMLPMRD_MLPMD3_MASK           (0x7 << 9)
#define MXC_CRMAP_AMLPMRD_MLPMD2_OFFSET         6
#define MXC_CRMAP_AMLPMRD_MLPMD2_MASK           (0x7 << 6)
#define MXC_CRMAP_AMLPMRD_MLPMD0_OFFSET         0
#define MXC_CRMAP_AMLPMRD_MLPMD0_MASK           0x7

#define MXC_CRMAP_AMLPMRE1_MLPME9_OFFSET        28
#define MXC_CRMAP_AMLPMRE1_MLPME9_MASK          (0x7 << 28)
#define MXC_CRMAP_AMLPMRE1_MLPME8_OFFSET        25
#define MXC_CRMAP_AMLPMRE1_MLPME8_MASK          (0x7 << 25)
#define MXC_CRMAP_AMLPMRE1_MLPME7_OFFSET        22
#define MXC_CRMAP_AMLPMRE1_MLPME7_MASK          (0x7 << 22)
#define MXC_CRMAP_AMLPMRE1_MLPME6_OFFSET        19
#define MXC_CRMAP_AMLPMRE1_MLPME6_MASK          (0x7 << 19)
#define MXC_CRMAP_AMLPMRE1_MLPME5_OFFSET        16
#define MXC_CRMAP_AMLPMRE1_MLPME5_MASK          (0x7 << 16)
#define MXC_CRMAP_AMLPMRE1_MLPME4_OFFSET        12
#define MXC_CRMAP_AMLPMRE1_MLPME4_MASK          (0x7 << 12)
#define MXC_CRMAP_AMLPMRE1_MLPME3_OFFSET        9
#define MXC_CRMAP_AMLPMRE1_MLPME3_MASK          (0x7 << 9)
#define MXC_CRMAP_AMLPMRE1_MLPME2_OFFSET_PASS2  6
#define MXC_CRMAP_AMLPMRE1_MLPME2_MASK_PASS2    (0x7 << 6)
#define MXC_CRMAP_AMLPMRE1_MLPME1_OFFSET        3
#define MXC_CRMAP_AMLPMRE1_MLPME1_MASK          (0x7 << 3)
#define MXC_CRMAP_AMLPMRE1_MLPME0_OFFSET        0
#define MXC_CRMAP_AMLPMRE1_MLPME0_MASK          0x7

#define MXC_CRMAP_AMLPMRE2_MLPME0_OFFSET        0
#define MXC_CRMAP_AMLPMRE2_MLPME0_MASK          0x7

#define MXC_CRMAP_AMLPMRF_MLPMF6_OFFSET         19
#define MXC_CRMAP_AMLPMRF_MLPMF6_MASK           (0x7 << 19)
#define MXC_CRMAP_AMLPMRF_MLPMF5_OFFSET         16
#define MXC_CRMAP_AMLPMRF_MLPMF5_MASK           (0x7 << 16)
#define MXC_CRMAP_AMLPMRF_MLPMF3_OFFSET         9
#define MXC_CRMAP_AMLPMRF_MLPMF3_MASK           (0x7 << 9)
#define MXC_CRMAP_AMLPMRF_MLPMF2_OFFSET         6
#define MXC_CRMAP_AMLPMRF_MLPMF2_MASK           (0x7 << 6)
#define MXC_CRMAP_AMLPMRF_MLPMF1_OFFSET         3
#define MXC_CRMAP_AMLPMRF_MLPMF1_MASK           (0x7 << 3)
#define MXC_CRMAP_AMLPMRF_MLPMF0_OFFSET_PASS2   0
#define MXC_CRMAP_AMLPMRF_MLPMF0_MASK_PASS2     (0x7 << 0)

#define MXC_CRMAP_AMLPMRG_MLPMG9_OFFSET         28
#define MXC_CRMAP_AMLPMRG_MLPMG9_MASK           (0x7 << 28)
#define MXC_CRMAP_AMLPMRG_MLPMG7_OFFSET         22
#define MXC_CRMAP_AMLPMRG_MLPMG7_MASK           (0x7 << 22)
#define MXC_CRMAP_AMLPMRG_MLPMG6_OFFSET         19
#define MXC_CRMAP_AMLPMRG_MLPMG6_MASK           (0x7 << 19)
#define MXC_CRMAP_AMLPMRG_MLPMG5_OFFSET         16
#define MXC_CRMAP_AMLPMRG_MLPMG5_MASK           (0x7 << 16)
#define MXC_CRMAP_AMLPMRG_MLPMG4_OFFSET         12
#define MXC_CRMAP_AMLPMRG_MLPMG4_MASK           (0x7 << 12)
#define MXC_CRMAP_AMLPMRG_MLPMG3_OFFSET         9
#define MXC_CRMAP_AMLPMRG_MLPMG3_MASK           (0x7 << 9)
#define MXC_CRMAP_AMLPMRG_MLPMG2_OFFSET         6
#define MXC_CRMAP_AMLPMRG_MLPMG2_MASK           (0x7 << 6)
#define MXC_CRMAP_AMLPMRG_MLPMG1_OFFSET         3
#define MXC_CRMAP_AMLPMRG_MLPMG1_MASK           (0x7 << 3)
#define MXC_CRMAP_AMLPMRG_MLPMG0_OFFSET         0
#define MXC_CRMAP_AMLPMRG_MLPMG0_MASK           0x7

#if defined(CONFIG_MOT_FEAT_GPIO_API_ETM)
#define MXC_CRMAP_AGPR_IPUPAD_OFFSET            20
#define MXC_CRMAP_AGPR_IPUPAD_MASK              (0x7 << 20)
#endif

#define MXC_CRMAP_APRA_UART1EN                  0x1
#define MXC_CRMAP_APRA_UART2EN                  0x100
#define MXC_CRMAP_APRA_SAHARA_DIV2_CLK_EN_PASS2 0x4000
#define MXC_CRMAP_APRA_MQSPI_EN_PASS2           0x2000
#define MXC_CRMAP_APRA_UART3EN                  0x10000
#define MXC_CRMAP_APRA_SIMEN                    0x1000000
#define MXC_CRMAP_APRA_UART3DIV_OFFSET          17
#define MXC_CRMAP_APRA_UART3DIV_MASK            (0xF << 17)
#define MXC_CRMAP_APRA_EL1T_EN_PASS2            0x20000000

#define MXC_CRMAP_APRB_SDHC1EN                  0x1
#define MXC_CRMAP_APRB_SDHC1_DIV_OFFSET_PASS2   1
#define MXC_CRMAP_APRB_SDHC1_DIV_MASK_PASS2     (0xF << 1)
#define MXC_CRMAP_APRB_SDHC1_ISEL_OFFSET_PASS2  5
#define MXC_CRMAP_APRB_SDHC1_ISEL_MASK_PASS2    (0x7 << 5)
#define MXC_CRMAP_APRB_SDHC2EN                  0x100
#define MXC_CRMAP_APRB_SDHC2_DIV_OFFSET_PASS2   9
#define MXC_CRMAP_APRB_SDHC2_DIV_MASK_PASS2     (0xF << 9)
#define MXC_CRMAP_APRB_SDHC2_ISEL_OFFSET_PASS2  13
#define MXC_CRMAP_APRB_SDHC2_ISEL_MASK_PASS2    (0x7 << 13)

#define MXC_CRMAP_ACSR_ADS_OFFSET_PASS2         8
#define MXC_CRMAP_ACSR_ADS_PASS2                (0x1 << 8)
#define MXC_CRMAP_ACSR_ACS                      0x1

#define MXC_CRMAP_ADCR_LFDF_0                   (0x0 << 8)
#define MXC_CRMAP_ADCR_LFDF_2                   (0x1 << 8)
#define MXC_CRMAP_ADCR_LFDF_4                   (0x2 << 8)
#define MXC_CRMAP_ADCR_LFDF_8                   (0x3 << 8)
#define MXC_CRMAP_ADCR_LFDF_OFFSET              8
#define MXC_CRMAP_ADCR_LFDF_MASK                (0x3 << 8)
#define MXC_CRMAP_ADCR_ALT_PLL                  0x80
#define MXC_CRMAP_ADCR_DFS_DIVEN                0x20
#define MXC_CRMAP_ADCR_DIV_BYP                  0x2
#define MXC_CRMAP_ADCR_VSTAT			0x8
#define MXC_CRMAP_ADCR_TSTAT			0x10
#define MXC_CRMAP_ADCR_DVFS_VCTRL		0x10
#define MXC_CRMAP_ADCR_CLK_ON			0x40

#define MXC_CRMAP_ADFMR_FC_OFFSET               16
#define MXC_CRMAP_ADFMR_FC_MASK                 (0x1F << 16)
#define MXC_CRMAP_ADFMR_MF_OFFSET               1
#define MXC_CRMAP_ADFMR_MF_MASK                 (0x3FF << 1)
#define MXC_CRMAP_ADFMR_DFM_CLK_READY           0x1
#define MXC_CRMAP_ADFMR_DFM_PWR_DOWN            0x8000

#define MXC_CRMAP_ACR_CKOHS_HIGH		(1 << 18)
#define MXC_CRMAP_ACR_CKOS_HIGH			(1 << 16)
#define MXC_CRMAP_ACR_CKOHS_MASK		(0x7 << 12)
#define MXC_CRMAP_ACR_CKOHDIV_MASK		(0xF << 8)
#define MXC_CRMAP_ACR_CKOHDIV_OFFSET		8
#define MXC_CRMAP_ACR_CKOHD			(1 << 11)
#define MXC_CRMAP_ACR_CKOS_MASK			(0x7 << 4)
#define MXC_CRMAP_ACR_CKOD			(1 << 7)

/* CRM COM Register Offsets */
#define MXC_CRMCOM_CSCR				(MXC_CRM_COM_BASE + 0x0C)
#define MXC_CRMCOM_CCCR                         (MXC_CRM_COM_BASE + 0x10)

/* CRM COM Bit Definitions */
#define MXC_CRMCOM_CCCR_CC_DIV_OFFSET           8
#define MXC_CRMCOM_CCCR_CC_DIV_MASK             (0x1F << 8)
#define MXC_CRMCOM_CCCR_CC_SEL_OFFSET           0
#define MXC_CRMCOM_CCCR_CC_SEL_MASK             0x3
#define MXC_CRMCOM_CSCR_CKOHSEL			(1 << 18)
#define MXC_CRMCOM_CSCR_CKOSEL			(1 << 17)

/* DSM Register Offsets */
#define MXC_DSM_CRM_CONTROL                     (MXC_DSM_BASE + 0x50)
#define MXC_DSM_CONTROL1                        (MXC_DSM_BASE + 0x24)
#define MXC_DSM_CONTROL0                        (MXC_DSM_BASE + 0x20)
#define MXC_DSM_SLEEP_TIME                      (MXC_DSM_BASE + 0x0c)
#define MXC_DSM_WARM_PER                        (MXC_DSM_BASE + 0x40)
#define MXC_DSM_LOCK_PER                        (MXC_DSM_BASE + 0x44)
#define MXC_DSM_CTREN                           (MXC_DSM_BASE + 0x28)
#ifdef CONFIG_MOT_FEAT_PM
#define	MXC_DSM_MGPER				(MXC_DSM_BASE + 0x4c)
#endif

/*
 * Bit definitions of various registers in DSM
 */
#define MXC_DSM_CRM_CTRL_DVFS_BYP                       0x00000008
#define MXC_DSM_CRM_CTRL_DVFS_VCTRL                     0x00000004
#define MXC_DSM_CRM_CTRL_LPMD1                          0x00000002
#define MXC_DSM_CRM_CTRL_LPMD0                          0x00000001
#define MXC_DSM_CONTROL0_STBY_COMMIT_EN                 0x00000200
#define MXC_DSM_CONTROL0_MSTR_EN                        0x00000001
#define MXC_DSM_CONTROL0_RESTART                        0x00000010
#ifdef CONFIG_MOT_FEAT_PM
/* Counter Block reset */
#define MXC_DSM_CONTROL1_CB_RST                         0x00000002
/* State Machine reset */
#define MXC_DSM_CONTROL1_SM_RST                         0x00000004
/* Bit needed to reset counter block */
#define MXC_CONTROL1_RST_CNT32                          0x00000008
#endif
#define MXC_DSM_CONTROL1_RST_CNT32_EN                   0x00000800
#define MXC_DSM_CONTROL1_SLEEP                          0x00000100
#define MXC_DSM_CONTROL1_WAKEUP_DISABLE                 0x00004000
#define MXC_DSM_CTREN_CNT32                             0x00000001

#ifdef CONFIG_MOT_FEAT_PM
/* Magic Fix enable bit */
#define MXC_DSM_MGPER_EN_MGFX                           0x80000000
#define MXC_DSM_MGPER_PER_MASK                          0x000003FF
#define MXC_DSM_MGPER_PER(n)          (MXC_DSM_MGPER_PER_MASK & n)
#endif

/*
 * Bit definitions of ACGCR in CRM_AP for tree level clock gating
 */
#define MXC_ACGCR_ACG0_STOP_WAIT                          0x00000001
#define MXC_ACGCR_ACG0_STOP                               0x00000003
#define MXC_ACGCR_ACG0_RUN                                0x00000007
#define MXC_ACGCR_ACG0_DISABLED                           0x00000000

#define MXC_ACGCR_ACG1_STOP_WAIT                          0x00000008
#define MXC_ACGCR_ACG1_STOP                               0x00000018
#define MXC_ACGCR_ACG1_RUN                                0x00000038
#define MXC_ACGCR_ACG1_DISABLED                           0x00000000

#define MXC_ACGCR_ACG2_STOP_WAIT                          0x00000040
#define MXC_ACGCR_ACG2_STOP                               0x000000C0
#define MXC_ACGCR_ACG2_RUN                                0x000001C0
#define MXC_ACGCR_ACG2_DISABLED                           0x00000000

#define MXC_ACGCR_ACG3_STOP_WAIT                          0x00000200
#define MXC_ACGCR_ACG3_STOP                               0x00000600
#define MXC_ACGCR_ACG3_RUN                                0x00000E00
#define MXC_ACGCR_ACG3_DISABLED                           0x00000000

#define MXC_ACGCR_ACG4_STOP_WAIT                          0x00001000
#define MXC_ACGCR_ACG4_STOP                               0x00003000
#define MXC_ACGCR_ACG4_RUN                                0x00007000
#define MXC_ACGCR_ACG4_DISABLED                           0x00000000

#define MXC_ACGCR_ACG5_STOP_WAIT                          0x00010000
#define MXC_ACGCR_ACG5_STOP                               0x00030000
#define MXC_ACGCR_ACG5_RUN                                0x00070000
#define MXC_ACGCR_ACG5_DISABLED                           0x00000000

#define MXC_ACGCR_ACG6_STOP_WAIT                          0x00080000
#define MXC_ACGCR_ACG6_STOP                               0x00180000
#define MXC_ACGCR_ACG6_RUN                                0x00380000
#define MXC_ACGCR_ACG6_DISABLED                           0x00000000

#define NUM_GATE_CTRL                           6

#ifdef CONFIG_MOT_FEAT_PM
/*
 * Address offsets of the CRM_COM registers
 */
#define CRM_COM_CSCR_PPD1                       0x08000000

/*
 * Address offsets of DPLL registers
 * Originally in mxc_pm.c
 */
#define PLL0_DP_CTL                             (MXC_PLL0_BASE + MXC_PLL_DP_CTL)
#define PLL0_DP_CONFIG                          (MXC_PLL0_BASE + MXC_PLL_DP_CONFIG)
#define PLL0_DP_OP                              (MXC_PLL0_BASE + MXC_PLL_DP_OP)
#define PLL0_DP_MFD                             (MXC_PLL0_BASE + MXC_PLL_DP_MFD)
#define PLL0_DP_MFN                             (MXC_PLL0_BASE + MXC_PLL_DP_MFN)
#define PLL0_DP_MFN_MINUS                       IO_ADDRESS(PLL0_BASE_ADDR + 0x14)
#define PLL0_DP_MFN_PLUS                        IO_ADDRESS(PLL0_BASE_ADDR + 0x18)
#define PLL0_DP_HFSOP                           (MXC_PLL0_BASE + MXC_PLL_DP_HFS_OP)
#define PLL0_DP_HFSMFD                          (MXC_PLL0_BASE + MXC_PLL_DP_HFS_MFD)
#define PLL0_DP_HFSMFN                          (MXC_PLL0_BASE + MXC_PLL_DP_HFS_MFN)
#define PLL0_DP_MFN_TOGC                        IO_ADDRESS(PLL0_BASE_ADDR + 0x28)
#define PLL0_DP_DESTAT                          IO_ADDRESS(PLL0_BASE_ADDR + 0x2c)

#define PLL1_DP_CONFIG                          IO_ADDRESS(PLL1_BASE_ADDR + 0x04)
#define PLL1_DP_MFN_MINUS                       IO_ADDRESS(PLL1_BASE_ADDR + 0x14)
#define PLL1_DP_MFN_PLUS                        IO_ADDRESS(PLL1_BASE_ADDR + 0x18)
#define PLL1_DP_MFN_TOGC                        IO_ADDRESS(PLL1_BASE_ADDR + 0x28)
#define PLL1_DP_DESTAT                          IO_ADDRESS(PLL1_BASE_ADDR + 0x2c)

#define PLL2_DP_CONFIG                          IO_ADDRESS(PLL2_BASE_ADDR + 0x04)
#define PLL2_DP_MFN_MINUS                       IO_ADDRESS(PLL2_BASE_ADDR + 0x14)
#define PLL2_DP_MFN_PLUS                        IO_ADDRESS(PLL2_BASE_ADDR + 0x18)
#define PLL2_DP_MFN_TOGC                        IO_ADDRESS(PLL2_BASE_ADDR + 0x28)
#define PLL2_DP_DESTAT                          IO_ADDRESS(PLL2_BASE_ADDR + 0x2c)
#define DP_LDREQ                                0x00000001
#define DP_HFSM                                 0x00000080

#define PLL1_DP_CTL                             IO_ADDRESS(PLL1_BASE_ADDR + 0x00)
#define PLL1_DP_OP                              IO_ADDRESS(PLL1_BASE_ADDR + 0x08)
#define PLL1_DP_MFD                             IO_ADDRESS(PLL1_BASE_ADDR + 0x0c)
#define PLL1_DP_MFN                             IO_ADDRESS(PLL1_BASE_ADDR + 0x10)
#define PLL1_DP_HFSOP                           IO_ADDRESS(PLL1_BASE_ADDR + 0x1c)
#define PLL1_DP_HFSMFD                          IO_ADDRESS(PLL1_BASE_ADDR + 0x20)
#define PLL1_DP_HFSMFN                          IO_ADDRESS(PLL1_BASE_ADDR + 0x24)

#define PLL2_DP_CTL                             IO_ADDRESS(PLL2_BASE_ADDR + 0x00)
#define PLL2_DP_OP                              IO_ADDRESS(PLL2_BASE_ADDR + 0x08)
#define PLL2_DP_MFD                             IO_ADDRESS(PLL2_BASE_ADDR + 0x0c)
#define PLL2_DP_MFN                             IO_ADDRESS(PLL2_BASE_ADDR + 0x10)
#define PLL2_DP_HFSOP                           IO_ADDRESS(PLL2_BASE_ADDR + 0x1c)
#define PLL2_DP_HFSMFD                          IO_ADDRESS(PLL2_BASE_ADDR + 0x20)
#define PLL2_DP_HFSMFN                          IO_ADDRESS(PLL2_BASE_ADDR + 0x24)

/* Bits used for aggressive clock gating */
#define APRA_ALL_CLK_EN         (MXC_CRMAP_APRA_SIMEN   | \
                                 MXC_CRMAP_APRA_UART1EN | \
                                 MXC_CRMAP_APRA_UART2EN | \
                                 MXC_CRMAP_APRA_UART3EN | \
                                 MXC_CRMAP_APRA_SAHARA_DIV2_CLK_EN_PASS2)

#define APRB_ALL_CLK_EN         (MXC_CRMAP_APRB_SDHC1EN | \
                                 MXC_CRMAP_APRB_SDHC2EN)

#define ACDER1_ALL_CLK_EN       (MXC_CRMAP_ACDER1_CSIEN_PASS2 | \
                                 MXC_CRMAP_ACDER1_SSI1EN | \
                                 MXC_CRMAP_ACDER1_SSI2EN)

#define ACDER2_ALL_CLK_EN       (MXC_CRMAP_ACDER2_USBEN | \
                                 MXC_CRMAP_ACDER2_NFCEN)

#endif /* CONFIG_MOT_FEAT_PM */

#if defined(CONFIG_MOT_FEAT_PM)

#define MXC_CLKCTL_BASE             (IO_ADDRESS(CLKCTL_BASE_ADDR))

#define MXC_CLKCTL_GP_CTRL          (MXC_CLKCTL_BASE + 0x00)
#define MXC_CLKCTL_GP_SER           (MXC_CLKCTL_BASE + 0x04)
#define MXC_CLKCTL_GP_CER           (MXC_CLKCTL_BASE + 0x08)

#define MXC_CLKCTL_GP_CTRL_BIT7     0x80

#endif /* CONFIG_MOT_FEAT_PM */

#endif				/* _ARCH_ARM_MACH_MXC91231_CRM_REGS_H_ */
