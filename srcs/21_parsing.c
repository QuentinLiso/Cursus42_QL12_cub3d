/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   21_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:50:54 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 15:12:30 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// ========================================== Parse args : store file data
int	store_file_data(t_game *game, char *filepath)
{
	if (get_line_count(filepath, &game->mapdata.line_count))
		return (EOPEN);
	game->mapdata.filepath = filepath;
	game->mapdata.filecontent = ft_calloc(game->mapdata.line_count + 1,
			sizeof(char *));
	if (!game->mapdata.filecontent)
		return (perror_c3d(EMALLOC));
	game->mapdata.fd = open(filepath, O_RDONLY);
	if (game->mapdata.fd < 0)
		return (perror_c3d(EOPEN));
	if (fill_filecontent(game))
	{
		close (game->mapdata.fd);
		return (EMALLOC);
	}
	close (game->mapdata.fd);
	return (0);
}

int	get_line_count(char *file, int *map_line_count)
{
	int		fd;
	int		line_count;
	char	*line;

	line_count = 0;
	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (perror_c3d(EOPEN));
	line = get_next_line(fd);
	while (line)
	{
		line_count++;
		free_str(&line);
		line = get_next_line(fd);
	}
	free_str(&line);
	close(fd);
	*map_line_count = line_count;
	return (0);
}

int	fill_filecontent(t_game *game)
{
	char	*line;
	int		fd;
	int		i;

	fd = game->mapdata.fd;
	line = get_next_line(fd);
	i = -1;
	while (line)
	{
		game->mapdata.filecontent[++i] = ft_strdup(line);
		free_str(&line);
		if (!game->mapdata.filecontent[i])
		{
			free_arr((void **)game->mapdata.filecontent);
			return (perror_c3d(EMALLOC));
		}
		line = get_next_line(fd);
	}
	game->mapdata.filecontent[++i] = NULL;
	return (0);
}

// =========================================== Parse args : Parse file content
int	parse_filecontent(t_game *game)
{
	char	**content;
	int		i;
	int		status;

	content = game->mapdata.filecontent;
	if (!content)
		return (1);
	i = 0;
	while (content[i] && tex_and_colors_empty(game))
	{
		status = parse_line_tex(game, content[i++]);
		if (status)
			return (perror_c3d(status));
	}
	status = tex_and_colors_empty(game);
	if (status)
		return (perror_c3d(status));
	status = build_map(game, content, i);
	if (status)
		return (perror_c3d(status));
	return (0);
}

// ====================================== PARSE TEXTURES AND COLORS LINES
int	tex_and_colors_empty(t_game *game)
{
	t_tex	*texs;

	texs = game->texs;
	if (!texs[NORTH].filled)
		return (EMISSINGTEXN);
	if (!texs[SOUTH].filled)
		return (EMISSINGTEXS);
	if (!texs[WEST].filled)
		return (EMISSINGTEXW);
	if (!texs[EAST].filled)
		return (EMISSINGTEXE);
	if (!texs[FLOOR].filled)
		return (EMISSINGCOLF);
	if (!texs[CEILING].filled)
		return (EMISSINGCOLC);
	return (0);
}
