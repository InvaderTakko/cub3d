/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:15:15 by sruff             #+#    #+#             */
/*   Updated: 2025/08/07 15:22:56 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static uint32_t	get_pixel(mlx_texture_t *texture, int32_t x, int32_t y)
{
	int32_t	pos;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;

	pos = (y * texture->width + x) * 4; // 4 bits per pixel
	r = texture->pixels[pos];
	g = texture->pixels[pos + 1];
	b = texture->pixels[pos + 2];
	a = texture->pixels[pos + 3];
	return (get_rgba(r, g, b, a));
}

static uint32_t	get_texture_color(t_app *app, int32_t tex_num, int32_t tex_x,
		int32_t tex_y)
{
	if (tex_num == 0)
		return (get_pixel(app->img->txt_no, tex_x, tex_y));
	else if (tex_num == 1)
		return (get_pixel(app->img->txt_so, tex_x, tex_y));
	else if (tex_num == 2)
		return (get_pixel(app->img->txt_ea, tex_x, tex_y));
	else
		return (get_pixel(app->img->txt_we, tex_x, tex_y));
}

static int32_t	get_texture_num(double ray_dir_x, double ray_dir_y,
		int32_t side)
{
	if (side == 0)
	{
		if (ray_dir_x > 0)
			return (2);
		else
			return (3);
	}
	else
	{
		if (ray_dir_y > 0)
			return (1);
		else
			return (0);
	}
}

static void	init_dda(t_app *app, t_dda_params *params)
{
	*(params->map_x) = (int32_t)app->player.pos_x;
	*(params->map_y) = (int32_t)app->player.pos_y;
	if (params->ray_dir_x < 0)
	{
		*(params->step_x) = -1;
		*(params->side_dist_x) = (app->player.pos_x - *(params->map_x))
			* params->delta_dist_x;
	}
	else
	{
		*(params->step_x) = 1;
		*(params->side_dist_x) = (*(params->map_x) + 1.0 - app->player.pos_x)
			* params->delta_dist_x;
	}
	if (params->ray_dir_y < 0)
	{
		*(params->step_y) = -1;
		*(params->side_dist_y) = (app->player.pos_y - *(params->map_y))
			* params->delta_dist_y;
	}
	else
	{
		*(params->step_y) = 1;
		*(params->side_dist_y) = (*(params->map_y) + 1.0 - app->player.pos_y)
			* params->delta_dist_y;
	}
}


static void	execute_dda(t_app *app, t_dda_params *params)
{
	while (*(params->hit) == 0)
	{
		if (*(params->side_dist_x) < *(params->side_dist_y))
		{
			*(params->side_dist_x) += params->delta_dist_x;
			*(params->map_x) += *(params->step_x);
			*(params->side) = 0;
		}
		else
		{
			*(params->side_dist_y) += params->delta_dist_y;
			*(params->map_y) += *(params->step_y);
			*(params->side) = 1;
		}
		if (*(params->map_x) < 0 || *(params->map_y) < 0
			|| *(params->map_x) >= app->map->grid_width
			|| *(params->map_y) >= app->map->grid_height)
			break ;
		if (app->map->grid[*(params->map_y)][*(params->map_x)] == '1')
			*(params->hit) = 1;
	}
}

// helper function for calculating wall distance and line height
static void	calculate_wall_params(t_app *app, t_wall_params *params)
{
	double	raw_height;
	int32_t	half;

	if (params->side == 0)
		*(params->perp_wall_dist) = (params->map_x - app->player.pos_x + (1
					- params->step_x) / 2) / params->ray_dir_x;
	else
		*(params->perp_wall_dist) = (params->map_y - app->player.pos_y + (1
					- params->step_y) / 2) / params->ray_dir_y;
	// clamp before casting to int32 to avoid overflow
	raw_height = WINDOW_HEIGHT / *params->perp_wall_dist;
	if (raw_height > INT32_MAX)
		raw_height = INT32_MAX;
	*params->line_height = (int32_t)raw_height;
	// calc half-height first then offset from center
	half = *params->line_height / 2;
	*params->draw_start = WINDOW_HEIGHT / 2 - half;
	if (*params->draw_start < 0)
		*params->draw_start = 0;
	*params->draw_end = WINDOW_HEIGHT / 2 + half;
	if (*params->draw_end >= WINDOW_HEIGHT)
		*params->draw_end = WINDOW_HEIGHT - 1;
}

// Helper function for texture mapping
static void	calculate_texture_params(t_app *app, t_texture_params *params)
{
	*(params->tex_num) = get_texture_num(params->ray_dir_x, params->ray_dir_y,
			params->side);
	if (params->side == 0)
		*(params->wall_x) = app->player.pos_y + params->perp_wall_dist
			* params->ray_dir_y;
	else
		*(params->wall_x) = app->player.pos_x + params->perp_wall_dist
			* params->ray_dir_x;
	*(params->wall_x) -= floor(*(params->wall_x));
	*(params->tex_x) = (int32_t)(*(params->wall_x) * 64.0);
	if (params->side == 0 && params->ray_dir_x > 0)
		*(params->tex_x) = 64 - *(params->tex_x) - 1;
	if (params->side == 1 && params->ray_dir_y < 0)
		*(params->tex_x) = 64 - *(params->tex_x) - 1;
	*(params->tex_step) = 1.0 * 64 / params->line_height;
	*(params->tex_pos) = (params->draw_start - WINDOW_HEIGHT / 2
			+ params->line_height / 2) * (*(params->tex_step));
}


static void	draw_vertical_strip(t_app *app, t_vertical_strip_params *params)
{
	int32_t		y;
	int32_t		tex_y;
	uint32_t	color;

	y = params->draw_start;
	while (y < params->draw_end)
	{
		tex_y = (int32_t)params->tex_pos & (64 - 1);
		params->tex_pos += params->tex_step;
		color = get_texture_color(app, params->tex_num, params->tex_x, tex_y);
		if (params->side == 1)
			color = (color >> 1) & 0x7F7F7F; // darken side walls
		mlx_put_pixel(app->img->screen, params->x, y, color);
		y++;
	}
}

static void	init_wall_params(t_wall_params *params, t_ray_data *ray_data)
{
	params->ray_dir_x = ray_data->ray_dir_x;
	params->ray_dir_y = ray_data->ray_dir_y;
	params->map_x = ray_data->map_x;
	params->map_y = ray_data->map_y;
	params->side = ray_data->side;
	params->step_x = ray_data->step_x;
	params->step_y = ray_data->step_y;
	params->perp_wall_dist = &ray_data->perp_wall_dist;
	params->line_height = &ray_data->line_height;
	params->draw_start = &ray_data->draw_start;
	params->draw_end = &ray_data->draw_end;
}

static void	init_texture_params(t_texture_params *params, t_ray_data *ray_data)
{
	params->perp_wall_dist = ray_data->perp_wall_dist;
	params->ray_dir_x = ray_data->ray_dir_x;
	params->ray_dir_y = ray_data->ray_dir_y;
	params->side = ray_data->side;
	params->tex_num = &ray_data->tex_num;
	params->wall_x = &ray_data->wall_x;
	params->tex_x = &ray_data->tex_x;
	params->tex_step = &ray_data->tex_step;
	params->tex_pos = &ray_data->tex_pos;
	params->draw_start = ray_data->draw_start;
	params->line_height = ray_data->line_height;
}

static void	init_strip_params(t_vertical_strip_params *params, int32_t x,
		t_ray_data *ray_data)
{
	params->x = x;
	params->draw_start = ray_data->draw_start;
	params->draw_end = ray_data->draw_end;
	params->tex_num = ray_data->tex_num;
	params->tex_x = ray_data->tex_x;
	params->tex_step = ray_data->tex_step;
	params->tex_pos = ray_data->tex_pos;
	params->side = ray_data->side;
}

static void	calculate_and_draw_wall(t_app *app, int32_t x, t_ray_data *ray_data)
{
	t_wall_params			wall_params;
	t_texture_params		texture_params;
	t_vertical_strip_params	strip_params;

	init_wall_params(&wall_params, ray_data);
	calculate_wall_params(app, &wall_params);
	init_texture_params(&texture_params, ray_data);
	calculate_texture_params(app, &texture_params);
	init_strip_params(&strip_params, x, ray_data);
	draw_vertical_strip(app, &strip_params);
}

static void	init_dda_params(t_dda_params *params, t_ray_data *ray_data)
{
	params->ray_dir_x = ray_data->ray_dir_x;
	params->ray_dir_y = ray_data->ray_dir_y;
	params->map_x = &ray_data->map_x;
	params->map_y = &ray_data->map_y;
	params->side_dist_x = &ray_data->side_dist_x;
	params->side_dist_y = &ray_data->side_dist_y;
	params->delta_dist_x = ray_data->delta_dist_x;
	params->delta_dist_y = ray_data->delta_dist_y;
	params->step_x = &ray_data->step_x;
	params->step_y = &ray_data->step_y;
	params->hit = &ray_data->hit;
	params->side = &ray_data->side;
}

void	process_ray(t_app *app, int32_t x, t_ray_data *ray_data)
{
	t_dda_params	dda_params;

	ray_data->camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
	ray_data->ray_dir_x = app->player.dir_x + app->player.plane_x
		* ray_data->camera_x;
	ray_data->ray_dir_y = app->player.dir_y + app->player.plane_y
		* ray_data->camera_x;
	ray_data->delta_dist_x = fabs(1 / ray_data->ray_dir_x);
	ray_data->delta_dist_y = fabs(1 / ray_data->ray_dir_y);
	ray_data->hit = 0;
	init_dda_params(&dda_params, ray_data);
	init_dda(app, &dda_params);
	execute_dda(app, &dda_params);
	if (ray_data->hit == 0)
		return ;
	calculate_and_draw_wall(app, x, ray_data);
}

void	render(t_app *app)
{
	handle_movement(app);
	draw_frame(app);
}