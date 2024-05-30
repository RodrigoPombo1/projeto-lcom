#ifndef _LCOM_GAME_STATE_H_
#define _LCOM_GAME_STATE_H_

#include <lcom/lcf.h>

enum game_state {
    MAIN_MENU,
    GAME,
    GAME_OVER,
    HIGH_SCORE,
};

enum letter_pressed { // [TODO] remove? no longer used
    NO_LETTER_PRESSED,
    W,
    A,
    S,
    D,
};

struct keyboard_keys_pressed {
    bool W;
    bool A;
    bool S;
    bool D;
};

struct all_devices_interrupts {
    bool is_timer_second_interrupt;
    bool is_timer_tick_interrupt;
    bool is_keyboard_interrupt;
    bool is_mouse_interrupt;
};

enum interrupt_type { // [TODO] remove? no longer used
    TIMER_SECOND,
    TIMER_TICK,
    KEYBOARD,
    MOUSE,
};

void do_nothing(void);

#endif
