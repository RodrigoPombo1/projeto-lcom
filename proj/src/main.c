#include <lcom/lcf.h>
#include <lcom/timer.h>

// include the xpm files
#include "xpm_files/LCOM_0.xpm"
#include "xpm_files/LCOM_1.xpm"
#include "xpm_files/LCOM_2.xpm"
#include "xpm_files/LCOM_3.xpm"
#include "xpm_files/LCOM_4.xpm"
#include "xpm_files/LCOM_5.xpm"
#include "xpm_files/LCOM_6.xpm"
#include "xpm_files/LCOM_7.xpm"
#include "xpm_files/LCOM_8.xpm"
#include "xpm_files/LCOM_9.xpm"
#include "xpm_files/LCOM_-.xpm"
#include "xpm_files/LCOM_2_pontos.xpm"
#include "xpm_files/LCOM_character.xpm"
#include "xpm_files/LCOM_cursor.xpm"
#include "xpm_files/LCOM_game_over.xpm"
#include "xpm_files/LCOM_high_scores.xpm"
#include "xpm_files/LCOM_mapa.xpm"
#include "xpm_files/LCOM_menu.xpm"
#include "xpm_files/LCOM_monster.xpm"

#include "devices/keyboard/KBC_keyboard.h"
#include "devices/mouse/KBC_mouse.h"
#include "devices/graphics/graphics.h"
#include "devices/rtc/rtc.h"
#include "game_state/game_state.h"

uint8_t irq_set_timer; 
uint8_t irq_set_keyboard;
uint8_t irq_set_mouse;
// uint8_t irq_set_rtc;

uint8_t status = 0;
extern struct packet final_packet;
struct mouse_ev* mouse; 
extern rtc_info_t rtc;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g5/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g5/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (setup_game)() {
  printf("Break point 0\n");
  // Initiate 32 bits per pixel video mode
  if (set_video_mode(0x115) != 0) {
    return 1;
  }
  printf("Break point 1\n");
  // Subscribe interruptions of all necessary devices
  if (timer_subscribe_int(&irq_set_timer) != 0) {
    return 1;
  }
  printf("Break point 2\n");
  if (keyboard_subscribe(&irq_set_keyboard) != 0) {
    return 1;
  }
  printf("Break point 3\n");
  // Enable mouse data report 
  if (mouse_write_command(ENABLE_DR) != 0) {
    return 1;
  }
  if (mouse_subscribe(&irq_set_mouse) != 0) {
    return 1;
  }
  printf("Break point 4\n");
  printf("Break point 5\n");
  return 0;
}

