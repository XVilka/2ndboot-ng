/*
 * Copyright 2005-2006 Freescale Semiconductor, Inc.
 * Copyright (C) 2006-2007 Motorola, Inc. 
 */
/*
 * The code contained herein is licensed under the GNU Lesser General
 * Public License.  You may obtain a copy of the GNU Lesser General
 * Public License Version 2.1 or later at the following locations:
 *
 * http://www.opensource.org/licenses/lgpl-license.html
 * http://www.gnu.org/copyleft/lgpl.html
 *
 * Date     Author      Comment
 * 10/2006  Motorola    Added support for QVGA Sanyo display, additional pixel
 *                      packing formats, and a configurable memory location.
 * 01/2007  Motorola    Added support for configurable IPU memory size.
 * 06/2007  Motorola    Fix to support setting of ADC serial interface bit
 *                      width in ipu driver.
 * 08/2007  Motorola    Add comments for oss compliance.
 * 08/2007  Motorola    Add comments.
 * 11/2007  Motorola    Add declaration of a function
 * 11/2007  Motorola    Added Dynamic AP Clock Gating for IPU clock.
 * 12/2007  Motorola  Removed Dynamic AP Clock Gating due to the introduced issue that phone resets
 */

/*!
 * @defgroup IPU MXC Image Processing Unit (IPU) Driver
 */
/*!
 * @file ipu.h
 * 
 * @brief This file contains the IPU driver API declarations. 
 * 
 * @ingroup IPU
 */

#ifndef _INCLUDE_IPU_H_
#define _INCLUDE_IPU_H_

#include "types.h"

/*!
 * Enumeration of IPU rotation modes
 */
typedef enum {
	/* Note the enum values correspond to BAM value	*/
	IPU_ROTATE_NONE = 0,
	IPU_ROTATE_VERT_FLIP = 1,
	IPU_ROTATE_HORIZ_FLIP = 2,
	IPU_ROTATE_180 = 3,
	IPU_ROTATE_90_RIGHT = 4,
	IPU_ROTATE_90_RIGHT_VFLIP = 5,
	IPU_ROTATE_90_RIGHT_HFLIP = 6,
	IPU_ROTATE_90_LEFT = 7,
} ipu_rotate_mode_t;

/*!
 * Enumeration of Post Filter modes
 */
typedef enum {
	PF_DISABLE_ALL = 0,
	PF_MPEG4_DEBLOCK = 1,
	PF_MPEG4_DERING = 2,
	PF_MPEG4_DEBLOCK_DERING = 3,
	PF_H264_DEBLOCK = 4,
} pf_operation_t;

/*!
 * Enumeration of Synchronous (Memory-less) panel types
 */
typedef enum {
	IPU_PANEL_SHARP_TFT,
	IPU_PANEL_TFT,
	IPU_PANEL_SANYO_TFT,
} ipu_panel_t;

/*  IPU Pixel format definitions */
/*  Four-character-code (FOURCC) */
#define ipu_fourcc(a,b,c,d)\
        (((__u32)(a)<<0)|((__u32)(b)<<8)|((__u32)(c)<<16)|((__u32)(d)<<24))

/*!
 * @name IPU Pixel Formats
 *
 * Pixel formats are defined with ASCII FOURCC code. The pixel format codes are
 * the same used by V4L2 API.
 */

/*! @{ */
/*! @name Generic or Raw Data Formats *//*! @{ */
#define IPU_PIX_FMT_GENERIC ipu_fourcc('I','P','U','0')	/*!< IPU Generic Data */
#define IPU_PIX_FMT_GENERIC_32 ipu_fourcc('I','P','U','1')	/*!< IPU Generic Data */
/*! @} */

