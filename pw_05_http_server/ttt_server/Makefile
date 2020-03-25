CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0` -Wall -Wextra -pthread -fsanitize=address
LDFLAGS = `pkg-config --libs glib-2.0` -pthread -fsanitize=address

PRG = ttt_server

ttt_server: ttt_server.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

all: $(PRG)


.PHONY: clean

clean:
	rm -f $(PRG) *.o
