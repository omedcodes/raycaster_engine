#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

#define WIDTH 900
#define HEIGHT 600

#define PLAYER_FOV 80

#define COLOR_WHITE 0xFFFFFFFF
#define PLAYER_COLOR 0xFF00FF00
#define CELL_SIZE 50

#define MAP_WIDTH 5
#define MAP_HEIGHT 5

#define RAY_STEP_SIZE 0.05

#define RENDER_DISTANCE 1000

#define SCENE_WIDTH MAP_WIDTH*CELL_SIZE
#define SCENE_HEIGHT MAP_HEIGHT*CELL_SIZE

int map[MAP_HEIGHT][MAP_WIDTH] = {
    {0, 0, 0, 1, 0},
    {0, 0, 1, 1, 0},
    {1, 0, 0, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 0, 1},
};

typedef struct {
    double x;
    double y;
    double angle;
} Player;

Player player = { .x = 0.0, .y = 0.0, .angle = 45.0 };

void drawPlayer(SDL_Surface* surface, Player p) {
    SDL_Rect player_rect = { .x = (int)p.x, .y = (int)p.y, .w = 10, .h = 10 };
    SDL_FillRect(surface, &player_rect, PLAYER_COLOR);
}

/* Returns the distance to the next wall 
 * assuming the player looks in the given  direction
 */
double get_distance(Player player, double angle) 
{
    // cast a ray
    bool wall_detected = false;

    double ray_x = player.x;
    double ray_y = player.y;
    double ray_distance = sqrt(pow(ray_x - player.x, 2) + pow(ray_y - player.y, 2));
    while (!wall_detected)
    {
        if (ray_distance > RENDER_DISTANCE) {
            return -1;
        }
        // which cell is the ray inside now?
        int cell_x = ray_x / SCENE_WIDTH;
        int cell_y = ray_y / SCENE_HEIGHT;

        // is the ray inside a wall?
        if (map[cell_y][cell_x] == 1) {
            wall_detected = 1;
            return ray_distance;
        }
        else
        {
            ray_x += cos(angle) * RAY_STEP_SIZE;
            ray_y += sin(angle) * RAY_STEP_SIZE;
        }
    }
    printf("error in get distance");
    exit(-1);
    return -1;
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
        double distance = get_distance(player, 45);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}