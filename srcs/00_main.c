/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 08:16:09 by qliso             #+#    #+#             */
/*   Updated: 2025/03/26 17:34:08 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int main(int ac, char **av)
{
    (void)ac;(void)av;
    t_game  game;

    if (ac != 2)
        return (perror_c3d(EARGS));
    init_game(&game);
    parse_args(&game, av);
    init_mlx(&game);
    init_texs(&game);
    update_render_frame(&game);
    handle_input(&game);
    mlx_loop_hook(game.mlx, update_render, &game);
    mlx_loop(game.mlx);
    return (0);
}