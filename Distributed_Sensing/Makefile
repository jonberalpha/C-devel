# General Global Variables
CC       = gcc
RM       = rm -rf
CFLAGS   = -Wall --pedantic -Wextra -g
PROCPATH = processing
SENSPATH = sensor

# Target 1 - Processing Task Program
TARGET1   = proc
SRCPATH1  = ./$(PROCPATH)/src
INCPATH1  = -I./$(PROCPATH)/inc

# Target 2 - Sensor Task Program
TARGET2   = sens
SRCPATH2  = ./$(SENSPATH)/src
INCPATH2  = -I./$(SENSPATH)/inc

# OBJS     = $(SRC:.c=.o)
SRC1      = $(wildcard $(SRCPATH1)/*.c)
SRC2      = $(wildcard $(SRCPATH2)/*.c)

all : $(TARGET1) $(TARGET2)

$(TARGET1) : $(SRC1)
	$(CC) -o $(TARGET1) $(CFLAGS) $(SRC1) $(INCPATH1)

$(TARGET2) : $(SRC2)
	$(CC) -o $(TARGET2) $(CFLAGS) $(SRC2) $(INCPATH2)

clean:
	$(RM) $(TARGET1) $(TARGET2) core *~
