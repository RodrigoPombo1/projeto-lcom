#ifndef _LCOM_i8042_H_
#define _LCOM_i8042_H_

#include <lcom/lcf.h>

// IRQ
#define KEYBOARD_IRQ 1
#define MOUSE_IRQ 12

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

// Mouse
#define CONTROL_BIT BIT(3)
#define LEFT_BUTTON BIT(0)
#define RIGHT_BUTTON BIT(1)
#define MIDDLE_BUTTON BIT(2)
#define X_SIGN_BIT BIT(4)
#define Y_SIGN_BIT BIT(5)
#define X_OVERFLOW BIT(6)
#define Y_OVERFLOW BIT(7)
#define MOUSE_BYTE 0xD4

// Mouse data report
#define DISABLE_DR 0xF5
#define ENABLE_DR 0XF4
#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

// Extra
#define MAX_16_BITS 0x8000 

#endif
