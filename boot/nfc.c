#include "memory.h"
#include "nfc.h"
#include "common.h"
#include "mxc91231.h"

#define NFC_BUF_REG_ADDR (NFC_BASE_ADDR + 0xe04)
#define NFC_ADDR_REG_ADDR (NFC_BASE_ADDR + 0xe06)
#define NFC_CMD_REG_ADDR (NFC_BASE_ADDR + 0xe08)
#define NFC_CFG_REG_ADDR (NFC_BASE_ADDR + 0xe0a)
#define NFC_STAT_REG_ADDR (NFC_BASE_ADDR + 0xe0c)
#define NFC_WP_REG_ADDR (NFC_BASE_ADDR + 0xe12)
#define NFC_CFG1_REG_ADDR (NFC_BASE_ADDR + 0xe1a)
#define NFC_CFG2_REG_ADDR (NFC_BASE_ADDR + 0xe1c)

#define NFC_RAMBUF0_BASE_ADDR (NFC_BASE_ADDR)
#define NAND_CMD_RESET 0xff
#define NAND_CMD_READSTART 0x30
#define NAND_CMD_READID 0x90
#define NAND_CMD_STATUS 0x70
#define NAND_CMD_READ0 0x00

#define IS_LARGE_PAGE() (device.page_size == 2048)
#define IS_WIDE_BUS() (device.bus_width == 16)

struct nand_dev_id {
  uint8_t man_id;
  uint8_t dev_id;
  unsigned int size;
  unsigned int page_size;
  unsigned int erase_size;
};

struct nand_dev {
  struct nand_dev_id *id;
  int bus_width;
  int page_size;
};

static struct nand_dev_id nand_dev_ids[] = {
  {0x98, 0xb1, 128, 0, 0},
  {0x20, 0xb1, 128, 0, 0},
  {0x2c, 0xb1, 128, 0, 0},
  {0x98, 0x72, 128, 512, 0x4000},
  {0xad, 0x49, 128, 512, 0x4000},
};

static struct nand_dev device;

static void nfc_enable_clock() {
  modify_register32(CRM_AP_BASE_ADDR+0xc, 0, 1 << 20);
}

static void nfc_preset() {
  write16(0x2, NFC_CFG_REG_ADDR); // Unlock RAMbuffers.
  write16(0x2, NFC_WP_REG_ADDR); // Lock all NAND blocks.
  write16(0x18, NFC_CFG1_REG_ADDR); // Mask inerrupt.
}

static inline int nfc_wait() {
  while ((read16(NFC_CFG2_REG_ADDR) & (1 << 15)) == 0) {
  }
  modify_register16(NFC_CFG2_REG_ADDR, 0x80, 0);
  return 0;
}

static int nfc_send_command(uint16_t cmd) {
  write16(cmd, NFC_CMD_REG_ADDR);
  write16(0x1, NFC_CFG2_REG_ADDR);
  nfc_wait();
  return 0;
}

static int nfc_send_address(uint16_t addr) {
  write16(addr, NFC_ADDR_REG_ADDR);
  write16(0x2, NFC_CFG2_REG_ADDR);
  nfc_wait();
  return 0;
}

static int nfc_get_data(int buffer) {
  write16(buffer & 0xf, NFC_BUF_REG_ADDR);
  write16(0x18, NFC_CFG1_REG_ADDR);
  write16(0x8, NFC_CFG2_REG_ADDR);
  nfc_wait();
  return (read16(NFC_STAT_REG_ADDR) & 0x8) != 0;
}

static int nfc_status() {
  nfc_preset();
  write16(0x0, NFC_BUF_REG_ADDR);
  nfc_send_command(NAND_CMD_STATUS);
  write16(0x20, NFC_CFG2_REG_ADDR);
  nfc_wait();
  return 0;
}

static int nfc_id() {
  nfc_preset();
  write16(0x0, NFC_BUF_REG_ADDR);
  nfc_send_command(NAND_CMD_READID);
  write16(0x10, NFC_CFG2_REG_ADDR);
  nfc_wait();
  return 0;
}

static int nfc_read_page(uint16_t col, uint32_t row) {
  int i, ecc = 0;

  nfc_preset();
  write16(0x0, NFC_BUF_REG_ADDR);
  nfc_send_command(NAND_CMD_READ0);
  nfc_send_address(col & 0xff);
  if (IS_LARGE_PAGE()) {
    nfc_send_address((col >> 8) & 0xff);
  }
  nfc_send_address(row & 0xff);
  nfc_send_address((row >> 8) & 0xff);
  nfc_send_address((row >> 16) & 0xff);

  if (IS_LARGE_PAGE()) {
    nfc_send_command(NAND_CMD_READSTART);
    for (i = 0; i <= 3; ++i) {
      ecc += nfc_get_data(i);
    }
  } else {
    ecc = nfc_get_data(0);
  }
  if (ecc) {
    printf("ecc error(s) occured\n");
  }
  return ecc;
}

