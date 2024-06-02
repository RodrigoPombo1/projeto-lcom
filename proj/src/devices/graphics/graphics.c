#include "graphics.h"

//static char* video_mem;

static unsigned h_res;
static unsigned v_res;
static unsigned bits_per_pixel;
static unsigned bytes_per_pixel;
static unsigned vram_size;

static vbe_mode_info_t mode_info;

/// @brief Sets the video mode
/// @param mode The mode to set
/// @return 0 if successful, 1 otherwise
int (set_video_mode)(uint16_t mode) {
    struct reg86 reg;
    memset(&reg, 0, sizeof(reg));
    reg.intno = 0x10;
    reg.ax = 0x4F02;
    reg.bx = mode | BIT(14);
    if (sys_int86(&reg) != 0) {
        return 1;
    }
    return 0;
}

/// @brief Changes the pointer to the address of the frame buffer for the specified video mode
/// @param mode The mode to set
/// @param video_mem The pointer to the address of the frame buffer
/// @return 0 if successful, 1 otherwise
int (build_frame_buffer)(uint16_t mode, uint8_t **video_mem) {
    memset(&mode_info, 0, sizeof(mode_info));
    if (vbe_get_mode_info(mode, &mode_info) != 0) {
        return 1;
    }

    h_res = mode_info.XResolution;
    v_res = mode_info.YResolution;
    bits_per_pixel = mode_info.BitsPerPixel;
    bytes_per_pixel = (bits_per_pixel + 7) / 8;

    vram_size = h_res * v_res * bytes_per_pixel;

    struct minix_mem_range address;
    address.mr_base = mode_info.PhysBasePtr;
    address.mr_limit = address.mr_base + vram_size;

    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &address) != OK) {
        return 1;
    }

    *video_mem = vm_map_phys(SELF, (void *) address.mr_base, vram_size);

    return 0;
}

/// @brief Gets the length of the frame buffer
/// @return The length of the frame buffer
int (get_length_frame_buffer)(void) {
    return vram_size;
}

/// @brief Draws the pixel at the specified coordinates in specified frame buffer
/// @param x The x coordinate of the pixel
/// @param y The y coordinate of the pixel
/// @param color The color of the pixel
/// @param video_mem The address of the frame buffer
/// @return 0 if successful, 1 otherwise
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *video_mem) {
    if (x >= h_res || y >= v_res) {
        return 1;
    }
    uint8_t *pixel_pos = video_mem + ((y * h_res + x) * bytes_per_pixel);
    for (uint i = 0; i < bytes_per_pixel; i++) {
        memcpy(pixel_pos, &color, bytes_per_pixel);
    }

    return 0;
}


int (xpm_print)(xpm_map_t xpm, uint16_t x, uint16_t y, uint8_t *video_mem) {
    xpm_image_t img;

    uint8_t *pixels = xpm_load(xpm, XPM_INDEXED, &img);
    int image_h_res = img.width;
    int image_v_res = img.height;
    for (int i = 0; i < image_v_res; i++) {
        for (int j = 0; j < image_h_res; j++) {
            if (vg_draw_pixel(x + j, y + i, *pixels, video_mem) != 0) {
                return 1;
            }
            pixels++;
        }
    }

    return 0;
}

/// @brief Loads xpm file to image struct
/// @param xpm The xpm file to load
/// @param image The image struct to store the xpm file
/// @return 0 if successful, 1 otherwise
int (xpm_load_to_image)(xpm_map_t xpm, struct image_struct *image) {
    xpm_image_t img;

    image->color_array = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &img);

    return 0;
}

/// @brief Loads image struct to frame buffer
/// @param image The image struct to load
/// @param x The x coordinate of the image
/// @param y The y coordinate of the image
/// @param video_mem The address of the frame buffer
/// @return 0 if successful, 1 otherwise
int (image_load_to_frame_buffer)(struct image_struct *image, uint16_t x, uint16_t y, uint8_t *video_mem) {
    int image_h_res = image->width;
    int image_v_res = image->height;
    int counter = 0;
    for (int i = 0; i < image_v_res; i++) {
        for (int j = 0; j < image_h_res; j++) {
            if (!(image->color_array[counter] == TRANSPARENT_COLOR)) {
                vg_draw_pixel(x + j, y + i, image->color_array[counter], video_mem);
            }
            counter++;
        }
    }
    return 0;
}
