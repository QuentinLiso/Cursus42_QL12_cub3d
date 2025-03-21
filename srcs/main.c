/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 08:16:09 by qliso             #+#    #+#             */
/*   Updated: 2025/03/22 13:28:30 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int main(int ac, char **av)
{
    (void)ac;(void)av;
    t_game  game;

    // if (ac != 2)
    //     return (perror_c3d("Wrong args", EINVAL));
    init_game(&game);
    parse_args(&game, av);
    print_textures(game.textures);
    print_strarr(game.map);
    return (0);
}