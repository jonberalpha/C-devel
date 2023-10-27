CC = gcc
CFLAGS = -Wall -Wextra -pedantic -c
LDFLAGS = -L$(OUT_DIR)
LDTESTFLAGS = "-Wl,-rpath,./$(OUT_DIR)/"
LIBRARY_NAME = mylibrary

SOURCE_DIR = src
INCLUDE_DIR = include
OBJECT_DIR = obj
OUT_DIR = lib
TEST_DIR = test

SOURCE_FILES = $(wildcard $(SOURCE_DIR)/*.c)
OBJECT_FILES = $(patsubst $(SOURCE_DIR)/%.c, $(OBJECT_DIR)/%.o, $(SOURCE_FILES))
LIBRARY_FILE = $(OUT_DIR)/lib$(LIBRARY_NAME).so

TEST_SOURCE_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_HEADER_FILES = $(wildcard $(TEST_DIR)/*.h)
TEST_OBJECT_FILES = $(patsubst $(TEST_DIR)/%.c, $(OBJECT_DIR)/%.o, $(TEST_SOURCE_FILES))
TEST_EXECUTABLE = $(OUT_DIR)/test_program

all: $(LIBRARY_FILE)

$(LIBRARY_FILE): $(OBJECT_FILES)
	@mkdir -p $(OUT_DIR)
	$(CC) $(LDFLAGS) -shared -o $@ $^

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(OBJECT_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -fPIC -o $@ $<

$(OBJECT_DIR)/%.o: $(TEST_DIR)/%.c $(TEST_HEADER_FILES)
	@mkdir -p $(OBJECT_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(TEST_DIR) -o $@ $<

test: $(LIBRARY_FILE) $(TEST_OBJECT_FILES)
	@mkdir -p $(OUT_DIR)
	$(CC) -o $(TEST_EXECUTABLE) $(TEST_OBJECT_FILES) -l$(LIBRARY_NAME) $(LDFLAGS) $(LDTESTFLAGS)
	$(TEST_EXECUTABLE)

clean:
	rm -f $(LIBRARY_FILE) $(OBJECT_DIR)/*.o $(OUT_DIR)/test_program
	rm -rf $(OUT_DIR) $(OBJECT_DIR)

