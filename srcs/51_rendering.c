/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   51_rendering.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:54:11 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 15:54:14 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// ========================================= RENDERING : RAYCAST

void	raycast_algo(t_game *game)
{
	t_raycast	*ray;
	t_player	*player;
	int			x;

	ray = &game->ray;
	player = &game->player;
	x = -1;
	while (++x < game->win_width)
	{
		init_player_raycast(ray, player, x);
		init_dda_raycast(ray, player);
		launch_dda_raycast(ray, game);
		set_line_height(ray, player, game);
		set_draw_pixels(game, ray, x);
	}
}

void	init_player_raycast(t_raycast *ray, t_player *player, int x)
{
	init_raycast(ray);
	ray->camera_x = 2 * x / (double)WIDTH - 1;
	ray->dir.x = player->dir.x + player->plane.x * ray->camera_x;
	ray->dir.y = player->dir.y + player->plane.y * ray->camera_x;
	ray->map.x = (int)player->pos.x;
	ray->map.y = (int)player->pos.y;
	if (ray->dir.x == 0)
		ray->deltadist.x = INFINITY;
	else
		ray->deltadist.x = (fabs(1 / ray->dir.x));
	if (ray->dir.y == 0)
		ray->deltadist.y = INFINITY;
	else
		ray->deltadist.y = (fabs(1 / ray->dir.y));
}

void	init_dda_raycast(t_raycast *ray, t_player *player)
{
	if (ray->dir.x < 0)
	{
		ray->step.x = -1;
		ray->sidedist.x = (player->pos.x - ray->map.x) * ray->deltadist.x;
	}
	else
	{
		ray->step.x = 1;
		ray->sidedist.x
			= (ray->map.x + 1.0 - player->pos.x) * ray->deltadist.x;
	}
	if (ray->dir.y < 0)
	{
		ray->step.y = -1;
		ray->sidedist.y = (player->pos.y - ray->map.y) * ray->deltadist.y;
	}
	else
	{
		ray->step.y = 1;
		ray->sidedist.y
			= (ray->map.y + 1.0 - player->pos.y) * ray->deltadist.y;
	}
}

void	launch_dda_raycast(t_raycast *ray, t_game *game)
{
	while (1)
	{
		if (ray->sidedist.x < ray->sidedist.y)
		{
			ray->sidedist.x += ray->deltadist.x;
			ray->map.x += ray->step.x;
			ray->side = 0;
		}
		else
		{
			ray->sidedist.y += ray->deltadist.y;
			ray->map.y += ray->step.y;
			ray->side = 1;
		}
		if (ray_collision(ray, game))
			break ;
	}
}

bool	ray_collision(t_raycast *ray, t_game *game)
{
	if (collide_boundaries_i(game, ray->map))
	{
		ray->collide = '1';
		return (true);
	}
	if (collide_walls_i(game, ray->map))
	{
		ray->collide = game->map[ray->map.y][ray->map.x];
		return (true);
	}
	return (false);
}
