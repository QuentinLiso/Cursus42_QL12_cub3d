/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 08:16:11 by qliso             #+#    #+#             */
/*   Updated: 2025/03/25 21:37:08 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include "mlx.h"
# include <X11/keysym.h>
# include <X11/X.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <math.h>
# include <string.h>
# include <stdint.h>
# include <errno.h>

// Define
# define WIDTH 1024
# define HEIGHT 720
# define BLOCK 64
# define FOV 0.66
# define MOVESPEED 0.1
# define ROTSPEED 0.05
# define MOUSE_SENS 0.0001

typedef uint32_t    uint;

// Enums
typedef enum    e_orient
{
    NORTH,
    SOUTH,
    WEST,
    EAST,
    FLOOR,
    CEILING
}   t_orient;

// Structs

typedef struct s_vec2Di
{
    int x;
    int y;
}   t_vec2Di;

typedef struct s_vec2D
{
    double x;
    double y;
}   t_vec2D;

typedef struct s_player
{
    char        orientation;
    t_vec2Di    spawn;
    t_vec2D     pos;
    t_vec2D     dir;
    t_vec2D     plane;
    int         has_moved;
    t_vec2Di    move;
    int         rotate;
    int         mouse_rotate;
}   t_player;

typedef struct s_raycast
{
    double      camera_x;
    t_vec2D     dir;
    t_vec2Di    map;
    t_vec2Di    step;
    t_vec2D     sidedist;
    t_vec2D     deltadist;
    double      wall_dist;
    double      wall_x;
    int         side;
    int         line_height;
    int         draw_start;
    int         draw_end;
}   t_raycast;

typedef struct s_img
{
    void    *img;
    int     *addr;
    int     bpp;
    int     size_line;
    int     endian;
}   t_img;

typedef struct s_texture
{
    char            *north;
    char            *south;
    char            *west;
    char            *east;
    int             *floor;
    int             *ceil;
    bool            floorbool;
    bool            ceilbool;
    uint            hex_floor;
    uint            hex_ceil;
    // int             size;
    int             width;
    int             height;
    int             index;
    double          step;
    double          pos;
    t_vec2Di        coord;
}   t_texture;

typedef struct s_mapdata
{
    int     fd;
    int     line_count;
    char    *filepath;
    char    **filecontent;
    int     height;
    int     width;
    int     end_index;
}   t_mapdata;

typedef struct s_minimap
{
    char        **map;
    t_img       *img;
    int         size;
    t_vec2Di    offset;
    int         view_dist;
    int         tile_size;
}   t_minimap;

typedef struct s_game
{
    void        *mlx;
    void        *win;
    int         win_height;
    int         win_width;

    t_player    player;
    t_raycast   ray;

    t_texture   textures;
    int         **tex_pixels;
    int         **tex_array;
    
    t_mapdata   mapdata;
    char        **map;
    t_img       minimap;
}   t_game;

// Helpers
void    print_strarr(char **arr);
void    print_textures(t_texture textures);
void    show_player_stats(t_player player);


// Functions
bool    is_space(char c);
void    skip_blank(char *line, int *i);

int perror_c3d(char *msg, int errnum);

int quit_c3d(t_game *game);
void    clean_c3d_exit(t_game *game, int errnum);
void    free_game(t_game *game);
void    free_game_mlx(t_game *game);
void    free_game_textures(t_game *game);
void    free_game_pixels(t_game *game);
void    free_game_map(t_game *game);
void    free_arr(void **arr);
void    free_str(char **str);

void    init_game(t_game *game);
void    init_player(t_player *player);
void    init_textures(t_texture *textures);
void    init_mapdata(t_mapdata *mapdata);
void    init_empty_img(t_img *img);

int parse_args(t_game *game, char **av);

int check_valid_file(char *av, char *ext);
bool    check_file_isdir(char *av);
bool    check_file_ext(char *av, char *ext);

int    store_file_data(t_game *game, char *filepath);
int get_line_count(char *filepath);
int fill_filecontent(t_game *game);

int     parse_filecontent(t_game *game);

bool    tex_and_colors_filled(t_game *game);
int parse_line_tex(t_game *game, char *line);
int get_texture_and_colors(t_game *game, char *line, int i);
int get_text_path(t_game *game, char *line, int i, t_orient orient);
int get_text_file_path(char *line, int i, char **filepath);
int get_cf_color(t_game *game, char *line, int i, t_orient orient);
int get_rgb(char *line, uint *rgb, int i);
int get_r(char *line, uint *rgb, int *i);
int get_g(char *line, uint *rgb, int *i);
int get_b(char *line, uint *rgb, int *i);

int     build_map(t_game *game, char **content, int i);
int     skip_empty_lines(char **content, int *i);
bool    is_empty_line(char *line);
int     check_validity(t_game *game, char **map);
int     check_edge_line(t_game *game, char *line);
int     check_other_line(t_game *game, char **map, int i);
bool    str_contain(char *str, char c);
bool    check_nswe(t_game *game, char **map, int i, int j);
int     fill_game_map(t_game *game, char **map);
bool    valid_nswe_side(t_game *game);
int check_flood_fill(t_game *game);
int flood_fill(char **map, t_vec2Di pos, int width, int height);
void    fill_one(t_game *game);

void    set_player_start_pos(t_game *game);
void    init_player_north(t_player *player);
void    init_player_south(t_player *player);
void    init_player_west(t_player *player);
void    init_player_east(t_player *player);

int set_player_movement(t_game *game);
int move_player(t_game *game, t_vec2D move);
int check_valid_move(t_game *game, t_vec2D new_pos);
bool    collide_boundaries(t_game *game, t_vec2D pos);
bool    collide_walls(t_game *game, t_vec2D pos);
int set_player_rotation(t_game *game);
void    rotate_vec2D(t_vec2D *v, double angle);
void    handle_input(t_game *game);
int handle_key_press(int key, t_game *game);
int handle_key_release(int key, t_game *game);
int handle_mouse(int x, int y, t_game *game);

void    init_mlx(t_game *game);
void    init_tex_array(t_game *game);
void    init_img(t_game *game, t_img *img, int width, int height);
void    init_tex_img(t_game *game, t_img *img, char *path);
int *xpm_to_img(t_game *game, char *path);
void    put_pixel_to_img(t_img *img, t_vec2Di coord, int color);

int     update_render(t_game *game);
void    update_render_frame(t_game *game);
void    init_tex_pixels(t_game *game);
void    init_raycast(t_raycast *ray);
void    raycast_algo(t_game *game);
void    init_player_raycast(t_raycast *ray, t_player *player, int x);
void    init_dda_raycast(t_raycast *ray, t_player *player);
void    launch_dda_raycast(t_raycast *ray, t_game *game);
bool    collide_boundaries_i(t_game *game, t_vec2Di pos);
bool    collide_walls_i(t_game *game, t_vec2Di pos);
void    set_line_height(t_raycast *ray, t_player *player, t_game *game);
void    set_game_tex_pixels(t_game *game, t_raycast *ray, int x);
t_orient    get_texture_orientation(t_raycast *ray);
void    put_frame_to_screen(t_game *game);
void    update_frame_pixel(t_game *game, t_img *img, t_vec2Di coord);


#endif