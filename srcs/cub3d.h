/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:29:07 by qliso             #+#    #+#             */
/*   Updated: 2025/03/17 18:50:04 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>


# define WIDTH 1280
# define HEIGHT 720
# define BLOCK 64
# define PI 3.14159265359

typedef struct s_player
{
    float   x;
    float   y;
    int     speed;
    float   angle;
    float   x_angle;
    float   y_angle;
    float   angle_speed;

    float   x_ray;
    float   y_ray;

    bool    key_up;
    bool    key_down;
    bool    key_left;
    bool    key_right;
    bool    left_rotate;
    bool    right_rotate;
}   t_player;

typedef struct s_line
{
    float   dist;
    float   height;
    int     start_y;
    int     end;
}   t_line;


typedef struct s_game
{
    void    *mlx;
    void    *win;
    void    *img;

    char    *data;
    int     bpp;
    int     size_line;
    int     endian;
    t_player    player;

    char    **map;
    bool    debug;
    bool    update;
}   t_game;

char    **get_map(void);
int closing_window();

void    init_player(t_player *player);
int key_press(int keycode, t_game *game);
int key_release(int keycode, t_game *game);
void    move_player(t_player *player);

#endif