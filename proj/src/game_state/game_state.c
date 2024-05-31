#include "game_state.h"

// this handle only happens when m1 is pressed
int handle_main_menu(int mouse_position_x, int mouse_position_y, enum game_state* game_state, bool *is_start_of_game_state, bool *close_application) {
    // check if the mouse is over the play button
    if (224 <= mouse_position_x && mouse_position_x <= 560 && 144 <= mouse_position_y && mouse_position_y <= 192) {
        *game_state = GAME;
        *is_start_of_game_state = true;
        return 0;
    }
    // check if the mouse if over the high score button
    if (224 <= mouse_position_x && mouse_position_x <= 560 && 272 <= mouse_position_y && mouse_position_y <= 320) {
        *game_state = HIGH_SCORE;
        *is_start_of_game_state = true;
        return 0;
    }
    // check if the mouse is over the exit button
    if (224 <= mouse_position_x && mouse_position_x <= 560 && 400 <= mouse_position_y && mouse_position_y <= 448) {
        *close_application = true;
        return 0;
    }
    return 1;
}
