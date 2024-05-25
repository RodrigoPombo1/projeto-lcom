#ifndef _KEYBOARD_AND_MOUSE_H_
#define _KEYBOARD_AND_MOUSE_H_

#include "../../controllers/i8042.h"

int (kbc_read_output)(uint8_t port, uint8_t* output);
int (kbc_write_command)(uint8_t port, uint8_t commandByte);

#endif