/*! @name RGB Formats *//*! @{ */
#define IPU_PIX_FMT_RGB332  ipu_fourcc('R','G','B','1')	/*!<  8  RGB-3-3-2     */
#define IPU_PIX_FMT_RGB555  ipu_fourcc('R','G','B','O')	/*!< 16  RGB-5-5-5     */
#define IPU_PIX_FMT_RGB565  ipu_fourcc('R','G','B','P')	/*!< 16  RGB-5-6-5     */
// #if defined(CONFIG_MOT_FEAT_FB_MXC_RGB)
#define IPU_PIX_FMT_BGR565  ipu_fourcc('B','G','R','P') /*!< 16  BGR-5-6-5     */
// #endif
#define IPU_PIX_FMT_RGB666  ipu_fourcc('R','G','B','6')	/*!< 18  RGB-6-6-6     */
#define IPU_PIX_FMT_BGR666  ipu_fourcc('B','G','R','0') /*!< 18  BGR-6-6-6     */
#define IPU_PIX_FMT_BGR24   ipu_fourcc('B','G','R','3')	/*!< 24  BGR-8-8-8     */
#define IPU_PIX_FMT_RGB24   ipu_fourcc('R','G','B','3')	/*!< 24  RGB-8-8-8     */
#define IPU_PIX_FMT_BGRA6666  ipu_fourcc('B','G','R','6') /*!< 24  BGR-6-6-6-6 */
#define IPU_PIX_FMT_BGR32   ipu_fourcc('B','G','R','4')	/*!< 32  BGR-8-8-8-8   */
#define IPU_PIX_FMT_BGRA32  ipu_fourcc('B','G','R','A')	/*!< 32  BGR-8-8-8-8   */
#define IPU_PIX_FMT_RGB32   ipu_fourcc('R','G','B','4')	/*!< 32  RGB-8-8-8-8   */
#define IPU_PIX_FMT_RGBA32  ipu_fourcc('R','G','B','A')	/*!< 32  RGB-8-8-8-8   */
#define IPU_PIX_FMT_ABGR32  ipu_fourcc('A','B','G','R')	/*!< 32  ABGR-8-8-8-8  */
/*! @} */

/*! @name YUV Interleaved Formats *//*! @{ */
#define IPU_PIX_FMT_YUYV    ipu_fourcc('Y','U','Y','V')	/*!< 16  YUV 4:2:2     */
#define IPU_PIX_FMT_UYVY    ipu_fourcc('U','Y','V','Y')	/*!< 16  YUV 4:2:2     */
#define IPU_PIX_FMT_Y41P    ipu_fourcc('Y','4','1','P')	/*!< 12  YUV 4:1:1     */
/*! @} */

/*! @name YUV Planar Formats *//*! @{ */
#define IPU_PIX_FMT_GREY    ipu_fourcc('G','R','E','Y')	/*!< 8  Greyscale     */
#define IPU_PIX_FMT_YVU410P ipu_fourcc('Y','V','U','9')	/*!< 9  YVU 4:1:0     */
#define IPU_PIX_FMT_YUV410P ipu_fourcc('Y','U','V','9')	/*!< 9  YUV 4:1:0     */
#define IPU_PIX_FMT_YVU420P ipu_fourcc('Y','V','1','2')	/*!< 12  YVU 4:2:0     */
#define IPU_PIX_FMT_YUV420P ipu_fourcc('I','4','2','0')	/*!< 12  YUV420 planar */
#define IPU_PIX_FMT_YUV420P2 ipu_fourcc('Y','U','1','2')	/*!< 12  YUV 4:2:0 planar */
#define IPU_PIX_FMT_YVU422P ipu_fourcc('Y','V','1','6')	/*!< 16  YVU422 planar */
/*! @} */

/*#define IPU_PIX_FMT_YUV422P ipu_fourcc('4','2','2','P')  16  YVU422 planar */
/*#define IPU_PIX_FMT_YUV411P ipu_fourcc('4','1','1','P')  16  YVU411 planar */
/*#define IPU_PIX_FMT_YYUV    ipu_fourcc('Y','Y','U','V')  16  YUV 4:2:2     */

/* IPU Driver channels definitions.	*/
/* Note these are different from IDMA channels */
#define _MAKE_CHAN(num, in, out, sec)    ((num << 24) | (sec << 16) | (out << 8) | in)
#define IPU_CHAN_ID(ch)         (ch >> 24)
#define IPU_CHAN_SEC_DMA(ch)    ((uint32_t) (ch >> 16) & 0xFF)
#define IPU_CHAN_OUT_DMA(ch)    ((uint32_t) (ch >> 8) & 0xFF)
#define IPU_CHAN_IN_DMA(ch)     ((uint32_t) (ch & 0xFF))

/*!
 * Enumeration of IPU logical channels. An IPU logical channel is defined as a
 * combination of an input (memory to IPU), output (IPU to memory), and/or
 * secondary input IDMA channels and in some cases an Image Converter task.
 * Some channels consist of only an input or output.
 */
