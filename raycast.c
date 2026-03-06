#include <SDL2/SDL.h>
#include <stdbool.h>

#define WIDTH 900
#define HEIGHT 600

#define PLAYER_FOV 80

#define COLOR_WHITE 0xFFFFFFFF
#define PLAYER_COLOR 0xFF00FF00
#define CELL_SIZE 50

int map[5][5] = {
    {0, 0, 0, 1, 0},
    {0, 0, 1, 1, 0},
    {1, 0, 0, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 0, 1},
};

typedef struct {
    float x;
    float y;
    float angle;
} Player;

Player player = { .x = 0.0f, .y = 0.0f, .angle = 45.0f };

void drawPlayer(SDL_Surface* surface, Player p) {
    SDL_Rect player_rect = { .x = (int)p.x, .y = (int)p.y, .w = 10, .h = 10 };
    SDL_FillRect(surface, &player_rect, PLAYER_COLOR);
}

int main(int argc, char* argv[]) 
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("Raycasting Engine",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    SDL_Surface* surface = SDL_GetWindowSurface(window);
    bool running = true;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        SDL_FillRect(surface, NULL, 0x00000000);
        drawPlayer(surface, player);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}