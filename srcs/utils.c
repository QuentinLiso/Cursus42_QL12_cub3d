/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 15:54:40 by qliso             #+#    #+#             */
/*   Updated: 2025/03/15 16:44:46 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int    init_scene(t_scene *scene)
{
    scene->win_w = WINDOW_WIDTH;
    scene->win_h = WINDOW_HEIGHT;
    scene->color = WHITE;
    scene->to_update = true;
    scene->img = ft_calloc(1, sizeof(t_img));
    if (!scene->img)
        return (ENOMEM);
    return (0);
}

int set_display_elems(t_scene *scene)
{
    scene->mlx_disp = mlx_init();
    
    if (!scene->mlx_disp)
        return (1);
    scene->mlx_win = mlx_new_window(scene->mlx_disp,
            WINDOW_WIDTH, WINDOW_HEIGHT, "CUB3D");
    if (!scene->mlx_win)
        return (2);
    scene->img->mlx_img = mlx_new_image(scene->mlx_disp,
            WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!scene->img->mlx_img)
        return (3);
    scene->img->addr = mlx_get_data_addr(scene->img->mlx_img,
            &(scene->img->bpp), &(scene->img->size_line), &(scene->img->endian));
    return (0);
}

int set_loops(t_scene *scene)
{
    mlx_loop_hook(scene->mlx_disp, &update_render, scene);
    mlx_hook(scene->mlx_win, KeyPress, KeyPressMask,
		&handle_keypress_input, scene);
    mlx_hook(scene->mlx_win, DestroyNotify, Button1Mask,
            &closing_window, scene);
    mlx_loop(scene->mlx_disp);
    return (0);
}

int update_render(t_scene *scene)
{
    if (!scene->to_update)
        return (0);
    if (!scene->mlx_win)
        return (1);
    draw_square(scene->img, 100, 100, scene->color);
    mlx_put_image_to_window(scene->mlx_disp, scene->mlx_win,
            scene->img->mlx_img, 0, 0);
    scene->to_update = false;
    return (0);
}

void    draw_square(t_img *img, int x, int y, int color)
{
    int x_iter;
    int y_iter;

    x_iter = 0;
    while (x_iter < 100)
    {
        y_iter = 0;
        while (y_iter < 100)
        {
            put_pix_to_img(img, x + x_iter, y + y_iter, color);
            y_iter++;
        }
        x_iter++;
    }
}

void	put_pix_to_img(t_img *img, int x, int y, int color)
{
	char	*pixel;
	int		i;

	if (x < 0 || y < 0 || x > img->width || y > img->height)
		return ;
	i = img->bpp - 8;
	pixel = img->addr + (y * img->size_line) + (x * (img->bpp / 8));
	while (i >= 0)
	{
		if (img->endian != 0)
			*pixel++ = (color >> i) & 0xFF;
		else
			*pixel++ = (color >> (img->bpp - 8 - i) & 0xFF);
		i -= 8;
	}
}

int	handle_keypress_input(int keysym, t_scene *scene)
{
    scene->to_update = true;
	if (keysym == XK_Escape)
		closing_window(scene);
    else if (keysym == XK_Left)
		scene->color = BLUE;
	else if (keysym == XK_Right)
        scene->color = GREEN;
	else if (keysym == XK_Up)
        scene->color = RED;
	else if (keysym == XK_Down)
        scene->color = WHITE;
    return (1);
}

int     closing_window(t_scene *scene)
{
    scene_cleanup(scene);
    exit (0);
}

void    safe_free(void **ptr)
{
    if (!*ptr)
        return ;
    free(*ptr);
    *ptr = NULL;
}

void    scene_cleanup(t_scene *scene)
{
    if (scene->mlx_disp && scene->img->mlx_img)
        mlx_destroy_image(scene->mlx_disp, scene->img->mlx_img);
    if (scene->mlx_disp && scene->mlx_win)
        mlx_destroy_window(scene->mlx_disp, scene->mlx_win);
    if (scene->mlx_disp)
        mlx_destroy_display(scene->mlx_disp);
    free(scene->mlx_disp);
    free(scene->img);
}


