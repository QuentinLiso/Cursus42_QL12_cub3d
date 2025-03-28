/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   24_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:50:54 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 15:27:15 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// =========================================== BUILD MAP

int	build_map(t_game *game, char **content, int i)
{
	char	**map;
	int		status;

	status = skip_empty_lines(content, &i);
	if (status)
		return (status);
	map = &content[i];
	status = check_validity(game, map);
	if (status)
		return (status);
	status = fill_game_map(game, map);
	if (status)
		return (status);
	status = valid_nswe_side(game);
	if (status)
		return (status);
	status = check_flood_fill(game);
	if (status)
		return (status);
	fill_one(game);
	return (0);
}

int	skip_empty_lines(char **content, int *i)
{
	while (content[*i] && is_empty_line(content[*i]))
		(*i)++;
	if (!content[*i])
		return (EEMPTYMAP);
	return (0);
}

bool	is_empty_line(char *line)
{
	int	i;

	i = 0;
	skip_blank(line, &i);
	if (!line[i])
		return (true);
	return (false);
}

int	check_validity(t_game *game, char **map)
{
	int	i;
	int	status;

	status = check_edge_line(game, map[0]);
	if (status)
		return (status);
	i = 1;
	while (map[i] && map[i + 1])
	{
		status = check_other_line(game, map, i);
		if (status)
			return (status);
		i++;
	}
	if (i < 3)
		return (EMAPTOOSMALL);
	status = check_edge_line(game, map[i]);
	if (status)
		return (status);
	if (!game->player.orientation)
		return (EMISSINGORIENT);
	game->mapdata.height = i + 1;
	return (0);
}