typedef enum {
	CSI_MEM = _MAKE_CHAN(1, 0xFF, 7, 0xFF),	/*!< CSI raw sensor data to memory */

	CSI_PRP_ENC_MEM = _MAKE_CHAN(2, 0xFF, 0, 0xFF),	/*!< CSI to IC Encoder PreProcessing to Memory */
	MEM_PRP_ENC_MEM = _MAKE_CHAN(3, 6, 0, 0xFF),	/*!< Memory to IC Encoder PreProcessing to Memory */
	MEM_ROT_ENC_MEM = _MAKE_CHAN(4, 10, 8, 0xFF),	/*!< Memory to IC Encoder Rotation to Memory */

	CSI_PRP_VF_MEM = _MAKE_CHAN(5, 0xFF, 1, 0xFF),	/*!< CSI to IC Viewfinder PreProcessing to Memory */
	CSI_PRP_VF_ADC = _MAKE_CHAN(6, 0xFF, 1, 0xFF),	/*!< CSI to IC Viewfinder PreProcessing to ADC */
	MEM_PRP_VF_MEM = _MAKE_CHAN(7, 6, 1, 3),	/*!< Memory to IC Viewfinder PreProcessing to Memory */
	MEM_PRP_VF_ADC = _MAKE_CHAN(8, 6, 1, 3),	/*!< Memory to IC Viewfinder PreProcessing to ADC */
	MEM_ROT_VF_MEM = _MAKE_CHAN(9, 11, 9, 0xFF),	/*!< Memory to IC Viewfinder Rotation to Memory */

	MEM_PP_MEM = _MAKE_CHAN(10, 5, 2, 4),	/*!< Memory to IC PostProcessing to Memory */
	MEM_ROT_PP_MEM = _MAKE_CHAN(11, 13, 12, 0xFF),	/*!< Memory to IC PostProcessing Rotation to Memory */
	MEM_PP_ADC = _MAKE_CHAN(12, 5, 2, 4),	/*!< Memory to IC PostProcessing to ADC */

	MEM_SDC_BG = _MAKE_CHAN(14, 14, 0xFF, 0xFF),	/*!< Memory to SDC Background plane */
	MEM_SDC_FG = _MAKE_CHAN(15, 15, 0xFF, 0xFF),	/*!< Memory to SDC Foreground plane */
	MEM_SDC_MASK = _MAKE_CHAN(16, 16, 0xFF, 0xFF),	/*!< Memory to SDC Mask */

	ADC_SYS1 = _MAKE_CHAN(17, 18, 22, 20),	/*!< Memory to ADC System Channel 1 */
	ADC_SYS2 = _MAKE_CHAN(18, 19, 23, 21),	/*!< Memory to ADC System Channel 2 */

	MEM_PF_Y_MEM = _MAKE_CHAN(19, 26, 29, 24),	/*!< Y and PF Memory to Post-filter to Y Memory */
	MEM_PF_U_MEM = _MAKE_CHAN(20, 27, 30, 25),	/*!< U and PF Memory to Post-filter to U Memory */
	MEM_PF_V_MEM = _MAKE_CHAN(21, 28, 31, 0xFF),	/*!< V Memory to Post-filter to V Memory */

	CHAN_NONE = 0xFFFFFFFF,
} ipu_channel_t;

/*!
 * Enumeration of types of buffers for a logical channel.
 */
typedef enum {
	IPU_INPUT_BUFFER,	/*!< Buffer for input to IPU */
	IPU_OUTPUT_BUFFER,	/*!< Buffer for output from IPU */
	IPU_SEC_INPUT_BUFFER,	/*!< Buffer for second input to IPU */
} ipu_buffer_t;

/*!
 * Enumeration of DI ports for ADC.
 */
typedef enum {
	DISP0,
	DISP1,
	DISP2,
	DISP3
} display_port_t;

/*!
 * Enumeration of ADC channels.
typedef enum
{
        PRPCHAN,
        PPCHAN,
        SYSCHAN1,
        SYSCHAN2
}sys_channel_t;
 */

/*!
 * Enumeration of ADC channel operation mode.
 */
typedef enum {
	Disable,
	WriteTemplateNonSeq,
	ReadTemplateNonSeq,
	WriteTemplateUnCon,
	ReadTemplateUnCon,
	WriteDataWithRS,
	WriteDataWoRS,
	WriteCmd
} mcu_mode_t;

/*!
 * Enumeration of ADC channel addressing mode.
 */
typedef enum {
	FullWoBE,
	FullWithBE,
	XY
} display_addressing_t;

/*!
 * Union of initialization parameters for a logical channel.
 */
