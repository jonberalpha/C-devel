# General Global Variables
CC         = gcc
RM         = rm -rf
CFLAGS     = -Wall --pedantic -Wextra -g
SERVERPATH = server
CLIENTPATH = client

# Target 1 - Server Program
TARGET1  = ascii_server
SRCPATH1 = ./$(SERVERPATH)/src
INCPATH1 = -I./$(SERVERPATH)/inc

# Target 2 - Client Program
TARGET2  = ascii_client
SRCPATH2 = ./$(CLIENTPATH)/src
INCPATH2 = -I./$(CLIENTPATH)/inc

# OBJS   = $(SRC:.c=.o)
SRC1     = $(wildcard $(SRCPATH1)/*.c)
SRC2     = $(wildcard $(SRCPATH2)/*.c)

all : $(TARGET1) $(TARGET2)

$(TARGET1) : $(SRC1)
	$(CC) -o $(TARGET1) $(CFLAGS) $(SRC1) $(INCPATH1)

$(TARGET2) : $(SRC2)
	$(CC) -o $(TARGET2) $(CFLAGS) $(SRC2) $(INCPATH2)

clean:
	$(RM) $(TARGET1) $(TARGET2) core *~
