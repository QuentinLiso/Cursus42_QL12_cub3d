/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   53_rendering.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:54:11 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 15:59:12 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_draw(t_draw *draw, t_game *game, t_raycast *ray)
{
	draw->orient = get_texture_orientation(ray);
	draw->step = 1.0 * BLOCK / ray->line_height;
	draw->pos = (ray->draw_start - game->win_height / 2
			+ ray->line_height / 2) * draw->step;
	draw->pixel.x = (int)(ray->wall_x * BLOCK);
	if ((ray->side == 0 && ray->dir.x < 0)
		|| (ray->side == 1 && ray->dir.y > 0))
		draw->pixel.x = BLOCK - 1 - draw->pixel.x;
}

t_orient	get_texture_orientation(t_raycast *ray)
{
	if (ray->collide == 'D')
		return (DOOR);
	if (ray->side == 0)
	{
		if (ray->dir.x > 0)
			return (EAST);
		else
			return (WEST);
	}
	else
	{
		if (ray->dir.y > 0)
			return (NORTH);
		else
			return (SOUTH);
	}
}

void	fill_draw_pixels(t_draw *draw, t_game *game, t_raycast *ray, int x)
{
	int		y;
	int		color;
	t_tex	tex;

	y = ray->draw_start - 1;
	tex = game->texs[draw->orient];
	while (++y < ray->draw_end)
	{
		draw->pixel.y = (int)draw->pos & (BLOCK - 1);
		draw->pos += draw->step;
		color = tex.pixels[BLOCK * draw->pixel.y + draw->pixel.x];
		if (color > 0)
			draw->pixels[y][x] = color;
	}
}

// ========================================= RENDERING : CROSSHAIR + MINIMAP

void	draw_crosshair(t_game *game)
{
	t_draw		*draw;
	t_vec2Di	start;
	t_vec2Di	end;

	draw = &game->draw;
	start.x = game->win_width / 2 - CROSSHAIR / 2;
	start.y = game->win_height / 2 - CROSSHAIR / 2;
	end.x = game->win_width / 2 + CROSSHAIR / 2;
	end.y = game->win_height / 2 + CROSSHAIR / 2;
	draw_square(draw, start, end, 0x5CB338);
}

void	draw_square(t_draw *draw, t_vec2Di start, t_vec2Di end, int color)
{
	int	x;
	int	y;

	y = start.y - 1;
	while (++y < end.y)
	{
		x = start.x - 1;
		while (++x < end.x)
			draw->pixels[y][x] = color;
	}
}
