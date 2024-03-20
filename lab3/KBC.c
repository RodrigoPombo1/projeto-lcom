#include "KBC.h"

int (kbc_read_output)(uint8_t port, uint8_t* output) {
  extern uint8_t status; // Status is defined on lab3.c

  if ((status & FULL_OUT_BUFFER) != 0) { // Let's check if the output buffer is full (ready to be read)
    if (util_sys_inb(port, output) != 0) { // We test the function that reads the output
      printf("Error: Could not read output!\n");
      return 1;
    }
    // Let's check if everything's alright with the status
    if ((status & TIMEOUT_ERROR) != 0){  // Timeout error?
      printf("Error: Parity error!\n");           
      return 1;
    }
    if ((status & PARITY_ERROR) != 0){  // Parity error?            
      printf("Error: Timeout error!\n");          
      return 1;
    } 
    return 0;
  }
  tickdelay(micros_to_ticks(20000)); // To avoid miscoordinations, we interrupt the process for 20 ms
  return 1;
}

int (kbc_write_command)(uint8_t port, uint8_t commandByte) {
  extern uint8_t status; // Status is defined on lab3.c~

  if ((status & FULL_IN_BUFFER) == 0) { // Let's check if the output buffer is full (ready to be read)
    if (sys_outb(port, commandByte) != 0) { // We test the function that reads the output
      printf("Error: Could not write commandByte!\n");
      return 1;
    }
    return 0;
  }
  tickdelay(micros_to_ticks(20000)); // To avoid miscoordinations, we interrupt the process for 20 ms
  return 1;
}
