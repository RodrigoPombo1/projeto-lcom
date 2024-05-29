#include <lcom/lcf.h>
#include <lcom/timer.h>

#include "devices/keyboard/KBC_keyboard.h"
#include "devices/mouse/KBC_mouse.h"
#include "devices/graphics/graphics.h"
#include "game_state/game_state.h"

uint8_t irq_set_timer; 
uint8_t irq_set_keyboard;
uint8_t irq_set_mouse;
// uint8_t irq_set_rtc;

uint8_t status = 0;

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
  if (mouse_subscribe(&irq_set_mouse) != 0) {
    return 1;
  }
  printf("Break point 4\n");
  // Enable mouse data report 
  if (mouse_write_command(ENABLE_DR) != 0) {
    return 1;
  }
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
  enum game_state current_game_state = MAIN_MENU;
  if (setup_game() != 0) {
    printf("Game could not be opened");
    return close_game();
  }
  printf("Break point 6\n");
  int ipc_status;
  message msg;

  uint8_t timer_counter = 1; // just so it doesn't immediately update the character positions
//  enum letter_pressed last_key_pressed = NO_LETTER_PRESSED;
//  bool is_key_being_pressed = false;
//  bool is_mouse_button_being_pressed = false;
  bool interrupt_received = false;
  enum interrupt_type interrupt_received_type;
//  int mouse_position_x = 0;
//  int mouse_position_y = 0;

//   [TODO] initialize all arrays for every element in the game with memset
//   [TODO] load xpm images into those array of colors
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
  while (true) {
    printf("Breakpoint 12\n");
    if (close_application) {
        break;
    }

    // [TODO] REMOVE THIS!!!!!!!!!!!!! ONLY HERE SO IT DOESN'T GET STUCK IN AN INFINITE LOOP
    sleep(3);
    close_application = true;
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
      printf("Interruption error");
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
        }
        if (msg.m_notify.interrupts & irq_set_keyboard) {
            interrupt_received = true;
    //      [TODO] get the key that was pressed and associate it with the last_key_pressed enum
    //      [TODO] check if the key is being pressed and set is_key_being_pressed to true
        }
        if (msg.m_notify.interrupts & irq_set_mouse) {
            interrupt_received = true;
    //      [TODO] get the mouse deviation and add it to the mouse position
    //      [TODO] get if mouse has pressed m1 (check if left mouse button was pressed) update is_mouse_button_being_pressed accordingly
        }
    }
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