static int nfc_reset() {
  nfc_preset();
  return nfc_send_command(0xff);
}

static uint8_t nfc_read8(uint32_t offset) {
  uint16_t word;

  word = read16(NFC_RAMBUF0_BASE_ADDR + (offset & (~0x1)));
  if (offset & 0x1) {
    return (word >> 8) & 0xff;
  } else {
    return word & 0xff;
  }
}

static size_t nfc_copy_from_rambuf(uint8_t *dest, uint32_t offset, size_t size) {
  size_t ssize = size;
  addr_t adest = (addr_t)dest;

  if (offset & 1) {
    write8(nfc_read8(offset++), adest++);
  }
  for (; size >= 2; size -= 2, offset += 2) {
    write16(read16(NFC_RAMBUF0_BASE_ADDR+offset), adest);
    adest += 2;
  }
  if (size) {
    write8(nfc_read8(offset), adest);
  }
  return ssize;
}

static int nfc_detect_device() {
  int i;
  uint8_t mid, did;
  volatile uint16_t *rambuf = (volatile uint16_t*)NFC_RAMBUF0_BASE_ADDR;

  nfc_id();
  if (IS_WIDE_BUS()) {
    rambuf[0] = (rambuf[0] & 0xff) | ((rambuf[1] & 0xff) << 8);
    rambuf[1] = (rambuf[2] & 0xff) | ((rambuf[3] & 0xff) << 8);
    rambuf[2] = (rambuf[4] & 0xff) | ((rambuf[5] & 0xff) << 8);
  }
  mid = nfc_read8(0);
  did = nfc_read8(1);

  device.id = NULL;
  for (i = 0; i < sizeof(nand_dev_ids)/sizeof(struct nand_dev_id); ++i) {
    if (nand_dev_ids[i].man_id == mid &&
	nand_dev_ids[i].dev_id == did) {
      device.id = &nand_dev_ids[i];
      if (device.id->page_size == 0) {
	uint8_t eid;

	eid = nfc_read8(3);
	device.id->page_size = 1024 << (eid & 0x3);
	eid >>= 4;
	device.id->erase_size = 65536 << (eid & 0x3);
      }
    }
  }
  if (device.id == NULL) {
    printf("unsupported nand\n");
    printf("m: %02x, d: %02x\n", (uint32_t)mid, (uint32_t)did);
    return -1;
  }
  return 0;
}

int nfc_init() {
  int i = 0;
  uint32_t w;

  nfc_enable_clock();
  nfc_reset();
  device.page_size = (read32(CRM_COM_BASE_ADDR+0xc) & (1 << 20)) ? 2048 : 512;
  device.bus_width = (read32(CRM_COM_BASE_ADDR+0xc) & (1 << 19)) ? 16 : 8;
  if (nfc_detect_device() != 0) {
    return -1;
  }
}

size_t nfc_read_data(uint8_t *dest, uint32_t offset, size_t size) {
  size_t ssize = size;
  uint32_t fpart;
  unsigned int pagesize;
  unsigned int page;

  if (((addr_t)dest & 0x1) != (offset & 0x1)) {
    printf("misalignment\n");
    return 0;
  }
  if (device.id == NULL) {
    printf("nand unitialized\n");
    return 0;
  }
  if (offset + size > device.id->size*1024*1024) {
    printf("offset+size too big\n");
    return 0;
  }
  pagesize = device.id->page_size;
  page = offset / pagesize;
  fpart = offset & (pagesize - 1);
  offset = 0;
  if (fpart) {
    uint32_t count;

    count = ((pagesize - fpart) < size ? (pagesize - fpart) : size);
    nfc_read_page(0, page++);
    nfc_copy_from_rambuf(dest, fpart, count);
    offset = count;
    size -= count;
  }
  for (; size >= pagesize; size -= pagesize, offset += pagesize) {
    nfc_read_page(0, page++);
    nfc_copy_from_rambuf(dest + offset, 0, pagesize);
  }
  if (size) {
    nfc_read_page(0, page);
    nfc_copy_from_rambuf(dest + offset, 0, size);
  }
  return ssize;
}

