/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:21:41 by qliso             #+#    #+#             */
/*   Updated: 2025/03/24 10:30:36 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void    print_strarr(char **arr)
{
    if (!arr)
        return ;
    while (*arr)
        ft_putendl_fd(*arr++, STDOUT_FILENO);
    // ft_putchar_fd('\n', STDOUT_FILENO);
}

void    print_textures(t_texture textures)
{
    printf("North : %s\n", textures.north);
    printf("South : %s\n", textures.south);
    printf("West : %s\n", textures.west);
    printf("East : %s\n", textures.east);
    printf("Ceiling : %u, %u, %u\n",
        textures.hex_ceil >> 16 & 0xff,
        textures.hex_ceil >> 8 & 0xff,
        textures.hex_ceil & 0xff);
    printf("Floor : %u, %u, %u\n",
        textures.hex_floor >> 16 & 0xff,
        textures.hex_floor >> 8 & 0xff,
        textures.hex_floor & 0xff);
}

void    show_player_stats(t_player player)
{
    printf("****************************\n");
    printf("Orientation : %c\n", player.orientation);
    printf("Pos : %f %f\n", player.pos.x, player.pos.y);
    printf("Dir : %f %f\n", player.dir.x, player.dir.y);
    printf("Plane : %f %f\n", player.plane.x, player.plane.y);
    printf("****************************\n");
}