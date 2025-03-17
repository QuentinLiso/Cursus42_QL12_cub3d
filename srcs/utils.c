/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:29:25 by qliso             #+#    #+#             */
/*   Updated: 2025/03/17 19:15:21 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void    init_player(t_player *player)
{
    player->x = WIDTH / 2;
    player->y = HEIGHT / 2;
    player->speed = 3;
    player->angle = PI;
    player->angle_speed = 0.03;
    
    player->key_up = false;
    player->key_down = false;
    player->key_left = false;
    player->key_right = false;
    player->left_rotate = false;
    player->right_rotate = false;
}

int key_press(int keycode, t_game *game)
{
    t_player    *player;

    player = &game->player;
    if (keycode == XK_z)
        player->key_up = true;
    if (keycode == XK_s)
        player->key_down = true;
    if (keycode == XK_q)
        player->key_left = true;
    if (keycode == XK_d)
        player->key_right = true;
    if (keycode == XK_Left)
        player->left_rotate = true;
    if (keycode == XK_Right)
        player->right_rotate = true;
    if (keycode == XK_p)
        game->debug = !game->debug;
    if (keycode == XK_Escape)
        closing_window();
    return (0);
}

int key_release(int keycode, t_game *game)
{
    t_player    *player;

    player = &game->player;
    if (keycode == XK_z)
        player->key_up = false;
    if (keycode == XK_s)
        player->key_down = false;
    if (keycode == XK_q)
        player->key_left = false;
    if (keycode == XK_d)
        player->key_right = false;
    if (keycode == XK_Left)
        player->left_rotate = false;
    if (keycode == XK_Right)
        player->right_rotate = false;
    return (0);
}


void    rotate_player(t_player *player)
{
    player->x_angle = cos(player->angle);
    player->y_angle = sin(player->angle);
    if (player->left_rotate)
        player->angle -= player->angle_speed;
    if (player->right_rotate)
        player->angle += player->angle_speed;
    if (player->angle > 2 * PI)
        player->angle = 0;
    if (player->angle < 0)
        player->angle = 2 * PI;
}

void    translate_player(t_player *player)
{
    if (player->key_up)
    {
        player->x += player->x_angle * player->speed;
        player->y += player->y_angle * player->speed;
    }
    if (player->key_down)
    {
        player->x -= player->x_angle * player->speed;
        player->y -= player->y_angle * player->speed;
    }
    if (player->key_left)
    {
        player->x += player->y_angle * player->speed;
        player->y -= player->x_angle * player->speed;
    }
    if (player->key_right)
    {
        player->x -= player->y_angle * player->speed;
        player->y += player->x_angle * player->speed;
    }
}

void    move_player(t_player *player)
{
    rotate_player(player);
    translate_player(player);
}