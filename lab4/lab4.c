// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "KBC_mouse.h"

extern struct packet final_packet; // Defined in KBC_mouse.c
uint8_t status = 0;
extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
    int ipc_status, r;
    message msg;
    uint8_t irq_set;

    // Usar esta função ao invés da de baixo para os testes passarem

    if (mouse_enable_data_reporting() != 0) {
      return 1;
    }

    // if (mouse_write_command(ENABLE_DR) != 0) {
    //   return 1;
    // }

    if (mouse_subscribe(&irq_set) != 0) {
      return 1;
    }

    while (cnt != 0) {
      if (driver_receive(ANY, &msg, &ipc_status) != 0) {
        continue;
      }

      if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
            if (msg.m_notify.interrupts & irq_set) {
              if (util_sys_inb(KBC_STATUS_REG, &status) != 0) { // We test the function that reads the status from the status register, to check if we didn't have a communication error
                printf("Could not read status");
                return 1;
              }
              mouse_ih();
              r = mouse_build_packet();
              if (r == 0) {
                break;
              }
              else if (r == 1) {
                return 1;
              }
              else if (r == 2) {
                mouse_print_packet(&final_packet);
              }
              cnt--;
            }
        }
      }
    }
    if (mouse_write_command(DISABLE_DR) != 0) {
      return 1;
    }

    if (mouse_unsubscribe() != 0) {
      return 1;
    }
    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  int ipc_status, r;
  message msg;
  uint8_t seconds = 0;
  uint8_t mouse_irq_set = 0, timer_irq_set = 0; 

  // if (mouse_write_command(ENABLE_DR) != 0) {
  //   return 1;
  // }

  if (mouse_enable_data_reporting() != 0) {
    return 1;
  }

  if (mouse_subscribe(&mouse_irq_set) != 0) {
    return 1;
  }

  if (timer_subscribe_int(&timer_irq_set) != 0) {
    return 1;
  }

  while (seconds < idle_time) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
            if (counter % sys_hz() == 0) {
              seconds++;
            }
          }
          if (msg.m_notify.interrupts & mouse_irq_set) {
            if (util_sys_inb(KBC_STATUS_REG, &status) != 0) { // We test the function that reads the status from the status register, to check if we didn't have a communication error
              printf("Could not read status");
              return 1;
            }
            mouse_ih();
            r = mouse_build_packet();
            if (r == 0) {
              break;
            }
            else if (r == 1) {
              return 1;
            }
            else if (r == 2) {
              mouse_print_packet(&final_packet);
            }
            seconds = 0;
            counter = 0;
          }
      }
    }
  }
  if (mouse_write_command(DISABLE_DR) != 0) {
    return 1;
  }

  if (timer_unsubscribe_int() != 0) {
    return 1;
  }

  if (mouse_unsubscribe() != 0) {
    return 1;
  }
  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
