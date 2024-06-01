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
#include "game_state/game_state.h"

uint8_t irq_set_timer; 
uint8_t irq_set_keyboard;
uint8_t irq_set_mouse;
// uint8_t irq_set_rtc;

uint8_t status = 0;
extern struct packet final_packet;
struct mouse_ev* mouse;
// [TODO] POR NO SITIO ONDE VAI ESTAR O RTC TAMBEM!!!!!!!!
//extern rtc_info_t rtc;
/////////////////
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
//  lcf_trace_calls("/home/lcom/labs/g5/proj/src/trace.txt");
//  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
//  lcf_log_output("/home/lcom/labs/g5/proj/src/output.txt");
//  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

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
//  printf("Break point 0\n");
  // Initiate 32 bits per pixel video mode
  if (set_video_mode(0x115) != 0) {
    return 1;
  }
  // if (timer_set_frequency(0, 20) != 0) {
  //   return 1;
  // }
//  printf("Break point 1\n");
  // Subscribe interruptions of all necessary devices
  if (timer_subscribe_int(&irq_set_timer) != 0) {
    return 1;
  }
//  printf("Break point 2\n");
  if (keyboard_subscribe(&irq_set_keyboard) != 0) {
    return 1;
  }
//  printf("Break point 3\n");
  // Enable mouse data report 
  if (mouse_write_command(ENABLE_DR) != 0) {
    return 1;
  }
  if (mouse_subscribe(&irq_set_mouse) != 0) {
    return 1;
  }
//  printf("Break point 4\n");
//  printf("Break point 5\n");
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
  // [TODO] SECÇÃO RTC, é preciso por no sitio correto
//  int decimal = 0;
//  int base = 1;
//  if (rtc_read_date_time() != 0) {
//    printf("Falhou\n");
//  }
//  while (rtc.seconds > 0) {
//    int rightmost_digit = rtc.seconds & 0xF;
//    decimal += rightmost_digit * base;
//    base *= 10;
//    rtc.seconds >>= 4;
//  }
//  rtc.seconds = decimal;
//
//  decimal = 0;
//  base = 1;
//
//  while (rtc.minutes > 0) {
//    int rightmost_digit = rtc.minutes & 0xF;
//    decimal += rightmost_digit * base;
//    base *= 10;
//    rtc.minutes >>= 4;
//  }
//  rtc.minutes = decimal;
//
//  decimal = 0;
//  base = 1;
//
//  while (rtc.hours > 0) {
//    int rightmost_digit = rtc.hours & 0xF;
//    decimal += rightmost_digit * base;
//    base *= 10;
//    rtc.hours >>= 4;
//  }
//  rtc.hours = decimal;
//
//  decimal = 0;
//  base = 1;
//
//  while (rtc.day > 0) {
//    int rightmost_digit = rtc.day & 0xF;
//    decimal += rightmost_digit * base;
//    base *= 10;
//    rtc.day >>= 4;
//  }
//  rtc.day = decimal;
//
//  decimal = 0;
//  base = 1;
//
//  while (rtc.month > 0) {
//    int rightmost_digit = rtc.month & 0xF;
//    decimal += rightmost_digit * base;
//    base *= 10;
//    rtc.month >>= 4;
//  }
//  rtc.month = decimal;
//
//  decimal = 0;
//  base = 1;
//
//  while (rtc.year > 0) {
//    int rightmost_digit = rtc.year & 0xF;
//    decimal += rightmost_digit * base;
//    base *= 10;
//    rtc.year >>= 4;
//  }
//  rtc.year = decimal;
//
//  printf("Segundos: %u\n", rtc.seconds);
//  printf("Minutos: %u\n", rtc.minutes);
//  printf("Horas: %u\n", rtc.hours);
//  printf("Dia: %u\n", rtc.day);
//  printf("Mes: %u\n", rtc.month);
//  printf("Ano: %u\n", rtc.year);
//  return 0;
//    //////////////////////
  enum game_state current_game_state = MAIN_MENU;
  if (setup_game() != 0) {
    printf("Game could not be opened\n");
    return close_game();
  }
