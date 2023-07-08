CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g
SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(patsubst %.c, %.o, $(SRC_FILES))

all: MAIN

MAIN: $(OBJ_FILES)
	$(CC) -o MAIN $(CFLAGS) $(OBJ_FILES) -I.

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I.

clean:
	rm -f MAIN $(OBJ_FILES)
