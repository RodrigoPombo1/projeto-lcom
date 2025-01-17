#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "KBC_keyboard.h"

extern uint8_t scancode; // Scancode is defined on KBC.c
extern uint32_t INB_counter; // INB_counter is defined on utils.c
extern int counter; // Counter is defined on timer.c

uint8_t status = 0; // We'll read the status from the status register

int num_bytes = 1;

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
  int ipc_status;
  message msg;
  uint8_t irq_set;
  uint8_t* full_scancode = (uint8_t*)malloc(2 * sizeof(uint8_t));

  if (keyboard_subscribe(&irq_set) != 0) { // We'll mask the IRQ keyboard line to enable interruptions
    return 1;
  }

  while (scancode != ESC_BREAK_CODE) {  // The release of the ESC key terminates the program
    if (driver_receive(ANY, &msg, &ipc_status)) { // We test the function that allows us to receive request messages
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
            if (scancode == FIRST_BYTE) { // Let's evaluate if the first byte of the scancode is 0xE0 (which would mean that the scancode has 2 bytes to be read)
              full_scancode[0] = FIRST_BYTE; // Then the first byte is 0xE0
              num_bytes = 2;
              break; // We wait for the next interrupt to know what will the second byte be
            }
            else if (full_scancode[0] == FIRST_BYTE) { // This condition is only true in the interruption immediatly after the scancode is 0xE0
              full_scancode[1] = scancode; // We know the full 2 byte scancode
            }
            else { // When the scancode is not 0xE0 and 0xE0 is not the first byte, we know that the scancode only has 1 byte
              full_scancode[0] = scancode;
            }
            kbd_print_scancode(!(scancode & BIT(7)), num_bytes, full_scancode); // This function prints the scancode, by checking if it is a make or a break code, checking its size and its value 
            memset(full_scancode, 0, 2 * sizeof(uint8_t)); // We reset the scancode array
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
  uint8_t* full_scancode = (uint8_t*)malloc(2 * sizeof(uint8_t));
  while (scancode != ESC_BREAK_CODE) {
    if (util_sys_inb(KBC_STATUS_REG, &status) != 0) { // We test the function that reads the status from the status register, to check if we didn't have a communication error
      return 1;
    }
    if (kbc_read_output(KBC_OUT_BUFFER, &scancode) == 0) {
      if (scancode == FIRST_BYTE) { // Let's evaluate if the first byte of the scancode is 0xE0 (which would mean that the scancode has 2 bytes to be read)
        full_scancode[0] = FIRST_BYTE; // Then the first byte is 0xE0
        num_bytes = 2;
        continue; // we restart the cicle to read the second byte
      }
      else if (full_scancode[0] == FIRST_BYTE) { // This condition is only true in the interruption immediatly after the scancode is 0xE0
        full_scancode[1] = scancode; // We know the full 2 byte scancode
      }
      else { // When the scancode is not 0xE0 and 0xE0 is not the first byte, we know that the scancode only has 1 byte
        full_scancode[0] = scancode;
      }
      kbd_print_scancode(!(scancode & BIT(7)), num_bytes, full_scancode);// This function prints the scancode, by checking if it is a make or a break code, checking its size and its value
      memset(full_scancode, 0, 2 * sizeof(uint8_t)); // We reset the scancode array
    }
  }
  restore_keyboard();
  return 0; // After 
}

int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status, seconds = 0;
  message msg;
  uint8_t irq_set_KBC, irq_set_TIMER;
  uint8_t* full_scancode = (uint8_t*)malloc(2 * sizeof(uint8_t));

  if (timer_subscribe_int(&irq_set_TIMER) != 0) { // We'll mask the IRQ keyboard line to enable interruptions
    return 1;
  }

  if (keyboard_subscribe(&irq_set_KBC) != 0) { // We'll mask the IRQ keyboard line to enable interruptions
    return 1;
  }

  while (scancode != ESC_BREAK_CODE && seconds < n) {  // The release of the ESC key terminates the program
    if (driver_receive(ANY, &msg, &ipc_status) != 0) { // We test the function that allows us to receive request messages
      continue;
    }

    if (is_ipc_notify(ipc_status)) { // This will evaluate if the message received was a notification or a standard message
      switch (_ENDPOINT_P(msg.m_source)) { // We'll extract the process identifier from "msg" endpoint
        case HARDWARE: // If the interrupt notification is a HW one...
          if (msg.m_notify.interrupts & irq_set_KBC) { // Let's see if the interruption notification is masked...
            if (util_sys_inb(KBC_STATUS_REG, &status) != 0) { // We test the function that reads the status from the status register, to check if we didn't have a communication error
              return 1;
            }
            kbc_ih(); // We call the KBC interrupt handler, that is going to read the output from the output buffer
            if (scancode == FIRST_BYTE) { // Let's evaluate if the first byte of the scancode is 0xE0 (which would mean that the scancode has 2 bytes to be read)
              full_scancode[0] = FIRST_BYTE; // Then the first byte is 0xE0
              num_bytes = 2;
              break; // We wait for the next interrupt to know what will the second byte be
            }
            else if (full_scancode[0] == FIRST_BYTE) { // This condition is only true in the interruption immediatly after the scancode is 0xE0
              full_scancode[1] = scancode; // We know the full 2 byte scancode
            }
            else { // When the scancode is not 0xE0 and 0xE0 is not the first byte, we know that the scancode only has 1 byte
              full_scancode[0] = scancode;
            }
            kbd_print_scancode(!(scancode & BIT(7)), num_bytes, full_scancode); // This function prints the scancode, by checking if it is a make or a break code, checking its size and its value 
            memset(full_scancode, 0, 2 * sizeof(uint8_t)); // We reset the scancode array
            // We reset the timer
            seconds = 0; 
            counter = 0;
          }
          // This condition is full commented on lab2.c "timer_test_int()"
          if (msg.m_notify.interrupts & irq_set_TIMER) {
            timer_int_handler();
            if (counter % 60 == 0) {
              timer_print_elapsed_time();
              seconds++;
            }
          }
      }
    }
  }
  if (timer_unsubscribe_int() != 0) {
    return 1;
  }
  if (keyboard_unsubscribe() != 0) { // Again, (C)arnival is over...
    return 1;
  }
  if (kbd_print_no_sysinb(INB_counter) != 0) { // We print how many calls to sys_inb() the program did
    return 1;
  }
  return 0;
}
