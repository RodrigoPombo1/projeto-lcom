#include "rtc.h"

int rtc_hook_id = 5;
rtc_info_t rtc;

int (rtc_subscribe)(uint8_t* bit_no) {
  *bit_no = BIT(rtc_hook_id);
  return sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hook_id);
}

int (rtc_unsubscribe)() {
  return sys_irqrmpolicy(rtc_hook_id);
}

int (rtc_read_output)(uint8_t command, uint8_t* output) {
  if (rtc_write_command(0x70, command) != 0) {
    return 1;
  }

  if (util_sys_inb(0x71, output) != 0) {
    return 1;
  }

  return 0;
}

int (rtc_write_command)(uint8_t port, uint8_t command) {
  if (sys_outb(port, command) != 0) {
    return 1;
  }
  return 0;
}

int (rtc_read_update_status)() {
  uint8_t update_status;
  if (rtc_read_output(10, &update_status) != 0) {
    return 1;
  }

  if (update_status & BIT(7) == 0) {
    return 1;
  }
  return 0;
}

int (rtc_read_date_time)() {
  if (rtc_read_update_status() != 0) {
    return 1;
  }

  uint8_t output;

  if (rtc_read_output(0, &output) != 0) {
    return 1;
  }
  rtc.seconds = output;

  if (rtc_read_output(2, &output) != 0) {
    return 1;
  }
  rtc.minutes = output;

  if (rtc_read_output(4, &output) != 0) {
    return 1;
  }
  rtc.hours = output;

  if (rtc_read_output(7, &output) != 0) {
    return 1;
  }
  rtc.day = output;

  if (rtc_read_output(8, &output) != 0) {
    return 1;
  }
  rtc.month = output;
  
  if (rtc_read_output(9, &output) != 0) {
    return 1;
  }
  rtc.year = output;

  return 0;
}
