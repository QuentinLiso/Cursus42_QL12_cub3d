/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 08:16:07 by qliso             #+#    #+#             */
/*   Updated: 2025/03/26 17:27:59 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool    is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

void    skip_blank(char *line, int *i)
{
    while (is_space(line[*i]))
        (*i)++;
}

int quit_c3d(t_game *game)
{
    clean_c3d_exit(game, 0);
    return (0);
}

void    clean_c3d_exit(t_game *game, int errnum)
{
    if (!game)
        exit(errnum);
    free_game(game);
    exit(errnum);
}

