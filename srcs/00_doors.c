/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_doors.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:11:14 by qliso             #+#    #+#             */
/*   Updated: 2025/03/27 17:14:04 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int parsing_door(t_game *game)
{
    int status;

    status = check_valid_file(game->texs[DOOR].path, ".xpm");
    return (status);
}