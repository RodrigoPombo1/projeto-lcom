#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include "i8042.h"

int (mouse_subscribe)(uint8_t *bit_no);

int (mouse_unsubscribe)();

int (mouse_build_packet)();

int (kbc_read_output)(uint8_t port, uint8_t* output);

int (mouse_write_command)(uint8_t command);

int (kbc_write_command)(uint8_t port, uint8_t commandByte);

#endif