typedef union {
	struct {
		uint32_t temp;
	} csi_mem;
	struct {
		uint32_t in_width;
		uint32_t in_height;
		uint32_t in_pixel_fmt;
		uint32_t out_width;
		uint32_t out_height;
		uint32_t out_pixel_fmt;
	} csi_prp_enc_mem;
	struct {
		uint32_t in_width;
		uint32_t in_height;
		uint32_t in_pixel_fmt;
		uint32_t out_width;
		uint32_t out_height;
		uint32_t out_pixel_fmt;
	} mem_prp_enc_mem;
	struct {
		uint32_t in_width;
		uint32_t in_height;
		uint32_t in_pixel_fmt;
		uint32_t out_width;
		uint32_t out_height;
		uint32_t out_pixel_fmt;
	} mem_rot_enc_mem;
	struct {
		uint32_t in_width;
		uint32_t in_height;
		uint32_t in_pixel_fmt;
		uint32_t out_width;
		uint32_t out_height;
		uint32_t out_pixel_fmt;
		bool graphics_combine_en;
		bool global_alpha_en;
		bool key_color_en;
	} csi_prp_vf_mem;
	struct {
		uint32_t in_width;
		uint32_t in_height;
		uint32_t in_pixel_fmt;
		uint32_t out_width;
		uint32_t out_height;
		uint32_t out_pixel_fmt;
		bool graphics_combine_en;
		bool global_alpha_en;
		bool key_color_en;
		display_port_t disp;
		uint32_t out_left;
		uint32_t out_top;
	} csi_prp_vf_adc;
	struct {
		uint32_t in_width;
		uint32_t in_height;
		uint32_t in_pixel_fmt;
		uint32_t out_width;
		uint32_t out_height;
		uint32_t out_pixel_fmt;
		bool graphics_combine_en;
		bool global_alpha_en;
		bool key_color_en;
	} mem_prp_vf_mem;
	struct {
		uint32_t temp;
	} mem_prp_vf_adc;
	struct {
		uint32_t temp;
	} mem_rot_vf_mem;
	struct {
		uint32_t in_width;
		uint32_t in_height;
		uint32_t in_pixel_fmt;
		uint32_t out_width;
		uint32_t out_height;
		uint32_t out_pixel_fmt;
		bool graphics_combine_en;
		bool global_alpha_en;
		bool key_color_en;
	} mem_pp_mem;
	struct {
		uint32_t temp;
	} mem_rot_mem;
	struct {
		uint32_t in_width;
		uint32_t in_height;
		uint32_t in_pixel_fmt;
		uint32_t out_width;
		uint32_t out_height;
		uint32_t out_pixel_fmt;
		bool graphics_combine_en;
		bool global_alpha_en;
		bool key_color_en;
		display_port_t disp;
		uint32_t out_left;
		uint32_t out_top;
	} mem_pp_adc;

	struct {
		pf_operation_t operation;
	} mem_pf_mem;

	struct {
		uint32_t temp;
	} mem_sdc_fg;
	struct {
		uint32_t temp;
	} mem_sdc_bg;
	struct {
		display_port_t disp;
		mcu_mode_t ch_mode;
		uint32_t out_left;
		uint32_t out_top;
	} adc_sys1;
	struct {
		display_port_t disp;
		mcu_mode_t ch_mode;
		uint32_t out_left;
		uint32_t out_top;
	} adc_sys2;
} ipu_channel_params_t;

/*!
 * Bitfield of Display Interface signal polarities.
 */
typedef struct {
	unsigned datamask_en:1;
	unsigned clksel_en:1;
	unsigned clkidle_en:1;
	unsigned data_pol:1;	/* true = inverted */
	unsigned clk_pol:1;	/* true = rising edge */
	unsigned enable_pol:1;
	unsigned Hsync_pol:1;	/* true = active high */
	unsigned Vsync_pol:1;
} ipu_di_signal_cfg_t;

/*!
 * Bitfield of CSI signal polarities and modes.
 */
typedef struct {
	unsigned data_width:3;
	unsigned clk_mode:2;
	unsigned ext_vsync:1;
	unsigned Vsync_pol:1;
	unsigned Hsync_pol:1;
	unsigned pixclk_pol:1;
	unsigned data_pol:1;
	unsigned sens_clksrc:1;
} ipu_csi_signal_cfg_t;

/*!
 * Enumeration of CSI data bus widths.
 */
enum {
	IPU_CSI_DATA_WIDTH_4,
	IPU_CSI_DATA_WIDTH_8,
	IPU_CSI_DATA_WIDTH_10,
	IPU_CSI_DATA_WIDTH_16,
};

