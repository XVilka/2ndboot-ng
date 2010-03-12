#ifndef __HW_NFC_H__
#define __HW_NFC_H__

#include "types.h"

int nfc_init();
size_t nfc_read_data(uint8_t *dest, uint32_t offset, size_t size);

#endif // __HW_NFC_H__
