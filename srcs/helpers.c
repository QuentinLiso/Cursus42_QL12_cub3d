/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:21:41 by qliso             #+#    #+#             */
/*   Updated: 2025/03/26 17:46:15 by qliso            ###   ########.fr       */
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

void    print_tex(t_tex *texs)
{
    printf("North : %s\n", texs[NORTH].path);
    printf("South : %s\n", texs[SOUTH].path);
    printf("West : %s\n", texs[WEST].path);
    printf("East : %s\n", texs[EAST].path);
    printf("Floor : %u, %u, %u\n",
        texs[FLOOR].color >> 16 & 0xff,
        texs[FLOOR].color >> 8 & 0xff,
        texs[FLOOR].color & 0xff);
    printf("Ceiling : %u, %u, %u\n",
        texs[CEILING].color >> 16 & 0xff,
        texs[CEILING].color >> 8 & 0xff,
        texs[CEILING].color & 0xff);    
}

void    print_texbool(t_tex *texs)
{
    printf("North : %d\n", texs[NORTH].filled);
    printf("South : %d\n", texs[SOUTH].filled);
    printf("West : %d\n", texs[WEST].filled);
    printf("East : %d\n", texs[EAST].filled);
    printf("Floor : %d\n", texs[FLOOR].filled);
    printf("Ceiling : %d\n", texs[CEILING].filled);
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