/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   23_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:50:54 by qliso             #+#    #+#             */
/*   Updated: 2025/03/28 15:24:31 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// ======================================== GET COLORS for CEIL FLOOR

int	get_cf_color(t_game *game, char *line, int i, t_orient orient)
{
	t_uint	rgb;
	int		status;

	i++;
	rgb = 0x0;
	status = get_rgb(line, &rgb, i);
	if (status)
		return (status);
	if (game->texs[orient].filled)
		return (EDOUBLETEX);
	else
	{
		game->texs[orient].color = rgb;
		game->texs[orient].filled = true;
	}
	return (0);
}

int	get_rgb(char *line, t_uint *rgb, int i)
{
	int	status;

	status = get_r(line, rgb, &i);
	if (status)
		return (status);
	status = get_g(line, rgb, &i);
	if (status)
		return (status);
	status = get_b(line, rgb, &i);
	if (status)
		return (status);
	return (0);
}

int	get_r(char *line, t_uint *rgb, int *i)
{
	int		count;
	char	*r;
	t_uint	r_value;

	count = 0;
	skip_blank(line, i);
	while (count < 3 && ft_isdigit(line[*i + count]))
		count++;
	if (count == 0)
		return (ERGBFORMAT);
	r = ft_substr(&line[*i], 0, count + 1);
	if (!r)
		return (EMALLOC);
	r_value = ft_atoi(r);
	free_str(&r);
	if (r_value > 255)
		return (ERGBVAL);
	*i += count;
	skip_blank(line, i);
	if (line[*i] != ',')
		return (ERGBFORMAT);
	(*i)++;
	*rgb += (r_value << 16);
	return (0);
}

int	get_g(char *line, t_uint *rgb, int *i)
{
	int		count;
	char	*g;
	t_uint	g_value;

	count = 0;
	skip_blank(line, i);
	while (count < 3 && ft_isdigit(line[*i + count]))
		count++;
	if (count == 0)
		return (ERGBFORMAT);
	g = ft_substr(&line[*i], 0, count + 1);
	if (!g)
		return (EMALLOC);
	g_value = ft_atoi(g);
	free_str(&g);
	if (g_value > 255)
		return (ERGBVAL);
	*i += count;
	skip_blank(line, i);
	if (line[*i] != ',')
		return (ERGBFORMAT);
	(*i)++;
	*rgb += (g_value << 8);
	return (0);
}

int	get_b(char *line, t_uint *rgb, int *i)
{
	int		count;
	char	*b;
	t_uint	b_value;

	count = 0;
	skip_blank(line, i);
	while (count < 3 && ft_isdigit(line[*i + count]))
		count++;
	if (count == 0)
		return (ERGBFORMAT);
	b = ft_substr(&line[*i], 0, count + 1);
	if (!b)
		return (EMALLOC);
	b_value = ft_atoi(b);
	free_str(&b);
	if (b_value > 255)
		return (ERGBVAL);
	*i += count;
	skip_blank(line, i);
	if (line[*i])
		return (ERGBFORMAT);
	*rgb += b_value;
	return (0);
}
