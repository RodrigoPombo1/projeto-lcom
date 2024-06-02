#include "KBC_keyboard.h"

uint8_t scancode = 0; // We'll extract the scancode 
int keyboard_hook_id = 1; // We'll mask the keyboard IRQ

/// @brief Subscribes and enables Keyboard interrupts
/// @param bit_no The bit number to be set in the mask
/// @return 0 if successful, 1 otherwise
int (keyboard_subscribe)(uint8_t *bit_no) { // Put the mask on, it's (C)arnival time!
    *bit_no = BIT(keyboard_hook_id);
    return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

/// @brief Unsubscribes Keyboard interrupts
/// @return 0 if successful, 1 otherwise
int (keyboard_unsubscribe)() { // (C)arnival is over
    return sys_irqrmpolicy(&keyboard_hook_id);
}

/// @brief Handles the Keyboard Interrupts
void (kbc_ih)() { // We test the function that invokes the Interrupt Handler (it will only read the scancode from the output buffer, in this implementation)
    if (kbc_read_output(KBC_OUT_BUFFER, &scancode) != 0) {
        printf("Error: Could not read scancode!\n");
    }
}

/// @brief Restores the keyboard to default configuration
/// @return 0 if successful, 1 otherwise
int (restore_keyboard)() {
    uint8_t cmdByte; // Command byte

    if (kbc_write_command(KBC_IN_BUFFER, KBC_READ_CMD) != 0) { // We tell the KBC that we want to read the command byte
        return 1;
    }

    if (kbc_read_output(KBC_OUT_BUFFER, &cmdByte) != 0) { // We read the command byte
        return 1;
    }

    cmdByte |= BIT(0); // We change the command byte to enable interruptions

    if (kbc_write_command(KBC_IN_BUFFER, KBC_WRITE_CMD) !=
        0) {  // We tell the KBC that we want to write a new command byte
        return 1;
    }

    if (kbc_write_command(KBC_WRITE_CMD, cmdByte) != 0) { // We put the new command byte on the KBC
        return 1;
    }
    return 0;
}
