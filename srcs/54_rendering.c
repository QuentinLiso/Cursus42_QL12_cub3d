/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   54_rendering.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:54:11 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 16:02:19 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_minimap(t_game *game)
{
	t_minimap	*mmap;
	int			color;

	mmap = &game->mmap;
	get_minimap_submap(game);
	mmap->pos.y = -1;
	while (++mmap->pos.y < MINIMAP_H)
	{
		mmap->pos.x = -1;
		while (++mmap->pos.x < MINIMAP_W)
		{
			if (mmap->map[mmap->pos.y][mmap->pos.x] == '1')
				color = 0x2C3930;
			else if (mmap->map[mmap->pos.y][mmap->pos.x] == 'D')
				color = 0xA1E3F9;
			else
				color = 0xA27B5C;
			if (mmap->pos.y == MINIMAP_H / 2 && mmap->pos.x == MINIMAP_W / 2)
				color = 0xFB4141;
			draw_minimap_tile(&game->mmap, mmap->pos, &mmap->img, color);
		}
	}
	draw_minimap_orient(mmap);
}

void	get_minimap_submap(t_game *game)
{
	t_vec2Di	map_start;
	t_vec2Di	map_pos;
	t_minimap	*mmap;

	mmap = &game->mmap;
	map_start.x = (int)game->player.pos.x - MINIMAP_W / 2;
	map_start.y = (int)game->player.pos.y - MINIMAP_H / 2;
	mmap->pos.y = -1;
	while (++mmap->pos.y < MINIMAP_H)
	{
		mmap->pos.x = -1;
		while (++mmap->pos.x < MINIMAP_W)
		{
			map_pos.x = map_start.x + mmap->pos.x;
			map_pos.y = map_start.y + mmap->pos.y;
			if (is_in_map(map_pos, game))
				mmap->map[mmap->pos.y][mmap->pos.x]
					= game->map[map_pos.y][map_pos.x];
			else
				mmap->map[mmap->pos.y][mmap->pos.x] = '1';
		}
	}
}

bool	is_in_map(t_vec2Di pos, t_game *game)
{
	int	height;
	int	width;

	height = game->mapdata.height;
	width = game->mapdata.width;
	return (pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height);
}

void	draw_minimap_tile(t_minimap *mmap, t_vec2Di tile, t_img *img, int color)
{
	int			x;
	int			y;
	t_vec2Di	start;
	t_vec2Di	pixel;

	start.x = tile.x * mmap->tile_size;
	start.y = tile.y * mmap->tile_size;
	y = -1;
	while (++y < mmap->tile_size)
	{
		x = -1;
		while (++x < mmap->tile_size)
		{
			pixel.x = start.x + x;
			pixel.y = start.y + y;
			put_pixel_to_img(img, pixel, color);
		}
	}
}

void	draw_minimap_orient(t_minimap *mmap)
{
	t_vec2Di	player_pos;
	t_vec2Di	ray_end;

	player_pos.x = MINIMAP_W / 2 * mmap->tile_size + mmap->tile_size / 2 ;
	player_pos.y = MINIMAP_H / 2 * mmap->tile_size + mmap->tile_size / 2 ;
	ray_end.x = player_pos.x + (int)(mmap->player->dir.x * mmap->player_ray);
	ray_end.y = player_pos.y + (int)(mmap->player->dir.y * mmap->player_ray);
	draw_line(&mmap->img, player_pos, ray_end, 0xFB4141);
}
