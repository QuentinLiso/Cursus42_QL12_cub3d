/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:16:42 by qliso             #+#    #+#             */
/*   Updated: 2025/03/15 15:56:01 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int main(void)
{
    t_scene scene;

    ft_bzero(&scene, sizeof(t_scene));
    init_scene(&scene);
    set_display_elems(&scene);
    set_loops(&scene);
    return (0);
}