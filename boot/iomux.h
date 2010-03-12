#ifndef __HW_IOMUX_COM_H__
#define __HW_IOMUX_COM_H__

#include "types.h"

#define IOMUX_AP 0
#define IOMUX_COM 1

#define IOMUX_MAX_NUM IOMUX_COM

int iomux_set_function(int iomux, unsigned int pin, unsigned int infunc, unsigned int outfunc);

#endif // __HW_IOMUX_COM_H__
