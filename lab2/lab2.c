#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  uint8_t conf; // 8 bits (1 byte) that describe the timer configuration
  if (timer_get_conf(timer, &conf) != 0) {  // We test the function that gets the "timer" configuration and saves it on "conf"
    return 1;
  }
  if (timer_display_conf(timer, conf, field) != 0) { // We test the function that shows us a specific configuration of the timer, analyzing the variable conf
    return 1;
  }
  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  if (timer_set_frequency(timer, freq) != 0) { // We test the function that will set a new frequency "freq" to a timer "timer"
    return 1;
  };
  return 0;
}

int(timer_test_int)(uint8_t time) {
  int r, ipc_status; // To receive a notification and store its status
  message msg; // To store the notification message
  uint8_t irq_set; // To choose a IRQ

  if (timer_subscribe_int(&irq_set) != 0) { // We test the function that makes a subscription on  "irq_set", that simulates PIC 1
    return 1;
  }

  while (counter < time * 60) {  // As the system frequency is 60 Hz (3600 BPM), we need to increase the test time 60 times
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) { // We test the function that allows us to receive request messages
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { // This will evaluate if the message received was a notification or a standard message
      switch (_ENDPOINT_P(msg.m_source)) { // We'll extract the process identifier from "msg" endpoint
        case HARDWARE: // If the interrupt notification is a HW one...
          if (msg.m_notify.interrupts & irq_set) { // Let's see if the interruption notification is masked...
            timer_int_handler(); // Increase the counter
            if (counter % (int) sys_hz() == 0) { // We'll call the timer_print_elapsed_time() function when counter % sys_hz(), which is 60 Hz, is 0
              timer_print_elapsed_time();
            }
          }
          break;

        default: // To avoid bugs
          break;
      }
    }
    else { // Again, to avoid bugs... this is tricky
     
    }
  }

  if (timer_unsubscribe_int() != 0) { // We test the function that undoes a subscription on  "irq_set", that simulates PIC 1
    return 1;
  }
  return 0;
}
