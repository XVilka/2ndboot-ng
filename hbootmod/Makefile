# In this string we need to use -D__SOME__ command for define which platform we use
# And C preprocessor use only code for platform, we defined
# For example if we use Texas Instruments OMAP 3430 - we should use
# EXTRA_CFLAGS += -D__PLAT_TI_OMAP3430__
# Else if we need for Freescale i.MX31 - we should use
# EXTRA_CFLAGS += -D__PLAT_FREESCALE_IMX31__

EXTRA_CFLAGS += -D__PLAT_TI_OMAP3430__ -Wall -march=armv7-a -mtune=cortex-a8 -mfpu=neon

obj-m:= hboot.o xboot.o
hboot-objs:= buffers.o crc32.o