/*!
 * Enumeration of CSI clock modes.
 */
enum {
	IPU_CSI_CLK_MODE_GATED_CLK,
	IPU_CSI_CLK_MODE_NONGATED_CLK,
	IPU_CSI_CLK_MODE_CCIR656_PROGRESSIVE,
	IPU_CSI_CLK_MODE_CCIR656_INTERLACED,
};

typedef enum {
	RGB,
	YCbCr,
	YUV
} ipu_color_space_t;

/*!
 * Enumeration of ADC vertical sync mode.
 */
typedef enum {
	VsyncNone,
	VsyncInternal,
	VsyncCSI,
	VsyncExternal
} vsync_t;

/*!
 * Enumeration of command and data transfer modes.
 */
typedef enum
{
        DAT,      /* Send only data to the display */
        CMD,      /* Send a command and then associated data */
}cmddata_t;

/*!
 * Enumeration of ADC display update mode.
 */
typedef enum {
	IPU_ADC_REFRESH_NONE,
	IPU_ADC_AUTO_REFRESH,
	IPU_ADC_AUTO_REFRESH_SNOOP,
	IPU_ADC_SNOOPING,
} ipu_adc_update_mode_t;

/*!
 * Enumeration of ADC display interface types (serial or parallel).
 */
enum {
	IPU_ADC_IFC_MODE_SYS80_TYPE1,
	IPU_ADC_IFC_MODE_SYS80_TYPE2,
	IPU_ADC_IFC_MODE_SYS68K_TYPE1,
	IPU_ADC_IFC_MODE_SYS68K_TYPE2,
	IPU_ADC_IFC_MODE_3WIRE_SERIAL,
	IPU_ADC_IFC_MODE_4WIRE_SERIAL,
	IPU_ADC_IFC_MODE_5WIRE_SERIAL_CLK,
	IPU_ADC_IFC_MODE_5WIRE_SERIAL_CS,
};

enum {
	IPU_ADC_IFC_WIDTH_8,
	IPU_ADC_IFC_WIDTH_16,
};

/*!
 * Enumeration of ADC display interface burst mode.
 */
enum {
	IPU_ADC_BURST_WCS,
	IPU_ADC_BURST_WBLCK,
	IPU_ADC_BURST_NONE,
	IPU_ADC_BURST_SERIAL,
};

/*!
 * Enumeration of ADC display interface RW signal timing modes.
 */
enum {
	IPU_ADC_SER_NO_RW,
	IPU_ADC_SER_RW_BEFORE_RS,
	IPU_ADC_SER_RW_AFTER_RS,
};

/*!
 * Bitfield of ADC signal polarities and modes.
 */
typedef struct {
	unsigned data_pol:1;
	unsigned clk_pol:1;
	unsigned cs_pol:1;
	unsigned addr_pol:1;
	unsigned read_pol:1;
	unsigned write_pol:1;
	unsigned Vsync_pol:1;
	unsigned burst_pol:1;
	unsigned burst_mode:2;
	unsigned ifc_mode:3;
	unsigned ifc_width:5;
	unsigned ser_preamble_len:4;
	unsigned ser_preamble:8;
	unsigned ser_rw_mode:2;
} ipu_adc_sig_cfg_t;

/*!
 * Enumeration of ADC template commands.
 */
enum {
	RD_DATA,
	RD_ACK,
	RD_WAIT,
	WR_XADDR,
	WR_YADDR,
	WR_ADDR,
	WR_CMND,
	WR_DATA,
};

/*!
 * Enumeration of ADC template command flow control.
 */
enum {
	SINGLE_STEP,
	PAUSE,
	STOP,
};

/*Define template constants*/
#define     ATM_ADDR_RANGE      0x20	/*offset address of DISP */
#define     TEMPLATE_BUF_SIZE   0x20	/*size of template */

/*!
 * Define to create ADC template command entry.
 */
#define ipu_adc_template_gen(oc, rs, fc, dat) ( ((rs) << 29) | ((fc) << 27) | \
                                                ((oc) << 24) | (dat) )

typedef struct {
	u32 reg;
	u32 value;
} ipu_lpmc_reg_t;

#define IPU_LPMC_REG_READ       0x80000000L

#define CSI_MCLK_VF  1
#define CSI_MCLK_ENC 2
#define CSI_MCLK_RAW 4
#define CSI_MCLK_I2C 8

#endif
