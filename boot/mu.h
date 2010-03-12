#ifndef __HW_MU_H__
#define __HW_MU_H__
#include "types.h"

int mu_reset(void);
int mu_dsp_reset(void);

int mu_write_noblock(int reg, uint32_t data);
int mu_read(int reg, uint32_t *data);
int mu_write_noblock(int reg, uint32_t data);
int mu_read(int reg, uint32_t *data);

void mu_dump_mrrs();
void mu_dump_mtrs();

#endif // __HW_MU_H__
