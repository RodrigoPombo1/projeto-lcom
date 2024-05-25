# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/devices/
.PATH: ${.CURDIR}/devices/graphics
.PATH: ${.CURDIR}/devices/keyboard
.PATH: ${.CURDIR}/devices/mouse
.PATH: ${.CURDIR}/utils

# source code files to be compiled
SRCS = main.c KBC_keyboard.c KBC_mouse.c graphics.c utils.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
