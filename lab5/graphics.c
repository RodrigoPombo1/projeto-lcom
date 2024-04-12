#include "graphics.h"

int (set_video_mode)(uint16_t mode) {
  struct reg86 reg;
  memset(&reg, 0, sizeof(reg));
  reg.intno = 0x10;
  reg.ax = 0x4F02;
  reg.bx = mode | BIT(14);
  if (sys_int86(&reg) != 0) {
    printf("Graphic mode error");
    return 1;
  }
  return 0;
}
