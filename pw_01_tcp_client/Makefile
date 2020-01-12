CC = gcc
CFLAGS  = -fsanitize=address -Wall -Wextra -std=c99 -O2
LDFLAGS = -fsanitize=address

PRG = main

main: print_page.o main.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

all: $(PRG)

.PHONY: clean

clean:
	rm -f $(PRG) *.o
