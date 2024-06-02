#ifndef _LCOM_GAME_STATE_H_
#define _LCOM_GAME_STATE_H_
///////////////////////////
///// define high_scores.txt file path name (change this to the correct path name for your computer)
#define HIGH_SCORES_FILE_PATH_NAME "/home/lcom/labs/g5/proj/src/high_scores.txt"
//#define HIGH_SCORES_FILE_PATH_NAME "/home/lcom/labs/proj/src/high_scores.txt"
///////////////////////////
#include <lcom/lcf.h>
#include "../devices/graphics/graphics.h"
#include "../devices/rtc/rtc.h"

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
    bool move_horizontally;
};

struct game_entities_position {
    enum type_of_entity array_of_rows_of_entities[14][23];
    struct position player_position;
    struct enemy enemy_structs[8];
    struct position spawners_positions[4];
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

struct date_time {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t month;
    uint8_t year;
};

struct high_score {
    struct game_values game_values;
    struct date_time date_time_of_score;
    bool is_active;
};

int handle_main_menu(int mouse_position_x, int mouse_position_y, enum game_state* game_state, bool *is_start_of_game_state, bool *close_application);

int load_game_state_to_game_buffer(struct game_entities_position *all_game_state_entities_position, struct game_values *current_game_state_values, struct game_images *all_game_images, uint8_t* video_mem);

int get_image_from_number_game(struct image_struct**  result_image, struct game_images *all_game_images, int number);

int convert_game_position_to_frame_buffer_position(int game_position_x, int game_position_y, int *frame_buffer_position_x, int *frame_buffer_position_y);

int spawn_monsters(struct game_entities_position *all_game_state_entities_position);

int increment_score(struct game_values *current_game_state_values);

int increment_timer(struct game_values *current_game_state_values);

int fill_spawns(struct game_entities_position *all_game_state_entities_position);

int convert_frame_buffer_position_to_game_position(int frame_buffer_position_x, int frame_buffer_position_y, int *game_position_x, int *game_position_y);

int check_monster_if_the_space_can_be_moved_into(struct game_entities_position *all_game_state_entities_position, int x, int y, bool *can_move, bool *killed_player);

int check_player_if_the_space_can_be_moved_into(struct game_entities_position *all_game_state_entities_position, int x, int y, bool *can_move);

int handle_game_m1_interrupt(struct game_entities_position *all_game_state_entities_position, struct game_values *current_game_state_values, int mouse_position_x, int mouse_position_y, bool *was_game_state_changed, bool *is_game_quit);

int handle_game_timer_tick_interrupt(struct game_entities_position *all_game_state_entities_position, struct game_values *current_game_state_values, bool *was_game_state_changed, bool *is_game_over);

int handle_game_timer_second_interrupt(struct game_entities_position *all_game_state_entities_position, struct game_values *current_game_state_values, enum letter_pressed *last_key_pressed, bool *was_game_state_changed);

int handle_game_over_interrupt(int mouse_position_x, int mouse_position_y, enum game_state* game_state, bool *is_start_of_screen);

int handle_high_score_interrupt(int mouse_position_x, int mouse_position_y, enum game_state* game_state, bool *is_start_of_screen);

int store_high_score_at_this_time(struct game_values *game_values);

int write_high_scores(struct high_score high_scores[5]);

int read_high_scores(struct high_score high_scores[5]);

int get_image_from_number_high_score(struct image_struct**  result_image, struct high_score_images *all_high_score_images, int number);

int load_high_scores_to_game_buffer(struct high_score_images *all_high_score_images, uint8_t* video_mem);

#endif
