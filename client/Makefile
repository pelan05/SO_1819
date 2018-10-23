CC		:= gcc
C_FLAGS := -Wall -Wextra

BIN		:= bin
SRC		:= src

LIBRARIES	:= -lncurses

EXECUTABLE	:= client

all: $(BIN)/$(EXECUTABLE)

clean:
	-$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*
	$(CC) $(C_FLAGS) $^ -o $@ $(LIBRARIES)