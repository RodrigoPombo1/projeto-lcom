#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"

extern uint8_t scancode; // Scancode is defined on keyboard.c
extern uint32_t INB_counter; // INB_counter is defined on utils.c

uint8_t status = 0; // We'll read the status from the status register

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  int r, ipc_status;
  message msg;
  uint8_t irq_set;

  if (keyboard_subscribe(&irq_set) != 0) { // We'll mask the IRQ keyboard line to enable interruptions
    return 1;
  }

  while (scancode != ESC_BREAK_CODE) {  // The release of the ESC key terminates the program
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) { // We test the function that allows us to receive request messages
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { // This will evaluate if the message received was a notification or a standard message
      switch (_ENDPOINT_P(msg.m_source)) { // We'll extract the process identifier from "msg" endpoint
        case HARDWARE: // If the interrupt notification is a HW one...
          if (msg.m_notify.interrupts & irq_set) { // Let's see if the interruption notification is masked...
            if (util_sys_inb(KBC_STATUS_REG, &status) != 0) { // We test the function that reads the status from the status register, to check if we didn't have a communication error
              return 1;
            }
            kbc_ih(); // We call the KBC interrupt handler, that is going to read the output from the output buffer
            kbd_print_scancode(!(scancode & BIT(7)), scancode == TWO_BYTES ? 2 : 1, &scancode); // This function prints the scancode, by checking if it is a make or a break code, checking its size and its value 
            // !! IMPORTANT: Still doesn't work for 2 byte long scancodes
          }
      }
    }
  }
  if (keyboard_unsubscribe() != 0) { // Again, (C)arnival is over...
    return 1;
  }
  if (kbd_print_no_sysinb(INB_counter) != 0) { // We print how many calls to sys_inb() the program did
    return 1;
  }
  return 0;
}

int(kbd_test_poll)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
