# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/devices/
.PATH: ${.CURDIR}/devices/common_keyboard_and_mouse
.PATH: ${.CURDIR}/devices/graphics
.PATH: ${.CURDIR}/devices/keyboard
.PATH: ${.CURDIR}/devices/mouse
.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/devices/rtc
.PATH: ${.CURDIR}/utils
.PATH: ${.CURDIR}/game_state
.PATH: ${.CURDIR}/xpm_files
# source code files to be compiled
SRCS = main.c KBC_keyboard.c KBC_mouse.c graphics.c utils.c keyboard_and_mouse.c timer.c game_state.c rtc.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
