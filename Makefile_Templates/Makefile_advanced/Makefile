CC      := gcc
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
ARGS    := Test
TARGET  := prog

EXE := $(BIN_DIR)/$(TARGET)
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CFLAGS := -Wall -Wextra -pedantic -Iinclude -g
LDFLAGS  := -Llib
LDLIBS   := -lm

.PHONY: all clean

all: $(EXE)

release: CFLAGS=-Wall -Wextra -pedantic -O2 -Iinclude
release: clean
release: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@
	@echo "Linking complete!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)
	@echo "Cleanup complete!"

run:
	./$(EXE) $(ARGS)

gdb:
	gdb --args $(EXE) $(ARGS)

-include $(OBJ:.o=.d)
