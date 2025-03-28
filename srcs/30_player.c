/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   30_player.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:51:54 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 15:35:52 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_player_start_pos(t_game *game)
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

void	init_player_north(t_player *player)
{
	player->dir.x = 0.0;
	player->dir.y = -1.0;
	player->plane.x = -player->dir.y * FOV;
	player->plane.y = player->dir.x * FOV;
}

void	init_player_south(t_player *player)
{
	player->dir.x = 0.0;
	player->dir.y = 1.0;
	player->plane.x = -player->dir.y * FOV;
	player->plane.y = player->dir.x * FOV;
}

void	init_player_west(t_player *player)
{
	player->dir.x = -1.0;
	player->dir.y = 0.0;
	player->plane.x = -player->dir.y * FOV;
	player->plane.y = player->dir.x * FOV;
}

void	init_player_east(t_player *player)
{
	player->dir.x = 1.0;
	player->dir.y = 0.0;
	player->plane.x = -player->dir.y * FOV;
	player->plane.y = player->dir.x * FOV;
}
