/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:28:52 by qliso             #+#    #+#             */
/*   Updated: 2025/03/17 18:52:44 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


void    put_pixel(int x, int y, int color, t_game *game)
{
    int index;

    if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
        return ;
    index = y * game->size_line + x * game->bpp / 8;
    game->data[index] = color & 0xFF;
    game->data[index + 1] = (color >> 8) & 0xFF;
    game->data[index + 2] = (color >> 16) & 0xFF; 
    
}

void    draw_square(int x, int y, int size, int color, t_game *game)
{
    int i;

    i = -1;
    while (++i <= size)
        put_pixel(x + i, y, color, game);
    i = -1;
    while (++i <= size)
        put_pixel(x, y + i, color, game);
    i = -1;
    while (++i <= size)
        put_pixel(x + i, y + size, color, game);
    i = -1;
    while (++i <= size)
        put_pixel(x + size, y + i, color, game);
}

void    draw_map(t_game *game)
{
    char    **map;
    int     color;
    int     x;
    int     y;

    map = game->map;
    color = 0x0000FF;
    y = -1;
    while (map[++y])
    {
        x = -1;
        while (map[y][++x])
        {
            if (map[y][x] == '1')
                draw_square(x * BLOCK, y * BLOCK, BLOCK, color, game);
        }
    }
}

void    clear_image(t_game *game)
{
    int x;
    int y;

    y = -1;
    while (++y < HEIGHT)
    {
        x = -1;
        while (++x < WIDTH)
            put_pixel(x, y, 0x000000, game);
    }
}

void    init_game(t_game *game)
{
    init_player(&game->player);
    game->debug = false;
    game->update = true;
    game->map = get_map();
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Cub3D");
    game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

char    **get_map(void)
{
    char    **map;

    map = malloc(sizeof(char *) * 11);
    if (!map)
        return (NULL);
    map[0] = "111111111111111";
    map[1] = "100000000000001";
    map[2] = "100000000000001";
    map[3] = "100000000000001";
    map[4] = "100001000000001";
    map[5] = "100000000000001";
    map[6] = "100000000000001";
    map[7] = "100001000000001";
    map[8] = "101000000000001";
    map[9] = "111111111111111";
    map[10] = NULL;
    return (map);
}

bool    touch(float px, float py, t_game *game)
{
    int x;
    int y;

    x = px / BLOCK;
    y = py / BLOCK;
    if (game->map[y][x] == '1')
        return (true);
    return (false);
}

float   distance(float x, float y)
{
    return (sqrt(x * x + y * y));
}

float   fixed_distance(t_player *player)
{
    float   delta_x;
    float   delta_y;
    float   angle;
    float   fix_dist;

    delta_x = player->x_ray - player->x;
    delta_y = player->y_ray - player->y;
    angle = atan2(delta_y, delta_x) - player->angle;
    fix_dist = distance(delta_x, delta_y) * cos(angle);
    return (fix_dist);
}

void    draw_line(t_player *player, t_game *game, float start, int i)
{
    float   x_angle;
    float   y_angle;
    t_line  line;
    (void)i;

    x_angle = cos(start);
    y_angle = sin(start);
    player->x_ray = player->x;
    player->y_ray = player->y;
    while (!touch(player->x_ray, player->y_ray, game))
    {
        if (game->debug)
            put_pixel(player->x_ray, player->y_ray, 0xFF0000, game);
        player->x_ray += x_angle;
        player->y_ray += y_angle;
    }
    line.dist = fixed_distance(player);
    line.height = (BLOCK / line.dist) * (WIDTH / 2);
    line.start_y = (HEIGHT - line.height) / 2;
    line.end = line.start_y + line.height;
    while (line.start_y < line.end)
    {
        if (!game->debug)
            put_pixel(i, line.start_y, 0x0000FF, game);
        line.start_y++;
    }
}


void    draw_raycast(t_player *player, t_game *game)
{
    float   fraction;
    float   start;
    int     i;
    
    fraction = PI / 3 / WIDTH;
    start = player->angle - PI / 6;
    i = -1;
    while (++i < WIDTH)
    {
        draw_line(player, game, start, i);
        start += fraction;
    }
        
}


int draw_loop(t_game *game)
{
    t_player *player;

    player = &game->player;
    move_player(player);
    clear_image(game);
    if (game->debug)
        draw_square(player->x, player->y, 10, 0x00FF00, game);
    if (game->debug)
        draw_map(game);
    draw_raycast(player, game);
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
    return (0);
}

int closing_window()
{
    exit (0);
}

int main(void)
{
    t_game  game;
    
    init_game(&game);
    
    mlx_hook(game.win, KeyPress, KeyPressMask, key_press, &game);
    mlx_hook(game.win, KeyRelease, KeyReleaseMask, key_release, &game);
    mlx_hook(game.win, DestroyNotify, Button1Mask,
        &closing_window, NULL);
    mlx_loop_hook(game.mlx, draw_loop, &game);
    
    mlx_loop(game.mlx);
    return (0);
}