/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   20_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:50:54 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 17:01:13 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	parse_args(t_game *game, char **av)
{
	int	status;

	status = check_valid_file(av[1], ".cub");
	if (status)
		clean_c3d_exit(game, perror_c3d(status));
	status = store_file_data(game, av[1]);
	if (status)
		clean_c3d_exit(game, status);
	status = parse_filecontent(game);
	if (status)
		clean_c3d_exit(game, status);
	set_player_start_pos(game);
	return (0);
}

// ========================================== Parse args : check valid file
int	check_valid_file(char *av, char *ext)
{
	int	fd;

	if (check_file_isdir(av))
		return (EDIR);
	fd = open(av, O_RDONLY);
	if (fd < 0)
		return (EOPEN);
	close(fd);
	if (!check_file_ext(av, ext))
		return (EFILEEXT);
	return (0);
}

bool	check_file_isdir(char *av)
{
	int	fd;

	fd = open(av, __O_DIRECTORY);
	if (fd > 0)
	{
		close(fd);
		return (true);
	}
	return (false);
}

bool	check_file_ext(char *av, char *ext)
{
	size_t	av_len;
	size_t	ext_len;

	if (!av || !ext)
		return (false);
	av_len = ft_strlen(av);
	ext_len = ft_strlen(ext);
	if (ext_len >= av_len || !ext_len || !av_len)
		return (false);
	ext_len--;
	av_len--;
	while (ext_len > 0)
	{
		if (av[av_len] != ext[ext_len])
			return (false);
		ext_len--;
		av_len--;
	}
	if (ext[ext_len] != av[av_len])
		return (false);
	return (true);
}
