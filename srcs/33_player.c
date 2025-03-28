/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   33_player.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:51:54 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 15:49:12 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	handle_input(t_game *game)
{
	mlx_hook(game->win, ClientMessage, NoEventMask, quit_c3d, game);
	mlx_hook(game->win, KeyPress, KeyPressMask, handle_key_press, game);
	mlx_hook(game->win, KeyRelease, KeyReleaseMask, handle_key_release, game);
	mlx_hook(game->win, MotionNotify, PointerMotionMask, handle_mouse, game);
}

int	handle_key_press(int key, t_game *game)
{
	t_player	*player;

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
	if (key == XK_e)
		door_raycast(game);
	return (0);
}

int	handle_key_release(int key, t_game *game)
{
	t_player	*player;

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

int	handle_mouse(int x, int y, t_game *game)
{
	double		delta_angle;
	static int	old_x = -1;

	(void)y;
	if (old_x == -1)
		old_x = x;
	delta_angle = (double)(x - old_x) / game->win_width * 2 * PI;
	rotate_vec2d(&game->player.dir, delta_angle);
	rotate_vec2d(&game->player.plane, delta_angle);
	game->player.mouse_rotate = 1;
	old_x = x;
	return (0);
}
