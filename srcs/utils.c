/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 08:16:07 by qliso             #+#    #+#             */
/*   Updated: 2025/03/23 16:45:12 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool    is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

void    skip_blank(char *line, int *i)
{
    while (is_space(line[*i]))
        (*i)++;
}

int perror_c3d(char *msg, int errnum)
{
    ft_putstr_fd("CUB3D ERROR", STDERR_FILENO);
    if (msg)
    {
        ft_putstr_fd(": ", STDERR_FILENO);
        ft_putendl_fd(msg, STDERR_FILENO);
    }
    return (errnum);
}

void    clean_c3d_exit(t_game *game, int errnum)
{
    if (!game)
        exit(errnum);
    free_game(game);
    exit(errnum);
}

void     free_game(t_game *game)
{
    if (!game)
        return ;
    free_game_mlx(game);
    free_game_textures(game);
    free_game_map(game);
}

void    free_game_mlx(t_game *game)
{
    if (game->mlx && game->win)
        mlx_destroy_window(game->mlx, game->win);
    if (game->mlx)
    {
        mlx_destroy_display(game->mlx);
        mlx_loop_end(game->mlx);
        free(game->mlx);
    }
}

void    free_game_textures(t_game *game)
{
    if (game->textures.north)
        free(game->textures.north);
    if (game->textures.south)
        free(game->textures.south);
    if (game->textures.west)
        free(game->textures.west);
    if (game->textures.east)
        free(game->textures.east);
    if (game->textures.floor)
        free(game->textures.floor);
    if (game->textures.ceil)
        free(game->textures.ceil);
    if (game->tex_pixels)
        free_arr((void **)game->tex_pixels);
    if (game->tex_array)
        free_arr((void **)game->tex_array);
}

void    free_game_map(t_game *game)
{
    if (game->mapdata.fd > 0)
        close(game->mapdata.fd);
    if (game->mapdata.filecontent)
        free_arr((void **)game->mapdata.filecontent);
    if (game->map)
        free_arr((void **)game->map);
}

void    free_arr(void **arr)
{
    int i;

    if (!arr)
        return ;
    i = -1;
    while (arr[++i])
        free(arr[i]);
    free(arr);
}

void    free_str(char **str)
{
    if (!*str)
        return ;
    free(*str);
    *str = NULL;
}

// ===========================================

void    init_game(t_game *game)
{
    game->mlx = NULL;
    game->win = NULL;
    game->win_height = HEIGHT;
    game->win_width = WIDTH;
    init_player(&game->player);
    init_textures(&game->textures);
    game->tex_pixels = NULL;
    game->tex_array = NULL;
    init_mapdata(&game->mapdata);
    game->map = NULL;
    init_empty_img(&game->minimap);
}

void    init_player(t_player *player)
{
    player->orientation = 0;
    player->spawn = (t_vec2Di){0, 0};
    player->pos = (t_vec2D){0.0, 0.0};
    player->dir = (t_vec2D){0.0, 0.0};
    player->plane = (t_vec2D){0.0, 0.0};
    player->has_moved = 0;
    player->move = (t_vec2Di){0, 0};
    player->rotate = 0;
}

void    init_textures(t_texture *textures)
{
    textures->north = NULL;
    textures->south = NULL;
    textures->west = NULL;
    textures->east = NULL;
    textures->floor = NULL;
    textures->ceil = NULL;
    textures->hex_floor = 0;
    textures->hex_ceil = 0;
    textures->floorbool = false;
    textures->ceilbool = false;
    textures->size = BLOCK;
    textures->step = 0.0;
    textures->pos = 0.0;
    textures->coord = (t_vec2Di){0, 0};
}

void    init_mapdata(t_mapdata *mapdata)
{
    mapdata->fd = 0;
    mapdata->line_count = 0;
    mapdata->filepath = NULL;
    mapdata->filecontent = NULL;
    mapdata->height = 0;
    mapdata->width = 0;
    mapdata->end_index = 0;
}

void    init_empty_img(t_img *img)
{
    img->img = NULL;
    img->addr = NULL;
    img->bpp = 0;
    img->size_line = 0;
    img->endian = 0;
}

// ===========================================

int parse_args(t_game *game, char **av)
{
    if (check_valid_file(av[1], ".cub"))
        clean_c3d_exit(game, 1);
    if (store_file_data(game, av[1]))
        clean_c3d_exit(game, 1);
    if (parse_filecontent(game))
        clean_c3d_exit(game, 1);
    set_player_start_pos(game);
    return (0);
}

// ========================================== Parse args : check valid file

int check_valid_file(char *av, char *ext)
{
    int fd;
    
    if (check_file_isdir(av))
        return (perror_c3d("File is a directory", EISDIR));
    fd = open(av, O_RDONLY);
    if (fd < 0)
        return (perror_c3d(strerror(errno), 1));
    close(fd);
    if (!check_file_ext(av, ext))
        return (perror_c3d("Wrong file extension", 1));
    return (0);
}

bool    check_file_isdir(char *av)
{
    int fd;

    fd = open(av, __O_DIRECTORY);
    if (fd > 0)
    {
        close(fd);
        return (true);
    }
    return (false);
}

bool    check_file_ext(char *av, char *ext)
{
    size_t  av_len;
    size_t  ext_len;

    if (!av || !ext)
        return (false);
    av_len = ft_strlen(av);
    ext_len = ft_strlen(ext);
    if (ext_len >= av_len || !ext_len || !av_len)
        return (false);
    ext_len--;
    av_len--;
    while (ext_len > 0)
    {
        if (av[av_len] != ext[ext_len])
            return (false);
        ext_len--;
        av_len--;
    }
    if (ext[ext_len] != av[av_len])
        return (false);
    return (true);
}

// ========================================== Parse args : store file data

int    store_file_data(t_game *game, char *filepath)
{
    game->mapdata.line_count = get_line_count(filepath);
    game->mapdata.filepath = filepath;
    game->mapdata.filecontent = ft_calloc(game->mapdata.line_count + 1,
        sizeof(char *));
    if (!game->mapdata.filecontent)
        return (perror_c3d(strerror(ENOMEM), ENOMEM));
    game->mapdata.fd = open(filepath, O_RDONLY);
    if (game->mapdata.fd < 0)
        return (perror_c3d(strerror(errno), errno));
    if (fill_filecontent(game))
    {
        close (game->mapdata.fd);
        return (ENOMEM);
    }
    close (game->mapdata.fd);
    return (0);
}

int get_line_count(char *file)
{
    int     fd;
    int     line_count;
    char    *line;

    line_count = 0;
    fd = open(file, O_RDONLY);
    if (fd < 0)
        return (perror_c3d(strerror(errno), 0));
    line = get_next_line(fd);
    while (line)
    {
        line_count++;
        free_str(&line);
        line = get_next_line(fd);
    }
    free_str(&line);
    close(fd);
    return (line_count);
}

int fill_filecontent(t_game *game)
{
    char    *line;
    int     fd;
    int     i;

    fd = game->mapdata.fd;
    line = get_next_line(fd);
    i = -1;
    while (line)
    {
        game->mapdata.filecontent[++i] = ft_strdup(line);
        free_str(&line);
        if (!game->mapdata.filecontent[i])
        {
            free_arr((void **)game->mapdata.filecontent);
            return (perror_c3d(strerror(ENOMEM), ENOMEM));
        }
        line = get_next_line(fd);
    }
    game->mapdata.filecontent[++i] = NULL;
    return (0);
}

// =========================================== Parse args : Parse file content

int parse_filecontent(t_game *game)
{
    char    **content;
    int     i;
    int     status;

    content = game->mapdata.filecontent;
    if (!content)
        return (1);
    i = 0;
    while (content[i] && !tex_and_colors_filled(game))
    {
        status = parse_line_tex(game, content[i++]);
        if (status == -1)
            return (-1);
    }
    if (!tex_and_colors_filled(game))
        return (-1);
    status = build_map(game, content, i);
    return (status);
}

// ====================================== PARSE TEXTURES AND COLORS LINES

bool    tex_and_colors_filled(t_game *game)
{
    t_texture tex;
    
    tex = game->textures;
    return (tex.north && tex.south && tex.west && tex.east &&
        tex.floorbool && tex.ceilbool);
}

int parse_line_tex(t_game *game, char *line)
{
    int i;
    int status;

    i = 0;
    status = 0;
    skip_blank(line, &i);
    if (!line[i])
        return (1);
    if (ft_isprint(line[i]) && !ft_isdigit(line[i]))
        status = get_texture_and_colors(game, line, i);
    else
        return (-1);
    return (status);
}

// ======================================== GET TEXTURES AND COLORS

int get_texture_and_colors(t_game *game, char *line, int i)
{
    int status;

    status = 0;
    if (line[i] == 'C')
        status = get_cf_color(game, line, i, CEILING);
    else if (line[i] == 'F')
        status = get_cf_color(game, line, i, FLOOR);
    else if (line[i] == 'N' && line[i + 1] == 'O')
        status = get_text_path(game, line, i, NORTH);
    else if (line[i] == 'S' && line[i + 1] == 'O')
        status = get_text_path(game, line, i, SOUTH);
    else if (line[i] == 'W' && line[i + 1] == 'E')
        status = get_text_path(game, line, i, WEST);
    else if (line[i] == 'E' && line[i + 1] == 'A')
        status = get_text_path(game, line, i, EAST);
    else
        status = -1;
    return (status);
}

// ======================================== GET TEXT PATH

int get_text_path(t_game *game, char *line, int i, t_orient orient)
{
    int     status;
    char    *filepath;

    status = 0;
    i += 2;
    if ((orient == NORTH && game->textures.north) ||
        (orient == SOUTH && game->textures.south) ||
        (orient == WEST && game->textures.west) ||
        (orient == EAST && game->textures.east))
        return (-1);
    status = get_text_file_path(line, i, &filepath);
    if (status)
        return (status);
    if (orient == NORTH)
        game->textures.north = filepath; 
    else if (orient == SOUTH)
        game->textures.south = filepath;
    else if (orient == WEST)
        game->textures.west = filepath;
    else if (orient == EAST)
        game->textures.east = filepath;
    return (0);
}

int get_text_file_path(char *line, int i, char **filepath)
{
    int start;
    int end;

    skip_blank(line, &i);
    if (!line[i])
        return (-1);
    start = i;
    while (ft_isprint(line[i]) && line[i] != ' ')
        i++;
    end = i - 1;
    skip_blank(line, &i);
    if (line[i])
        return (-1);
    *filepath = ft_substr(line, start, end - start + 1);
    if (!*filepath)
        return (-1);
    if (check_valid_file(*filepath, ".xpm"))
        return (-1);
    return (0);
}

// ======================================== GET COLORS for CEIL FLOOR

int get_cf_color(t_game *game, char *line, int i, t_orient orient)
{
    uint rgb;
    int status;

    i++;
    rgb = 0;
    status = get_rgb(line, &rgb, i);
    if (status)
        return (-1);
    if ((orient == CEILING && game->textures.ceilbool) ||
        (orient == FLOOR && game->textures.floorbool))
        return (-1);
    else if (orient == CEILING && !game->textures.ceilbool)
    {
        game->textures.hex_ceil = rgb;
        game->textures.ceilbool = true;
    }
    else if (orient == FLOOR && !game->textures.floorbool)
    {
        game->textures.hex_floor = rgb;
        game->textures.floorbool = true;
    }
    return (0);
}

int get_rgb(char *line, uint *rgb, int i)
{
    int status;

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

int get_r(char *line, uint *rgb, int *i)
{
    int count;
    char    *r;
    uint     r_value;

    count = 0;
    skip_blank(line, i);
    while (count < 3 && ft_isdigit(line[*i + count]))
        count++;
    if (count == 0)
        return (-1);
    r = ft_substr(&line[*i], 0, count + 1);
    if (!r)
        return (-1);
    r_value = ft_atoi(r);
    free_str(&r);
    if (r_value > 255)
        return (-1);
    *i += count;
    skip_blank(line, i);
    if (line[*i] != ',')
        return (-1);
    (*i)++;
    *rgb += (r_value << 16);
    return (0);
}

int get_g(char *line, uint *rgb, int *i)
{
    int count;
    char    *g;
    uint     g_value;

    count = 0;
    skip_blank(line, i);
    while (count < 3 && ft_isdigit(line[*i + count]))
        count++;
    if (count == 0)
        return (-1);
    g = ft_substr(&line[*i], 0, count + 1);
    if (!g)
        return (-1);
    g_value = ft_atoi(g);
    free_str(&g);
    if (g_value > 255)
        return (-1);
    *i += count;
    skip_blank(line, i);
    if (line[*i] != ',')
        return (-1);
    (*i)++;
    *rgb += (g_value << 8);
    return (0);
}

int get_b(char *line, uint *rgb, int *i)
{
    int count;
    char    *b;
    uint     b_value;

    count = 0;
    skip_blank(line, i);
    while (count < 3 && ft_isdigit(line[*i + count]))
        count++;
    if (count == 0)
        return (-1);
    b = ft_substr(&line[*i], 0, count + 1);
    if (!b)
        return (-1);
    b_value = ft_atoi(b);
    free_str(&b);
    if (b_value > 255)
        return (-1);
    *i += count;
    skip_blank(line, i);
    if (line[*i])
        return (-1);
    *rgb += b_value;
    return (0);
}

// =========================================== BUILD MAP

int build_map(t_game *game, char **content, int i)
{
    char    **map;

    if (skip_empty_lines(content, &i))
        return (-1);
    map = &content[i];
    if (check_validity(game, map))
        return (-1);
    if (fill_game_map(game, map))
        return (-1);
    if (!valid_nswe_side(game))
        return (-1);
    if (check_flood_fill(game))
        return (-1);
    fill_one(game);
    return (0);
}

int     skip_empty_lines(char **content, int *i)
{
    while (content[*i] && is_empty_line(content[*i]))
        (*i)++;
    if (!content[*i])
        return (-1);
    return (0);
}

bool    is_empty_line(char *line)
{
    int i;

    i = 0;
    skip_blank(line, &i);
    if (!line[i])
        return (true);
    return(false);
}

int     check_validity(t_game *game, char **map)
{
    int i;

    if (check_edge_line(game, map[0]))
        return (-1);
    i = 1;
    while (map[i] && map[i + 1])
    {
        if (check_other_line(game, map, i))
            return (-1);
        i++;
    }
    if (i < 3)
        return (-1);
    if (check_edge_line(game, map[i]))
        return (-1);
    if (!game->player.orientation)
        return (-1);
    game->mapdata.height = i + 1;
    return (0);
}

int check_edge_line(t_game *game, char *line)
{
    int j;

    j = 0;
    while (line[j] && line[j] != '\n')
    {
        if (line[j] != '1' && line[j] != ' ')
            return (-1);
        j++;
    }
    if (!j)
        return (-1);
    if (j > game->mapdata.width)
        game->mapdata.width = j;
    return (0);
}

int check_other_line(t_game *game, char **map, int i)
{
    int j;

    j = 0;
    if (str_contain("0NSWE", map[i][0]))
        return (-1);
    while (map[i][j] && map[i][j] != '\n')
    {
        if (str_contain("NSWE", map[i][j]) && !check_nswe(game, map, i, j))
            return (-1);
        else if (str_contain("NSWE", map[i][j]) && check_nswe(game, map, i, j))
        {
            game->player.orientation = map[i][j];
            game->player.spawn = (t_vec2Di){j, i};
            map[i][j] = '0';
        }
        else if (!str_contain("10 ", map[i][j]))
            return (-1);
        j++;
    }
    if (j > 0 && str_contain("0NSWE", map[i][j - 1]))
        return (-1);
    if (j > game->mapdata.width)
        game->mapdata.width = j;
    return (0);
}

bool    str_contain(char *str, char c)
{
    int i;

    i = -1;
    while (str[++i])
        if (c == str[i])
            return (true);
    return (false);
}

bool    check_nswe(t_game *game, char **map, int i, int j)
{
    if (game->player.orientation ||
        map[i][j + 1] == '\0' || map[i][j + 1] == '\n')
        return (false);
    return (true);
}

int fill_game_map(t_game *game, char **map)
{
    int i;
    int j;
    int height;
    int width;

    height = game->mapdata.height;
    width = game->mapdata.width;
    game->map = ft_calloc(height + 1, sizeof(char *));
    if (!game->map)
        return (-1);
    i = -1;
    while (++i < height)
    {
        game->map[i] = ft_calloc(width + 1, sizeof(char));
        if (!game->map[i])
            return (-1);
        j = -1;
        while (map[i][++j] && map[i][j] != '\n')
            game->map[i][j] = map[i][j];
        while (j < width)
            game->map[i][j++] = ' ';
        game->map[i][j] = '\0';
    }
    game->map[i] = NULL;
    return (0);
}

bool valid_nswe_side(t_game *game)
{
    t_vec2Di    spawn;
    char        **map;

    spawn = game->player.spawn;
    map = game->map;
    return ((map[spawn.y - 1][spawn.x] == '0' ||
            map[spawn.y + 1][spawn.x] == '0' ||
            map[spawn.y][spawn.x - 1] == '0' || 
            map[spawn.y][spawn.x + 1] == '0'));
}

int check_flood_fill(t_game *game)
{
    int x;
    int y;
    int width;
    int height;

    width = game->mapdata.width;
    height = game->mapdata.height;
    x = -1;
    while (++x < width)
    {
        if (flood_fill(game->map, (t_vec2Di){x, 0}, width, height))
            return (-1);
        if (flood_fill(game->map, (t_vec2Di){x, height - 1}, width, height))
            return (-1);
    }
    y = 0;
    while (++y < height - 1)
    {
        if (flood_fill(game->map, (t_vec2Di){0, y}, width, height))
            return (-1);
        if (flood_fill(game->map, (t_vec2Di){width - 1, y}, width, height))
            return (-1);
    }
    return (0);
}

int flood_fill(char **map, t_vec2Di pos, int width, int height)
{
    if (pos.x < 0 || pos.y < 0 || pos.x >= width || pos.y >= height)
        return (0);
    if (str_contain("0NSWE", map[pos.y][pos.x]))
        return (-1);
    if (str_contain("1X", map[pos.y][pos.x]))
        return (0);
    map[pos.y][pos.x] = 'X';
    if (flood_fill(map, (t_vec2Di){pos.x - 1, pos.y}, width, height == -1) ||
        flood_fill(map, (t_vec2Di){pos.x + 1, pos.y}, width, height == -1) ||
        flood_fill(map, (t_vec2Di){pos.x, pos.y - 1}, width, height == -1) ||
        flood_fill(map, (t_vec2Di){pos.x, pos.y + 1}, width, height == -1))
        return (-1);
    return (0);
}

void    fill_one(t_game *game)
{
    char    **map;
    int     i;
    int     j;

    map = game->map;
    i = -1;
    while (map[++i])
    {
        j = -1;
        while (map[i][++j])
        {
            if (map[i][j] == ' ' || map[i][j] == 'X')
                map[i][j] = '1';
        }
    }
}

// ======================================= PLAYER INITIAL POS

void    set_player_start_pos(t_game *game)
{
    game->player.pos.x = (double)game->player.spawn.x + 0.5;
    game->player.pos.y = (double)game->player.spawn.y + 0.5;
    if (game->player.orientation == 'N')
        init_player_north(&game->player);
    else if (game->player.orientation == 'S')
        init_player_south(&game->player);
    else if (game->player.orientation == 'W')
        init_player_west(&game->player);
    else if (game->player.orientation == 'E')
        init_player_east(&game->player);
}

void    init_player_north(t_player *player)
{
    player->dir.x = 0.0;
    player->dir.y = -1.0;
    player->plane.x = -player->dir.y * FOV;
    player->plane.y = player->dir.x * FOV;
}

void    init_player_south(t_player *player)
{
    player->dir.x = 0.0;
    player->dir.y = 1.0;
    player->plane.x = -player->dir.y * FOV;
    player->plane.y = player->dir.x * FOV;
}

void    init_player_west(t_player *player)
{
    player->dir.x = -1.0;
    player->dir.y = 0.0;
    player->plane.x = -player->dir.y * FOV;
    player->plane.y = player->dir.x * FOV;
}

void    init_player_east(t_player *player)
{
    player->dir.x = 1.0;
    player->dir.y = 0.0;
    player->plane.x = -player->dir.y * FOV;
    player->plane.y = player->dir.x * FOV;
}

// ====================================== PLAYER MOVEMENT

int determine_player_movement(t_game *game)
{
    int moved;
    t_vec2D     dir;
    t_vec2Di    move;

    moved = 0;
    dir = game->player.dir;
    move = game->player.move;   
    if (move.y == -1 && move.x == -1)   // Diagonal up left
        moved += move_player(game, (t_vec2D){dir.x + dir.y, dir.y - dir.x});
    else if (move.y == -1 && move.x == 1)   // Diagonal up right
        moved += move_player(game, (t_vec2D){dir.x - dir.y, dir.y + dir.x});
    else if (move.y == 1 && move.x == -1) // Diagonal down left
        moved += move_player(game, (t_vec2D){-dir.x + dir.y, -dir.y - dir.x});
    else if (move.y == 1 && move.x == 1) // Diagonal down right
        moved += move_player(game, (t_vec2D){-dir.x - dir.y, -dir.y + dir.x});
    else if (move.y == -1)  // Up
        moved += move_player(game, (t_vec2D){dir.x, dir.y});
    else if (move.y == 1)   // Down
        moved += move_player(game, (t_vec2D){-dir.x, -dir.y});
    else if (move.x == -1)  // Left
        moved += move_player(game, (t_vec2D){dir.y, -dir.x});
    else if (move.x == 1)   // Right
        moved += move_player(game, (t_vec2D){-dir.y, dir.x});
    return (moved);
}

int move_player(t_game *game, t_vec2D move)
{
    t_vec2D pos;
    t_vec2D new_pos;
    double magnitude;

    pos = game->player.pos;
    magnitude = sqrt(move.x * move.x + move.y * move.y);
    if (magnitude > 0)
    {
        move.x = (move.x / magnitude) * MOVESPEED;
        move.y = (move.y / magnitude) * MOVESPEED;
    }
    new_pos.x = pos.x + move.x;
    new_pos.y = pos.y + move.y;
    return (check_valid_move(game, new_pos));
}

int check_valid_move(t_game *game, t_vec2D new_pos)
{
    
}