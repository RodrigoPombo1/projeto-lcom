#include "game_state.h"

extern rtc_info_t rtc;

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

    get_image_from_number_game(&number_image, all_game_images, current_game_state_values->time_digits[2]);
    image_load_to_frame_buffer(number_image, 128, 32, video_mem);

    get_image_from_number_game(&number_image, all_game_images, current_game_state_values->time_digits[3]);
    image_load_to_frame_buffer(number_image, 148, 32, video_mem);

    image_load_to_frame_buffer(all_game_images->character_2_pontos, 168, 32, video_mem);

    get_image_from_number_game(&number_image, all_game_images, current_game_state_values->time_digits[0]);
    image_load_to_frame_buffer(number_image, 188, 32, video_mem);

    get_image_from_number_game(&number_image, all_game_images, current_game_state_values->time_digits[1]);
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
        if (i == 8) {
            continue;
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
    if (608 <= mouse_position_x && mouse_position_x <= 752 && 16 <= mouse_position_y && mouse_position_y <= 80) {
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
            if (!all_game_state_entities_position->enemy_structs[i].is_alive) {
                continue;
            }
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
    *can_move = false;
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

    // check if it's empty
    if (all_game_state_entities_position->array_of_rows_of_entities[y][x] == EMPTY) {
        *can_move = true;
        return 0;
    }
    return 0;
}

int handle_game_timer_tick_interrupt(struct game_entities_position *all_game_state_entities_position, struct game_values *current_game_state_values, bool *was_game_state_changed, bool *is_game_over) {
    bool has_monster_moved = false;
    // move monsters towards the player
    for (int i = 0; i < 8; i++) {
        has_monster_moved = false;
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
                has_monster_moved = true;
                all_game_state_entities_position->array_of_rows_of_entities[enemy_y][enemy_x] = EMPTY;
                all_game_state_entities_position->array_of_rows_of_entities[enemy_y][enemy_x - 1] = ENEMY;
                all_game_state_entities_position->enemy_structs[i].position.x--;
            }
        }
        if (has_monster_moved) {
            has_monster_moved = false;
            if (killed_player) {
                *is_game_over = true;
                return 0;
            }
            continue;
        }
        // check if the player is to the right of the monster
        if (player_x > enemy_x) {
            // check if the monster can move to the right
            check_monster_if_the_space_can_be_moved_into(all_game_state_entities_position, enemy_x + 1, enemy_y, &can_move,
                                                 &killed_player);
            if (can_move) {
                *was_game_state_changed = true;
                has_monster_moved = true;
                all_game_state_entities_position->array_of_rows_of_entities[enemy_y][enemy_x] = EMPTY;
                all_game_state_entities_position->array_of_rows_of_entities[enemy_y][enemy_x + 1] = ENEMY;
                all_game_state_entities_position->enemy_structs[i].position.x++;
            }
        }
        if (has_monster_moved) {
            has_monster_moved = false;
            if (killed_player) {
                *is_game_over = true;
                return 0;
            }
            continue;
        }
        // check if the player is above the monster
        if (player_y < enemy_y) {
            // check if the monster can move up
            check_monster_if_the_space_can_be_moved_into(all_game_state_entities_position, enemy_x, enemy_y - 1, &can_move,
                                                 &killed_player);
            if (can_move) {
                *was_game_state_changed = true;
                has_monster_moved = true;
                all_game_state_entities_position->array_of_rows_of_entities[enemy_y][enemy_x] = EMPTY;
                all_game_state_entities_position->array_of_rows_of_entities[enemy_y - 1][enemy_x] = ENEMY;
                all_game_state_entities_position->enemy_structs[i].position.y--;
            }
        }
        if (has_monster_moved) {
            has_monster_moved = false;
            if (killed_player) {
                *is_game_over = true;
                return 0;
            }
            continue;
        }
        // check if the player is below the monster
        if (player_y > enemy_y) {
            // check if the monster can move down
            check_monster_if_the_space_can_be_moved_into(all_game_state_entities_position, enemy_x, enemy_y + 1, &can_move,
                                                 &killed_player);
            if (can_move) {
                *was_game_state_changed = true;
                has_monster_moved = true;
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

int handle_game_timer_second_interrupt(struct game_entities_position *all_game_state_entities_position, struct game_values *current_game_state_values, enum letter_pressed *last_key_pressed, bool *was_game_state_changed) {
    increment_timer(current_game_state_values);
    *was_game_state_changed = true;
    // spawn monsters every 4th second
    if (current_game_state_values->time_in_seconds % 4 == 0) {
        spawn_monsters(all_game_state_entities_position);
        *was_game_state_changed = true;
    }
    int player_x = all_game_state_entities_position->player_position.x;
    int player_y = all_game_state_entities_position->player_position.y;
    // check if there was a key pressed
    switch (*last_key_pressed) {
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
    *last_key_pressed = NO_LETTER_PRESSED;
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

int handle_game_over_interrupt(int mouse_position_x, int mouse_position_y, enum game_state* game_state, bool *is_start_of_screen) {
    // check if the mouse is over the quit button
    if (608 <= mouse_position_x && mouse_position_x <= 752 && 16 <= mouse_position_y && mouse_position_y <= 64) {
        *game_state = MAIN_MENU;
        *is_start_of_screen = true;
    }
    return 0;
}

int store_high_score_at_this_time(struct game_values *game_value) {
    int decimal = 0;
    int base = 1;
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
    printf("Seconds: %d\n", rtc.seconds);
    decimal = 0;
    base = 1;

    while (rtc.minutes > 0) {
        int rightmost_digit = rtc.minutes & 0xF;
        decimal += rightmost_digit * base;
        base *= 10;
        rtc.minutes >>= 4;
    }
    rtc.minutes = decimal;
    printf("Minutes: %d\n", rtc.minutes);

    decimal = 0;
    base = 1;

    while (rtc.hours > 0) {
        int rightmost_digit = rtc.hours & 0xF;
        decimal += rightmost_digit * base;
        base *= 10;
        rtc.hours >>= 4;
    }
    rtc.hours = decimal;
    printf("Hours: %d\n", rtc.hours);

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

    struct high_score high_score_of_the_game = {
        .is_active = true,
        .game_values = {
            .score = game_value->score,
            .time_in_seconds = game_value->time_in_seconds,
            .score_digits = {
                    game_value->score_digits[0],
                    game_value->score_digits[1],
                    game_value->score_digits[2],
                    game_value->score_digits[3]},
        },
        .date_time_of_score = {
            .seconds = rtc.seconds,
            .minutes = rtc.minutes,
            .hours = rtc.hours,
            .day = rtc.day,
            .month = rtc.month,
            .year = rtc.year
        }
    };
    printf("Score: %d\n", high_score_of_the_game.game_values.score);
    // create the array to read the high_scores
    struct high_score high_scores[5] = {
            {
                    .is_active = false,
                    .game_values = {
                            .score = 0,
                            .time_in_seconds = 0,
                            .score_digits = {0, 0, 0, 0},
                            .time_digits = {0, 0, 0, 0}
                    },
                    .date_time_of_score = {
                            .seconds = 0,
                            .minutes = 0,
                            .hours = 0,
                            .day = 0,
                            .month = 0,
                            .year = 0
                    }
            },
            {
                    .is_active = false,
                    .game_values = {
                            .score = 0,
                            .time_in_seconds = 0,
                            .score_digits = {0, 0, 0, 0},
                            .time_digits = {0, 0, 0, 0}
                    },
                    .date_time_of_score = {
                            .seconds = 0,
                            .minutes = 0,
                            .hours = 0,
                            .day = 0,
                            .month = 0,
                            .year = 0
                    }
            },
            {
                    .is_active = false,
                    .game_values = {
                            .score = 0,
                            .time_in_seconds = 0,
                            .score_digits = {0, 0, 0, 0},
                            .time_digits = {0, 0, 0, 0}
                    },
                    .date_time_of_score = {
                            .seconds = 0,
                            .minutes = 0,
                            .hours = 0,
                            .day = 0,
                            .month = 0,
                            .year = 0
                    }
            },
            {
                    .is_active = false,
                    .game_values = {
                            .score = 0,
                            .time_in_seconds = 0,
                            .score_digits = {0, 0, 0, 0},
                            .time_digits = {0, 0, 0, 0}
                    },
                    .date_time_of_score = {
                            .seconds = 0,
                            .minutes = 0,
                            .hours = 0,
                            .day = 0,
                            .month = 0,
                            .year = 0
                    }
            },
            {
                    .is_active = false,
                    .game_values = {
                            .score = 0,
                            .time_in_seconds = 0,
                            .score_digits = {0, 0, 0, 0},
                            .time_digits = {0, 0, 0, 0}
                    },
                    .date_time_of_score = {
                            .seconds = 0,
                            .minutes = 0,
                            .hours = 0,
                            .day = 0,
                            .month = 0,
                            .year = 0
                    }
            }
    };


    if (read_high_scores(high_scores)) {
        printf("Failed to read high scores\n");
        return 1;
    }

    // get the index of the last score that is active
    int last_active_score = 0;
    for (int i = 0; i < 5; i++) {
        if (!high_scores[i].is_active) {
            break;
        }
        printf("Incrementing last active score\n");
        // don't increment the final time
        if (i == 4) {
            continue;
        }
        last_active_score++;
    }

    printf("Last active score: %d\n", last_active_score);
    // if there are 0 high scores in the file
    if (last_active_score == 0) {
        // put the score in the last active score (in the empty score position)
        high_scores[last_active_score] = high_score_of_the_game;
        printf("putting the recent game score has the only score Score: %d\n", high_scores[0].game_values.score);
        write_high_scores(high_scores);
        return 0;
    }

//    // check if there are less than 5 high scores, but more than 0
//    if (last_active_score != 4) {
//        // put the score in the last active score (in the empty score position)
//        high_scores[last_active_score] = high_score_of_the_game;
//        printf("putting the recent game score has the only score Score: %d\n", high_scores[0].game_values.score);
//        write_high_scores(high_scores);
//        return 0;
//    }

    // check if the score is higher than the lowest high score
    if (high_score_of_the_game.game_values.score > high_scores[last_active_score].game_values.score || last_active_score < 4) {
        // insert the new high score in the position of the lowest high score which we know is the lowest
        high_scores[last_active_score] = high_score_of_the_game;
        // sort the high scores
        for (int i = 0; i < last_active_score + 1; i++) {
            for (int j = i + 1; j < last_active_score + 1; j++) {
                if (high_scores[i].game_values.score < high_scores[j].game_values.score) {
                    struct high_score temp = high_scores[i];
                    high_scores[i] = high_scores[j];
                    high_scores[j] = temp;
                }
            }
        }
        /// [TODO] REMOVE THIS FOR TEST PURPOSES ONLY
        for (int i = 0; i < 5; i++) {
            printf("Score: %d\n", high_scores[i].game_values.score);
            printf("Is active: %d\n", high_scores[i].is_active);
        }
        // write the sorted high scores to the file
        if (write_high_scores(high_scores)) {
            printf("Failed to write high scores\n");
            return 1;
        }
    }
    return 0;
}

int write_high_scores(struct high_score high_scores[5]) {
    printf("Writing high scores\n");
    FILE *file = fopen(HIGH_SCORES_FILE_PATH_NAME, "w");
    if (file == NULL) {
        fclose(file);
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        printf("Writing high score %d\n", i);
        if (!high_scores[i].is_active) {
            break;
        }
        int score = high_scores[i].game_values.score;
        int time_minutes = high_scores[i].game_values.time_in_seconds / 60;
        int time_seconds = high_scores[i].game_values.time_in_seconds % 60;

        fprintf(file, "%d %d %d %d %d %d %d %d %d\n", score, time_minutes, time_seconds, high_scores[i].date_time_of_score.hours, high_scores[i].date_time_of_score.minutes, high_scores[i].date_time_of_score.seconds, high_scores[i].date_time_of_score.day, high_scores[i].date_time_of_score.month, high_scores[i].date_time_of_score.year);
    }

    fclose(file);
    return 0;
}

int read_high_scores(struct high_score high_scores[5]) {
    FILE *file = fopen(HIGH_SCORES_FILE_PATH_NAME, "r");
    if (file == NULL) {
        return 0;
    }
    char line[100];
    int count = 0;
    printf("count = %d\n", count);

    while (fgets(line, 100, file) != NULL) {
        if (count >= 5) {
            break;
        }
        printf("count = %d\n", count);
        printf("Entrou para aqui para dentro\n");
        printf("line that just read: %s\n", line);
        high_scores[count].is_active = true;

        int score, time_minutes, time_seconds, hour, minute, second, day, month, year;
        sscanf(line, "%d %d %d %d %d %d %d %d %d", &score, &time_minutes, &time_seconds, &hour, &minute, &second, &day, &month, &year);

        // split score into digits
        high_scores[count].game_values.score_digits[0] = (score / 1000) % 10;
        high_scores[count].game_values.score_digits[1] = (score / 100) % 10;
        high_scores[count].game_values.score_digits[2] = (score / 10) % 10;
        high_scores[count].game_values.score_digits[3] = score % 10;

        // split time into digits
        high_scores[count].game_values.time_digits[0] = (time_minutes / 10) % 10;
        high_scores[count].game_values.time_digits[1] = time_minutes % 10;
        high_scores[count].game_values.time_digits[2] = (time_seconds / 10) % 10;
        high_scores[count].game_values.time_digits[3] = time_seconds % 10;

        // load the other game_values struct fields
        high_scores[count].game_values.score = score;
        high_scores[count].game_values.time_in_seconds = time_minutes * 60 + time_seconds;

        // load the date_time_of_score struct fields
        high_scores[count].date_time_of_score.hours = hour;
        high_scores[count].date_time_of_score.minutes = minute;
        high_scores[count].date_time_of_score.seconds = second;
        high_scores[count].date_time_of_score.day = day;
        high_scores[count].date_time_of_score.month = month;
        high_scores[count].date_time_of_score.year = year;

        count++;
    }
    printf("Read %d high scores\n", count);
    fclose(file);
    return 0;
}

int handle_high_score_interrupt(int mouse_position_x, int mouse_position_y, enum game_state* game_state, bool *is_start_of_screen) {
    // check if the mouse is over the quit button
    if (608 <= mouse_position_x && mouse_position_x <= 752 && 16 <= mouse_position_y && mouse_position_y <= 80) {
        *game_state = MAIN_MENU;
        *is_start_of_screen = true;
    }
    return 0;
}


int get_image_from_number_high_score(struct image_struct** result_image, struct high_score_images *all_high_score_images, int number) {
    switch (number) {
        case 0:
            *result_image = all_high_score_images->number_0;
            return 0;
        case 1:
            *result_image = all_high_score_images->number_1;
            return 0;
        case 2:
            *result_image = all_high_score_images->number_2;
            return 0;
        case 3:
            *result_image = all_high_score_images->number_3;
            return 0;
        case 4:
            *result_image = all_high_score_images->number_4;
            return 0;
        case 5:
            *result_image = all_high_score_images->number_5;
            return 0;
        case 6:
            *result_image = all_high_score_images->number_6;
            return 0;
        case 7:
            *result_image = all_high_score_images->number_7;
            return 0;
        case 8:
            *result_image = all_high_score_images->number_8;
            return 0;
        case 9:
            *result_image = all_high_score_images->number_9;
            return 0;
        default:
            return 1;
    }
}

int load_high_scores_to_game_buffer(struct high_score_images *all_high_score_images, uint8_t* video_mem) {
    // create the array to read the high_scores
    struct high_score high_scores[5] = {
            {
                    .is_active = false,
                    .game_values = {
                            .score = 0,
                            .time_in_seconds = 0,
                            .score_digits = {0, 0, 0, 0},
                            .time_digits = {0, 0, 0, 0}
                    },
                    .date_time_of_score = {
                            .seconds = 0,
                            .minutes = 0,
                            .hours = 0,
                            .day = 0,
                            .month = 0,
                            .year = 0
                    }
            },
            {
                    .is_active = false,
                    .game_values = {
                            .score = 0,
                            .time_in_seconds = 0,
                            .score_digits = {0, 0, 0, 0},
                            .time_digits = {0, 0, 0, 0}
                    },
                    .date_time_of_score = {
                            .seconds = 0,
                            .minutes = 0,
                            .hours = 0,
                            .day = 0,
                            .month = 0,
                            .year = 0
                    }
            },
            {
                    .is_active = false,
                    .game_values = {
                            .score = 0,
                            .time_in_seconds = 0,
                            .score_digits = {0, 0, 0, 0},
                            .time_digits = {0, 0, 0, 0}
                    },
                    .date_time_of_score = {
                            .seconds = 0,
                            .minutes = 0,
                            .hours = 0,
                            .day = 0,
                            .month = 0,
                            .year = 0
                    }
            },
            {
                    .is_active = false,
                    .game_values = {
                            .score = 0,
                            .time_in_seconds = 0,
                            .score_digits = {0, 0, 0, 0},
                            .time_digits = {0, 0, 0, 0}
                    },
                    .date_time_of_score = {
                            .seconds = 0,
                            .minutes = 0,
                            .hours = 0,
                            .day = 0,
                            .month = 0,
                            .year = 0
                    }
            },
            {
                    .is_active = false,
                    .game_values = {
                            .score = 0,
                            .time_in_seconds = 0,
                            .score_digits = {0, 0, 0, 0},
                            .time_digits = {0, 0, 0, 0}
                    },
                    .date_time_of_score = {
                            .seconds = 0,
                            .minutes = 0,
                            .hours = 0,
                            .day = 0,
                            .month = 0,
                            .year = 0
                    }
            }
    };

    // read the high scores
    if (read_high_scores(high_scores)) {
        printf("Failed to read high scores\n");
        return 1;
    }

    int desvio_y_tracinho = 14;
    int desvio_date_time = -40;

    // load the active high scores to the screen
    for (int i = 0; i < 5; i++) {
        int y_position_1 = 176 + (80 * i);
        int y_position_2 = 212 + (80 * i);
        if (!high_scores[i].is_active) {
            break;
        }
        // load the score
        struct image_struct *number_image = NULL;

        // load the day
        // extract the value digit by digit
        int date_time = high_scores[i].date_time_of_score.day;
        int digit_1 = date_time % 10;
        date_time /= 10;
        int digit_2 = date_time % 10;
        // print the day digit by digit
        get_image_from_number_high_score(&number_image, all_high_score_images, digit_2);
        image_load_to_frame_buffer(number_image, 240 + desvio_date_time, y_position_1, video_mem);
        get_image_from_number_high_score(&number_image, all_high_score_images, digit_1);
        image_load_to_frame_buffer(number_image, 260 + desvio_date_time, y_position_1, video_mem);

        // load the -
        image_load_to_frame_buffer(all_high_score_images->character_tracinho, 280 + desvio_date_time , y_position_1 + desvio_y_tracinho, video_mem);

        // load the month

        // extract the value digit by digit
        date_time = high_scores[i].date_time_of_score.month;
        digit_1 = date_time % 10;
        date_time /= 10;
        digit_2 = date_time % 10;
        // print the month digit by digit
        get_image_from_number_high_score(&number_image, all_high_score_images, digit_2);
        image_load_to_frame_buffer(number_image, 300 + desvio_date_time, y_position_1, video_mem);
        get_image_from_number_high_score(&number_image, all_high_score_images, digit_1);
        image_load_to_frame_buffer(number_image, 320 + desvio_date_time, y_position_1, video_mem);

        // load the -
        image_load_to_frame_buffer(all_high_score_images->character_tracinho, 340 + desvio_date_time, y_position_1 + desvio_y_tracinho, video_mem);

        // load the year
        // extract the value digit by digit
        date_time = high_scores[i].date_time_of_score.year;
        digit_1 = date_time % 10;
        date_time /= 10;
        digit_2 = date_time % 10;
        // print the year digit by digit
        get_image_from_number_high_score(&number_image, all_high_score_images, digit_2);
        image_load_to_frame_buffer(number_image, 360 + desvio_date_time, y_position_1, video_mem);
        get_image_from_number_high_score(&number_image, all_high_score_images, digit_1);
        image_load_to_frame_buffer(number_image, 380 + desvio_date_time, y_position_1, video_mem);

        // load the hour
        // extract the value digit by digit
        date_time = high_scores[i].date_time_of_score.hours;
        digit_1 = date_time % 10;
        date_time /= 10;
        digit_2 = date_time % 10;
        // print the hour digit by digit
        get_image_from_number_high_score(&number_image, all_high_score_images, digit_2);
        image_load_to_frame_buffer(number_image, 240 + desvio_date_time, y_position_2, video_mem);
        get_image_from_number_high_score(&number_image, all_high_score_images, digit_1);
        image_load_to_frame_buffer(number_image, 260 + desvio_date_time, y_position_2, video_mem);

        // load the :
        image_load_to_frame_buffer(all_high_score_images->character_2_pontos, 280 + desvio_date_time, y_position_2, video_mem);

        // load the minute
        // extract the value digit by digit
        date_time = high_scores[i].date_time_of_score.minutes;
        digit_1 = date_time % 10;
        date_time /= 10;
        digit_2 = date_time % 10;
        // print the minute digit by digit
        get_image_from_number_high_score(&number_image, all_high_score_images, digit_2);
        image_load_to_frame_buffer(number_image, 300 + desvio_date_time, y_position_2, video_mem);
        get_image_from_number_high_score(&number_image, all_high_score_images, digit_1);
        image_load_to_frame_buffer(number_image, 320 + desvio_date_time, y_position_2, video_mem);

        // load the :
        image_load_to_frame_buffer(all_high_score_images->character_2_pontos, 340 + desvio_date_time, y_position_2, video_mem);

        // load the second
        // extract the value digit by digit
        date_time = high_scores[i].date_time_of_score.seconds;
        digit_1 = date_time % 10;
        date_time /= 10;
        digit_2 = date_time % 10;
        // print the second digit by digit
        get_image_from_number_high_score(&number_image, all_high_score_images, digit_2);
        image_load_to_frame_buffer(number_image, 360 + desvio_date_time, y_position_2, video_mem);
        get_image_from_number_high_score(&number_image, all_high_score_images, digit_1);
        image_load_to_frame_buffer(number_image, 380 + desvio_date_time, y_position_2, video_mem);


        // load the game time
        // load the minutes
        get_image_from_number_high_score(&number_image, all_high_score_images, high_scores[i].game_values.time_digits[0]);
        image_load_to_frame_buffer(number_image, 416, y_position_1, video_mem);
        get_image_from_number_high_score(&number_image, all_high_score_images, high_scores[i].game_values.time_digits[1]);
        image_load_to_frame_buffer(number_image, 436, y_position_1, video_mem);

        // load the :
        image_load_to_frame_buffer(all_high_score_images->character_2_pontos, 456, y_position_1, video_mem);

        // load the seconds
        get_image_from_number_high_score(&number_image, all_high_score_images, high_scores[i].game_values.time_digits[2]);
        image_load_to_frame_buffer(number_image, 476, y_position_1, video_mem);
        get_image_from_number_high_score(&number_image, all_high_score_images, high_scores[i].game_values.time_digits[3]);
        image_load_to_frame_buffer(number_image, 496, y_position_1, video_mem);


        // load the game score
        get_image_from_number_high_score(&number_image, all_high_score_images, high_scores[i].game_values.score_digits[0]);
        image_load_to_frame_buffer(number_image, 592, y_position_1, video_mem);
        get_image_from_number_high_score(&number_image, all_high_score_images, high_scores[i].game_values.score_digits[1]);
        image_load_to_frame_buffer(number_image, 612, y_position_1, video_mem);
        get_image_from_number_high_score(&number_image, all_high_score_images, high_scores[i].game_values.score_digits[2]);
        image_load_to_frame_buffer(number_image, 632, y_position_1, video_mem);
        get_image_from_number_high_score(&number_image, all_high_score_images, high_scores[i].game_values.score_digits[3]);
        image_load_to_frame_buffer(number_image, 652, y_position_1, video_mem);
    }

    return 0;
}




