#ifndef _LCOM_KBC_KEYBOARD_H_
#define _LCOM_KBC_KEYBOARD_H_

#include "../../controllers/i8042.h"

int (keyboard_subscribe)(uint8_t *bit_no);

int (keyboard_unsubscribe)();

int (kbc_read_output)(uint8_t port, uint8_t* output);

int (kbc_write_command)(uint8_t port, uint8_t commandByte);

int (restore_keyboard)();

#endif
