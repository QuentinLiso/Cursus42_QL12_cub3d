/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:21:41 by qliso             #+#    #+#             */
/*   Updated: 2025/03/22 18:53:51 by qliso            ###   ########.fr       */
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