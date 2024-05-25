#ifndef _LCOM_KBC_KEYBOARD_H_
#define _LCOM_KBC_KEYBOARD_H_

#include "../../controllers/i8042.h"
#include "../common_keyboard_and_mouse/keyboard_and_mouse.h"

int (keyboard_subscribe)(uint8_t *bit_no);

int (keyboard_unsubscribe)();

int (restore_keyboard)();

#endif
