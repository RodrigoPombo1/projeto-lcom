#ifndef _LCOM_i8042_H_
#define _LCOM_i8042_H_

#include <lcom/lcf.h>

// IRQ
#define KEYBOARD_IRQ 1

// Scancode / Command Byte
#define ESC_BREAK_CODE 0x81
#define FIRST_BYTE 0xE0

// KBC
#define KBC_STATUS_REG 0x64
#define KBC_IN_BUFFER 0x64
#define KBC_OUT_BUFFER 0x60

// Command Byte
#define KBC_READ_CMD 0x20
#define KBC_WRITE_CMD 0x60

// Verifications
#define FULL_OUT_BUFFER BIT(0)
#define FULL_IN_BUFFER BIT(1)
#define TIMEOUT_ERROR BIT(6)
#define PARITY_ERROR BIT(7)

#endif
