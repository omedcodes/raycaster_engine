#include <SDL2/SDL.h>

#define WIDTH 900
#define HEIGHT 600

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Raycasting Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Delay(3000);
    SDL_Quit();
    return 0;
}
