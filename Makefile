CC = gcc
CFLAGS = -Wall -Wextra -pedantic

.PHONY: all clean

all: server client

server: server.o 
	$(CC) $(CFLAGS) -o $@ $^

client: client.o 
	$(CC) $(CFLAGS) -o $@ $^

%.0: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	rm -f server client *.0