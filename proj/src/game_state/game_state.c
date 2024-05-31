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

int convert_game_position_to_frame_buffer_position(int game_position_x, int game_position_y, int *frame_buffer_position_x, int *frame_buffer_position_y) {
    *frame_buffer_position_x = 32 + 32 * game_position_x;
    *frame_buffer_position_y = 112 + 32 * game_position_y;
    return 0;
}

int load_game_state_to_game_buffer(struct game_entities_position *all_game_state_entities_position, struct game_values *current_game_state_values, struct game_images *all_game_images, uint8_t* video_mem) {
    int frame_buffer_position_x = 0;
    int frame_buffer_position_y = 0;
    // draw player
    convert_game_position_to_frame_buffer_position(all_game_state_entities_position->player_position.x, all_game_state_entities_position->player_position.y, &frame_buffer_position_x, &frame_buffer_position_y);
    image_load_to_frame_buffer(all_game_images->player, frame_buffer_position_x, frame_buffer_position_y, video_mem);
    // draw all the enemies
    for (int i = 0; i < 8; i++) {
        if (all_game_state_entities_position->enemy_structs[i].is_alive == false) {
            continue;
        }
        convert_game_position_to_frame_buffer_position(all_game_state_entities_position->enemy_structs[i].position.x, all_game_state_entities_position->enemy_structs[i].position.x, &frame_buffer_position_x, &frame_buffer_position_y);
        image_load_to_frame_buffer(all_game_images->enemy, frame_buffer_position_x, frame_buffer_position_y, video_mem);
    }
    return 0;
}


