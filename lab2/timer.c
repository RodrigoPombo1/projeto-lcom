#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t controlWord; // This variable will store the control word
  timer_get_conf(timer, &controlWord); // We extract the control word of the timer "timer" and pass it by reference to the previous variable
  controlWord = ((BIT(3) | BIT(2) | BIT(1) | BIT(0)) & controlWord) | TIMER_LSB_MSB; // We establish a new control word, where we assign the original LSB and activate the '11' initialization mode

  if (sys_outb(TIMER_CTRL, controlWord) != 0) { // We test the function that sends the new controlWord to the timer control register
    return 1;
  }

  uint16_t div = TIMER_FREQ / freq; // We extract the initial counter value to the variable "div"
  uint8_t LSB, MSB; 
  // We extract the LSB and MSB of "div"
  util_get_LSB(div, &LSB);
  util_get_MSB(div, &MSB);
  // We test the functions that do the '11' initialization mode on the correspondent "timer" port
  if (sys_outb(TIMER_0 + timer, LSB) != 0) { 
    return 1;
  };
  if (sys_outb(TIMER_0 + timer, MSB) != 0) {
    return 1;
  };
  return 0;
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
