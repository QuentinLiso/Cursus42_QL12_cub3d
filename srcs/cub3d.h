/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:17:38 by qliso             #+#    #+#             */
/*   Updated: 2025/03/15 16:20:20 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include "mlx.h"

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <X11/X.h>
# include <X11/keysym.h>
# include <math.h>
# include <errno.h>
# include <stdbool.h>

# define WINDOW_WIDTH 1920
# define WINDOW_HEIGHT 1080

# define RED 0x992E24
# define BLUE 0x4287f5
# define GREEN 0x1F6650
# define YELLOW 0xFFE26F
# define PINK 0xFDADC7
# define WHITE 0xFBFAFC

typedef struct s_img
{
    void    *mlx_img;
    char    *addr;
    int     bpp;
    int     size_line;
    int     endian;
    int     width;
    int     height;
    char    *img_file;
}   t_img;


typedef struct s_scene
{
    void    *mlx_disp;
    void    *mlx_win;
    t_img   *img;
    bool    to_update;
    unsigned int    win_w;
    unsigned int    win_h;
    int     color;
}   t_scene;

int     init_scene(t_scene *scene);
int     set_display_elems(t_scene *scene);
int     set_loops(t_scene *scene);
int     update_render(t_scene *scene);
void    draw_square(t_img *img, int x, int y, int color);
void	put_pix_to_img(t_img *img, int x, int y, int color);
int	    handle_keypress_input(int keysym, t_scene *scene);
int     closing_window(t_scene *scene);
void    safe_free(void **ptr);
void    scene_cleanup(t_scene *scene);


#endif