#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <lcom/lcf.h>

int (set_video_mode)(uint16_t mode);

int (build_frame_buffer)(uint16_t mode);

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

#endif
