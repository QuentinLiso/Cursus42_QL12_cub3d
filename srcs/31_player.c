/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   31_player.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:51:54 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 15:39:36 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// ====================================== PLAYER MOVEMENT & ROTATION

int	set_player_movement(t_game *game)
{
	int			moved;
	t_vec2D		dir;
	t_vec2Di	move;

	moved = 0;
	dir = game->player.dir;
	move = game->player.move;
	if (move.y == 1 && move.x == -1)
		moved += move_player(game, (t_vec2D){dir.x + dir.y, dir.y - dir.x});
	else if (move.y == 1 && move.x == 1)
		moved += move_player(game, (t_vec2D){dir.x - dir.y, dir.y + dir.x});
	else if (move.y == -1 && move.x == -1)
		moved += move_player(game, (t_vec2D){-dir.x + dir.y, -dir.y - dir.x});
	else if (move.y == -1 && move.x == 1)
		moved += move_player(game, (t_vec2D){-dir.x - dir.y, -dir.y + dir.x});
	else if (move.y == 1)
		moved += move_player(game, (t_vec2D){dir.x, dir.y});
	else if (move.y == -1)
		moved += move_player(game, (t_vec2D){-dir.x, -dir.y});
	else if (move.x == -1)
		moved += move_player(game, (t_vec2D){dir.y, -dir.x});
	else if (move.x == 1)
		moved += move_player(game, (t_vec2D){-dir.y, dir.x});
	return (moved);
}

int	move_player(t_game *game, t_vec2D move)
{
	t_vec2D	pos;
	t_vec2D	new_pos;
	double	magnitude;

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

int	check_valid_move(t_game *game, t_vec2D new_pos)
{
	int	moved;

	moved = 0;
	if (!collide_boundaries(game, (t_vec2D){new_pos.x, game->player.pos.y})
		&& !collide_walls(game, (t_vec2D){new_pos.x, game->player.pos.y}))
	{
		game->player.pos.x = new_pos.x;
		moved = 1;
	}
	if (!collide_boundaries(game, (t_vec2D){game->player.pos.x, new_pos.y})
		&& !collide_walls(game, (t_vec2D){game->player.pos.x, new_pos.y}))
	{
		game->player.pos.y = new_pos.y;
		moved = 1;
	}
	return (moved);
}

bool	collide_boundaries(t_game *game, t_vec2D pos)
{
	if (pos.x < 0.25 || pos.x >= game->mapdata.width - 0.25)
		return (true);
	if (pos.y < 0.25 || pos.y >= game->mapdata.height - 0.25)
		return (true);
	return (false);
}

bool	collide_walls(t_game *game, t_vec2D pos)
{
	if (str_contain("1D", game->map[(int)pos.y][(int)pos.x]))
		return (true);
	return (false);
}
