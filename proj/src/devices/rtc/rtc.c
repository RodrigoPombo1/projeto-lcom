#include "rtc.h"

rtc_info_t rtc;

/// @brief Subscribes and enables RTC interrupts
/// @return 0 if the RTC interrupts were successfully subscribed and enabled, 1 otherwise
int (rtc_read_output)(uint8_t command, uint8_t *output) {
    if (rtc_write_command(0x70, command) != 0) {
        return 1;
    }

    if (util_sys_inb(0x71, output) != 0) {
        return 1;
    }

    return 0;
}

/// @brief Writes a command to the RTC
/// @param port The port to write the command to
/// @param command The command to write
/// @return 0 if the command was written successfully, 1 otherwise
int (rtc_write_command)(uint8_t port, uint8_t command) {
    if (sys_outb(port, command) != 0) {
        return 1;
    }
    return 0;
}

/// @brief Reads the update status of the RTC
/// @return 0 if the RTC is updating, 1 otherwise
int (rtc_read_update_status)() {
    uint8_t update_status;
    if (rtc_read_output(10, &update_status) != 0) {
        printf("Falhou a ler o output\n");
        return 1;
    }

    if ((update_status & BIT(7)) != 0) {
        return 1;
    }
    return 0;
}

/// @brief Reads the date and time from the RTC
/// @return 0 if the date and time were read successfully, 1 otherwise
int (rtc_read_date_time)() {
    // if (rtc_read_update_status() != 0) {
    //   printf("Falhou a ler o status\n");
    //   return 1;
    // }
    int update_status = 1;
    while (update_status == 1) {
        update_status = rtc_read_update_status();
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
