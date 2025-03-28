/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   25_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:50:54 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 15:29:52 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_edge_line(t_game *game, char *line)
{
	int	j;

	j = 0;
	while (line[j] && line[j] != '\n')
	{
		if (line[j] != '1' && line[j] != ' ')
			return (ECHAREDGE);
		j++;
	}
	if (!j)
		return (EEMPTYMAP);
	if (j > game->mapdata.width)
		game->mapdata.width = j;
	return (0);
}

int	check_other_line(t_game *game, char **map, int i)
{
	int	j;
	int	status;

	j = 0;
	if (map[i][0] == '0' || map[i][0] == 'D')
		return (EVOIDONSIDE);
	status = check_other_line_loop(game, map, i, &j);
	if (status)
		return (status);
	if (j > 0 && str_contain("0NSWED", map[i][j - 1]))
		return (EVOIDONSIDE);
	if (j > game->mapdata.width)
		game->mapdata.width = j;
	return (0);
}

int	check_other_line_loop(t_game *game, char **map, int i, int *j)
{
	int	status;

	while (map[i][*j] && map[i][*j] != '\n')
	{
		if (str_contain("NSWE", map[i][*j]))
		{
			status = valid_nswe(game, map, i, *j);
			if (status)
				return (status);
		}
		else if (!str_contain("10D ", map[i][*j]))
			return (ECHARMAP);
		(*j)++;
	}
	return (0);
}

bool	str_contain(char *str, char c)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (c == str[i])
			return (true);
	}
	return (false);
}

int	valid_nswe(t_game *game, char **map, int i, int j)
{
	if (game->player.orientation)
		return (EDOUBLEORIENT);
	if (j == 0 || map[i][j + 1] == '\0' || map[i][j + 1] == '\n')
		return (EORIENTONSIDE);
	game->player.orientation = map[i][j];
	game->player.spawn = (t_vec2Di){j, i};
	map[i][j] = '0';
	return (0);
}
