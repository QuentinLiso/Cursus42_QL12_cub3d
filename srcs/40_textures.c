/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   40_textures.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:53:33 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 17:02:04 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_mlx(t_game *game)
{
	t_minimap	*mmap;
	int			y;

	mmap = &game->mmap;
	game->mlx = mlx_init();
	if (!game->mlx)
		clean_c3d_exit(game, perror_c3d(EMLXINIT));
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3d");
	if (!game->win)
		clean_c3d_exit(game, perror_c3d(EMLXWIN));
	init_img(game, &game->frame, game->win_width, game->win_height);
	init_img(game, &mmap->img,
		MINIMAP_W * mmap->tile_size, MINIMAP_H * mmap->tile_size);
	game->draw.pixels = ft_calloc(game->win_height, sizeof(int *));
	if (!game->draw.pixels)
		clean_c3d_exit(game, perror_c3d(EMALLOC));
	y = -1;
	while (++y < game->win_height)
	{
		game->draw.pixels[y] = ft_calloc(game->win_width, sizeof(int));
		if (!game->draw.pixels[y])
			clean_c3d_exit(game, EMALLOC);
	}
}

void	init_texs(t_game *game)
{
	load_tex(game, NORTH);
	load_tex(game, SOUTH);
	load_tex(game, WEST);
	load_tex(game, EAST);
	if (check_valid_file(game->texs[DOOR].path, ".xpm"))
		load_tex_solid(game, DOOR, 0x945034);
	else
		load_tex(game, DOOR);
}

void	load_tex(t_game *game, t_orient orient)
{
	t_tex	*tex;
	t_img	*img;

	tex = &game->texs[orient];
	img = &tex->img;
	init_empty_img(img);
	img->img = mlx_xpm_file_to_image(game->mlx, tex->path,
			&tex->width, &tex->height);
	if (!img->img)
		clean_c3d_exit(game, perror_c3d(ELOADTEXTURE));
	img->addr = (int *)mlx_get_data_addr(img->img, &img->bpp,
			&img->size_line, &img->endian);
	if (!img->addr)
		clean_c3d_exit(game, perror_c3d(EGETIMGADDR));
	fill_tex_pixels(img, tex);
	mlx_destroy_image(game->mlx, img->img);
}

void	fill_tex_pixels(t_img *img, t_tex *tex)
{
	t_vec2Di	img_pixel;
	t_vec2Di	tex_pixel;
	t_vec2Di	scale;

	scale.x = (tex->width << 16) / BLOCK;
	scale.y = (tex->height << 16) / BLOCK;
	tex_pixel.y = -1;
	while (++tex_pixel.y < BLOCK)
	{
		tex_pixel.x = -1;
		while (++tex_pixel.x < BLOCK)
		{
			img_pixel.x = (tex_pixel.x * scale.x) >> 16;
			img_pixel.y = (tex_pixel.y * scale.y) >> 16;
			tex->pixels[tex_pixel.y * BLOCK + tex_pixel.x]
				= img->addr[img_pixel.y * tex->width + img_pixel.x];
		}
	}
}

void	load_tex_solid(t_game *game, t_orient orient, int color)
{
	t_tex		*tex;
	t_vec2Di	tex_pixel;

	tex = &game->texs[orient];
	tex_pixel.x = -1;
	while (++tex_pixel.x < BLOCK)
		tex->pixels[tex_pixel.x] = 0x441752;
	tex_pixel.y = 0;
	while (++tex_pixel.y < BLOCK - 1)
	{
		tex->pixels[tex_pixel.y * BLOCK] = 0x441752;
		tex_pixel.x = 0;
		while (++tex_pixel.x < BLOCK - 1)
			tex->pixels[tex_pixel.y * BLOCK + tex_pixel.x] = color;
		tex->pixels[tex_pixel.y * BLOCK + tex_pixel.x] = 0x441752;
	}
	tex_pixel.x = -1;
	while (++tex_pixel.x < BLOCK)
		tex->pixels[tex_pixel.y * BLOCK + tex_pixel.x] = 0x441752;
}
