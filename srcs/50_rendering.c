/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   50_rendering.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:54:11 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 15:50:52 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	update_render(t_game *game)
{
	game->player.has_moved += set_player_movement(game);
	game->player.has_moved += set_player_rotation(game);
	if (!game->player.has_moved && !game->player.mouse_rotate)
		return (0);
	update_render_frame(game);
	game->player.mouse_rotate = 0;
	return (0);
}

void	update_render_frame(t_game *game)
{
	reset_draw_pixels(game);
	init_raycast(&game->ray);
	raycast_algo(game);
	draw_crosshair(game);
	draw_minimap(game);
	put_frame_to_screen(game);
}

// ========================================= RENDERING : INIT

void	reset_draw_pixels(t_game *game)
{
	int	x;
	int	y;

	y = -1;
	while (++y < game->win_height)
	{
		x = -1;
		while (++x < game->win_width)
		{
			game->draw.pixels[y][x] = 0x0;
		}
	}
}

void	init_raycast(t_raycast *ray)
{
	ray->camera_x = 0.0;
	ray->dir = (t_vec2D){0.0, 0.0};
	ray->map = (t_vec2Di){0, 0};
	ray->step = (t_vec2Di){0, 0};
	ray->sidedist = (t_vec2D){0.0, 0.0};
	ray->deltadist = (t_vec2D){0.0, 0.0};
	ray->wall_dist = 0.0;
	ray->wall_x = 0.0;
	ray->side = 0;
	ray->line_height = 0;
	ray->draw_start = 0;
	ray->draw_end = 0;
	ray->collide = '\0';
}
