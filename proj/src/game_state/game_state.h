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

enum type_of_entity {
    EMPTY,
    PLAYER,
    ENEMY,
    WALL, // spawners also count as walls
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

struct main_menu_images {
    struct image_struct* background;
    struct image_struct* cursor;
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

void do_nothing(void);

#endif
