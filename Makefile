CC = gcc
CFLAGS = -std=c23 -Wall -Isrc
LIBS = -lmingw32 -lSDL2main -lSDL2 \
       -mwindows -Wl,--dynamicbase -Wl,--nxcompat -Wl,--high-entropy-va \
       -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 \
       -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid
       
SRC = src/raycast.c
OBJ_DIR = build
TARGET = $(OBJ_DIR)/raycaster.exe

all: debug

debug: CFLAGS += -g
debug: $(TARGET)

release: CFLAGS += -O2 -static
release: $(TARGET)

$(TARGET): $(SRC)
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)