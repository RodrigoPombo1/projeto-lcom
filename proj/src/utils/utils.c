#include <lcom/lcf.h>

#include <stdint.h>

/// @brief Function that reads the 8 list significant bits of a 16 bit value
/// @param val 16 bit value
/// @param lsb Pointer to the variable where the 8 least significant bits of val will be stored
/// @return 0 upon success, non-zero otherwise
int (util_get_LSB)(uint16_t val, uint8_t *lsb) {
    *lsb = val & 0xFF; // We extract the last 8 bits (1 byte) of val (div)
    return 0;
}

/// @brief Function that reads the 8 most significant bits of a 16 bit value
/// @param val 16 bit value
/// @param msb Pointer to the variable where the 8 most significant bits of val will be stored
/// @return 0 upon success, non-zero otherwise
int (util_get_MSB)(uint16_t val, uint8_t *msb) {
    *msb = val >> 8; // We extract the first 8 bits (1 byte) of val (div)
    return 0;
}

/// @brief Function that reads a byte from a given port
/// @param port Port to read from
/// @param value Pointer to the variable where the byte read from the port will be stored
/// @return 0 upon success, non-zero otherwise
int (util_sys_inb)(int port, uint8_t *value) {
    uint32_t value_32; // Variable with the return 32 bits (4 bytes) value of sys_inb()
    if (sys_inb(port, &value_32) != 0) {
        return 1;
    }
    *value = 0xFF & value_32; // We only want the 8 LSB of the sys_inb() second argument value
    return 0;
}
