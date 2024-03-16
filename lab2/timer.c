#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  u_int8_t RBC = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer)); //The read-back command specifies itself. Reads the programmed mode and makes the selection of Counter 0
  if (sys_outb(TIMER_CTRL, RBC) != 0) { // We send the RBC value to the timer control register
    return 1;
  }
  if (util_sys_inb(TIMER_0 + timer, st) != 0) { // We test the function that reads the timer configuration
    return 1;
  }
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val timer_data; // Variable to be analyzed
  
  switch (field) { // Let's see what field is being requested
    case tsf_all:
      timer_data.byte = st;
      break;

    case tsf_initial:
      timer_data.in_mode = (st & TIMER_LSB_MSB) >> 4;
      break;

    case tsf_mode:
      timer_data.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;
      break;

    case tsf_base:
      timer_data.bcd = st & TIMER_BCD;
      break;

    default:
      return 1;
  }

  if (timer_print_config(timer, field, timer_data) != 0) { // We test the function that prints the requested configuration info
    return 1;
  }
  return 0;
}
