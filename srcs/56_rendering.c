/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   56_rendering.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:54:11 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 16:07:37 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// ========================================= RENDERING : UPDATE FRAME

void	put_frame_to_screen(t_game *game)
{
	t_vec2Di	pixel;

	pixel.y = -1;
	while (++pixel.y < game->win_height)
	{
		pixel.x = -1;
		while (++pixel.x < game->win_width)
			update_frame_pixel(game, &game->frame, pixel);
	}
	put_minimap_to_frame(&game->frame, &game->mmap);
	mlx_put_image_to_window(game->mlx, game->win, game->frame.img, 0, 0);
}

void	init_img(t_game *game, t_img *img, int width, int height)
{
	init_empty_img(img);
	img->img = mlx_new_image(game->mlx, width, height);
	if (!img->img)
		clean_c3d_exit(game, perror_c3d(EMLXIMG));
	img->addr = (int *)mlx_get_data_addr(img->img, &img->bpp,
			&img->size_line, &img->endian);
	img->width = width;
	img->height = height;
}

void	update_frame_pixel(t_game *game, t_img *img, t_vec2Di pixel)
{
	t_draw	*draw;

	draw = &game->draw;
	if (draw->pixels[pixel.y][pixel.x] > 0)
		put_pixel_to_img(img, pixel, draw->pixels[pixel.y][pixel.x]);
	else if (pixel.y < game->win_height / 2)
		put_pixel_to_img(img, pixel, game->texs[CEILING].color);
	else
		put_pixel_to_img(img, pixel, game->texs[FLOOR].color);
}

void	put_pixel_to_img(t_img *img, t_vec2Di coord, int color)
{
	int	pixel;
	int	pixels_per_row;

	pixels_per_row = img->size_line / sizeof(int);
	pixel = coord.y * pixels_per_row + coord.x;
	img->addr[pixel] = color;
}

int	get_pixel_from_img(t_img *img, t_vec2Di coord)
{
	int	pixel;
	int	pixels_per_row;

	pixels_per_row = img->size_line / sizeof(int);
	pixel = coord.y * pixels_per_row + coord.x;
	return (((int *)img->addr)[pixel]);
}
