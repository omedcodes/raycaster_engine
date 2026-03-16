#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

#define WIDTH 900
#define HEIGHT 600
#define MAP_SIZE 12

typedef struct {
    double x, y;
    double dirX, dirY;
    double planeX, planeY;
} Player;

Uint32 get_color(int texNum, int side);