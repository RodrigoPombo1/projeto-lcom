#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <lcom/lcf.h>

#define RTC_IRQ 8

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} rtc_info_t;

int (rtc_read_output)(uint8_t command, uint8_t *output);

int (rtc_write_command)(uint8_t port, uint8_t command);

int (rtc_read_update_status)();

int (rtc_read_date_time)();

#endif
