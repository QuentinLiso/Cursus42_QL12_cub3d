/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   26_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:50:54 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 15:34:31 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	fill_game_map(t_game *game, char **map)
{
	int	i;
	int	j;
	int	height;
	int	width;

	height = game->mapdata.height;
	width = game->mapdata.width;
	game->map = ft_calloc(height + 1, sizeof(char *));
	if (!game->map)
		return (EMALLOC);
	i = -1;
	while (++i < height)
	{
		game->map[i] = ft_calloc(width + 1, sizeof(char));
		if (!game->map[i])
			return (EMALLOC);
		j = -1;
		while (map[i][++j] && map[i][j] != '\n')
			game->map[i][j] = map[i][j];
		while (j < width)
			game->map[i][j++] = ' ';
		game->map[i][j] = '\0';
	}
	game->map[i] = NULL;
	return (0);
}

int	valid_nswe_side(t_game *game)
{
	t_vec2Di	spawn;
	char		**map;

	spawn = game->player.spawn;
	map = game->map;
	if ((map[spawn.y - 1][spawn.x] == '0'
		|| map[spawn.y + 1][spawn.x] == '0'
		|| map[spawn.y][spawn.x - 1] == '0'
		|| map[spawn.y][spawn.x + 1] == '0'))
		return (0);
	return (EPLAYERNOSPACE);
}

int	check_flood_fill(t_game *game)
{
	int	x;
	int	y;
	int	width;
	int	height;

	width = game->mapdata.width;
	height = game->mapdata.height;
	x = -1;
	while (++x < width)
	{
		if (flood_fill(game->map, (t_vec2Di){x, 0}, width, height))
			return (EMISSINGWALLS);
		if (flood_fill(game->map, (t_vec2Di){x, height - 1}, width, height))
			return (EMISSINGWALLS);
	}
	y = 0;
	while (++y < height - 1)
	{
		if (flood_fill(game->map, (t_vec2Di){0, y}, width, height))
			return (EMISSINGWALLS);
		if (flood_fill(game->map, (t_vec2Di){width - 1, y}, width, height))
			return (EMISSINGWALLS);
	}
	return (0);
}

int	flood_fill(char **map, t_vec2Di pos, int width, int height)
{
	if (pos.x < 0 || pos.y < 0 || pos.x >= width || pos.y >= height)
		return (0);
	if (str_contain("0NSWED", map[pos.y][pos.x]))
		return (EMISSINGWALLS);
	if (str_contain("1X", map[pos.y][pos.x]))
		return (0);
	map[pos.y][pos.x] = 'X';
	if (flood_fill(map, (t_vec2Di){pos.x - 1, pos.y}, width, height)
		|| flood_fill(map, (t_vec2Di){pos.x + 1, pos.y}, width, height)
		|| flood_fill(map, (t_vec2Di){pos.x, pos.y - 1}, width, height)
		|| flood_fill(map, (t_vec2Di){pos.x, pos.y + 1}, width, height))
		return (EMISSINGWALLS);
	return (0);
}

void	fill_one(t_game *game)
{
	char	**map;
	int		i;
	int		j;

	map = game->map;
	i = -1;
	while (map[++i])
	{
		j = -1;
		while (map[i][++j])
		{
			if (map[i][j] == ' ' || map[i][j] == 'X')
				map[i][j] = '1';
		}
	}
}
