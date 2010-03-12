#include "types.h"
#include "stdio.h"
#include "error.h"
#include "atag.h"
#include "common.h"
#include "memory.h"
#include "hw_misc.h"
#include "images.h"
#include "dsp.h"
#include "atlas.h"

void critical_error(error_t err) {
  if (console_initialized()) {
    printf("Critical error %d\n", (int)err);
  }
  while (1);
}

int main() {
  struct memory_image image;
  
  image_complete();

  printf("e8 loader rev %s.\n", LDR_VERSION);
  image_dump_stats();

  atlas_init();
  if (atlas_test_io() != 0) {
    printf("btw, atlas i/o is not working\n");
  }
  dsp_reboot();
  hw_preboot();

  if (image_find(IMG_LINUX, &image) != NULL) {
    jump_to_linux(image.data, 1024, atag_build());
  } else {
    critical_error(IMG_NOT_PROVIDED);
  }

  return 0;
}
