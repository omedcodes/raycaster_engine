#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define WIDTH 900
#define HEIGHT 600

#define PLAYER_FOV 80

#define COLOR_WHITE 0xFFFFFFFF
#define PLAYER_COLOR 0xFF00FF00
#define WALL_COLOR 0xFFAAAAAA
#define CELL_SIZE 50

#define MAP_WIDTH 5
#define MAP_HEIGHT 5

#define RAY_STEP_SIZE 0.5 
#define RENDER_DISTANCE 1000

#define SCENE_WIDTH MAP_WIDTH*CELL_SIZE
#define SCENE_HEIGHT MAP_HEIGHT*CELL_SIZE

int map[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1},
};

typedef struct {
    double x;
    double y;
    double angle;
} Player;

Player player = { .x = 100.0, .y = 100.0, .angle = 0.0 };

void drawPlayer(SDL_Surface* surface, Player p) {
    SDL_Rect player_rect = { .x = (int)p.x, .y = (int)p.y, .w = 10, .h = 10 };
    SDL_FillRect(surface, &player_rect, PLAYER_COLOR);
}

double get_distance(Player p, double angle) {
    double ray_x = p.x;
    double ray_y = p.y;
    double cos_a = cos(angle);
    double sin_a = sin(angle);

    while (true) {
        ray_x += cos_a * RAY_STEP_SIZE;
        ray_y += sin_a * RAY_STEP_SIZE;

        double ray_distance = sqrt(pow(ray_x - p.x, 2) + pow(ray_y - p.y, 2));
        if (ray_distance > RENDER_DISTANCE) return -1;

        int cell_x = (int)ray_x / CELL_SIZE;
        int cell_y = (int)ray_y / CELL_SIZE;

        if (cell_x < 0 || cell_x >= MAP_WIDTH || cell_y < 0 || cell_y >= MAP_HEIGHT) 
            return -1;

        if (map[cell_y][cell_x] == 1) {
            return ray_distance;
        }
    }
    return -1;
}

void draw_fov(SDL_Surface* surface, Player p) {
    double fov_rad = PLAYER_FOV * M_PI / 180.0;
    double start_angle = p.angle - (fov_rad / 2.0);

    for (int x = 0; x < WIDTH; x++) {
        double current_angle = start_angle + (x / (double)WIDTH) * fov_rad;
        double dist = get_distance(p, current_angle);

        if (dist > 0) {
            dist *= cos(current_angle - p.angle);

            int wall_height = (CELL_SIZE * HEIGHT) / dist;
            if (wall_height > HEIGHT) wall_height = HEIGHT;

            int draw_start = (HEIGHT / 2) - (wall_height / 2);
            
            SDL_Rect wall_slice = { .x = x, .y = draw_start, .w = 1, .h = wall_height };
            SDL_FillRect(surface, &wall_slice, WALL_COLOR);
        }
    }
}

int main(int argc, char* argv[]) {
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

        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_W]) {
            player.x += cos(player.angle) * 3.0;
            player.y += sin(player.angle) * 3.0;
        }
        if (state[SDL_SCANCODE_S]) {
            player.x -= cos(player.angle) * 3.0;
            player.y -= sin(player.angle) * 3.0;
        }
        if (state[SDL_SCANCODE_A]) player.angle -= 0.05;
        if (state[SDL_SCANCODE_D]) player.angle += 0.05;

        SDL_FillRect(surface, NULL, 0x00000000);

        draw_fov(surface, player);
        drawPlayer(surface, player);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}