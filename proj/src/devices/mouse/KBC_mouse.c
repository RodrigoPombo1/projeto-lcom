#include "KBC_mouse.h"

uint8_t byte, packet[3]; // We'll extract the mouse bytes
int byte_count = 0, mouse_hook_id = 2; // We'll mask the mouse IRQ
struct packet final_packet;
struct mouse_ev mouse_struct;
struct mouse_ev *mouse = &mouse_struct;

bool was_left_button_pressed = false;

int (mouse_subscribe)(uint8_t *bit_no) { // Put the mask on, it's (C)arnival time!
    *bit_no = BIT(mouse_hook_id);
    return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe)() { // (C)arnival is over
    return sys_irqrmpolicy(&mouse_hook_id);
}

int (mouse_build_packet)() {
//  printf("Byte: %u\n", byte);
//  printf("Byte count: %d\n", byte_count);
    if (byte_count == 0 && ((byte & CONTROL_BIT) == 0)) {
        printf("Build error");
        return 1;
    }
    packet[byte_count] = byte;
    byte_count++;

    if (byte_count == 3) {
        for (int i = 0; i < 3; i++) {
            final_packet.bytes[i] = packet[i];
        }

        final_packet.lb = packet[0] & LEFT_BUTTON;
        final_packet.mb = packet[0] & MIDDLE_BUTTON;
        final_packet.rb = packet[0] & RIGHT_BUTTON;
        final_packet.x_ov = packet[0] & X_OVERFLOW;
        final_packet.y_ov = packet[0] & Y_OVERFLOW;

        if (packet[0] & X_SIGN_BIT) {
            final_packet.delta_x = 0xFF00 | packet[1];
        } else {
            final_packet.delta_x = packet[1];
        }

        if (packet[0] & Y_SIGN_BIT) {
            final_packet.delta_y = 0xFF00 | packet[2];
        } else {
            final_packet.delta_y = packet[2];
        }
        byte_count = 0;
        return 2;
    }
    return 0;
}

void (mouse_ih)() {
    if (kbc_read_output(KBC_OUT_BUFFER, &byte) != 0) {
        printf("Error: Could not read byte!\n");
    }
}

int (mouse_write_command)(uint8_t command) {
    uint8_t request_report = 0;
    uint8_t attempts = 10;
    while (attempts && request_report != ACK) {

        if (kbc_write_command(KBC_IN_BUFFER, MOUSE_BYTE) != 0) {
            return 1;
        }

        if (kbc_write_command(KBC_WRITE_CMD, command) != 0) {
            return 1;
        }

        tickdelay(micros_to_ticks(20000));
        attempts--;

        if (util_sys_inb(KBC_OUT_BUFFER, &request_report) != 0) {
            return 1;
        }

        if (request_report == ACK) {
            // printf("ACK");
            return 0;
        } else if (request_report == NACK) {
            printf("NACK");
        } else if (request_report == ERROR) {
            printf("ERROR");
        }
    }

    return 1;
}

struct mouse_ev *(mouse_detect_events)(struct packet *pp) {
    mouse->type = MOUSE_MOV;
//  printf("Breakpoint 30\n");
    bool was_type_set;
//  printf("Breakpoint 50\n");
    if (pp->lb && !was_left_button_pressed) {
//    printf("Breakpoint 40\n");
        mouse->type = LB_PRESSED;
        was_left_button_pressed = true;
        was_type_set = true;
    }
//printf("Breakpoint 31\n");
    if (!pp->lb && was_left_button_pressed) {
//    printf("Breakpoint 41\n");
        mouse->type = LB_RELEASED;
        was_left_button_pressed = false;
        was_type_set = true;
    }
//printf("Breakpoint 32\n");
    mouse->delta_x = pp->delta_x;
    mouse->delta_y = pp->delta_y;
//printf("Breakpoint 36\n");
    if ((mouse->delta_x != 0 || mouse->delta_y != 0) && !was_type_set) {
        mouse->type = MOUSE_MOV;
        was_type_set = true;
    }
    return mouse;
}
