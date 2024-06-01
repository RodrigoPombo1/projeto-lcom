#ifndef _LCOM_GAME_STATE_H_
#define _LCOM_GAME_STATE_H_

#include <lcom/lcf.h>
#include "../devices/graphics/graphics.h"

enum game_state {
    MAIN_MENU,
    GAME,
    GAME_OVER,
    HIGH_SCORE,
};

enum letter_pressed { // [TODO] remove? no longer used, probably could be used inside the game state functions despite not being used in main
    NO_LETTER_PRESSED,
    W,
    A,
    S,
    D,
};

struct position {
    int x;
    int y;
};

enum type_of_entity {
    EMPTY,
    PLAYER,
    ENEMY,
    WALL, // spawners also count as walls
};

struct enemy {
    struct position position;
    int id;
    bool is_alive;
};

struct game_entities_position {
    enum type_of_entity array_of_rows_of_entities[14][23];
    struct position player_position;
    struct enemy enemy_structs[8];
    struct position spawner_position[4];
    int last_spawn_used;
    int array_of_entities_h_size;
    int array_of_entities_v_size;
};

struct game_values {
    int score_digits[4];
    int score;
    int time_digits[4];
    int time_in_seconds;
};

struct all_devices_interrupts {
    bool is_timer_second_interrupt;
    bool is_timer_tick_interrupt;
    bool W;
    bool A;
    bool S;
    bool D;
    bool is_mouse_move_interrupt;
    bool m1;
};

struct game_images {
    struct image_struct* background;
    struct image_struct* player;
    struct image_struct* enemy;
    struct image_struct* cursor;
    struct image_struct* game_over;
    struct image_struct* character_2_pontos;
    struct image_struct* number_0;
    struct image_struct* number_1;
    struct image_struct* number_2;
    struct image_struct* number_3;
    struct image_struct* number_4;
    struct image_struct* number_5;
    struct image_struct* number_6;
    struct image_struct* number_7;
    struct image_struct* number_8;
    struct image_struct* number_9;
};

struct high_score_images {
    struct image_struct* background;
    struct image_struct* cursor;
    struct image_struct* character_2_pontos;
    struct image_struct* character_tracinho;
    struct image_struct* number_0;
    struct image_struct* number_1;
    struct image_struct* number_2;
    struct image_struct* number_3;
    struct image_struct* number_4;
    struct image_struct* number_5;
    struct image_struct* number_6;
    struct image_struct* number_7;
    struct image_struct* number_8;
    struct image_struct* number_9;
};

enum interrupt_type { // [TODO] remove? no longer used
    TIMER_SECOND,
    TIMER_TICK,
    KEYBOARD,
    MOUSE,
};

int handle_main_menu(int mouse_position_x, int mouse_position_y, enum game_state* game_state, bool *is_start_of_game_state, bool *close_application);

int load_game_state_to_game_buffer(struct game_entities_position *all_game_state_entities_position, struct game_values *current_game_state_values, struct game_images *all_game_images, uint8_t* video_mem);

int get_image_from_number_game(struct image_struct* result_image, struct game_images *all_game_images, int number);

int convert_game_position_to_frame_buffer_position(int game_position_x, int game_position_y, int *frame_buffer_position_x, int *frame_buffer_position_y);
// update game state mouse just takes the array and the mouse position
// update game state keyboard just takes the array and the last key pressed
// load game state on background just takes the array and the background and the game frame buffer
#endif
