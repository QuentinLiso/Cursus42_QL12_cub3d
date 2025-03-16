/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 15:54:40 by qliso             #+#    #+#             */
/*   Updated: 2025/03/16 22:28:06 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int    init_scene(t_scene *scene, int *map)
{
    scene->win_w = WINDOW_WIDTH;
    scene->win_h = WINDOW_HEIGHT;
    scene->color = WHITE;
    scene->to_update = true;
    scene->p_x = 200.0f;
    scene->p_y = 200.0f;
    scene->p_size = 10;
    scene->p_a = 0.0f;
    scene->p_dx = cos(scene->p_a) * 5.0;
    scene->p_dy = sin(scene->p_a) * 5.0;
    scene->map_x = 8;
    scene->map_y = 8;
    scene->map_s = 64;
    scene->map = map;
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
    draw_elements(scene);
    mlx_put_image_to_window(scene->mlx_disp, scene->mlx_win,
            scene->img->mlx_img, 0, 0);
    scene->to_update = false;
    return (0);
}


// =======================================================


void    draw_elements(t_scene *scene)
{
    printf("============================\n");
    draw_background(scene->img);
    draw_map(scene);
    draw_raycast(scene);
    draw_player(scene);
}

void    draw_background(t_img *img)
{
    draw_rect(img, (t_rect){0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, GRAY);
}

void    draw_map(t_scene *scene)
{
    int x;
    int y;
    int x0;
    int y0;

    x = -1;
    while (++x < scene->map_x)
    {
        y = -1;
        while (++y < scene->map_y)
        {
            x0 = x * scene->map_s;
            y0 = y * scene->map_s;
            if (scene->map[y * scene->map_x + x] == 1)
                draw_rect(scene->img,
                    (t_rect){x0 + 1, y0 + 1, scene->map_s - 1, scene->map_s - 1},
                    WHITE);
            else
                draw_rect(scene->img,
                    (t_rect){x0 + 1, y0 + 1, scene->map_s - 1, scene->map_s - 1},
                    BLACK);
        }
    }
}

void    draw_player(t_scene *scene)
{
    t_vector2   player;
    t_vector2   direction;

    player.x = scene->p_x;
    player.y = scene->p_y;
    direction.x = player.x + scene->p_dx * 5;
    direction.y = player.y +  scene->p_dy * 5;
    draw_line(scene->img, player, direction, RED);
    draw_rect(scene->img,
            (t_rect){scene->p_x - scene->p_size / 2, 
                scene->p_y - scene->p_size / 2, scene->p_size, scene->p_size},
            YELLOW);
}


void    draw_raycast(t_scene *scene)
{
    t_raycast   ray;
    
    ray.r = 0;
    set_ray_fov_and_angle(scene, &ray);
    ray.h_dist = 1000000;
    ray.v_dist = 1000000;
    while (ray.r++ < ray.fov)
    {
        printf("******** Ray n° %d : ***********\n", ray.r);
        set_ray_horizontal_start_orientation(scene, &ray);
        set_ray_horizontal_collision(scene, &ray);
        set_ray_vertical_start_orientation(scene, &ray);
        set_ray_vertical_collision(scene, &ray);
        if (ray.h_dist <  ray.v_dist)
            ray.ray_vect = (t_vector2){ray.hx, ray.hy};
        if (ray.v_dist <  ray.h_dist)
            ray.ray_vect = (t_vector2){ray.vx, ray.vy};
        draw_line(scene->img, (t_vector2){scene->p_x, scene->p_y},
                ray.ray_vect, GREEN);
        increment_ray_angle(&ray);
        // printf("Horizontal : (%f, %f) ; Vertical : (%f, %f)\n", ray.hx, ray.hy, ray.vx, ray.vy);
    }
    
}


void    set_ray_fov_and_angle(t_scene *scene, t_raycast *ray)
{
    ray->fov = 60;
    ray->ra = scene->p_a - DR * (ray->fov / 2);
    if (ray->ra < 0)
        ray->ra += 2 * PI;
    if (ray->ra > 2 * PI)
        ray->ra -= 2 * PI;
}

void    increment_ray_angle(t_raycast *ray)
{
    ray->ra += DR;
    if (ray->ra < 0)
        ray->ra += 2 * PI;
    if (ray->ra > 2 * PI)
        ray->ra -= 2 * PI;
}

void    set_ray_horizontal_start_orientation(t_scene *scene, t_raycast *ray)
{
    ray->dof = 0;
    ray->aTan = -1/tan(ray->ra);
    if (ray->ra > PI)
    {
        ray->hy = (((int)scene->p_y >> 6) << 6) - 0.001;
        ray->hx = (scene->p_y - ray->hy) * ray->aTan + scene->p_x;
        ray->yo = -64;
        ray->xo = -ray->yo * ray->aTan;
    }
    if (ray->ra < PI && ray->ra > 0)
    {
        ray->hy = (((int)scene->p_y >> 6) << 6) + 64;
        ray->hx = (scene->p_y - ray->hy) * ray->aTan + scene->p_x;
        ray->yo = 64;
        ray->xo = -ray->yo * ray->aTan;
    }
    if (ray->ra == 0 || ray->ra == PI)
    {
        ray->hx = scene->p_x;
        ray->hy = scene->p_y;
        ray->dof = 8;
    }
}

void    set_ray_horizontal_collision(t_scene *scene, t_raycast *ray)
{
    while (ray->dof < 8)
    {
        ray->mx = (int)(ray->hx) >> 6;
        ray->my = (int)(ray->hy) >> 6;
        ray->mp = ray->my * scene->map_x + ray->mx;
        if ((ray->mp >= 0) && (ray->mp < scene->map_x * scene->map_y) && (scene->map[ray->mp] == 1))
        {
            ray->dof = 8;
            ray->h_dist = distance((t_vector2){scene->p_x, scene->p_y},
                            (t_vector2){ray->hx, ray->hy});
        }
        else
        {
            ray->hx = clamp(ray->hx + ray->xo, scene->map_s, scene->map_s * (scene->map_x - 1));
            ray->hy += ray->yo;
            // ray->hy = clamp(ray->hy + ray->yo, scene->map_s, scene->map_s * (scene->map_y - 1));
            ray->dof++;
        }
    }
}

void    set_ray_vertical_start_orientation(t_scene *scene, t_raycast *ray)
{
    ray->dof = 0;
    ray->nTan = -tan(ray->ra);
    if (ray->ra > PI / 2 && ray->ra < 3 * PI / 2)   // Looking left
    {
        ray->vx = (((int)scene->p_x >> 6) << 6) - 0.0001;
        ray->vy = (scene->p_x - ray->vx) * ray->nTan + scene->p_y;
        ray->xo = -64;
        ray->yo = -ray->xo * ray->nTan;
    }
    if (ray->ra < PI / 2 || ray->ra > 3 * PI / 2)   // Looking right
    {
        ray->vx = (((int)scene->p_x >> 6) << 6) + 64;
        ray->vy = (scene->p_x - ray->vx) * ray->nTan + scene->p_y;
        ray->xo = 64;
        ray->yo = -ray->xo * ray->nTan;
    }
    if (ray->ra == PI / 2 || ray->ra == 3 * PI / 2)
    {
        ray->vx = scene->p_x;
        ray->vy = scene->p_y;
        ray->dof = 8;
    }
}

void    set_ray_vertical_collision(t_scene *scene, t_raycast *ray)
{
    while (ray->dof < 8)
    {
        printf("x : %f, y : %f\n", ray->vx, ray->vy);
        ray->mx = (int)(ray->vx) >> 6;
        ray->my = (int)(ray->vy) >> 6;
        ray->mp = ray->my * scene->map_x + ray->mx;
        if (
            (ray->mp >= 0) && (ray->mp < scene->map_x * scene->map_y) && (scene->map[ray->mp] == 1))
        {
            ray->dof = 8;
            ray->v_dist = distance((t_vector2){scene->p_x, scene->p_y},
                            (t_vector2){ray->vx, ray->vy});
        }
        else
        {
            // ray->vx = clamp(ray->vx + ray->xo, scene->map_s, scene->map_s * (scene->map_x - 1));
            ray->vx += ray->xo;
            ray->vy = clamp(ray->vy + ray->yo, scene->map_s, scene->map_s * (scene->map_y - 1));
            ray->dof++;
        }
    }
}

// =======================================================

void    draw_rect(t_img *img, t_rect rect, int color)
{
    int x_iter;
    int y_iter;
    
    x_iter = 0;
    while (x_iter < rect.width)
    {
        y_iter = 0;
        while (y_iter < rect.height)
        {
            put_pix_to_img(img, rect.x + x_iter, rect.y + y_iter, color);
            y_iter++;
        }
        x_iter++;
    }
}

void    draw_line(t_img *img, t_vector2 u, t_vector2 v, int color)
{
    t_vector2   d;
    int         pixels;

    d.x = v.x - u.x;
    d.y = v.y - u.y;
    pixels = sqrt(d.x * d.x + d.y * d.y);
    d.x /= pixels;
    d.y /= pixels;
    while (pixels--)
    {
        put_pix_to_img(img, u.x, u.y, color);
        u.x += d.x;
        u.y += d.y;
    }
}

void	put_pix_to_img(t_img *img, int x, int y, int color)
{
	char	*pixel;
	int		i;

	if (x < 0 || y < 0 || x > WINDOW_WIDTH || y > WINDOW_HEIGHT)
		return ;
	i = img->bpp - 8;
	pixel = img->addr + (y * img->size_line + x * (img->bpp / 8));
	while (i >= 0)
	{
		if (img->endian != 0)
			*pixel++ = (color >> i) & 0xFF;
		else
			*pixel++ = (color >> (img->bpp - 8 - i) & 0xFF);
		i -= 8;
	}
}

// ======================================================

float   distance(t_vector2 u, t_vector2 v)
{
    return (sqrt((v.x - u.x) * (v.x - u.x) + (v.y - u.y) * (v.y - u.y)));
}

float   clamp(float value, float min, float max)
{
    if (value < min)
        return (min);
    else if (value > max)
        return (max);
    else
        return (value);
}

// =======================================================

int	handle_keypress_input(int keysym, t_scene *scene)
{
    scene->to_update = true;
	if (keysym == XK_Escape)
		closing_window(scene);
    handle_keypress_wasd(keysym, scene);
    handle_keypress_arrow(keysym, scene);
    return (1);
}

void handle_keypress_wasd(int keysym, t_scene *scene)
{
    if (keysym == XK_z)
    {
        scene->p_x += scene->p_dx;
        scene->p_y += scene->p_dy;
    }
    else if (keysym == XK_q)
    {
        scene->p_x += scene->p_dy;
        scene->p_y += -scene->p_dx;
    }
    else if (keysym == XK_s)
    {
        scene->p_x -= scene->p_dx;
        scene->p_y -= scene->p_dy;
    }
    else if (keysym == XK_d)
    {
        scene->p_x += -scene->p_dy;
        scene->p_y += scene->p_dx;
    }
}

void handle_keypress_arrow(int keysym, t_scene *scene)
{
    if (keysym == XK_Left)
    {
        scene->p_a -= 0.1f;
        if (scene->p_a < 0)
            scene->p_a += 2 * PI;
        scene->p_dx = cos(scene->p_a) * 5.0;
        scene->p_dy = sin(scene->p_a) * 5.0;
    }
	else if (keysym == XK_Right)
    {
        scene->p_a += 0.1f;
        if (scene->p_a > 2 * PI)
            scene->p_a -= 2 * PI;
        scene->p_dx = cos(scene->p_a) * 5.0;
        scene->p_dy = sin(scene->p_a) * 5.0;
    }
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


