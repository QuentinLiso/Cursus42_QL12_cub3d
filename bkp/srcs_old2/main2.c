#include <math.h>
#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 640
#define HEIGHT 480
#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define MOVE_SPEED 0.1
#define ROTATE_SPEED 0.05

// Keycodes
#define KEY_W 13
#define KEY_S 1
#define KEY_A 0
#define KEY_D 2
#define KEY_LEFT 123
#define KEY_RIGHT 124
#define KEY_ESC 53

// Player structure
typedef struct s_player {
    double x, y;        // Position
    double dirX, dirY;  // Direction vector
    double planeX, planeY; // Camera plane
} t_player;

// MiniLibX structure
typedef struct s_mlx {
    void *mlx;
    void *win;
    void *img;
    char *addr;
    int bpp;
    int line_length;
    int endian;
} t_mlx;

// Game structure
typedef struct s_game {
    t_mlx mlx;
    t_player player;
} t_game;

// Map (1 = wall, 0 = empty space)
int worldMap[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,1,0,1,1,1,0,0,1},
    {1,0,1,0,0,0,1,0,0,1},
    {1,0,1,1,1,0,1,0,0,1},
    {1,0,0,0,0,0,1,0,0,1},
    {1,0,1,1,1,1,1,0,0,1},
    {1,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};

// Put pixel in image
void my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        char *dst = mlx->addr + (y * mlx->line_length + x * (mlx->bpp / 8));
        *(unsigned int*)dst = color;
    }
}

// Raycasting function
void raycast(t_game *game) {
    t_mlx *mlx = &game->mlx;
    t_player *player = &game->player;

    // Clear previous frame
    mlx_destroy_image(mlx->mlx, mlx->img);
    mlx->img = mlx_new_image(mlx->mlx, WIDTH, HEIGHT);
    mlx->addr = mlx_get_data_addr(mlx->img, &mlx->bpp, &mlx->line_length, &mlx->endian);

    for (int x = 0; x < WIDTH; x++) {
        double cameraX = 2 * x / (double)WIDTH - 1;
        double rayDirX = player->dirX + player->planeX * cameraX;
        double rayDirY = player->dirY + player->planeY * cameraX;

        int mapX = (int)player->x;
        int mapY = (int)player->y;

        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
        double sideDistX, sideDistY;

        int stepX = (rayDirX < 0) ? -1 : 1;
        int stepY = (rayDirY < 0) ? -1 : 1;

        if (rayDirX < 0)
            sideDistX = (player->x - mapX) * deltaDistX;
        else
            sideDistX = (mapX + 1.0 - player->x) * deltaDistX;
        
        if (rayDirY < 0)
            sideDistY = (player->y - mapY) * deltaDistY;
        else
            sideDistY = (mapY + 1.0 - player->y) * deltaDistY;

        int hit = 0;
        int side;

        while (!hit) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (worldMap[mapY][mapX] > 0)
                hit = 1;
        }

        double perpWallDist = (side == 0) ?
            (mapX - player->x + (1 - stepX) / 2) / rayDirX :
            (mapY - player->y + (1 - stepY) / 2) / rayDirY;

        int lineHeight = (int)(HEIGHT / perpWallDist);
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;

        int color = (side == 1) ? 0x555555 : 0xFFFFFF;
        for (int y = drawStart; y < drawEnd; y++)
            my_mlx_pixel_put(mlx, x, y, color);
    }

    // Display the image
    mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img, 0, 0);
}

// Player movement
int key_press(int keycode, t_game *game) {
    t_player *p = &game->player;

    if (keycode == KEY_W) {
        if (worldMap[(int)(p->y)][(int)(p->x + p->dirX * MOVE_SPEED)] == 0)
            p->x += p->dirX * MOVE_SPEED;
        if (worldMap[(int)(p->y + p->dirY * MOVE_SPEED)][(int)(p->x)] == 0)
            p->y += p->dirY * MOVE_SPEED;
    }
    if (keycode == KEY_S) {
        if (worldMap[(int)(p->y)][(int)(p->x - p->dirX * MOVE_SPEED)] == 0)
            p->x -= p->dirX * MOVE_SPEED;
        if (worldMap[(int)(p->y - p->dirY * MOVE_SPEED)][(int)(p->x)] == 0)
            p->y -= p->dirY * MOVE_SPEED;
    }
    if (keycode == KEY_LEFT || keycode == KEY_RIGHT) {
        double rot = (keycode == KEY_LEFT) ? -ROTATE_SPEED : ROTATE_SPEED;
        double oldDirX = p->dirX;
        double oldPlaneX = p->planeX;

        p->dirX = p->dirX * cos(rot) - p->dirY * sin(rot);
        p->dirY = oldDirX * sin(rot) + p->dirY * cos(rot);
        p->planeX = p->planeX * cos(rot) - p->planeY * sin(rot);
        p->planeY = oldPlaneX * sin(rot) + p->planeY * cos(rot);
    }
    if (keycode == KEY_ESC)
        exit(0);
    return 0;
}

// Main function
int main() {
    t_game game;
    game.mlx.mlx = mlx_init();
    game.mlx.win = mlx_new_window(game.mlx.mlx, WIDTH, HEIGHT, "Raycaster");
    game.mlx.img = mlx_new_image(game.mlx.mlx, WIDTH, HEIGHT);
    game.mlx.addr = mlx_get_data_addr(game.mlx.img, &game.mlx.bpp, &game.mlx.line_length, &game.mlx.endian);

    mlx_hook(game.mlx.win, 2, 1L << 0, key_press, &game);
    mlx_loop_hook(game.mlx.mlx, (void *)raycast, &game);
    mlx_loop(game.mlx.mlx);
}
