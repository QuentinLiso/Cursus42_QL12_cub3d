/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   32_player.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:51:54 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 15:49:00 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	set_player_rotation(t_game *game)
{
	t_player	*player;
	double		angle;

	player = &game->player;
	if (player->rotate != -1 && player->rotate != 1)
		return (0);
	angle = player->rotate * ROTSPEED;
	rotate_vec2d(&player->dir, angle);
	rotate_vec2d(&player->plane, angle);
	return (1);
}

void	rotate_vec2d(t_vec2D *v, double angle)
{
	double	cos_a;
	double	sin_a;
	double	cur_x;

	cos_a = cos(angle);
	sin_a = sin(angle);
	cur_x = v->x;
	v->x = v->x * cos_a - v->y * sin_a;
	v->y = cur_x * sin_a + v->y * cos_a;
}

void	door_raycast(t_game *game)
{
	t_raycast	ray;
	t_player	*player;

	player = &game->player;
	init_door_raycast(&ray, player);
	init_dda_raycast(&ray, player);
	if (launch_door_raycast(&ray, game, 2))
		game->map[ray.map.y][ray.map.x] = '0';
}

void	init_door_raycast(t_raycast *ray, t_player *player)
{
	init_raycast(ray);
	ray->dir.x = player->dir.x;
	ray->dir.y = player->dir.y;
	ray->map.x = (int)player->pos.x;
	ray->map.y = (int)player->pos.y;
	ray->deltadist.x = (fabs(1 / ray->dir.x));
	ray->deltadist.y = (fabs(1 / ray->dir.y));
}

bool	launch_door_raycast(t_raycast *ray, t_game *game, int max_dist)
{
	int	steps;

	steps = -1;
	while (++steps < max_dist)
	{
		if (ray->sidedist.x < ray->sidedist.y)
		{
			ray->sidedist.x += ray->deltadist.x;
			ray->map.x += ray->step.x;
		}
		else
		{
			ray->sidedist.y += ray->deltadist.y;
			ray->map.y += ray->step.y;
		}
		if (game->map[ray->map.y][ray->map.x] == 'D')
			return (true);
		if (game->map[ray->map.y][ray->map.x] == '1')
			break ;
	}
	return (false);
}
