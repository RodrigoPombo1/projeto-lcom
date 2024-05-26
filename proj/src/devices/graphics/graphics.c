#include "graphics.h"

//static char* video_mem;

static unsigned h_res;	        
static unsigned v_res;	        
static unsigned bits_per_pixel;
static unsigned bytes_per_pixel;
static unsigned vram_size; 

static vbe_mode_info_t mode_info;

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

int (build_frame_buffer)(uint16_t mode, uint8_t* video_mem) {
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

  video_mem = vm_map_phys(SELF, (void*) address.mr_base, vram_size);

  return 0;
}

int (get_length_frame_buffer)(void) {
    return vram_size;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* video_mem) {
  uint8_t* pixel_pos = video_mem + ((y * h_res + x) * bytes_per_pixel);
  for (uint i = 0; i < bytes_per_pixel; i++) {
    memcpy(pixel_pos, &color, bytes_per_pixel);
  }

  return 0;
}

int (xpm_print)(xpm_map_t xpm, uint16_t x, uint16_t y, uint8_t* video_mem) {
  xpm_image_t img;

  uint8_t* pixels = xpm_load(xpm, XPM_INDEXED, &img);

  for (int i = 0; i < img.height; i++) {
    for (int j = 0; j < img.width; j++) {
      if (vg_draw_pixel(x + j, y + i, *pixels, video_mem) != 0) {
        return 1;
      }
      pixels++;
    }
  }

  return 0;
}
