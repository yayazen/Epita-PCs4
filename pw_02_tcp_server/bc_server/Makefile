# Makefile

CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =

SRC = main.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
PRG = ${SRC:.c=}

main: main.o

all: ${PRG}


.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} ${PRG}

# END
