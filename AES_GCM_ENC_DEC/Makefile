CC = gcc
CFLAGS = -Wall
LFLAGS = -lssl -lcrypto
SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(patsubst %.c, %.o, $(SRC_FILES))
TARTGET = crypto

all: $(TARTGET)

$(TARTGET): $(OBJ_FILES)
	$(CC) -o $(TARTGET) $(CFLAGS) $(OBJ_FILES) $(LFLAGS) -I.

%.o: %.c
	$(CC) $(CFLAGS) -c $< $(LFLAGS) -o $@ -I.

clean:
	rm -f $(TARTGET) $(OBJ_FILES)
