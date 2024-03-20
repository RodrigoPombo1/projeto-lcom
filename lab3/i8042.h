#ifndef _LCOM_i8042_H_
#define _LCOM_i8042_H_

#include <lcom/lcf.h>

// IRQ
#define KEYBOARD_IRQ 1

// Scancode
#define ESC_MAKE_CODE 0x01
#define ESC_BREAK_CODE 0x81
#define TWO_BYTES 0xE0

// KBC
#define KBC_STATUS_REG 0x64
#define KBC_IN_CMD 0x64
#define KBC_OUT_CMD 0x60

// Status Register
#define FULL_OUT_BUFFER BIT(0)
#define FULL_IN_BUFFER BIT(1)
#define TIMEOUT_ERROR BIT(6)
#define PARITY_ERROR BIT(7)

#endif
