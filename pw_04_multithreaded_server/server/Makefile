#Makefile

CPPFLAGS = -MMD
CC = gcc -pthread -fsanitize=address
CFLAGS = -Wall -Wextra

SRC = shared_queue.c main.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: shared_queue.o main.o

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main

# END
