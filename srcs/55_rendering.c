/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   55_rendering.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:54:11 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 16:05:48 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_line(t_img *img, t_vec2Di start, t_vec2Di end, int color)
{
	t_line	line;

	init_line(&line, start, end, 2);
	while (start.x != end.x || start.y != end.y)
	{
		put_pixel_to_img(img, start, color);
		draw_thickness(img, &line, start, color);
		bresenham_next_pixel(&line, &start);
	}
}

void	init_line(t_line *line, t_vec2Di start, t_vec2Di end, int thickness)
{
	float	magnitude;

	line->delta.x = abs(end.x - start.x);
	line->delta.y = abs(end.y - start.y);
	if (start.x < end.x)
		line->step.x = 1;
	else
		line->step.x = -1;
	if (start.y < end.y)
		line->step.y = 1;
	else
		line->step.y = -1;
	line->error = line->delta.x - line->delta.y;
	if (thickness > 0)
		line->thickness = thickness;
	else
		line->thickness = 1;
	magnitude = sqrt(line->delta.x * line->delta.x
			+ line->delta.y * line->delta.y);
	line->perp_thick.x = (int)round(-line->delta.y / magnitude);
	line->perp_thick.y = (int)round(line->delta.x / magnitude);
}

void	draw_thickness(t_img *img, t_line *line, t_vec2Di start, int color)
{
	int			thickness_loop;
	int			i;
	t_vec2Di	pixel;

	thickness_loop = line->thickness / 2 + 1;
	i = 0;
	while (++i < thickness_loop)
	{
		pixel.x = start.x + line->perp_thick.x * i;
		pixel.y = start.y + line->perp_thick.y * i;
		if (pixel.x < img->width && pixel.y < img->height)
			put_pixel_to_img(img, pixel, color);
		pixel.x = start.x - line->perp_thick.x * i;
		pixel.y = start.y - line->perp_thick.y * i;
		if (pixel.x >= 0 && pixel.y >= 0)
			put_pixel_to_img(img, pixel, color);
	}
}

void	bresenham_next_pixel(t_line *line, t_vec2Di *start)
{
	line->error_2 = line->error * 2;
	if (line->error_2 > -line->delta.y)
	{
		line->error -= line->delta.y;
		start->x += line->step.x;
	}
	if (line->error_2 < line->delta.x)
	{
		line->error += line->delta.x;
		start->y += line->step.y;
	}
}

void	put_minimap_to_frame(t_img *img, t_minimap *mmap)
{
	int	x;
	int	y;
	int	color;

	y = -1;
	while (++y < MINIMAP_H * 8)
	{
		x = -1;
		while (++x < MINIMAP_W * 8)
		{
			color = get_pixel_from_img(&mmap->img, (t_vec2Di){x, y});
			put_pixel_to_img(img, (t_vec2Di){x, y}, color);
		}
	}
}
