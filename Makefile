CC=cc
CFLAGS=-Wall -Wextra -Weverything -Wpedantic -Wpedantic -std=c99
BIN=aoc2019
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

all: $(BIN)
	@echo "Compiled Advent of Code 2019!"

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN)

clean:
	rm -f $(OBJS)
	rm -f $(BIN)
