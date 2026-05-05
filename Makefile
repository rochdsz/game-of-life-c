CC = gcc

CFLAGS = -Wall -Wextra -std=gnu11 -O2

SRC = src/life.c src/grid.c
OBJ = $(SRC:.c=.o)

TARGET = life

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)