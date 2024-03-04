CC = gcc
CFLAGS = -Wall -Wextra -pedantic

.PHONY: all clean

all: main

program: main.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f main main.o