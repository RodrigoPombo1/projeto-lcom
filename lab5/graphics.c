#include "graphics.h"

static char* video_mem;

static unsigned h_res;	        
static unsigned v_res;	        
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

int (build_frame_buffer)(uint16_t mode) {
  memset(&mode_info, 0, sizeof(mode_info));
  if (vbe_get_mode_info(mode, &mode_info) != 0) {
    return 1;
  }

  h_res = mode_info.XResolution;
  v_res = mode_info.YResolution;
  bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;

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

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  char* pixel_pos = video_mem + ((y * h_res + x) * bytes_per_pixel);
  for (uint i = 0; i < bytes_per_pixel; i++) {
    memcpy(pixel_pos, &color, bytes_per_pixel);
  }

  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (uint16_t i = 0; i < len && x + i < h_res; i++) {
    vg_draw_pixel(x + i, y, color);
  }

  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (uint16_t i = 0; i < height && y + i < v_res; i++) {
    vg_draw_hline(x, y + i, width, color);
  }

  return 0;
}
