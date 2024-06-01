#include "game_state.h"

// this handle only happens when m1 is pressed
int handle_main_menu(int mouse_position_x, int mouse_position_y, enum game_state* game_state, bool *is_start_of_game_state, bool *close_application) {
    // check if the mouse is over the play button
    if (224 <= mouse_position_x && mouse_position_x <= 576 && 144 <= mouse_position_y && mouse_position_y <= 208) {
        *game_state = GAME;
        *is_start_of_game_state = true;
        return 0;
    }
    // check if the mouse if over the high score button
    if (224 <= mouse_position_x && mouse_position_x <= 576 && 272 <= mouse_position_y && mouse_position_y <= 336) {
        *game_state = HIGH_SCORE;
        *is_start_of_game_state = true;
        return 0;
    }
    // check if the mouse is over the exit button
    if (224 <= mouse_position_x && mouse_position_x <= 576 && 400 <= mouse_position_y && mouse_position_y <= 464) {
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

int get_image_from_number_game(struct image_struct** result_image, struct game_images *all_game_images, int number) {
    switch (number) {
        case 0:
            *result_image = all_game_images->number_0;
            return 0;
        case 1:
            *result_image = all_game_images->number_1;
            return 0;
        case 2:
            *result_image = all_game_images->number_2;
            return 0;
        case 3:
            *result_image = all_game_images->number_3;
            return 0;
        case 4:
            *result_image = all_game_images->number_4;
            return 0;
        case 5:
            *result_image = all_game_images->number_5;
            return 0;
        case 6:
            *result_image = all_game_images->number_6;
            return 0;
        case 7:
            *result_image = all_game_images->number_7;
            return 0;
        case 8:
            *result_image = all_game_images->number_8;
            return 0;
        case 9:
            *result_image = all_game_images->number_9;
            return 0;
        default:
            return 1;
    }
}

int load_game_state_to_game_buffer(struct game_entities_position *all_game_state_entities_position, struct game_values *current_game_state_values, struct game_images *all_game_images, uint8_t* video_mem) {
    image_load_to_frame_buffer(all_game_images->background, 0, 0, video_mem);

    int frame_buffer_position_x = 0;
    int frame_buffer_position_y = 0;
    // draw player
    convert_game_position_to_frame_buffer_position(all_game_state_entities_position->player_position.x, all_game_state_entities_position->player_position.y, &frame_buffer_position_x, &frame_buffer_position_y);
    image_load_to_frame_buffer(all_game_images->player, frame_buffer_position_x, frame_buffer_position_y, video_mem);
    // draw all the enemies
    for (int i = 0; i < 8; i++) {
        if (!all_game_state_entities_position->enemy_structs[i].is_alive) {
            continue;
        }
        convert_game_position_to_frame_buffer_position(all_game_state_entities_position->enemy_structs[i].position.x, all_game_state_entities_position->enemy_structs[i].position.y, &frame_buffer_position_x, &frame_buffer_position_y);
        image_load_to_frame_buffer(all_game_images->enemy, frame_buffer_position_x, frame_buffer_position_y, video_mem);
    }

    // load the timer
    struct image_struct *number_image = NULL;

    get_image_from_number_game(&number_image, all_game_images, current_game_state_values->time_digits[0]);
    image_load_to_frame_buffer(number_image, 128, 32, video_mem);

    get_image_from_number_game(&number_image, all_game_images, current_game_state_values->time_digits[1]);
    image_load_to_frame_buffer(number_image, 148, 32, video_mem);

    image_load_to_frame_buffer(all_game_images->character_2_pontos, 168, 32, video_mem);

    get_image_from_number_game(&number_image, all_game_images, current_game_state_values->time_digits[2]);
    image_load_to_frame_buffer(number_image, 188, 32, video_mem);

    get_image_from_number_game(&number_image, all_game_images, current_game_state_values->time_digits[3]);
    image_load_to_frame_buffer(number_image, 208, 32, video_mem);

    // load the score
    get_image_from_number_game(&number_image, all_game_images, current_game_state_values->score_digits[0]);
    image_load_to_frame_buffer(number_image, 416, 32, video_mem);

    get_image_from_number_game(&number_image, all_game_images, current_game_state_values->score_digits[1]);
    image_load_to_frame_buffer(number_image, 436, 32, video_mem);

    get_image_from_number_game(&number_image, all_game_images, current_game_state_values->score_digits[2]);
    image_load_to_frame_buffer(number_image, 456, 32, video_mem);

    get_image_from_number_game(&number_image, all_game_images, current_game_state_values->score_digits[3]);
    image_load_to_frame_buffer(number_image, 476, 32, video_mem);

    return 0;
}

int spawn_monsters(struct game_entities_position *all_game_state_entities_position) {
    // find a spawn
    for (int _ = 0; _ < 4; _++) {
        // go to the next spawn
        int current_spawn = (all_game_state_entities_position->last_spawn_used + 1) % 4;
        all_game_state_entities_position->last_spawn_used = current_spawn;
        // get the coordinates of that spawn
        int x_of_spawn = all_game_state_entities_position->spawners_positions[current_spawn].x;
        int y_of_spawn = all_game_state_entities_position->spawners_positions[current_spawn].y;
//        printf("Analysing spawn %d at %d, %d\n", current_spawn, x_of_spawn, y_of_spawn);
        // check if there is an entity at that spawn
        enum type_of_entity entity_at_the_spawn_position = all_game_state_entities_position->array_of_rows_of_entities[y_of_spawn][x_of_spawn];
        if (entity_at_the_spawn_position == ENEMY) {
            continue;
        }
        int i = 0;
        // find a monster that is dead
        while(i < 8) {
            if (all_game_state_entities_position->enemy_structs[i].is_alive) {
                i++;
                continue;
            }
            break;
        }
        // spawn the monster at that spawn
        all_game_state_entities_position->enemy_structs[i].position.x = x_of_spawn;
        all_game_state_entities_position->enemy_structs[i].position.y = y_of_spawn;
        all_game_state_entities_position->array_of_rows_of_entities[y_of_spawn][x_of_spawn] = ENEMY;
        all_game_state_entities_position->enemy_structs[i].is_alive = true;
    }

    return 0;
}

int convert_frame_buffer_position_to_game_position(int frame_buffer_position_x, int frame_buffer_position_y, int *game_position_x, int *game_position_y) {
    if (frame_buffer_position_x < 32 || frame_buffer_position_x > 768 || frame_buffer_position_y < 112 || frame_buffer_position_y > 560) {
        *game_position_x = -1;
        *game_position_y = -1;
        return 1;
    }
    *game_position_x = (frame_buffer_position_x - 32) / 32;
    *game_position_y = (frame_buffer_position_y - 112) / 32;
    return 0;
}

int fill_spawns(struct game_entities_position *all_game_state_entities_position) {
    for (int i = 0; i < 4; i++) {
        int x_of_spawn = all_game_state_entities_position->spawners_positions[i].x;
        int y_of_spawn = all_game_state_entities_position->spawners_positions[i].y;
        if (all_game_state_entities_position->array_of_rows_of_entities[y_of_spawn][x_of_spawn] == EMPTY) {
            all_game_state_entities_position->array_of_rows_of_entities[y_of_spawn][x_of_spawn] = WALL;
        }
    }
    return 0;
}

int increment_score(struct game_values *current_game_state_values) {
    current_game_state_values->score++;
    int score = current_game_state_values->score;
    current_game_state_values->score_digits[3] = score % 10;
    score /= 10;
    current_game_state_values->score_digits[2] = score % 10;
    score /= 10;
    current_game_state_values->score_digits[1] = score % 10;
    score /= 10;
    current_game_state_values->score_digits[0] = score % 10;
    return 0;
}

int increment_timer(struct game_values *current_game_state_values) {
    current_game_state_values->time_in_seconds++;
    int time = current_game_state_values->time_in_seconds;
    int minutes = time / 60;
    int seconds = time % 60;

    current_game_state_values->time_digits[3] = minutes % 10;
    minutes /= 10;
    current_game_state_values->time_digits[2] = minutes % 10;

    current_game_state_values->time_digits[1] = seconds % 10;
    seconds /= 10;
    current_game_state_values->time_digits[0] = seconds % 10;

    return 0;
}

int handle_game_m1_interrupt(struct game_entities_position *all_game_state_entities_position, struct game_values *current_game_state_values, int mouse_position_x, int mouse_position_y, bool *was_game_state_changed, bool *is_game_quit) {
    // check if the mouse is over the quit button
    if (608 <= mouse_position_x && mouse_position_x <= 752 && 16 <= mouse_position_y && mouse_position_y <= 64) {
        *is_game_quit = true;
        return 0;
    }
    int mouse_position_in_game_x;
    int mouse_position_in_game_y;
    // get the game coordinates the mouse is hovering
    convert_frame_buffer_position_to_game_position(mouse_position_x, mouse_position_y, &mouse_position_in_game_x, &mouse_position_in_game_y);
    // check if the mouse position is even in the game
    if (mouse_position_in_game_x == -1 || mouse_position_in_game_y == -1) {
        return 0;
    }
    // check if the mouse is over a space adjacent to a character
    bool mouse_is_adjacent_to_character = false;
    // check coordinate above
    if (mouse_position_in_game_y - 1 >= 0) {
        if (all_game_state_entities_position->array_of_rows_of_entities[mouse_position_in_game_y - 1][mouse_position_in_game_x] == PLAYER) {
            mouse_is_adjacent_to_character = true;
        }
    }
    // check coordinate below
    if (mouse_position_in_game_y + 1 < 14) {
        if (all_game_state_entities_position->array_of_rows_of_entities[mouse_position_in_game_y + 1][mouse_position_in_game_x] == PLAYER) {
            mouse_is_adjacent_to_character = true;
        }
    }
    // check coordinate to the left
    if (mouse_position_in_game_x - 1 >= 0) {
        if (all_game_state_entities_position->array_of_rows_of_entities[mouse_position_in_game_y][mouse_position_in_game_x - 1] == PLAYER) {
            mouse_is_adjacent_to_character = true;
        }
    }
    // check coordinate to the right
    if (mouse_position_in_game_x + 1 < 24) {
        if (all_game_state_entities_position->array_of_rows_of_entities[mouse_position_in_game_y][mouse_position_in_game_x + 1] == PLAYER) {
            mouse_is_adjacent_to_character = true;
        }
    }

    if (!mouse_is_adjacent_to_character) {
        return 0;
    }

    // check if the mouse is over an enemy
    if (all_game_state_entities_position->array_of_rows_of_entities[mouse_position_in_game_y][mouse_position_in_game_x] == ENEMY) {
        // kill the enemy
        for (int i = 0; i < 8; i++) {
            if (all_game_state_entities_position->enemy_structs[i].position.x == mouse_position_in_game_x && all_game_state_entities_position->enemy_structs[i].position.y == mouse_position_in_game_y) {
                *was_game_state_changed = true;
                all_game_state_entities_position->enemy_structs[i].is_alive = false;
                all_game_state_entities_position->array_of_rows_of_entities[mouse_position_in_game_y][mouse_position_in_game_x] = EMPTY;
                increment_score(current_game_state_values);
                break;
            }
        }
        // fill the empty space with a spawn if the monsters were in one
        fill_spawns(all_game_state_entities_position);
    }

    return 0;
}

int check_monster_if_the_space_can_be_moved_into(struct game_entities_position *all_game_state_entities_position, int x, int y, bool *can_move, bool *killed_player) {
    *can_move = true;
    *killed_player = false;
    // check if it's outside the map
    if (x < 0 || x >= 23 || y < 0 || y >= 14) {
        *can_move = false;
        return 0;
    }
    if (all_game_state_entities_position->array_of_rows_of_entities[y][x] == PLAYER) {
        *can_move = true;
        *killed_player = true;
        return 0;
    }

    // check if it's a wall
    if (all_game_state_entities_position->array_of_rows_of_entities[y][x] == EMPTY) {
        *can_move = true;
        return 0;
    }
    return 0;
}

int handle_game_timer_tick_interrupt(struct game_entities_position *all_game_state_entities_position, struct game_values *current_game_state_values, bool *was_game_state_changed, bool *is_game_over) {
    // move monsters towards the player
    for (int i = 0; i < 8; i++) {
        if (!all_game_state_entities_position->enemy_structs[i].is_alive) {
            continue;
        }
        int player_x = all_game_state_entities_position->player_position.x;
        int player_y = all_game_state_entities_position->player_position.y;
        int enemy_x = all_game_state_entities_position->enemy_structs[i].position.x;
        int enemy_y = all_game_state_entities_position->enemy_structs[i].position.y;
        bool can_move = false;
        bool killed_player = false;
        // check if the player is to the left of the monster
        if (player_x < enemy_x) {
            // check if the monster can move to the left
            check_monster_if_the_space_can_be_moved_into(all_game_state_entities_position, enemy_x - 1, enemy_y, &can_move,
                                                 &killed_player);
            if (can_move) {
                *was_game_state_changed = true;
                all_game_state_entities_position->array_of_rows_of_entities[enemy_y][enemy_x] = EMPTY;
                all_game_state_entities_position->array_of_rows_of_entities[enemy_y][enemy_x - 1] = ENEMY;
                all_game_state_entities_position->enemy_structs[i].position.x--;
            }
        }
        // check if the player is to the right of the monster
        else if (player_x > enemy_x) {
            // check if the monster can move to the right
            check_monster_if_the_space_can_be_moved_into(all_game_state_entities_position, enemy_x + 1, enemy_y, &can_move,
                                                 &killed_player);
            if (can_move) {
                *was_game_state_changed = true;
                all_game_state_entities_position->array_of_rows_of_entities[enemy_y][enemy_x] = EMPTY;
                all_game_state_entities_position->array_of_rows_of_entities[enemy_y][enemy_x + 1] = ENEMY;
                all_game_state_entities_position->enemy_structs[i].position.x++;
            }
        }
        // check if the player is above the monster
        else if (player_y < enemy_y) {
            // check if the monster can move up
            check_monster_if_the_space_can_be_moved_into(all_game_state_entities_position, enemy_x, enemy_y - 1, &can_move,
                                                 &killed_player);
            if (can_move) {
                *was_game_state_changed = true;
                all_game_state_entities_position->array_of_rows_of_entities[enemy_y][enemy_x] = EMPTY;
                all_game_state_entities_position->array_of_rows_of_entities[enemy_y - 1][enemy_x] = ENEMY;
                all_game_state_entities_position->enemy_structs[i].position.y--;
            }
        }
        // check if the player is below the monster
        else if (player_y > enemy_y) {
            // check if the monster can move down
            check_monster_if_the_space_can_be_moved_into(all_game_state_entities_position, enemy_x, enemy_y + 1, &can_move,
                                                 &killed_player);
            if (can_move) {
                *was_game_state_changed = true;
                all_game_state_entities_position->array_of_rows_of_entities[enemy_y][enemy_x] = EMPTY;
                all_game_state_entities_position->array_of_rows_of_entities[enemy_y + 1][enemy_x] = ENEMY;
                all_game_state_entities_position->enemy_structs[i].position.y++;
            }
        }
        if (killed_player) {
            *is_game_over = true;
            return 0;
        }
    }
    fill_spawns(all_game_state_entities_position);
    return 0;
}

int check_player_if_the_space_can_be_moved_into(struct game_entities_position *all_game_state_entities_position, int x, int y, bool *can_move) {
    *can_move = false;
    // check if it's outside the map
    if (x < 0 || x >= 23 || y < 0 || y >= 14) {
        *can_move = false;
        return 0;
    }
    // check if it's a wall
    if (all_game_state_entities_position->array_of_rows_of_entities[y][x] == EMPTY) {
        *can_move = true;
        return 0;
    }
    return 0;
}

int handle_game_timer_second_interrupt(struct game_entities_position *all_game_state_entities_position, struct game_values *current_game_state_values, enum letter_pressed last_key_pressed, bool *was_game_state_changed) {
    increment_timer(current_game_state_values);
    // spawn monsters every 4th second
    if (current_game_state_values->time_in_seconds % 4 == 0) {
        spawn_monsters(all_game_state_entities_position);
        *was_game_state_changed = true;
    }
    int player_x = all_game_state_entities_position->player_position.x;
    int player_y = all_game_state_entities_position->player_position.y;
    // check if there was a key pressed
    switch (last_key_pressed) {
        case NO_LETTER_PRESSED:
            return 0;
        case W:
            player_y--;
            break;
        case A:
            player_x--;
            break;
        case S:
            player_y++;
            break;
        case D:
            player_x++;
            break;
    }
    // move the player if the player can move there
    bool can_move = false;
    check_player_if_the_space_can_be_moved_into(all_game_state_entities_position, player_x, player_y, &can_move);
    if (can_move) {
        *was_game_state_changed = true;
        all_game_state_entities_position->array_of_rows_of_entities[all_game_state_entities_position->player_position.y][all_game_state_entities_position->player_position.x] = EMPTY;
        all_game_state_entities_position->array_of_rows_of_entities[player_y][player_x] = PLAYER;
        all_game_state_entities_position->player_position.x = player_x;
        all_game_state_entities_position->player_position.y = player_y;
    }
    return 0;
}

