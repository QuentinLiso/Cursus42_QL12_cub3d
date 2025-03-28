/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   11_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:50:23 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 15:04:20 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_mapdata(t_mapdata *mapdata)
{
	mapdata->fd = 0;
	mapdata->line_count = 0;
	mapdata->filepath = NULL;
	mapdata->filecontent = NULL;
	mapdata->height = 0;
	mapdata->width = 0;
}

void	init_minimap(t_game *game, t_minimap *mmap)
{
	mmap->tile_size = 8;
	mmap->player = &game->player;
	mmap->player_ray = 15;
}

void	init_empty_img(t_img *img)
{
	img->img = NULL;
	img->addr = NULL;
	img->bpp = 0;
	img->size_line = 0;
	img->endian = 0;
	img->width = 0;
	img->height = 0;
}
