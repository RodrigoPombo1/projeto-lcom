#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t value_32; // Variable with the return 32 bits (4 bytes) value of sys_inb()
  int ret = sys_inb(port, &value_32); // Variabe ret is returned to verify if the function worked correctly
  *value = 0xFF & value_32; // We only want the 8 LSB of the sys_inb() second argument value
  return ret;
}
