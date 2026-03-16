#include "raycast.h"

int worldMap[MAP_SIZE][MAP_SIZE] = {
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,2,2,2,0,0,0,3,3,0,1},
    {1,0,2,0,0,0,0,0,3,0,0,1},
    {1,0,2,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,4,4,0,0,0,0,1},
    {1,0,0,0,0,4,4,0,0,2,0,1},
    {1,0,0,0,0,0,0,0,0,2,0,1},
    {1,0,3,3,0,0,0,0,0,0,0,1},
    {1,0,3,0,0,0,0,4,4,0,0,1},
    {1,0,0,0,0,0,0,4,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
};


Uint32 get_color(int texNum, int side) {
    Uint32 color;
    switch(texNum) {
        case 1: color = 0xED6A5A; break;
        case 2: color = 0x9BC1BC; break;
        case 3: color = 0x5CA4A9; break;
        case 4: color = 0xF4F1BB; break;
        default: color = 0x333333; break;
    }
    if (side == 1) {
        Uint8 r = (color >> 16 & 0xFF) / 2;
        Uint8 g = (color >> 8 & 0xFF) / 2;
        Uint8 b = (color & 0xFF) / 2;
        color = (r << 16) | (g << 8) | b;
    }
    return color;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Raycaster Engine", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    Player p = {2, 2, -1, 0, 0, 0.66};
    bool running = true;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED)
                surface = SDL_GetWindowSurface(window);
        }

        SDL_LockSurface(surface);
        Uint32* pixels = (Uint32*)surface->pixels;
        int w = surface->w;
        int h = surface->h;

        for (int x = 0; x < w; x++) {
            double cameraX = 2 * x / (double)w - 1;
            double rayDirX = p.dirX + p.planeX * cameraX;
            double rayDirY = p.dirY + p.planeY * cameraX;

            int mapX = (int)p.x;
            int mapY = (int)p.y;

            double deltaDistX = fabs(1 / rayDirX);
            double deltaDistY = fabs(1 / rayDirY);
            
            int stepX, stepY, hit = 0, side;
            double sideDistX, sideDistY;

            if (rayDirX < 0) { stepX = -1; sideDistX = (p.x - mapX) * deltaDistX; }
            else { stepX = 1; sideDistX = (mapX + 1.0 - p.x) * deltaDistX; }
            if (rayDirY < 0) { stepY = -1; sideDistY = (p.y - mapY) * deltaDistY; }
            else { stepY = 1; sideDistY = (mapY + 1.0 - p.y) * deltaDistY; }

            while (hit == 0) {
                if (sideDistX < sideDistY) { sideDistX += deltaDistX; mapX += stepX; side = 0; }
                else { sideDistY += deltaDistY; mapY += stepY; side = 1; }
                if (worldMap[mapX][mapY] > 0) hit = 1;
            }

            double perpWallDist = (side == 0) ? (sideDistX - deltaDistX) : (sideDistY - deltaDistY);
            int lineHeight = (int)(h / perpWallDist);

            int drawStart = -lineHeight / 2 + h / 2;
            if (drawStart < 0) drawStart = 0;
            int drawEnd = lineHeight / 2 + h / 2;
            if (drawEnd >= h) drawEnd = h - 1;

            Uint32 wallColor = get_color(worldMap[mapX][mapY], side);

            for (int y = 0; y < h; y++) {
                if (y < drawStart) pixels[y * w + x] = 0x222222;
                else if (y > drawEnd) pixels[y * w + x] = 0x111111;
                else pixels[y * w + x] = wallColor;
            }
        }
        SDL_UnlockSurface(surface);

        const Uint8* keys = SDL_GetKeyboardState(NULL);
        double mSpd = 0.08, rSpd = 0.05;
        if (keys[SDL_SCANCODE_W]) {
            if (worldMap[(int)(p.x + p.dirX * mSpd)][(int)p.y] == 0) p.x += p.dirX * mSpd;
            if (worldMap[(int)p.x][(int)(p.y + p.dirY * mSpd)] == 0) p.y += p.dirY * mSpd;
        }
        if (keys[SDL_SCANCODE_S]) {
            if (worldMap[(int)(p.x - p.dirX * mSpd)][(int)p.y] == 0) p.x -= p.dirX * mSpd;
            if (worldMap[(int)p.x][(int)(p.y - p.dirY * mSpd)] == 0) p.y -= p.dirY * mSpd;
        }
        if (keys[SDL_SCANCODE_D]) {
            double oldDirX = p.dirX;
            p.dirX = p.dirX * cos(-rSpd) - p.dirY * sin(-rSpd);
            p.dirY = oldDirX * sin(-rSpd) + p.dirY * cos(-rSpd);
            double oldPlaneX = p.planeX;
            p.planeX = p.planeX * cos(-rSpd) - p.planeY * sin(-rSpd);
            p.planeY = oldPlaneX * sin(-rSpd) + p.planeY * cos(-rSpd);
        }
        if (keys[SDL_SCANCODE_A]) {
            double oldDirX = p.dirX;
            p.dirX = p.dirX * cos(rSpd) - p.dirY * sin(rSpd);
            p.dirY = oldDirX * sin(rSpd) + p.dirY * cos(rSpd);
            double oldPlaneX = p.planeX;
            p.planeX = p.planeX * cos(rSpd) - p.planeY * sin(rSpd);
            p.planeY = oldPlaneX * sin(rSpd) + p.planeY * cos(rSpd);
        }

        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}