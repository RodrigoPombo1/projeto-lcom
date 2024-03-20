#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include "i8042.h"

int (keyboard_subscribe)(uint8_t *bit_no);

int (keyboard_unsubscribe)();

void (kbc_ih)();

int (restore_keyboard)();

#endif
