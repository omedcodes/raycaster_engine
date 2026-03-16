CC = gcc
CFLAGS = -std=c23 -Wall -g
LIBS = -lmingw32 -lSDL2main -lSDL2
TARGET = raycaster
SRC = raycaster.c

all: $(TARGET)

$(TARGET): $(SRC)
		$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
		del $(TARGET).exe