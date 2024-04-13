#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include "i8042.h"

int (kbc_subscribe)(uint8_t *bit_no);

int (kbc_unsubscribe)();

void (kbc_ih)();

int (kbc_read_output)(uint8_t port, uint8_t* output);

int (kbc_write_command)(uint8_t port, uint8_t commandByte);

int (restore_keyboard)();

#endif
