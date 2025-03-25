/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 08:16:09 by qliso             #+#    #+#             */
/*   Updated: 2025/03/25 11:39:54 by qliso            ###   ########.fr       */
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
    printf("\n");
    print_strarr(game.map);
    show_player_stats(game.player);
    init_mlx(&game);
    init_tex_array(&game);
    printf("coucou\n");
    update_render_frame(&game);
    handle_input(&game);
    mlx_loop_hook(game.mlx, update_render, &game);
    mlx_loop(game.mlx);
    return (0);
}