int (close_game)() {
    // Close video mode
  if (vg_exit() != 0) {
    return 1;
  }

  // Unsubscribe interruptions of all used devices
  if (timer_unsubscribe_int() != 0) {
    return 1;
  }

  if (keyboard_unsubscribe() != 0) {
    return 1;
  }

  if (mouse_unsubscribe() != 0) {
    return 1;
  }

  // Disable mouse data report
  if (mouse_write_command(DISABLE_DR) != 0) {
    return 1;
  }

  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {
  int decimal = 0;
  int base = 1;
  enum game_state current_game_state = MAIN_MENU;
  if (rtc_read_date_time() != 0) {
    printf("Falhou\n");
  }
  while (rtc.seconds > 0) {
    int rightmost_digit = rtc.seconds & 0xF;
    decimal += rightmost_digit * base;
    base *= 10;
    rtc.seconds >>= 4;
  }
  rtc.seconds = decimal;

  decimal = 0;
  base = 1;

  while (rtc.minutes > 0) {
    int rightmost_digit = rtc.minutes & 0xF;
    decimal += rightmost_digit * base;
    base *= 10;
    rtc.minutes >>= 4;
  }
  rtc.minutes = decimal;

  decimal = 0;
  base = 1;

  while (rtc.hours > 0) {
    int rightmost_digit = rtc.hours & 0xF;
    decimal += rightmost_digit * base;
    base *= 10;
    rtc.hours >>= 4;
  }
  rtc.hours = decimal;

  decimal = 0;
  base = 1;

  while (rtc.day > 0) {
    int rightmost_digit = rtc.day & 0xF;
    decimal += rightmost_digit * base;
    base *= 10;
    rtc.day >>= 4;
  }
  rtc.day = decimal;

  decimal = 0;
  base = 1;

  while (rtc.month > 0) {
    int rightmost_digit = rtc.month & 0xF;
    decimal += rightmost_digit * base;
    base *= 10;
    rtc.month >>= 4;
  }
  rtc.month = decimal;

  decimal = 0;
  base = 1;

  while (rtc.year > 0) {
    int rightmost_digit = rtc.year & 0xF;
    decimal += rightmost_digit * base;
    base *= 10;
    rtc.year >>= 4;
  }
  rtc.year = decimal;

  printf("Segundos: %u\n", rtc.seconds);
  printf("Minutos: %u\n", rtc.minutes);
  printf("Horas: %u\n", rtc.hours);
  printf("Dia: %u\n", rtc.day);
  printf("Mes: %u\n", rtc.month);
  printf("Ano: %u\n", rtc.year);
  return 0;
  if (setup_game() != 0) {
    printf("Game could not be opened\n");
    return close_game();
  }
  printf("Break point 6\n");
  int ipc_status, r;
  message msg;

  uint8_t scancode = 0;
  uint8_t full_scancode[2];
  int num_bytes = 1;

  struct keyboard_keys_pressed keys_pressed = {false, false, false, false};

  uint8_t timer_counter = 0;
//  enum letter_pressed last_key_pressed = NO_LETTER_PRESSED;
//  bool is_key_being_pressed = false;
//  bool is_mouse_button_being_pressed = false;
  bool interrupt_received = false;
  enum interrupt_type interrupt_received_type;
//  int mouse_position_x = 0;
//  int mouse_position_y = 0;

// [TODO] initialize all arrays for every element in the game with memset
// [TODO] load xpm images into those array of colors
  // load all the xpms into image structs
  // loading all the numbers to image structs
  uint32_t number_0_color_array[32 * 16];
  struct image_struct number_0 = {32, 16, number_0_color_array};
  xpm_load_to_image((xpm_map_t) xpm_0, &number_0);

  uint32_t number_1_color_array[32 * 16];
  struct image_struct number_1 = {32, 16, number_1_color_array};
  xpm_load_to_image((xpm_map_t) xpm_1, &number_1);

  uint32_t number_2_color_array[32 * 16];
  struct image_struct number_2 = {32, 16, number_2_color_array};
  xpm_load_to_image((xpm_map_t) xpm_2, &number_2);

  uint32_t number_3_color_array[32 * 16];
  struct image_struct number_3 = {32, 16, number_3_color_array};
  xpm_load_to_image((xpm_map_t) xpm_3, &number_3);

  uint32_t number_4_color_array[32 * 16];
  struct image_struct number_4 = {32, 16, number_4_color_array};
  xpm_load_to_image((xpm_map_t) xpm_4, &number_4);

  uint32_t number_5_color_array[32 * 16];
  struct image_struct number_5 = {32, 16, number_5_color_array};
  xpm_load_to_image((xpm_map_t) xpm_5, &number_5);

  uint32_t number_6_color_array[32 * 16];
  struct image_struct number_6 = {32, 16, number_6_color_array};
  xpm_load_to_image((xpm_map_t) xpm_6, &number_6);

  uint32_t number_7_color_array[32 * 16];
  struct image_struct number_7 = {32, 16, number_7_color_array};
  xpm_load_to_image((xpm_map_t) xpm_7, &number_7);

  uint32_t number_8_color_array[32 * 16];
  struct image_struct number_8 = {32, 16, number_8_color_array};
  xpm_load_to_image((xpm_map_t) xpm_8, &number_8);

  uint32_t number_9_color_array[32 * 16];
  struct image_struct number_9 = {32, 16, number_9_color_array};
  xpm_load_to_image((xpm_map_t) xpm_9, &number_9);

  // loading the character 2 pontos to an image struct
  uint32_t character_2_pontos_color_array[12 * 4];
  struct image_struct character_2_pontos = {12, 4, character_2_pontos_color_array};
  xpm_load_to_image((xpm_map_t) xpm_2_pontos, &character_2_pontos);

  // loading the character - to an image struct
  uint32_t character_tracinho_color_array[4 * 16];
  struct image_struct character_tracinho = {4, 16, character_tracinho_color_array};
  xpm_load_to_image((xpm_map_t) xpm_traco, &character_tracinho);

  // loading the cursor to an image struct
  uint32_t cursor_color_array[32 * 32];
  struct image_struct cursor = {32, 32, cursor_color_array};
  xpm_load_to_image((xpm_map_t) cursor_xpm, &cursor);


  printf("Break point 7\n");
  uint8_t *frame_buffer = NULL;
  if (build_frame_buffer(0x115, &frame_buffer) != 0) {
    printf("Error while building the main frame buffer");
    return 1;
  }
  printf("Break point 8\n");
  uint32_t length_frame_buffer = get_length_frame_buffer();
  printf("Break point 9\n");
  printf("Length of frame buffer: %d\n", length_frame_buffer);
  memset(frame_buffer, 0, length_frame_buffer);
  printf("Break point 10\n");
  uint8_t game_frame_buffer[length_frame_buffer];
  memset(game_frame_buffer, 0, length_frame_buffer);
  printf("Break point 11\n");
  uint8_t mouse_frame_buffer[length_frame_buffer];
  memset(mouse_frame_buffer, 0, length_frame_buffer);

  bool is_start_of_screen = true;
  bool has_mouse_moved = false;
  bool was_game_frame_buffer_changed = false;
  bool close_application = false;

  // [TODO] REMOVE !!! FOR TEST PURPOSES ONLY
  image_load_to_frame_buffer(&number_0, 0, 0, frame_buffer);
  while (scancode != ESC_BREAK_CODE) {
    // printf("Breakpoint 12\n");
    if (close_application) {
        break;
    }

    // [TODO] REMOVE THIS!!!!!!!!!!!!! ONLY HERE SO IT DOESN'T GET STUCK IN AN INFINITE LOOP
    // sleep(30);
    // close_application = true;
    /////////////

    // checks if it is the start of a screen
    if (is_start_of_screen) {
        is_start_of_screen = false;
        // [TODO] switch statement for what to do in the start of each screen (load xpm to game_frame_buffer, memcpy game_frame_buffer to mouse_frame_buffer, put mouse in its position, memcpy mouse_frame_buffer to real frame buffer)
        switch (current_game_state) {
            case MAIN_MENU:
                // [TODO] load the main menu array into game_frame_buffer
                break;
            case GAME:
                timer_counter = 1; // just so it doesn't immediately update the game
                // [TODO] load the game array into game_frame_buffer
                // [TODO] set the game characters to their initial position
                break;
            case GAME_OVER:
                // [TODO] put gameover array over the game array on game_frame_buffer
                break;
            case HIGH_SCORE:
                // [TODO] load the high score array into game_frame_buffer
                break;
        }
        memcpy(game_frame_buffer, mouse_frame_buffer, length_frame_buffer);
        // [TODO] put the mouse in it's current position on the mouse_frame_buffer
        memcpy(mouse_frame_buffer, frame_buffer, length_frame_buffer);
        continue; // skip the rest of the loop
    }
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Interruption error\n");
      continue;
    }

    // checks if there was an interruption
    interrupt_received = false;

    if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & irq_set_timer) {
                    timer_counter++;
                    if (timer_counter == 60) {
                        interrupt_received = true;
                        interrupt_received_type = TIMER_SECOND;
                    } else if (timer_counter % 20 == 0) {
                        interrupt_received = true;
                        interrupt_received_type = TIMER_TICK;
                    }
                }
                if (msg.m_notify.interrupts & irq_set_keyboard) {
                    interrupt_received = true;
                    //  [TODO] get the key that was pressed and associate it with the last_key_pressed enum
                    if (util_sys_inb(KBC_STATUS_REG, &status) != 0) { // We test the function that reads the status from the status register, to check if we didn't have a communication error
                      return 1;
                    }
                    if (kbc_read_output(KBC_OUT_BUFFER, &scancode) != 0) {
                      return 1;
                    }
                    if (scancode == FIRST_BYTE) { // Let's evaluate if the first byte of the scancode is 0xE0 (which would mean that the scancode has 2 bytes to be read)
                      full_scancode[0] = FIRST_BYTE; // Then the first byte is 0xE0
                      num_bytes = 2;
                      break; // We wait for the next interrupt to know what will the second byte be
                    }
                    else if (full_scancode[0] == FIRST_BYTE) { // This condition is only true in the interruption immediatly after the scancode is 0xE0
                      full_scancode[1] = scancode; // We know the full 2 byte scancode
                    }
                    else { // When the scancode is not 0xE0 and 0xE0 is not the first byte, we know that the scancode only has 1 byte
                      full_scancode[0] = scancode;
                    }
                    switch (scancode) {
                      case 0x11:
                        printf("Tecla w pressionada\n");
                        keys_pressed.W = true;
                        break;
                      case 0x91:
                        printf("Tecla w largada\n");
                        keys_pressed.W = false;
                        break;
                      case 0x1e:
                        printf("Tecla a pressionada\n");
                        keys_pressed.A = true;
                        break;
                      case 0x9e:
                        printf("Tecla a largada\n");
                        keys_pressed.A = false;
                        break;
                      case 0x1f:
                        printf("Tecla s pressionada\n");
                        keys_pressed.S = true;
                        break;
                      case 0x9f:
                        printf("Tecla s largada\n");
                        keys_pressed.S = false;
                        break;
                      case 0x20:
                        printf("Tecla d pressionada\n");
                        keys_pressed.D = true;
                        break;
                      case 0xa0:
                        printf("Tecla d largada\n");
                        keys_pressed.D = false;
                        break;
                    }
                    memset(full_scancode, 0, 2 * sizeof(uint8_t)); // We reset the scancode array
                    //  [TODO] check if the key is being pressed and set is_key_being_pressed to true
                }
                if (msg.m_notify.interrupts & irq_set_mouse) {
                    if (util_sys_inb(KBC_STATUS_REG, &status) != 0) { // We test the function that reads the status from the status register, to check if we didn't have a communication error
                        return 1;
                    }
                    mouse_ih();
                    printf("Status: %d\n", status);
                    r = mouse_build_packet();
                    if (r == 0) {
                      break;
                    }
                    else if (r == 1) {
                      // Seria acabar a execução do programa
                      printf("Mouse error");
                    }
                    printf("Breakpoint 20");
                    mouse = mouse_detect_events(&final_packet);
                    printf("Breakpoint 21");
                    if (mouse->type == LB_PRESSED) {
                      printf("Botão esquerdo pressionado");
                    }
                    else if (mouse->type == LB_RELEASED) {
                      printf("Botão esquerdo largado");
                    }
                    printf("Posição do rato:\n");
                    printf("Delta x: %d", mouse->delta_x);
                    printf("Delta y: %d", mouse->delta_y);
                    interrupt_received = true;
                    //  [TODO] get the mouse deviation and add it to the mouse position
                    //  [TODO] get if mouse has pressed m1 (check if left mouse button was pressed) update is_mouse_button_being_pressed accordingly
                }

        }
    }
    // [TODO] remove this continue (only exists for testing purposes)
    continue;
    //////////////
    // if key if key is still being pressed, put interrupt from keyboard to true?

    // [TODO] check if interrupt was useful (depending on gamestate) if not, continue
    // in case there wasn't an interruption just continue (happens when it's a timer tick)
    if (!interrupt_received) {
        continue;
    }
    was_game_frame_buffer_changed = false; // pointer will be passed in the following functions inside the switch statement
//  [TODO] probably a switch statement between the different game states // handling each game state will mean a pointer to the game state buffer will be passed in the function
    switch(current_game_state) {
        case MAIN_MENU:
//          [TODO] function to handle interrupts while in the main menu state
            break;
        case GAME:
//          [TODO] function to handle interrupts while in the game state
            break;
        case GAME_OVER:
//          [TODO] function to handle interrupts while in the game over state (where nothing moves only quit can be pressed)
            break;
        case HIGH_SCORE:
//          [TODO] function to handle interrupts while in the the high score state
            break;
    }

    // if nothing has changed
    if (!was_game_frame_buffer_changed && !has_mouse_moved) {
        continue;
    }

    if (was_game_frame_buffer_changed) {
        memcpy(game_frame_buffer, mouse_frame_buffer, length_frame_buffer);
    }
    // [TODO] draw mouse on mouse_frame_buffer from the mouse position
    memcpy(mouse_frame_buffer, frame_buffer, length_frame_buffer);
  }

  if (close_game() != 0) {
    printf("Error in closing the game");
    return 1;
  }

  return 0;
}
