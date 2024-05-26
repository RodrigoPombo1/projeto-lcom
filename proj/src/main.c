#include <lcom/lcf.h>
#include <lcom/timer.h>

#include "devices/keyboard/KBC_keyboard.h"
#include "devices/mouse/KBC_mouse.h"
#include "devices/graphics/graphics.h"

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
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

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
  // Initiate 32 bits per pixel video mode
  if (set_video_mode(0x14C) != 0) {
    return 1;
  }

  // Subscribe interruptions of all necessary devices
  if (timer_subscribe_int(&irq_set_timer) != 0) {
    return 1;
  }

  if (keyboard_subscribe(&irq_set_keyboard) != 0) {
    return 1;
  }

  if (mouse_subscribe(&irq_set_mouse) != 0) {
    return 1;
  }

  // Enable mouse data report 
  if (mouse_write_command(ENABLE_DR) != 0) {
    return 1;
  }

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

enum game_state {
  MAIN_MENU,
  GAME,
  GAME_OVER,
  HIGH_SCORE,
};

enum letter_pressed {
    NO_LETTER_PRESSED,
    W,
    A,
    S,
    D,
};

enum interrupt_type {
    TIMER_SECOND,
    TIMER_TICK,
    KEYBOARD,
    MOUSE,
};

int (proj_main_loop)(int argc, char *argv[]) {
  enum game_state current_game_state = MAIN_MENU;
  if (setup_game() != 0) {
    printf("Game could not be opened");
    return close_game();
  }

  int ipc_status;
  message msg;

  uint8_t timer_counter = 0;
//  enum letter_pressed last_key_pressed = NO_LETTER_PRESSED;
//  bool is_key_being_pressed = false;
//  bool is_mouse_button_being_pressed = false;
  bool interrupt_received = false;
  enum interrupt_type interrupt_received_type;
//  int mouse_position_x = 0;
//  int mouse_position_y = 0;

//   [TODO] initialize all arrays for every element in the game with memset
//   [TODO] load xpm images into those array of colors

//   [TODO] build the real frame buffer and make it so it is the real frame buffer
  uint8_t *frame_buffer = NULL;
  if (build_frame_buffer(0x14C, frame_buffer) != 0) {
    printf("Error while building the main frame buffer");
    return 1;
  }
  uint32_t length_frame_buffer = get_length_frame_buffer();
  memset(frame_buffer, 0, get_length_frame_buffer());

//   [TODO] build the frame buffer for the game state (will store either the main menu, the game, or the highscore)
  uint8_t *game_frame_buffer[length_frame_buffer];
  memset(game_frame_buffer, 0, length_frame_buffer);
//   [TODO] build the frame buffer for the mouse on top of the game state
  uint8_t *mouse_frame_buffer[length_frame_buffer];
  memset(mouse_frame_buffer, 0, length_frame_buffer);

//   [TODO] load the main menu state array into game_frame_buffer
//   [TODO] memcpy the game_frame_buffer to the mouse_frame_buffer
//   [TODO] put the mouse in it's initial position on the mouse_frame_buffer
//   [TODO] memcpy the mouse_frame_buffer to the real frame buffer

//   [TODO] load the high score from the txt file

  while (true) {
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Interruption error");
      continue;
    }

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
    //            [TODO] get the key that was pressed and associate it with the last_key_pressed enum
    //            [TODO] check if the key is being pressed and set is_key_being_pressed to true
        }
        if (msg.m_notify.interrupts & irq_set_mouse) {
            interrupt_received = true;
    //           [TODO] get the mouse deviation and add it to the mouse position
    //           [TODO] get if mouse has pressed m1 (check if left mouse button was pressed) update is_mouse_button_being_pressed accordingly
        }
    }

    // [TODO] check if interrupt was useful (depending on gamestate) if not, continue

    if (interrupt_received) {
//        bool was_game_frame_buffer_changed = false; // pointer will be passed in the following functions inside the switch statement
//        [TODO] probably a switch statement between the different game states // handling each game state will mean a pointer to the game state buffer will be passed in the function
        switch(current_game_state) {
            case MAIN_MENU:
//              [TODO] function to handle interrupts while in the main menu state
                break;
            case GAME:
//              [TODO] function to handle interrupts while in the game state
                break;
            case GAME_OVER:
//              [TODO] function to handle interrupts while in the game over state (where nothing moves only quit can be pressed)
                break;
            case HIGH_SCORE:
//              [TODO] function to handle interrupts while in the the high score state
                break;
        }


//        [TODO] always put the current mouse position on a pointer to the game frame buffer, even if game frame buffer hasn't change
//        if mouse interrupt
//        [TODO] put the mouse on the mouse frame buffer
//        else if was_game_frame_buffer_changed
//        [TODO] memcpy the game frame buffer to the mouse frame buffer


//        [TODO] always memcpy the mouse frame buffer to the real frame buffer
    }
  }

  if (close_game() != 0) {
    printf("Error in closing the game");
    return 1;
  }

  return 0;
}
