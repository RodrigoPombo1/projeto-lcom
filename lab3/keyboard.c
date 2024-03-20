#include "keyboard.h"
#include "KBC.h"

uint8_t scancode = 0; // We'll extract the scancode 
int keyboard_hook_id = 1; // We'll mask the keyboard IRQ

int (keyboard_subscribe)(uint8_t *bit_no) { // Put the mask on, it's (C)arnival time!
    *bit_no = BIT(keyboard_hook_id);
    return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

int (keyboard_unsubscribe)() { // (C)arnival is over
    return sys_irqrmpolicy(&keyboard_hook_id);
}

void (kbc_ih)() { // We test the function that invokes the Interrupt Handler (it will only read the scancode from the output buffer, in this implementation)
    if (read_output(KBC_OUT_CMD, &scancode) != 0) {
        printf("Error: Could not read scancode!\n");
    }
}
