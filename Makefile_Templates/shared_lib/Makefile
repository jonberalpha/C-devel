CC = gcc
CFLAGS = -Wall -Wextra -pedantic -c
LDFLAGS = -L$(BIN_DIR)
LDTESTFLAGS = "-Wl,-rpath,./$(BIN_DIR)/"
LIBRARY_NAME = mylibrary

SOURCE_DIR = src
INCLUDE_DIR = include
OBJECT_DIR = obj
BIN_DIR = bin
TEST_DIR = test

SOURCE_FILES = $(wildcard $(SOURCE_DIR)/*.c)
OBJECT_FILES = $(patsubst $(SOURCE_DIR)/%.c, $(OBJECT_DIR)/%.o, $(SOURCE_FILES))
LIBRARY_FILE = $(BIN_DIR)/lib$(LIBRARY_NAME).so

TEST_SOURCE_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_HEADER_FILES = $(wildcard $(TEST_DIR)/*.h)
TEST_OBJECT_FILES = $(patsubst $(TEST_DIR)/%.c, $(OBJECT_DIR)/%.o, $(TEST_SOURCE_FILES))
TEST_EXECUTABLE = $(BIN_DIR)/test_program

all: $(LIBRARY_FILE)

$(LIBRARY_FILE): $(OBJECT_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) -shared -o $@ $^

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(OBJECT_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -fPIC -o $@ $<

$(OBJECT_DIR)/%.o: $(TEST_DIR)/%.c $(TEST_HEADER_FILES)
	@mkdir -p $(OBJECT_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(TEST_DIR) -o $@ $<

test: $(LIBRARY_FILE) $(TEST_OBJECT_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $(TEST_EXECUTABLE) $(TEST_OBJECT_FILES) -l$(LIBRARY_NAME) $(LDFLAGS) $(LDTESTFLAGS)
	$(TEST_EXECUTABLE)

clean:
	rm -f $(LIBRARY_FILE) $(OBJECT_DIR)/*.o $(BIN_DIR)/test_program
	rm -rf $(BIN_DIR) $(OBJECT_DIR)

