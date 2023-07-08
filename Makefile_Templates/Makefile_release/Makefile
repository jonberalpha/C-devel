# Requires: src, inc, obj and bin - directory, where the source/header files put in src and inc
#make ... compile/link with debug information
#make release ... compile/link without debug information
#make clean ... delete object-files and binary
#make remove ... delete binary

# project name (generate executable with this name)
TARGET   = DLL

# compiling flags here | # linking flags here
CFLAGS   = -Wall -pedantic -Wextra -g

CC       = gcc
LINKER   = gcc

# change these to proper directories where each file should be
SRCDIR   = ./src
INCDIR   = ./inc
OBJDIR   = ./obj
BINDIR   = ./bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm = rm -f

all: $(BINDIR)/$(TARGET)

release: CFLAGS=-Wall -pedantic -Wextra -O2 -DNDEBUG
release: clean
release: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@ -I$(INCDIR)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(INCDIR)
	@echo "Compiled "$<" successfully!"

.PHONY: remove
remove:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: clean
clean: remove
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
