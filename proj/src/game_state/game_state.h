#ifndef _LCOM_GAME_STATE_H_
#define _LCOM_GAME_STATE_H_

#include <lcom/lcf.h>

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


struct all_devices_interrupts {
    bool is_timer_interrupt;
    bool is_keyboard_interrupt;
    bool is_mouse_interrupt;
};

enum interrupt_type {
    TIMER_SECOND,
    TIMER_TICK,
    KEYBOARD,
    MOUSE,
};

void do_nothing(void);

#endif
