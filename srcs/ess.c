/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ess.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 12:08:01 by qliso             #+#    #+#             */
/*   Updated: 2025/03/22 19:55:48 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void    edit(char **str)
{
    str[0][0] = 'X';
}


int main(/*int ac, char **av*/)
{
    char **str = malloc(2 * sizeof(char *));
    str[0] = ft_strdup("bonjour");
    str[1] = ft_strdup("salut");
    printf("%s\n", str[0]);
    printf("%s\n", str[1]);
    edit(str);
    printf("%s\n", str[0]);
    printf("%s\n", str[1]);
    return (0);
}