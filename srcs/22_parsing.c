/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   22_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:50:54 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 15:16:42 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	missing_tex_msg(t_game *game)
{
	t_tex	*texs;

	texs = game->texs;
	if (!texs[NORTH].filled)
		perror_c3d(EMISSINGTEXN);
	if (!texs[SOUTH].filled)
		perror_c3d(EMISSINGTEXS);
	if (!texs[WEST].filled)
		perror_c3d(EMISSINGTEXW);
	if (!texs[EAST].filled)
		perror_c3d(EMISSINGTEXE);
	if (!texs[FLOOR].filled)
		perror_c3d(EMISSINGCOLF);
	if (!texs[CEILING].filled)
		perror_c3d(EMISSINGCOLC);
}

int	parse_line_tex(t_game *game, char *line)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	skip_blank(line, &i);
	if (!line[i])
		return (0);
	if (!ft_isprint(line[i]))
		return (perror_c3d(EWRONGCHAR));
	status = get_texture_and_colors(game, line, i);
	if (status)
		missing_tex_msg(game);
	return (status);
}

// ======================================== GET TEXTURES AND COLORS

int	get_texture_and_colors(t_game *game, char *line, int i)
{
	int	status;

	status = 0;
	if (line[i] == 'C')
		status = get_cf_color(game, line, i, CEILING);
	else if (line[i] == 'F')
		status = get_cf_color(game, line, i, FLOOR);
	else if (line[i] == 'N' && line[i + 1] == 'O')
		status = get_text_path(game, line, i, NORTH);
	else if (line[i] == 'S' && line[i + 1] == 'O')
		status = get_text_path(game, line, i, SOUTH);
	else if (line[i] == 'W' && line[i + 1] == 'E')
		status = get_text_path(game, line, i, WEST);
	else if (line[i] == 'E' && line[i + 1] == 'A')
		status = get_text_path(game, line, i, EAST);
	else
		status = EWRONGCHAR;
	return (status);
}

// ======================================== GET TEXT PATH

int	get_text_path(t_game *game, char *line, int i, t_orient orient)
{
	int		status;
	char	*filepath;

	status = 0;
	i += 2;
	if (game->texs[orient].filled)
		return (EDOUBLETEX);
	status = get_text_file_path(line, i, &filepath);
	if (status)
		return (status);
	game->texs[orient].path = filepath;
	game->texs[orient].filled = true;
	return (0);
}

int	get_text_file_path(char *line, int i, char **filepath)
{
	int	start;
	int	end;

	skip_blank(line, &i);
	if (!line[i])
		return (EMISSINGTEX);
	start = i;
	while (ft_isprint(line[i]) && line[i] != ' ')
		i++;
	end = i - 1;
	skip_blank(line, &i);
	if (line[i])
		return (EPATHTEX);
	*filepath = ft_substr(line, start, end - start + 1);
	if (!*filepath)
		return (EMALLOC);
	if (check_valid_file(*filepath, ".xpm"))
		return (EFILEEXT);
	return (0);
}
