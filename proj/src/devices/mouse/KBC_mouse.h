#ifndef _LCOM_KBC_MOUSE_H_
#define _LCOM_KBC_MOUSE_H_

#include "../../controllers/i8042.h"
#include "../common_keyboard_and_mouse/keyboard_and_mouse.h"

int (mouse_subscribe)(uint8_t *bit_no);

int (mouse_unsubscribe)();

int (mouse_build_packet)();

int (mouse_write_command)(uint8_t command);

struct mouse_ev *(mouse_detect_events)(struct packet *pp);

#endif