//  printf("Break point 6\n");
  int ipc_status, r;
  message msg;

  uint8_t scancode = 0;
  uint8_t full_scancode[2];
  int num_bytes = 1;

  uint8_t timer_counter = 0;
  enum letter_pressed last_key_pressed = NO_LETTER_PRESSED;
  struct all_devices_interrupts all_received_devices_interrupts = {false, false, false, false, false, false, false, false};
  int previous_mouse_position_x = 0;
  int previous_mouse_position_y = 0;
  int mouse_position_x = 0;
  int mouse_position_y = 0;

  // load all the xpms into image structs
  // loading all the numbers to image structs
  uint32_t number_0_color_array[16 * 32];
  struct image_struct number_0 = {16, 32, number_0_color_array};
  xpm_load_to_image((xpm_map_t) xpm_0, &number_0);

  uint32_t number_1_color_array[16 * 32];
  struct image_struct number_1 = {16, 32, number_1_color_array};
  xpm_load_to_image((xpm_map_t) xpm_1, &number_1);

  uint32_t number_2_color_array[16 * 32];
  struct image_struct number_2 = {16, 32, number_2_color_array};
  xpm_load_to_image((xpm_map_t) xpm_2, &number_2);

  uint32_t number_3_color_array[16 * 32];
  struct image_struct number_3 = {16, 32, number_3_color_array};
  xpm_load_to_image((xpm_map_t) xpm_3, &number_3);

  uint32_t number_4_color_array[16 * 32];
  struct image_struct number_4 = {16, 32, number_4_color_array};
  xpm_load_to_image((xpm_map_t) xpm_4, &number_4);

  uint32_t number_5_color_array[16 * 32];
  struct image_struct number_5 = {16, 32, number_5_color_array};
  xpm_load_to_image((xpm_map_t) xpm_5, &number_5);

  uint32_t number_6_color_array[16 * 32];
  struct image_struct number_6 = {16, 32, number_6_color_array};
  xpm_load_to_image((xpm_map_t) xpm_6, &number_6);

  uint32_t number_7_color_array[16 * 32];
  struct image_struct number_7 = {16, 32, number_7_color_array};
  xpm_load_to_image((xpm_map_t) xpm_7, &number_7);

  uint32_t number_8_color_array[16 * 32];
  struct image_struct number_8 = {16, 32, number_8_color_array};
  xpm_load_to_image((xpm_map_t) xpm_8, &number_8);

  uint32_t number_9_color_array[16 * 32];
  struct image_struct number_9 = {16, 32, number_9_color_array};
  xpm_load_to_image((xpm_map_t) xpm_9, &number_9);

  // loading the character 2 pontos to an image struct
  uint32_t character_2_pontos_color_array[16 * 32];
  struct image_struct character_2_pontos = {16, 32, character_2_pontos_color_array};
  xpm_load_to_image((xpm_map_t) xpm_2_pontos, &character_2_pontos);

  // loading the character - to an image struct
  uint32_t character_tracinho_color_array[16 * 4];
  struct image_struct character_tracinho = {16, 4, character_tracinho_color_array};
  xpm_load_to_image((xpm_map_t) xpm_traco, &character_tracinho);

  // loading the cursor to an image struct
  uint32_t cursor_color_array[32 * 32];
  struct image_struct cursor = {32, 32, cursor_color_array};
  xpm_load_to_image((xpm_map_t) cursor_xpm, &cursor);

  // loading the game screen with the map to an image struct (needs to be in the heap because it's too big)
  uint32_t* game_color_array = malloc(800 * 600 * sizeof(uint32_t));
  struct image_struct game = {800, 600, game_color_array};
  xpm_load_to_image((xpm_map_t) mapa_xpm, &game);

  // loading the game over xpm to an image struct (needs to be in the heap because it's too big)
  uint32_t* game_over_color_array = malloc(544 * 320 * sizeof(uint32_t));
  struct image_struct game_over = {544, 320, game_over_color_array};
  xpm_load_to_image((xpm_map_t) game_over_xpm, &game_over);

  // loading the character to an image struct
  uint32_t character_color_array[32 * 32];
  struct image_struct character = {32, 32, character_color_array};
  xpm_load_to_image((xpm_map_t) character_xpm, &character);

  // loading the monster to an image struct
  uint32_t monster_color_array[32 * 32];
  struct image_struct monster = {32, 32, monster_color_array};
  xpm_load_to_image((xpm_map_t) monster_xpm, &monster);

  // loading the main menu to an image struct (needs to be on the heap because it's too big)
  uint32_t* main_menu_color_array = malloc(800 * 600 * sizeof(uint32_t));
  struct image_struct main_menu = {800, 600, main_menu_color_array};
  xpm_load_to_image((xpm_map_t) menu_xpm, &main_menu);

  // loading the high scores screen to an image struct (needs to be on the heap because it's too big)
  uint32_t* high_scores_color_array = malloc(800 * 600 * sizeof(uint32_t));
  struct image_struct high_scores = {800, 600, high_scores_color_array};
  xpm_load_to_image((xpm_map_t) high_scores_xpm, &high_scores);

  struct game_images game_loaded_images = {&game, &character, &monster, &cursor, &game_over, &character_2_pontos, &number_0, &number_1, &number_2, &number_3, &number_4, &number_5, &number_6, &number_7, &number_8, &number_9};
