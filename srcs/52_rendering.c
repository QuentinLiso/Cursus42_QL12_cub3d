/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   52_rendering.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:54:11 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 15:55:34 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool	collide_boundaries_i(t_game *game, t_vec2Di pos)
{
	return (pos.x < 1 || pos.x >= game->mapdata.width
		|| pos.y < 1 || pos.y >= game->mapdata.height);
}

bool	collide_walls_i(t_game *game, t_vec2Di pos)
{
	return (game->map[pos.y][pos.x] != '0');
}

void	set_line_height(t_raycast *ray, t_player *player, t_game *game)
{
	if (ray->side == 0)
	{
		ray->wall_dist = (ray->sidedist.x - ray->deltadist.x);
		ray->wall_x = player->pos.y + ray->wall_dist * ray->dir.y;
	}
	else
	{
		ray->wall_dist = (ray->sidedist.y - ray->deltadist.y);
		ray->wall_x = player->pos.x + ray->wall_dist * ray->dir.x;
	}
	ray->wall_x -= floor(ray->wall_x);
	ray->line_height = (int)(game->win_height / ray->wall_dist);
	ray->draw_start = (game->win_height / 2) - (ray->line_height / 2);
	ray->draw_end = (game->win_height / 2) + (ray->line_height / 2);
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	if (ray->draw_end >= game->win_height)
		ray->draw_end = game->win_height - 1;
}

void	set_draw_pixels(t_game *game, t_raycast *ray, int x)
{
	t_draw	*draw;

	draw = &game->draw;
	init_draw(draw, game, ray);
	fill_draw_pixels(draw, game, ray, x);
}
