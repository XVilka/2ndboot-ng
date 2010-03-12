#ifndef __HW_CSPI_H__
#define __HW_CSPI_H__

#include "types.h"

struct cspi_config {
  int chip_select;
  int data_rate;
  int bit_count;
  int active_high_ss_polarity;
  int ss_waveform;
  int phase;
  int active_high_clk_polarity;
};

int cspi_init(int module, struct cspi_config *cfg);
int cspi_send(int module, uint32_t *data, size_t len, unsigned int *timeout);
int cspi_recv(int module, uint32_t *buf, size_t size, unsigned int *timeout);

#endif // __HW_CSPI_H__