//  struct high_score_images high_score_loaded_images = {&high_scores, &cursor, &character_2_pontos, &character_tracinho, &number_0, &number_1, &number_2, &number_3, &number_4, &number_5, &number_6, &number_7, &number_8, &number_9};

//  printf("Break point 7\n");
  uint8_t *frame_buffer = NULL;
  if (build_frame_buffer(0x115, &frame_buffer) != 0) {
    printf("Error while building the main frame buffer");
    return 1;
  }
//  printf("Break point 8\n");
  uint32_t length_frame_buffer = get_length_frame_buffer();
//  printf("Break point 9\n");
//  printf("Length of frame buffer: %d\n", length_frame_buffer);
  memset(frame_buffer, 0, length_frame_buffer);
//  printf("Break point 10\n");
  uint8_t* game_frame_buffer = malloc(length_frame_buffer * sizeof(uint8_t));
  memset(game_frame_buffer, 0, length_frame_buffer);
//  printf("Break point 11\n");
  uint8_t* mouse_frame_buffer = malloc(length_frame_buffer * sizeof(uint8_t));
  memset(mouse_frame_buffer, 0, length_frame_buffer);

  bool is_start_of_screen = true;
  bool was_game_frame_buffer_changed = false;
  bool close_application = false;

  // create game_position_array
  const int array_of_rows_of_entities_size = 14;
  const int row_of_entities_size = 23;
  struct position player_position = {9, 9};
  int last_spawn_used = 3;

  struct game_entities_position initial_all_game_entities_position = {
          .array_of_rows_of_entities = {
                  {WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
                  {WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
                  {WALL, EMPTY, WALL, EMPTY, WALL, WALL, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, EMPTY, WALL},
                  {WALL, EMPTY, EMPTY, EMPTY, WALL, WALL, WALL, EMPTY, EMPTY, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
                  {WALL, EMPTY, EMPTY, EMPTY, WALL, WALL, WALL, EMPTY, EMPTY, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, EMPTY, EMPTY, WALL, WALL, WALL},
                  {WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, WALL, WALL},
                  {WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
                  {WALL, WALL, WALL, WALL, WALL, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, WALL, WALL, EMPTY, EMPTY, EMPTY, WALL},
                  {WALL, WALL, WALL, WALL, WALL, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, WALL, EMPTY, EMPTY, WALL, WALL, WALL, EMPTY, WALL, EMPTY, WALL},
                  {WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PLAYER, EMPTY, EMPTY, WALL, WALL, EMPTY, EMPTY, WALL, WALL, WALL, EMPTY, WALL, EMPTY, WALL},
                  {WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
                  {WALL, EMPTY, WALL, EMPTY, WALL, WALL, WALL, WALL, WALL, WALL, EMPTY, EMPTY, WALL, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, EMPTY, WALL},
                  {WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL},
                  {WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL}
          },
          .player_position = player_position,
          .enemy_structs= {
                  {
                      .id = 0,
                      .position = {0, 0},
                      .is_alive = false,
                      .move_horizontally = false
                  },
                  {
                      .id = 1,
                      .position = {0, 0},
                      .is_alive = false,
                      .move_horizontally = true
                  },
                  {
                      .id = 2,
                      .position = {0, 0},
                      .is_alive = false,
                      .move_horizontally = false
                  },
                  {
                      .id = 3,
                      .position = {0, 0},
                      .is_alive = false,
                      .move_horizontally = true
                  },
                  {
                      .id = 4,
                      .position = {0, 0},
                      .is_alive = false,
                      .move_horizontally = false
                  },
                  {
                      .id = 5,
                      .position = {0, 0},
                      .is_alive = false,
                      .move_horizontally = true
                  },
                  {
                      .id = 6,
                      .position = {0, 0},
                      .is_alive = false,
                      .move_horizontally = false
                  },
                  {
                      .id = 7,
                      .position = {0, 0},
                      .is_alive = false,
                      .move_horizontally = true
                  },
          },
          .spawners_positions = {
                  {2, 2},
                  {2, 11},
                  {20, 2},
                  {20, 11}
          },
          .last_spawn_used = last_spawn_used,
          .array_of_entities_h_size = row_of_entities_size,
          .array_of_entities_v_size = array_of_rows_of_entities_size
  };

  struct game_entities_position all_game_entities_position = initial_all_game_entities_position;

  struct game_values initial_game_values = {
    .score_digits = {0, 0, 0, 0},
    .score = 0,
    .time_digits = {0, 0, 0, 0},
    .time_in_seconds = 0
  };

  struct game_values current_game_values = initial_game_values;

  bool is_game_over = false;
  bool is_game_quit = false;
  bool was_game_state_changed = false;

  bool was_change_during_keyboard_interrupts = false;

  while (scancode != ESC_BREAK_CODE) {
    // printf("Breakpoint 12\n");
    if (close_application) {
        break;
    }

    // checks if it is the start of a screen
    if (is_start_of_screen) {
        is_start_of_screen = false;
        switch (current_game_state) {
            case MAIN_MENU:
                image_load_to_frame_buffer(&main_menu, 0, 0, game_frame_buffer);
                break;
            case GAME:
                timer_counter = 0;
                is_game_over = false;
                is_game_quit = false;
                all_game_entities_position = initial_all_game_entities_position;
                current_game_values = initial_game_values;
                spawn_monsters(&all_game_entities_position);
                load_game_state_to_game_buffer(&all_game_entities_position, &current_game_values, &game_loaded_images, game_frame_buffer);
                break;
            case GAME_OVER:
                // put gameover image over the game image on game_frame_buffer
                image_load_to_frame_buffer(&game_over, 128, 176, game_frame_buffer);
                // store the information in the highscore.csv file if it is a highscore
                store_high_score_at_this_time(&current_game_values);
                break;
            case HIGH_SCORE:
                image_load_to_frame_buffer(&high_scores, 0, 0, game_frame_buffer);
                // [TODO] get the highscores from the storage file
                // [TODO] put the highscores in the game frame buffer
                break;
        }
        memcpy(mouse_frame_buffer, game_frame_buffer, length_frame_buffer);
        image_load_to_frame_buffer(&cursor, mouse_position_x - 16, mouse_position_y - 16, mouse_frame_buffer);
        memcpy(frame_buffer, mouse_frame_buffer, length_frame_buffer);
        continue; // skip the rest of the loop
    }
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Interruption error\n");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
//                printf("Msg: %u\n", msg.m_notify.interrupts);
                if (msg.m_notify.interrupts & irq_set_timer) {
                    timer_counter++;
//                    printf("Timer counter: %d\n", timer_counter);
                    if (timer_counter == 60) {
                        timer_counter = 0;
                        all_received_devices_interrupts.is_timer_second_interrupt = true;
//                        printf("1 segundo\n");
                    } 
                    else if (timer_counter == 30) {
                        all_received_devices_interrupts.is_timer_tick_interrupt = true;
//                        printf("0.5 segundos\n");
                    }
                }
//                printf("Breakpoint 70\n");
                if (msg.m_notify.interrupts & irq_set_keyboard) {
//                    printf("Inside keyboard interrupt\n");
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
//                        printf("Tecla w pressionada\n");
                        all_received_devices_interrupts.W = true;
                        was_change_during_keyboard_interrupts = true;
                        break;
                      case 0x91:
//                        printf("Tecla w largada\n");
                        all_received_devices_interrupts.W = false;
                        was_change_during_keyboard_interrupts = true;
                        break;
                      case 0x1e:
//                        printf("Tecla a pressionada\n");
                        all_received_devices_interrupts.A = true;
                        was_change_during_keyboard_interrupts = true;
                        break;
                      case 0x9e:
//                        printf("Tecla a largada\n");
                        all_received_devices_interrupts.A = false;
                        was_change_during_keyboard_interrupts = true;
                        break;
                      case 0x1f:
//                        printf("Tecla s pressionada\n");
                        all_received_devices_interrupts.S = true;
                        was_change_during_keyboard_interrupts = true;
                        break;
                      case 0x9f:
//                        printf("Tecla s largada\n");
                        all_received_devices_interrupts.S = false;
                        was_change_during_keyboard_interrupts = true;
                        break;
                      case 0x20:
//                        printf("Tecla d pressionada\n");
                        all_received_devices_interrupts.D = true;
                        was_change_during_keyboard_interrupts = true;
                        break;
                      case 0xa0:
//                        printf("Tecla d largada\n");
                        all_received_devices_interrupts.D = false;
                        was_change_during_keyboard_interrupts = true;
                        break;
                    }
                    memset(full_scancode, 0, 2 * sizeof(uint8_t)); // We reset the scancode array
                }
//                printf("Breakpoint 71\n");
                if (msg.m_notify.interrupts & irq_set_mouse) {
//                    printf("Inside mouse interrupt\n");
                    if (util_sys_inb(KBC_STATUS_REG, &status) != 0) { // We test the function that reads the status from the status register, to check if we didn't have a communication error
                        return 1;
                    }
                    mouse_ih();
//                    printf("Status: %d\n", status);
                    r = mouse_build_packet();
                    if (r == 0) {
                      break;
                    }
                    else if (r == 1) {
                      // Seria acabar a execução do programa
//                      printf("Mouse error\n");
                    }
//                    printf("Breakpoint 20\n");
                    mouse = mouse_detect_events(&final_packet);
//                    printf("Botão: %d", mouse->type);
//                    printf("Breakpoint 21\n");
                    if (mouse->type == LB_PRESSED) {
                      all_received_devices_interrupts.m1 = true;
//                      printf("Botão esquerdo pressionado\n");
                    }
                    else if (mouse->type == LB_RELEASED) {
                      all_received_devices_interrupts.m1 = false;
//                      printf("Botão esquerdo largado\n");
                    }
//                    printf("Posição do rato:\n");
//                    printf("Delta x: %d\n", mouse->delta_x);
//                    printf("Delta y: %d\n", mouse->delta_y);
                    previous_mouse_position_x = mouse_position_x;
                    previous_mouse_position_y = mouse_position_y;
                    mouse_position_x += mouse->delta_x;
                    mouse_position_y -= mouse->delta_y;
                    if (mouse_position_x < 0) {
                        mouse_position_x = 0;
                    }
                    if (mouse_position_y < 0) {
                        mouse_position_y = 0;
                    }
                    if (mouse_position_x > 799) {
                        mouse_position_x = 799;
                    }
                    if (mouse_position_y > 599) {
                        mouse_position_y = 599;
                    }
                    all_received_devices_interrupts.is_mouse_move_interrupt = true;
                }

        }
    }
//    printf("Finished interrupts\n");
    // in case there wasn't an interruption just continue (happens for example when it's a non-important timer tick)
    if (!all_received_devices_interrupts.is_timer_second_interrupt && !all_received_devices_interrupts.is_timer_tick_interrupt && !all_received_devices_interrupts.W && !all_received_devices_interrupts.A && !all_received_devices_interrupts.S && !all_received_devices_interrupts.D && !all_received_devices_interrupts.is_mouse_move_interrupt && !all_received_devices_interrupts.m1) {
        continue;
    }
//    printf("Interrupt to be handled\n");
    // checks if there are multiple key presses at the same time, in which case it should ignore them and put the last key pressed as NO_LETTER_PRESSED
    if (was_change_during_keyboard_interrupts) {
        was_change_during_keyboard_interrupts = false;
        int keys_pressed_at_the_same_time = 0;
        if (all_received_devices_interrupts.W) {
            last_key_pressed = W;
            keys_pressed_at_the_same_time++;
        }
        else if (all_received_devices_interrupts.A) {
            last_key_pressed = A;
            keys_pressed_at_the_same_time++;
        }
        else if (all_received_devices_interrupts.S) {
            last_key_pressed = S;
            keys_pressed_at_the_same_time++;
        }
        else if (all_received_devices_interrupts.D) {
            last_key_pressed = D;
            keys_pressed_at_the_same_time++;
        }
        if (keys_pressed_at_the_same_time > 1) {
            last_key_pressed = NO_LETTER_PRESSED;
        }
    }
    was_game_frame_buffer_changed = false; // pointer will be passed in the following functions inside the switch statement
    //  switch statement between the different game states
    switch(current_game_state) {
        case MAIN_MENU:
            if (!all_received_devices_interrupts.m1) {
                break;
            }
            handle_main_menu(mouse_position_x, mouse_position_y, &current_game_state, &is_start_of_screen, &close_application);
            break;
        case GAME:
            was_game_state_changed = false;
            // handle for mouse 1 input (quit game, kill monsters, update score)
            if (all_received_devices_interrupts.m1) {
                handle_game_m1_interrupt(&all_game_entities_position, &current_game_values, mouse_position_x,
                                         mouse_position_y, &was_game_state_changed, &is_game_quit);
            }
            if (is_game_quit) {
                current_game_state = MAIN_MENU;
                is_start_of_screen = true;
                break;
            }
            // handle for timer tick (move monsters and monster attack)
            if (all_received_devices_interrupts.is_timer_tick_interrupt) {
                handle_game_timer_tick_interrupt(&all_game_entities_position, &current_game_values,
                                                 &was_game_state_changed, &is_game_over);
            }
            if (is_game_over) {
                current_game_state = GAME_OVER;
                is_start_of_screen = true;
            }
            // handle for timer second (try move player last key pressed, update time, spawn monsters every 4th timer_second interrupt)
            if (all_received_devices_interrupts.is_timer_second_interrupt) {
                handle_game_timer_second_interrupt(&all_game_entities_position, &current_game_values, &last_key_pressed,
                                                   &was_game_state_changed);
            }

            if (was_game_state_changed) {
                load_game_state_to_game_buffer(&all_game_entities_position, &current_game_values, &game_loaded_images, game_frame_buffer);
                was_game_frame_buffer_changed = true;
            }
            break;
        case GAME_OVER:
            if (!all_received_devices_interrupts.m1) {
                break;
            }
            // function to handle interrupts while in the game over state (where nothing moves and only quit can be pressed)
            handle_game_over_interrupt(mouse_position_x, mouse_position_y, &current_game_state, &is_start_of_screen);
            break;
        case HIGH_SCORE:
            if (!all_received_devices_interrupts.m1) {
                break;
            }
//          [TODO] function to handle interrupts while in the the high score state
            break;
    }
    // remove the interrupts so that they don't get handled again
    all_received_devices_interrupts.is_timer_second_interrupt = false;
    all_received_devices_interrupts.is_timer_tick_interrupt = false;
    all_received_devices_interrupts.is_mouse_move_interrupt = false;
    // if nothing has changed
    if (!was_game_frame_buffer_changed && mouse_position_x == previous_mouse_position_x && mouse_position_y == previous_mouse_position_y) {
        continue;
    }
    memcpy(mouse_frame_buffer, game_frame_buffer, length_frame_buffer);
    image_load_to_frame_buffer(&cursor, mouse_position_x - 16, mouse_position_y - 16, mouse_frame_buffer);
    memcpy(frame_buffer, mouse_frame_buffer, length_frame_buffer);
  }

  if (close_game() != 0) {
    printf("Error in closing the game");
    return 1;
  }

  // free the arrays that needed be in the heap because they were too big
  free(game_color_array);
  free(high_scores_color_array);
  free(main_menu_color_array);
  free(game_over_color_array);

  // free auxiliary frame buffers
  free(game_frame_buffer);
  free(mouse_frame_buffer);

  return 0;
}
