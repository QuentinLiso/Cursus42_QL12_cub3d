/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   50_rendering.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qliso <qliso@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:54:11 by qliso             #+#    #+#             */
/*   Updated: 2025/03/27 18:50:18 by qliso            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int     update_render(t_game *game)
{
    game->player.has_moved += set_player_movement(game);
    game->player.has_moved += set_player_rotation(game);
    if (!game->player.has_moved && !game->player.mouse_rotate)
        return (0);
    update_render_frame(game);
    game->player.mouse_rotate = 0;
    return (0);
}

void    update_render_frame(t_game *game)
{
    init_draw_pixels(game);
    init_raycast(&game->ray);
    raycast_algo(game);
    draw_crosshair(game);
    draw_minimap(game);
    put_frame_to_screen(game);
}

// ========================================= RENDERING : INIT

void    init_draw_pixels(t_game *game)
{
    t_draw  *draw;
    int i;

    draw = &game->draw;
    if (draw->pixels)
    {
        i = -1;
        while (++i < game->win_height)
            free(draw->pixels[i]);
        free(draw->pixels);
    }
    draw->pixels = ft_calloc(game->win_height, sizeof(int *));
    if (!draw->pixels)
        clean_c3d_exit(game, perror_c3d(EMALLOC));
    i = -1;
    while (++i < game->win_height)
    {
        draw->pixels[i] = ft_calloc(game->win_width, sizeof(int));
        if (!draw->pixels[i])
            clean_c3d_exit(game, perror_c3d(EMALLOC));
    }
}

void    init_raycast(t_raycast *ray)
{
    ray->camera_x = 0.0;
    ray->dir = (t_vec2D){0.0, 0.0};
    ray->map = (t_vec2Di){0, 0};
    ray->step = (t_vec2Di){0, 0};
    ray->sidedist = (t_vec2D){0.0, 0.0};
    ray->deltadist = (t_vec2D){0.0, 0.0};
    ray->wall_dist = 0.0;
    ray->wall_x = 0.0;
    ray->side = 0;
    ray->line_height = 0;
    ray->draw_start = 0;
    ray->draw_end = 0;
    ray->collide = '\0';
}

// ========================================= RENDERING : RAYCAST

void    raycast_algo(t_game *game)
{
    t_raycast   *ray;
    t_player    *player;
    int         x;

    ray = &game->ray;
    player = &game->player;
    x = -1;
    while (++x < game->win_width)
    {
        init_player_raycast(ray, player, x);
        init_dda_raycast(ray, player);
        launch_dda_raycast(ray, game);
        set_line_height(ray, player, game);
        set_draw_pixels(game, ray, x);
    }
}

void    init_player_raycast(t_raycast *ray, t_player *player, int x)
{
    init_raycast(ray);
    ray->camera_x = 2 * x / (double)WIDTH - 1;
    ray->dir.x = player->dir.x + player->plane.x * ray->camera_x;
    ray->dir.y = player->dir.y + player->plane.y * ray->camera_x;
    ray->map.x = (int)player->pos.x;
    ray->map.y = (int)player->pos.y;
    if (ray->dir.x == 0)
        ray->deltadist.x = INFINITY;
    else
        ray->deltadist.x = (fabs(1 / ray->dir.x));
    if (ray->dir.y == 0)
        ray->deltadist.y = INFINITY;
    else
        ray->deltadist.y = (fabs(1 / ray->dir.y));
}

void    init_dda_raycast(t_raycast *ray, t_player *player)
{
    if (ray->dir.x < 0)
    {
        ray->step.x = -1;
        ray->sidedist.x = (player->pos.x - ray->map.x) * ray->deltadist.x;
    }
    else
    {
        ray->step.x = 1;
        ray->sidedist.x =
            (ray->map.x + 1.0 - player->pos.x) * ray->deltadist.x;
    }
    if (ray->dir.y < 0)
    {
        ray->step.y = -1;
        ray->sidedist.y = (player->pos.y - ray->map.y) * ray->deltadist.y;
    }
    else
    {
        ray->step.y = 1;
        ray->sidedist.y =
            (ray->map.y + 1.0 - player->pos.y) * ray->deltadist.y;
    }
}

void    launch_dda_raycast(t_raycast *ray, t_game *game)
{
    while (1)
    {
        if (ray->sidedist.x < ray->sidedist.y)
        {
            ray->sidedist.x += ray->deltadist.x;
            ray->map.x += ray->step.x;
            ray->side = 0;
        }
        else
        {
            ray->sidedist.y += ray->deltadist.y;
            ray->map.y += ray->step.y;
            ray->side = 1;
        }
        if (ray_collision(ray, game))
            break ;
    }
}

bool    ray_collision(t_raycast *ray, t_game *game)
{
    if (collide_boundaries_i(game, ray->map))
    {
        ray->collide = '1';
        return (true);
    }
    if (collide_walls_i(game, ray->map))
    {
        ray->collide = game->map[ray->map.y][ray->map.x];
        return (true);
    }
    return (false);
}

bool    collide_boundaries_i(t_game *game, t_vec2Di pos)
{
    return (pos.x < 1 || pos.x >= game->mapdata.width ||
        pos.y < 1 || pos.y >= game->mapdata.height);
}

bool    collide_walls_i(t_game *game, t_vec2Di pos)
{
    return (game->map[pos.y][pos.x] != '0');
}

void    set_line_height(t_raycast *ray, t_player *player, t_game *game)
{
    if (ray->side == 0)
    {
        ray->wall_dist = (ray->sidedist.x - ray->deltadist.x);
        ray->wall_x = player->pos.y + ray->wall_dist * ray->dir.y;
    }
    else
    {
        ray->wall_dist = (ray->sidedist.y - ray->deltadist.y);
        ray->wall_x = player->pos.x + ray->wall_dist * ray->dir.x;
    }
    ray->wall_x -= floor(ray->wall_x);
    ray->line_height = (int)(game->win_height / ray->wall_dist);
    ray->draw_start = (game->win_height / 2) - (ray->line_height / 2);
    ray->draw_end = (game->win_height / 2) + (ray->line_height / 2);
    if (ray->draw_start < 0)
        ray->draw_start = 0;
    if (ray->draw_end >= game->win_height)
        ray->draw_end = game->win_height - 1;
}

void    set_draw_pixels(t_game *game, t_raycast *ray, int x)
{
    t_draw  *draw;

    draw = &game->draw;
    init_draw(draw, game, ray);
    fill_draw_pixels(draw, game, ray, x);
}

void    init_draw(t_draw *draw, t_game *game, t_raycast *ray)
{
    draw->orient = get_texture_orientation(ray);
    draw->step = 1.0 * BLOCK / ray->line_height;
    draw->pos = (ray->draw_start - game->win_height / 2 
        + ray->line_height / 2) * draw->step;
    draw->pixel.x = (int)(ray->wall_x * BLOCK);
    if ((ray->side == 0 && ray->dir.x < 0) ||
        (ray->side == 1 && ray->dir.y > 0))
        draw->pixel.x = BLOCK - 1 - draw->pixel.x;
}

t_orient    get_texture_orientation(t_raycast *ray)
{
    if (ray->collide == 'D')
        return (DOOR);
    if (ray->side == 0)
    {
        if (ray->dir.x > 0)
            return (EAST);
        else
            return (WEST);
    }
    else
    {
        if (ray->dir.y > 0)
            return (NORTH);
        else
            return (SOUTH);
    }
}

void    fill_draw_pixels(t_draw *draw, t_game *game, t_raycast *ray, int x)
{
    int y;
    int color;
    t_tex   tex;

    y = ray->draw_start - 1;
    tex = game->texs[draw->orient];
    while (++y < ray->draw_end)
    {
        draw->pixel.y = (int)draw->pos & (BLOCK - 1);
        draw->pos += draw->step;
        color = tex.pixels[BLOCK * draw->pixel.y + draw->pixel.x];
        if (color > 0)
            draw->pixels[y][x] = color;
    }
}

// ========================================= RENDERING : CROSSHAIR + MINIMAP

void    draw_crosshair(t_game *game)
{
    t_draw  *draw;
    t_vec2Di    start;
    t_vec2Di    end;
 
    draw = &game->draw;
    start.x = game->win_width / 2 - CROSSHAIR / 2;
    start.y = game->win_height / 2 - CROSSHAIR / 2;
    end.x = game->win_width / 2 + CROSSHAIR / 2;
    end.y = game->win_height / 2 + CROSSHAIR / 2;
    draw_square(draw, start, end, 0x5CB338);
}

void    draw_square(t_draw *draw, t_vec2Di start, t_vec2Di end, int color)
{
    int x;
    int y;

    y = start.y - 1;
    while (++y < end.y)
    {
        x = start.x - 1;
        while (++x < end.x)
            draw->pixels[y][x] = color;
    }
}

void    draw_minimap(t_game *game)
{
    t_minimap   *mmap;
    int         color;

    mmap = &game->mmap;
    get_minimap_submap(game);
    mmap->pos.y = -1;
    while (++mmap->pos.y < MINIMAP_H)
    {
        mmap->pos.x = -1;
        while (++mmap->pos.x < MINIMAP_W)
        {
            if (mmap->map[mmap->pos.y][mmap->pos.x] == '1')
                color = 0x2C3930;
            else if (mmap->map[mmap->pos.y][mmap->pos.x] == 'D')
                color = 0xA1E3F9;
            else
                color = 0xA27B5C;
            if (mmap->pos.y == MINIMAP_H / 2 && mmap->pos.x == MINIMAP_W / 2)
                color = 0xFB4141;
            draw_minimap_tile(&game->mmap, mmap->pos, &mmap->img, color);
        }
    }
    draw_minimap_orient(mmap);
}

void    get_minimap_submap(t_game *game)
{
    t_vec2Di    map_start;
    t_vec2Di    map_pos;
    t_minimap   *mmap;

    mmap = &game->mmap;
    map_start.x = (int)game->player.pos.x - MINIMAP_W / 2;
    map_start.y = (int)game->player.pos.y - MINIMAP_H / 2;
    mmap->pos.y = -1;
    while (++mmap->pos.y < MINIMAP_H)
    {
        mmap->pos.x = -1;
        while (++mmap->pos.x < MINIMAP_W)
        {
            map_pos.x = map_start.x + mmap->pos.x;
            map_pos.y = map_start.y + mmap->pos.y;
            if (is_in_map(map_pos, game))
                mmap->map[mmap->pos.y][mmap->pos.x]
                    = game->map[map_pos.y][map_pos.x];
            else
                mmap->map[mmap->pos.y][mmap->pos.x] = '1';
        }
    }
}

bool    is_in_map(t_vec2Di pos, t_game *game)
{
    int height;
    int width;

    height = game->mapdata.height;
    width = game->mapdata.width;
    return (pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height);
}

void    draw_minimap_tile(t_minimap *mmap, t_vec2Di tile, t_img *img, int color)
{
    int x;
    int y;
    t_vec2Di    start;
    t_vec2Di    pixel;

    start.x = tile.x * mmap->tile_size;
    start.y = tile.y * mmap->tile_size;
    y = -1;
    while (++y < mmap->tile_size)
    {
        x = -1;
        while (++x < mmap->tile_size)
        {
            pixel.x = start.x + x;
            pixel.y = start.y + y;
            put_pixel_to_img(img, pixel, color);
        }
    }
}

void    draw_minimap_orient(t_minimap *mmap)
{
    t_vec2Di    player_pos;
    t_vec2Di    ray_end;

    player_pos.x = MINIMAP_W / 2 * mmap->tile_size + mmap->tile_size / 2 ;
    player_pos.y = MINIMAP_H / 2 * mmap->tile_size + mmap->tile_size / 2 ;
    ray_end.x = player_pos.x + (int)(mmap->player->dir.x * mmap->player_ray);
    ray_end.y = player_pos.y + (int)(mmap->player->dir.y * mmap->player_ray);
    draw_line(&mmap->img, player_pos, ray_end, 0xFB4141);
}

void    draw_line(t_img *img, t_vec2Di start, t_vec2Di end, int color)
{
    t_line  line;

    init_line(&line, start, end, 2);
    while (start.x != end.x || start.y != end.y)
    {
        put_pixel_to_img(img, start, color);
        draw_thickness(img, &line, start, color);
        bresenham_next_pixel(&line, &start);
    }
}

void    init_line(t_line *line, t_vec2Di start, t_vec2Di end, int thickness)
{
    float   magnitude;

    line->delta.x = abs(end.x - start.x);
    line->delta.y = abs(end.y - start.y);
    if (start.x < end.x)
        line->step.x = 1;
    else
        line->step.x = -1;
    if (start.y < end.y)
        line->step.y = 1;
    else
        line->step.y = -1;
    line->error = line->delta.x - line->delta.y;
    if (thickness > 0)
        line->thickness = thickness;
    else
        line->thickness = 1;
    magnitude = sqrt(line->delta.x * line->delta.x + line->delta.y * line->delta.y);
    line->perp_thick.x = (int)round(-line->delta.y / magnitude);
    line->perp_thick.y = (int)round(line->delta.x / magnitude);
}

void    draw_thickness(t_img *img, t_line *line, t_vec2Di start, int color)
{
    int         thickness_loop;
    int         i;
    t_vec2Di    pixel;
    
    thickness_loop = line->thickness / 2 + 1;
    i = 0;
    while (++i < thickness_loop)
    {
        pixel.x = start.x + line->perp_thick.x * i;
        pixel.y = start.y + line->perp_thick.y * i;
        if (pixel.x < img->width && pixel.y < img->height)
            put_pixel_to_img(img, pixel, color);
        pixel.x = start.x - line->perp_thick.x * i;
        pixel.y = start.y - line->perp_thick.y * i;
        if (pixel.x >= 0 && pixel.y >= 0)
            put_pixel_to_img(img, pixel, color);
    }
}

void    bresenham_next_pixel(t_line *line, t_vec2Di *start)
{
    line->error_2 = line->error * 2;
    if (line->error_2 > -line->delta.y)
    {
        line->error -= line->delta.y;
        start->x += line->step.x;
    }
    if(line->error_2 < line->delta.x)
    {
        line->error += line->delta.x;
        start->y += line->step.y;
    }
}

// ========================================= RENDERING : UPDATE FRAME

void    put_frame_to_screen(t_game *game)
{
    t_img   img;
    t_vec2Di pixel;

    init_img(game, &img, game->win_width, game->win_height);
    pixel.y = -1;
    while (++pixel.y < game->win_height)
    {
        pixel.x = -1;
        while (++pixel.x < game->win_width)
            update_frame_pixel(game, &img, pixel);
    }
    put_minimap_to_frame(&img, &game->mmap);
    mlx_put_image_to_window(game->mlx, game->win, img.img, 0, 0);
    mlx_destroy_image(game->mlx, img.img);
}

void    init_img(t_game *game, t_img *img, int width, int height)
{
    init_empty_img(img);
    img->img = mlx_new_image(game->mlx, width, height); 
    if (!img->img)
        clean_c3d_exit(game, perror_c3d(EMLXIMG));
    img->addr = (int *)mlx_get_data_addr(img->img, &img->bpp,
            &img->size_line, &img->endian);
    img->width = width;
    img->height = height;
}

void    update_frame_pixel(t_game *game, t_img *img, t_vec2Di pixel)
{
    t_draw  *draw;

    draw = &game->draw;
    if (draw->pixels[pixel.y][pixel.x] > 0)
        put_pixel_to_img(img, pixel, draw->pixels[pixel.y][pixel.x]);
    else if (pixel.y < game->win_height / 2)
        put_pixel_to_img(img, pixel, game->texs[CEILING].color);
    else
        put_pixel_to_img(img, pixel, game->texs[FLOOR].color);
}

void    put_pixel_to_img(t_img *img, t_vec2Di coord, int color)
{
    int pixel;
    int pixels_per_row;

    pixels_per_row = img->size_line / sizeof(int);
    pixel = coord.y * pixels_per_row + coord.x;
    img->addr[pixel] = color;
}

int     get_pixel_from_img(t_img *img, t_vec2Di coord)
{
    int pixel;
    int pixels_per_row;
    
    pixels_per_row = img->size_line / sizeof(int);
    pixel = coord.y * pixels_per_row + coord.x;
    return (((int *)img->addr)[pixel]);
}

void    put_minimap_to_frame(t_img *img, t_minimap *mmap)
{
    int x;
    int y;
    int color;

    y = -1;
    while (++y < MINIMAP_H * 8)
    {
        x = -1;
        while (++x < MINIMAP_W * 8)
        {
            color = get_pixel_from_img(&mmap->img, (t_vec2Di){x, y});
            put_pixel_to_img(img, (t_vec2Di){x, y}, color);
        }
    }
}

