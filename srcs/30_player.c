/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:51:54 by qliso             #+#    #+#             */
/*   Updated: 2025/03/26 14:52:27 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


void    set_player_start_pos(t_game *game)
{
    game->player.pos.x = (double)game->player.spawn.x + 0.5;
    game->player.pos.y = (double)game->player.spawn.y + 0.5;
    if (game->player.orientation == 'N')
        init_player_north(&game->player);
    else if (game->player.orientation == 'S')
        init_player_south(&game->player);
    else if (game->player.orientation == 'W')
        init_player_west(&game->player);
    else if (game->player.orientation == 'E')
        init_player_east(&game->player);
}

void    init_player_north(t_player *player)
{
    player->dir.x = 0.0;
    player->dir.y = -1.0;
    player->plane.x = -player->dir.y * FOV;
    player->plane.y = player->dir.x * FOV;
}

void    init_player_south(t_player *player)
{
    player->dir.x = 0.0;
    player->dir.y = 1.0;
    player->plane.x = -player->dir.y * FOV;
    player->plane.y = player->dir.x * FOV;
}

void    init_player_west(t_player *player)
{
    player->dir.x = -1.0;
    player->dir.y = 0.0;
    player->plane.x = -player->dir.y * FOV;
    player->plane.y = player->dir.x * FOV;
}

void    init_player_east(t_player *player)
{
    player->dir.x = 1.0;
    player->dir.y = 0.0;
    player->plane.x = -player->dir.y * FOV;
    player->plane.y = player->dir.x * FOV;
}

// ====================================== PLAYER MOVEMENT & ROTATION

int set_player_movement(t_game *game)
{
    int moved;
    t_vec2D     dir;
    t_vec2Di    move;

    moved = 0;
    dir = game->player.dir;
    move = game->player.move;
    if (move.y == 1 && move.x == -1)   // Diagonal forward left
        moved += move_player(game, (t_vec2D){dir.x + dir.y, dir.y - dir.x});
    else if (move.y == 1 && move.x == 1)   // Diagonal forward right
        moved += move_player(game, (t_vec2D){dir.x - dir.y, dir.y + dir.x});
    else if (move.y == -1 && move.x == -1) // Diagonal backward left
        moved += move_player(game, (t_vec2D){-dir.x + dir.y, -dir.y - dir.x});
    else if (move.y == -1 && move.x == 1) // Diagonal backward right
        moved += move_player(game, (t_vec2D){-dir.x - dir.y, -dir.y + dir.x});
    else if (move.y == 1)  // Forward
        moved += move_player(game, (t_vec2D){dir.x, dir.y});
    else if (move.y == -1)   // Backward
        moved += move_player(game, (t_vec2D){-dir.x, -dir.y});
    else if (move.x == -1)  // Left
        moved += move_player(game, (t_vec2D){dir.y, -dir.x});
    else if (move.x == 1)   // Right
        moved += move_player(game, (t_vec2D){-dir.y, dir.x});
    return (moved);
}

int move_player(t_game *game, t_vec2D move)
{
    t_vec2D pos;
    t_vec2D new_pos;
    double magnitude;

    pos = game->player.pos;
    magnitude = sqrt(move.x * move.x + move.y * move.y);
    if (magnitude > 0)
    {
        move.x = (move.x / magnitude) * MOVESPEED;
        move.y = (move.y / magnitude) * MOVESPEED;
    }
    new_pos.x = pos.x + move.x;
    new_pos.y = pos.y + move.y;
    return (check_valid_move(game, new_pos));
}

int check_valid_move(t_game *game, t_vec2D new_pos)
{
    int moved;

    moved = 0;
    if (!collide_boundaries(game, (t_vec2D){new_pos.x, game->player.pos.y}) &&
        !collide_walls(game, (t_vec2D){new_pos.x, game->player.pos.y}))
    {
        game->player.pos.x = new_pos.x;
        moved = 1;
    }
    if (!collide_boundaries(game, (t_vec2D){game->player.pos.x, new_pos.y}) &&
        !collide_walls(game, (t_vec2D){game->player.pos.x, new_pos.y}))
    {
        game->player.pos.y = new_pos.y;
        moved = 1;
    }
    return (moved);
}

bool    collide_boundaries(t_game *game, t_vec2D pos)
{
    if (pos.x < 0.25 || pos.x >= game->mapdata.width - 0.25)
        return (true);
    if (pos.y < 0.25 || pos.y >= game->mapdata.height - 0.25)
        return (true);
    return (false);
}

bool    collide_walls(t_game *game, t_vec2D pos)
{
    if (game->map[(int)pos.y][(int)pos.x] == '1')
        return (true);
    return (false);
}

int set_player_rotation(t_game *game)
{
    t_player    *player;
    double  angle;


    player = &game->player;
    if (player->rotate != -1 && player->rotate != 1)
        return (0);
    angle = player->rotate * ROTSPEED;
    rotate_vec2D(&player->dir, angle);
    rotate_vec2D(&player->plane, angle);
    return (1);
}

void    rotate_vec2D(t_vec2D *v, double angle)
{
    double  cos_a;
    double  sin_a;
    double  cur_x;

    cos_a = cos(angle);
    sin_a = sin(angle);
    cur_x = v->x;

    v->x = v->x * cos_a - v->y * sin_a;
    v->y = cur_x * sin_a + v->y * cos_a;
}

void    handle_input(t_game *game)
{
    mlx_hook(game->win, ClientMessage, NoEventMask, quit_c3d, game);
    mlx_hook(game->win, KeyPress, KeyPressMask, handle_key_press, game);
    mlx_hook(game->win, KeyRelease, KeyReleaseMask, handle_key_release, game);
    mlx_hook(game->win, MotionNotify, PointerMotionMask, handle_mouse, game);
}

int handle_key_press(int key, t_game *game)
{
    t_player    *player;

    player = &game->player;
    if (key == XK_Escape)
        clean_c3d_exit(game, 0);
    if (key == XK_Left)
        player->rotate -= 1;
    if (key == XK_Right)
        player->rotate += 1;
    if (key == XK_z)
        player->move.y = 1;
    if (key == XK_s)
        player->move.y = -1;
    if (key == XK_q)
        player->move.x = -1;
    if (key == XK_d)
        player->move.x = 1;
    return (0);
}

int handle_key_release(int key, t_game *game)
{
    t_player    *player;

    player = &game->player;
    if (key == XK_Escape)
        clean_c3d_exit(game, 0);
    if (key == XK_Left && player->rotate <= 1)
        player->rotate = 0;
    if (key == XK_Right && player->rotate >= -1)
        player->rotate = 0;
    if (key == XK_z && player->move.y == 1)
        player->move.y = 0;
    if (key == XK_s && player->move.y == -1)
        player->move.y = 0;
    if (key == XK_q && player->move.x == -1)
        player->move.x += 1;
    if (key == XK_d && player->move.x == 1)
        player->move.x -= 1;
    return (0);
}

int handle_mouse(int x, int y, t_game *game)
{
    t_player    *player;
    int delta_x;
    double angle;

    (void)y;
    player = &game->player;
    delta_x = x - WIDTH / 2;
    if (delta_x != 0)
    {
        angle = delta_x * MOUSE_SENS;
        rotate_vec2D(&player->dir, angle);
        rotate_vec2D(&player->plane, angle);
        player->mouse_rotate = 1;
    }
    if (x != WIDTH / 2)
        mlx_mouse_move(game->mlx, game->win, WIDTH / 2, HEIGHT / 2);
    return (0);
}
