/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:50:23 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 17:02:18 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_game(t_game *game)
{
	game->mlx = NULL;
	game->win = NULL;
	game->win_height = HEIGHT;
	game->win_width = WIDTH;
	init_player(&game->player);
	init_game_tex(game);
	game->draw.pixels = NULL;
	init_mapdata(&game->mapdata);
	game->map = NULL;
	init_minimap(game, &game->mmap);
}

void	init_player(t_player *player)
{
	player->orientation = 0;
	player->spawn = (t_vec2Di){0, 0};
	player->pos = (t_vec2D){0.0, 0.0};
	player->dir = (t_vec2D){0.0, 0.0};
	player->plane = (t_vec2D){0.0, 0.0};
	player->has_moved = 0;
	player->move = (t_vec2Di){0, 0};
	player->rotate = 0;
	player->mouse_rotate = 0;
}

void	init_game_tex(t_game *game)
{
	init_tex(&game->texs[NORTH]);
	init_tex(&game->texs[SOUTH]);
	init_tex(&game->texs[WEST]);
	init_tex(&game->texs[EAST]);
	init_tex(&game->texs[FLOOR]);
	init_tex(&game->texs[CEILING]);
	init_door_tex(&game->texs[DOOR]);
}

void	init_tex(t_tex *tex)
{
	tex->filled = false;
	tex->path = NULL;
	tex->color = 0x0;
	init_empty_img(&tex->img);
	ft_bzero(&tex->pixels, BLOCK * BLOCK * sizeof(int));
	tex->width = 0;
	tex->height = 0;
}

void	init_door_tex(t_tex *tex)
{
	tex->filled = true;
	tex->path = "./64x/xpm/Glass.xpm";
	tex->color = 0x0;
	init_empty_img(&tex->img);
	ft_bzero(&tex->pixels, BLOCK * BLOCK * sizeof(int));
	tex->width = 0;
	tex->height = 0;
}
