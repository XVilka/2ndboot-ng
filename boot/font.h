#ifndef __FONT_H__
#define __FONT_H__

struct font {
  unsigned char *fontdata;
  int xsize;
  int ysize;
};

extern struct font font_8x8;

void font_init();
#endif // __FONT_H__
