/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:17:38 by qliso             #+#    #+#             */
/*   Updated: 2025/03/16 22:13:48 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include "mlx.h"

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <X11/X.h>
# include <X11/keysym.h>
# include <math.h>
# include <errno.h>
# include <stdbool.h>

# define WINDOW_WIDTH 1024
# define WINDOW_HEIGHT 512
# define PI 3.14159265359
# define P2 1.57079632679
# define P3 4.71238898038
# define DR 0.0174533

# define BLACK 0x000000
# define DARK 0x0F0F0F
# define GRAY 0x4C585B
# define RED 0x992E24
# define BLUE 0x4287f5
# define GREEN 0x1F6650
# define YELLOW 0xFFE26F
# define PINK 0xFDADC7
# define WHITE 0xFBFAFC

typedef struct  s_vector2
{
    float   x;
    float   y;
}   t_vector2;

typedef struct  s_rect
{
    int x;
    int y;
    int width;
    int height;
}   t_rect;

typedef struct s_raycast
{
    int r;      // num of rays to cast
    int mx;     
    int my;
    int mp;     // to get the index in the map to check if it's 1 or 0
    int dof;    // depth of field
    int fov;

    t_vector2   ray_vect;
    float   ra;     // ray angle
    float   aTan;   // arc tan of ray angle
    float   nTan;
    float   xo;
    float   yo;
    float   hx;
    float   hy;
    float   h_dist;
    float   vx;
    float   vy;
    float   v_dist;
}   t_raycast;




typedef struct s_img
{
    void    *mlx_img;
    char    *addr;
    int     bpp;
    int     size_line;
    int     endian;
    int     width;
    int     height;
    char    *img_file;
}   t_img;


typedef struct s_scene
{
    void    *mlx_disp;
    void    *mlx_win;
    t_img   *img;
    bool    to_update;

    float   p_x;
    float   p_y;
    int     p_size;
    float   p_dx;
    float   p_dy;
    float   p_a;

    int     map_x;
    int     map_y;
    int     map_s;
    int     *map;
    
    unsigned int    win_w;
    unsigned int    win_h;
    int     color;
}   t_scene;

int     init_scene(t_scene *scene, int *map);
int     set_display_elems(t_scene *scene);
int     set_loops(t_scene *scene);
int     update_render(t_scene *scene);


void    draw_elements(t_scene *scene);
void    draw_background(t_img *img);
void    draw_map(t_scene *scene);
void    draw_player(t_scene *scene);
void    draw_raycast(t_scene *scene);
void    set_ray_fov_and_angle(t_scene *scene, t_raycast *ray);
void    increment_ray_angle(t_raycast *ray);
void    draw_raycast_check_horizontal(t_scene *scene);
void    set_ray_horizontal_start_orientation(t_scene *scene, t_raycast *ray);
void    set_ray_horizontal_collision(t_scene *scene, t_raycast *ray);
void    set_ray_vertical_start_orientation(t_scene *scene, t_raycast *ray);
void    set_ray_vertical_collision(t_scene *scene, t_raycast *ray);


void    draw_rect(t_img *img, t_rect rect, int color);
void    draw_line(t_img *img, t_vector2 u, t_vector2 v, int color);
void	put_pix_to_img(t_img *img, int x, int y, int color);

float   distance(t_vector2 u, t_vector2 v);
float   clamp(float value, float min, float max);

int	    handle_keypress_input(int keysym, t_scene *scene);
void    handle_keypress_wasd(int keysym, t_scene *scene);
void    handle_keypress_arrow(int keysym, t_scene *scene);
int     closing_window(t_scene *scene);
void    safe_free(void **ptr);
void    scene_cleanup(t_scene *scene);


#endif