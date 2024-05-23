#include <lcom/lcf.h>
#include <lcom/timer.h>

#include "devices/keyboard/KBC_keyboard.h"
#include "devices/mouse/KBC_mouse.h"
#include "devices/graphics/graphics.h"

uint8_t irq_set_timer; 
uint8_t irq_set_keyboard;
uint8_t irq_set_mouse;
// uint8_t irq_set_rtc;

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

int (setup_game)() {
  // Initiate 32 bits per pixel video mode
  if (set_video_mode(0x14C) != 0) { 
    return 1;
  }

  // Subscribe interruptions of all necessary devices
  if (timer_subscribe_int(&irq_set_timer) != 0) {
    return 1;
  }

  if (keyboard_subscribe(&irq_set_keyboard) != 0) {
    return 1;
  }

  if (mouse_subscribe(&irq_set_mouse) != 0) {
    return 1;
  }

  // Enable mouse data report 
  if (mouse_write_command(ENABLE_DR) != 0) {
    return 1;
  }

  // Close video mode
  if (vg_exit() != 0) {
    return 1;
  }

  // Run the game
  if (run() != 0) {
    return 1;
  }

  // Unsubscribe interruptions of all used devices
  if (timer_unsubscribe_int() != 0) {
    return 1;
  }

  if (keyboard_unsubscribe() != 0) {
    return 1;
  }

  if (mouse_unsubscribe() != 0) {
    return 1;
  }

  // Disable mouse data report
  if (mouse_write_command(DISABLE_DR) != 0) {
    return 1;
  }

  return 0;
}

int (run)() {
  int ipc_status;
  message msg;

  while (true) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Interruption error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_timer) {
            // to be completed
          }
          if (msg.m_notify.interrupts & irq_set_keyboard) {
            // to be completed
          }
          if (msg.m_notify.interrupts & irq_set_mouse) {
            // to be completed
          }
          // if (msg.m_notify.interrupts & irq_set_timer) {
          //   // to be completed
          // }
      }
    }
  }

  return 0;
}