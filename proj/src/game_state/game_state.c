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

int get_image_from_number_game(struct image_struct* result_image, struct game_images *all_game_images, int number) {
    switch (number) {
        case 0:
            result_image = all_game_images->number_0;
            return 0;
        case 1:
            result_image = all_game_images->number_1;
            return 0;
        case 2:
            result_image = all_game_images->number_2;
            return 0;
        case 3:
            result_image = all_game_images->number_3;
            return 0;
        case 4:
            result_image = all_game_images->number_4;
            return 0;
        case 5:
            result_image = all_game_images->number_5;
            return 0;
        case 6:
            result_image = all_game_images->number_6;
            return 0;
        case 7:
            result_image = all_game_images->number_7;
            return 0;
        case 8:
            result_image = all_game_images->number_8;
            return 0;
        case 9:
            result_image = all_game_images->number_9;
            return 0;
        default:
            return 1;
    }
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

    // load the timer
    struct image_struct *number_image = all_game_images->number_0;

    get_image_from_number_game(number_image, all_game_images, current_game_state_values->time_digits[0]);
    image_load_to_frame_buffer(number_image, 128, 32, video_mem);

    get_image_from_number_game(number_image, all_game_images, current_game_state_values->time_digits[1]);
    image_load_to_frame_buffer(number_image, 148, 32, video_mem);

    image_load_to_frame_buffer(all_game_images->character_2_pontos, 168, 32, video_mem);

    get_image_from_number_game(number_image, all_game_images, current_game_state_values->time_digits[2]);
    image_load_to_frame_buffer(number_image, 188, 32, video_mem);

    get_image_from_number_game(number_image, all_game_images, current_game_state_values->time_digits[3]);
    image_load_to_frame_buffer(number_image, 208, 32, video_mem);

    // load the score
    get_image_from_number_game(number_image, all_game_images, current_game_state_values->score_digits[0]);
    image_load_to_frame_buffer(number_image, 416, 32, video_mem);

    get_image_from_number_game(number_image, all_game_images, current_game_state_values->score_digits[1]);
    image_load_to_frame_buffer(number_image, 436, 32, video_mem);

    get_image_from_number_game(number_image, all_game_images, current_game_state_values->score_digits[2]);
    image_load_to_frame_buffer(number_image, 456, 32, video_mem);

    get_image_from_number_game(number_image, all_game_images, current_game_state_values->score_digits[3]);
    image_load_to_frame_buffer(number_image, 476, 32, video_mem);

    return 0;
}


