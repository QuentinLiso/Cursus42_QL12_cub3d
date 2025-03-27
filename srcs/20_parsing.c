/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   20_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:50:54 by qliso             #+#    #+#             */
/*   Updated: 2025/03/27 17:25:21 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int parse_args(t_game *game, char **av)
{
    int status;

    status = check_valid_file(av[1], ".cub");
    if (status)
        clean_c3d_exit(game, status);
    status = store_file_data(game, av[1]);
    if (status)
        clean_c3d_exit(game, status);
    status = parse_filecontent(game);
    if (status)
        clean_c3d_exit(game, status);
    set_player_start_pos(game);
    return (0);
}

// ========================================== Parse args : check valid file

int check_valid_file(char *av, char *ext)
{
    int fd;
    
    if (check_file_isdir(av))
        return (perror_c3d(EDIR));
    fd = open(av, O_RDONLY);
    if (fd < 0)
        return (perror_c3d(EOPEN));
    close(fd);
    if (!check_file_ext(av, ext))
        return (perror_c3d(EFILEEXT));
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
    if (get_line_count(filepath, &game->mapdata.line_count))
        return (EOPEN);
    game->mapdata.filepath = filepath;
    game->mapdata.filecontent = ft_calloc(game->mapdata.line_count + 1,
        sizeof(char *));
    if (!game->mapdata.filecontent)
        return (perror_c3d(EMALLOC));
    game->mapdata.fd = open(filepath, O_RDONLY);
    if (game->mapdata.fd < 0)
        return (perror_c3d(EOPEN));
    if (fill_filecontent(game))
    {
        close (game->mapdata.fd);
        return (EMALLOC);
    }
    close (game->mapdata.fd);
    return (0);
}

int get_line_count(char *file, int *map_line_count)
{
    int     fd;
    int     line_count;
    char    *line;

    line_count = 0;
    fd = open(file, O_RDONLY);
    if (fd < 0)
        return (perror_c3d(EOPEN));
    line = get_next_line(fd);
    while (line)
    {
        line_count++;
        free_str(&line);
        line = get_next_line(fd);
    }
    free_str(&line);
    close(fd);
    *map_line_count = line_count;
    return (0);
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
            return (perror_c3d(EMALLOC));
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
    while (content[i] && tex_and_colors_empty(game))
    {
        status = parse_line_tex(game, content[i++]);
        if (status)
            return (perror_c3d(status));
    }
    status = tex_and_colors_empty(game);
    if (status)
        return (perror_c3d(status));
    status = build_map(game, content, i);
    if (status)
        return (perror_c3d(status));
    return (0);
}

// ====================================== PARSE TEXTURES AND COLORS LINES

int    tex_and_colors_empty(t_game *game)
{
    t_tex   *texs;

    texs = game->texs;
    if (!texs[NORTH].filled)
        return (EMISSINGTEXN);
    if (!texs[SOUTH].filled)
        return (EMISSINGTEXS);
    if (!texs[WEST].filled)
        return (EMISSINGTEXW);
    if (!texs[EAST].filled)
        return (EMISSINGTEXE);
    if (!texs[FLOOR].filled)
        return (EMISSINGCOLF);
    if (!texs[CEILING].filled)
        return (EMISSINGCOLC);
    return (0);
}

void    missing_tex_msg(t_game *game)
{
    t_tex   *texs;

    texs = game->texs;
    if (!texs[NORTH].filled)
        perror_c3d(EMISSINGTEXN);
    if (!texs[SOUTH].filled)
        perror_c3d(EMISSINGTEXS);
    if (!texs[WEST].filled)
        perror_c3d(EMISSINGTEXW);
    if (!texs[EAST].filled)
        perror_c3d(EMISSINGTEXE);
    if (!texs[FLOOR].filled)
        perror_c3d(EMISSINGCOLF);
    if (!texs[CEILING].filled)
        perror_c3d(EMISSINGCOLC);
}

int parse_line_tex(t_game *game, char *line)
{
    int i;
    int status;

    i = 0;
    status = 0;
    skip_blank(line, &i);
    if (!line[i])
        return (0);
    if (!ft_isprint(line[i]))
        return (perror_c3d(EWRONGCHAR));
    status = get_texture_and_colors(game, line, i);
    if (status)
        missing_tex_msg(game);
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
        status = EWRONGCHAR;
    return (status);
}

// ======================================== GET TEXT PATH

int get_text_path(t_game *game, char *line, int i, t_orient orient)
{
    int     status;
    char    *filepath;

    status = 0;
    i += 2;
    if (game->texs[orient].filled)
        return (EDOUBLETEX);
    status = get_text_file_path(line, i, &filepath);
    if (status)
        return (status);
    game->texs[orient].path = filepath;
    game->texs[orient].filled = true;
    return (0);
}

int get_text_file_path(char *line, int i, char **filepath)
{
    int start;
    int end;

    skip_blank(line, &i);
    if (!line[i])
        return (EMISSINGTEX);
    start = i;
    while (ft_isprint(line[i]) && line[i] != ' ')
        i++;
    end = i - 1;
    skip_blank(line, &i);
    if (line[i])
        return (EPATHTEX);
    *filepath = ft_substr(line, start, end - start + 1);
    if (!*filepath)
        return (EMALLOC);
    if (check_valid_file(*filepath, ".xpm"))
        return (EFILEEXT);
    return (0);
}

// ======================================== GET COLORS for CEIL FLOOR

int get_cf_color(t_game *game, char *line, int i, t_orient orient)
{
    uint rgb;
    int status;

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

int get_rgb(char *line, t_uint *rgb, int i)
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

int get_r(char *line, t_uint *rgb, int *i)
{
    int count;
    char    *r;
    t_uint     r_value;

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

int get_g(char *line, t_uint *rgb, int *i)
{
    int count;
    char    *g;
    t_uint     g_value;

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

int get_b(char *line, t_uint *rgb, int *i)
{
    int count;
    char    *b;
    t_uint     b_value;

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

// =========================================== BUILD MAP

int build_map(t_game *game, char **content, int i)
{
    char    **map;
    int     status;

    status = skip_empty_lines(content, &i);
    if (status)
        return (status);
    map = &content[i];
    status = check_validity(game, map);
    if (status)
        return (status);
    status = fill_game_map(game, map);
    if (status)
        return (status);
    status = valid_nswe_side(game);
    if (status)
        return (status);
    status = check_flood_fill(game);
    if (status)
        return (status);
    fill_one(game);
    return (0);
}

int     skip_empty_lines(char **content, int *i)
{
    while (content[*i] && is_empty_line(content[*i]))
        (*i)++;
    if (!content[*i])
        return (EEMPTYMAP);
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
    int status;

    status = check_edge_line(game, map[0]);
    if (status)
        return (status);
    i = 1;
    while (map[i] && map[i + 1])
    {
        status = check_other_line(game, map, i);
        if (status)
            return (status);
        i++;
    }
    if (i < 3)
        return (EMAPTOOSMALL);
    status = check_edge_line(game, map[i]);
    if (status)
        return (status);
    if (!game->player.orientation)
        return (EMISSINGORIENT);
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
            return (ECHAREDGE);
        j++;
    }
    if (!j)
        return (EEMPTYMAP);
    if (j > game->mapdata.width)
        game->mapdata.width = j;
    return (0);
}

int check_other_line(t_game *game, char **map, int i)
{
    int j;
    int status;

    j = 0;
    if (map[i][0] == '0' || map[i][0] == 'D')
        return (EVOIDONSIDE);
    status = check_other_line_loop(game, map, i, &j);
    if (status)
        return (status);
    if (j > 0 && str_contain("0NSWED", map[i][j - 1]))
        return (EVOIDONSIDE);
    if (j > game->mapdata.width)
        game->mapdata.width = j;
    return (0);
}

int     check_other_line_loop(t_game *game, char **map, int i, int *j)
{
    int status;

    while (map[i][*j] && map[i][*j] != '\n')
    {
        if (str_contain("NSWE", map[i][*j]))
        {
            status = valid_nswe(game, map, i, *j);
            if (status)
                return (status);
        }
        else if (!str_contain("10D ", map[i][*j]))
            return (ECHARMAP);
        (*j)++;
    }
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

int     valid_nswe(t_game *game, char **map, int i, int j)
{
    if (game->player.orientation)
        return (EDOUBLEORIENT);
    if (j == 0 || map[i][j + 1] == '\0' || map[i][j + 1] == '\n')
        return (EORIENTONSIDE);
    game->player.orientation = map[i][j];
    game->player.spawn = (t_vec2Di){j, i};
    map[i][j] = '0';
    return (0);
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
        return (EMALLOC);
    i = -1;
    while (++i < height)
    {
        game->map[i] = ft_calloc(width + 1, sizeof(char));
        if (!game->map[i])
            return (EMALLOC);
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

int valid_nswe_side(t_game *game)
{
    t_vec2Di    spawn;
    char        **map;

    spawn = game->player.spawn;
    map = game->map;
    if ((map[spawn.y - 1][spawn.x] == '0' ||
            map[spawn.y + 1][spawn.x] == '0' ||
            map[spawn.y][spawn.x - 1] == '0' || 
            map[spawn.y][spawn.x + 1] == '0'))
        return (0);
    return (EPLAYERNOSPACE);
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
            return (EMISSINGWALLS);
        if (flood_fill(game->map, (t_vec2Di){x, height - 1}, width, height))
            return (EMISSINGWALLS);
    }
    y = 0;
    while (++y < height - 1)
    {
        if (flood_fill(game->map, (t_vec2Di){0, y}, width, height))
            return (EMISSINGWALLS);
        if (flood_fill(game->map, (t_vec2Di){width - 1, y}, width, height))
            return (EMISSINGWALLS);
    }
    return (0);
}

int flood_fill(char **map, t_vec2Di pos, int width, int height)
{
    if (pos.x < 0 || pos.y < 0 || pos.x >= width || pos.y >= height)
        return (0);
    if (str_contain("0NSWED", map[pos.y][pos.x]))
        return (EMISSINGWALLS);
    if (str_contain("1X", map[pos.y][pos.x]))
        return (0);
    map[pos.y][pos.x] = 'X';
    if (flood_fill(map, (t_vec2Di){pos.x - 1, pos.y}, width, height) ||
        flood_fill(map, (t_vec2Di){pos.x + 1, pos.y}, width, height) ||
        flood_fill(map, (t_vec2Di){pos.x, pos.y - 1}, width, height) ||
        flood_fill(map, (t_vec2Di){pos.x, pos.y + 1}, width, height))
        return (EMISSINGWALLS);
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
