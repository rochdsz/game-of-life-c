CC = gcc

CFLAGS = -Wall -Wextra -std=gnu11 \
         $(shell sdl2-config --cflags)

LDFLAGS = $(shell sdl2-config --libs)

SRC = src/life.c src/grid.c
OBJ = $(SRC:.c=.o)

TARGET = life

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)
