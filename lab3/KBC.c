#include "KBC.h"

uint8_t scancode = 0; // We'll extract the scancode 
int keyboard_hook_id = 1; // We'll mask the keyboard IRQ

int (kbc_subscribe)(uint8_t *bit_no) { // Put the mask on, it's (C)arnival time!
    *bit_no = BIT(KEYBOARD_IRQ);
    return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

int (kbc_unsubscribe)() { // (C)arnival is over
    return sys_irqrmpolicy(&keyboard_hook_id);
}

int (kbc_read_output)(uint8_t port, uint8_t* output) {
  extern uint8_t status; // Status is defined on lab3.c
  uint8_t attempts = 10;
  
  while (attempts) {
    if ((status & FULL_OUT_BUFFER) != 0) { // Let's check if the output buffer is full (ready to be read)
    // Let's check if everything's alright with the status
    if ((status & TIMEOUT_ERROR) != 0){  // Timeout error?
      printf("Error: Parity error!\n");           
      return 1;
    }
    if ((status & PARITY_ERROR) != 0){  // Parity error?            
      printf("Error: Timeout error!\n");          
      return 1;
    } 

    if (util_sys_inb(port, output) != 0) { // We test the function that reads the output
      printf("Error: Could not read output!\n");
      return 1;
    }
    return 0;
    }
    tickdelay(micros_to_ticks(20000)); // To avoid miscoordinations, we interrupt the process for 20 ms
    attempts--;
  }
  return 1;
}

void (kbc_ih)() { // We test the function that invokes the Interrupt Handler (it will only read the scancode from the output buffer, in this implementation)
    if (kbc_read_output(KBC_OUT_BUFFER, &scancode) != 0) {
        printf("Error: Could not read scancode!\n");
    }
}

int (kbc_write_command)(uint8_t port, uint8_t commandByte) {
  extern uint8_t status; // Status is defined on lab3.c

  if ((status & FULL_IN_BUFFER) == 0) { // Let's check if the output buffer is full (ready to be read)
    if (sys_outb(port, commandByte) != 0) { // We test the function that reads the output
      printf("Error: Could not write commandByte!\n");
      return 1;
    }
    return 0;
  }
  tickdelay(micros_to_ticks(20000)); // To avoid miscoordinations, we interrupt the process for 20 ms
  return 1;
}

int (restore_keyboard)() {
    uint8_t cmdByte; // Command byte

    if (kbc_write_command(KBC_IN_BUFFER, KBC_READ_CMD) != 0) { // We tell the KBC that we want to read the command byte
        return 1;
    }

    if (kbc_read_output(KBC_OUT_BUFFER, &cmdByte) != 0) { // We read the command byte
        return 1;
    }

    cmdByte |= BIT(0); // We change the command byte to enable interruptions

    if (kbc_write_command(KBC_IN_BUFFER, KBC_WRITE_CMD) != 0) {  // We tell the KBC that we want to write a new command byte
        return 1;
    }

    if (kbc_write_command(KBC_WRITE_CMD, cmdByte) != 0) { // We put the new command byte on the KBC
        return 1;
    }
    return 0;
}
