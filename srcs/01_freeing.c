/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_freeing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:49:50 by qliso             #+#    #+#             */
/*   Updated: 2025/03/27 15:36:57 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void     free_game(t_game *game)
{
    if (!game)
        return ;
    free_game_mlx(game);
    free_game_textures(game);
    free_game_pixels(game);
    free_game_map(game);
}

void    free_game_mlx(t_game *game)
{
    if (game->mlx && game->win)
    {
        if (game->mmap.img.img)
            mlx_destroy_image(game->mlx, game->mmap.img.img);
        mlx_hook(game->win, MotionNotify, 0, NULL, NULL);
        mlx_destroy_window(game->mlx, game->win);
    }
    if (game->mlx)
    {
        mlx_destroy_display(game->mlx);
        mlx_loop_end(game->mlx);
        free(game->mlx);
    }
}

void    free_game_textures(t_game *game)
{
    free_str(&game->texs[NORTH].path);
    free_str(&game->texs[SOUTH].path);
    free_str(&game->texs[WEST].path);
    free_str(&game->texs[EAST].path);
}

void    free_game_pixels(t_game *game)
{
    int i;
    t_draw  *draw;

    draw = &game->draw;
    if (draw->pixels)
    {
        i = -1;
        while (++i < game->win_height)
            free(draw->pixels[i]);
        free(draw->pixels);
    }
}

void    free_game_map(t_game *game)
{
    if (game->mapdata.fd > 0)
        close(game->mapdata.fd);
    if (game->mapdata.filecontent)
        free_arr((void **)game->mapdata.filecontent);
    if (game->map)
        free_arr((void **)game->map);
}

void    free_arr(void **arr)
{
    int i;

    if (!arr)
        return ;
    i = -1;
    while (arr[++i])
        free(arr[i]);
    free(arr);
}

void    free_str(char **str)
{
    if (!*str)
        return ;
    free(*str);
    *str = NULL;
}
