#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <lcom/lcf.h>

int (set_video_mode)(uint16_t mode);

int (build_frame_buffer)(uint16_t mode);

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int (vg_draw_matrix)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);

int (xpm_print)(xpm_map_t xpm, uint16_t x, uint16_t y);

#endif
