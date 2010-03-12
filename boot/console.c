#include "types.h"
#include "stdio.h"
#include "font.h"
#include "string.h"
#include "ipu.h"
#include "error.h"

#define LINE_INTERVAL 0
#pragma pack(push,1)
struct pixel {
  uint8_t blue:6;
  uint8_t green:6;
  uint8_t red:6;
  uint8_t a:6;
};
#pragma pack(pop)
struct screen {
  struct pixel *fb;
  int xres;
  int yres;
  int current_x;
  int current_y;
  struct pixel white;
  struct pixel black;
  int initialized;
};
static struct screen sdc_screen;
static struct font *console_font;

static inline struct pixel *get_pixel(int x, int y) {
  return &sdc_screen.fb[sdc_screen.xres*y + x];
}

static void region_fill(int x, int y, int width, int height, struct pixel sample) {
  int sx, sw;
  sx = x;
  sw = width;

  while (height-- > 0) {
    while (width-- > 0) {
      *get_pixel(x++, y) = sample;
    }
    x = sx;
    width = sw;
    y++;
  }
}

static void console_clear() {
  region_fill(0, 0, sdc_screen.xres, sdc_screen.yres, sdc_screen.black);
}

static void draw_symbol(int *x, int *y, unsigned char c, struct font *font) {
  unsigned char *symbol;
  unsigned char packed_line;
  struct pixel *fb_line;
  int i, j;

  symbol = &font->fontdata[font->ysize*c];
  for (i = 0; i < font->ysize; ++i) {
    packed_line = symbol[i];
    fb_line = get_pixel(*x, *y + i);

    for (j = font->xsize - 1; j >= 0; --j) {
      if (packed_line & (1 << j)) {
	*fb_line++ = sdc_screen.white;
      } else {
	*fb_line++ = sdc_screen.black;
      }
    }
  }
  *x += font->xsize;
}

static void console_scroll(int ydelta) {
  if (ydelta <= 0) {
    return;
  }
  memcpy(get_pixel(0, 0), get_pixel(0, ydelta), sizeof(struct pixel)*sdc_screen.xres*(sdc_screen.yres - ydelta));
  region_fill(0, sdc_screen.yres - ydelta, sdc_screen.xres, ydelta, sdc_screen.black);
}

static void console_newline(struct font *font) {
  if (sdc_screen.current_y + font->ysize + LINE_INTERVAL + font->ysize > sdc_screen.yres) {
    console_scroll(font->ysize + LINE_INTERVAL);
  } else {
    sdc_screen.current_y += font->ysize + LINE_INTERVAL;
  }
  sdc_screen.current_x = 0;
}

static void console_assert_newline(struct font *font) {
  if (sdc_screen.current_x + font->xsize > sdc_screen.xres) {
    console_newline(font);
  }
}

void console_init(struct font *font) {
  struct pixel b = {0x00, 0x00, 0x00, 0x01};
  struct pixel w = {0x3f, 0x3f, 0x3f, 0x3f};
  uint32_t params[10];

  sdc_screen.initialized = 0;
  if (ipu_get_sdc_channel_params(params, 10) < 0) {
    critical_error(HW_UNINITIALIZED_IPU);
  }
  sdc_screen.fb = (struct pixel*)params[5];
  sdc_screen.xres = ((params[3] >> 12) & 0xfff) + 1;
  sdc_screen.yres = (((params[4] & 0xff) << 8) | ((params[3] >> 24) & 0xff)) + 1;
  sdc_screen.current_x = 0;
  sdc_screen.current_y = 0;
  sdc_screen.white = w;
  sdc_screen.black = b;
  console_font = font;
  console_clear();
  sdc_screen.initialized = 1;
  printf("Initialized %dx%d screen\n", sdc_screen.xres, sdc_screen.yres);
}

int console_initialized() {
  return sdc_screen.initialized;
}

struct font *console_getfont() {
  return console_font;
}

void console_setfont(struct font *f) {
  console_font = f;
}

int putchar(int c) {
  if (c == '\n') {
    console_newline(console_font);
  } else {
    console_assert_newline(console_font);
    draw_symbol(&sdc_screen.current_x, &sdc_screen.current_y, (unsigned char)c, console_font);
  }
  return (unsigned char)c;
}

int puts(const char *s) {
  unsigned char c;

  while (c = (unsigned char)(*s++)) {
    if (putchar((int)c) == EOF) {
      return EOF;
    }
  }
  return 0;
}
