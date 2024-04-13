// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "graphics.h"
#include "KBC.h"

extern uint8_t scancode; // Scancode is defined on KBC.c

uint8_t status = 0; // We'll read the status from the status register

int num_bytes = 1;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (set_video_mode(mode) != 0) {
    return 1;
  }

  sleep(delay);

  if (vg_exit() != 0) {
    printf("Text mode error");
    return 1;
  }

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  int ipc_status;
  message msg;
  uint8_t irq_set;           
  uint8_t* full_scancode = (uint8_t*)malloc(2 * sizeof(uint8_t));

  if (kbc_subscribe(&irq_set) != 0) {
    return 1;
  }

  if (build_frame_buffer(mode) != 0) {
    return 1;
  }

  if (set_video_mode(mode) != 0) {
    return 1;
  }

  if (vg_draw_rectangle(x, y, width, height, color) != 0) {
    return 1;
  }

  while (scancode != ESC_BREAK_CODE) {  // The release of the ESC key terminates the program
    if (driver_receive(ANY, &msg, &ipc_status) != 0) { // We test the function that allows us to receive request messages
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
            memset(full_scancode, 0, 2 * sizeof(uint8_t)); // We reset the scancode array
          }
      }
    }
  }

  if (vg_exit() != 0) {
    return 1;
  }

  if (kbc_unsubscribe() != 0) {
    return 1;
  }

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* To be completed */
  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__,
         mode, no_rectangles, first, step);

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
