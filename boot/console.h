#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#include "types.h"
#include "font.h"

void console_init();
int console_initialized();
struct font *console_getfont();
void confole_setfont(struct font *f);

#endif // __CONSOLE_H__
