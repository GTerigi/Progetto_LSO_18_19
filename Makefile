CC		:= gcc
C_FLAGS := -Wall

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:= -lFunzioni

SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(BIN)/%.o, $(SOURCES))


all: $(OBJECTS)

$(BIN)/%.o: $(SRC)/%.c
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) -I$(LIB) $^ -o $@ $(LIBRARIES) -lpthread

clear:
	-rm $(BIN)/*

test:
	-bash ./test1.sh
