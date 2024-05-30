#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#define TRANSPARENT_COLOR 0xFEFFFF

#include <lcom/lcf.h>

struct image_struct {
    uint32_t width;
    uint32_t height;
    uint32_t* color_array;
};

int (set_video_mode)(uint16_t mode);

int (build_frame_buffer)(uint16_t mode, uint8_t** video_mem);

int (get_length_frame_buffer)(void);

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* video_mem);

int (xpm_print)(xpm_map_t xpm, uint16_t x, uint16_t y, uint8_t* video_mem);

int (xpm_load_to_image)(xpm_map_t, struct image_struct* image);

int (image_load_to_frame_buffer)(struct image_struct* image, uint16_t x, uint16_t y, uint8_t* video_mem);
#endif
