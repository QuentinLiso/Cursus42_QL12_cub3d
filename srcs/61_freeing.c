/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   61_freeing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:49:50 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 16:10:19 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_arr(void **arr)
{
	int	i;

	if (!arr)
		return ;
	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

void	free_str(char **str)
{
	if (!*str)
		return ;
	free(*str);
	*str = NULL;
}
