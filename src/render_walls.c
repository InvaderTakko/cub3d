/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:35:37 by stefan           #+#    #+#             */
/*   Updated: 2025/08/14 16:35:37 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	calculate_wall_params(t_app *app, t_wall_params *params)
{
	double	raw_height;
	int32_t	half;

	if (params->side == 0)
		*(params->perp_wall_dist) = (params->map_x - app->player.pos_x + (1
					- params->step_x) / 2) / params->ray_dir_x;
	else
		*(params->perp_wall_dist) = (params->map_y - app->player.pos_y + (1
					- params->step_y) / 2) / params->ray_dir_y;
	raw_height = app->window_height / *params->perp_wall_dist;
	if (raw_height > INT32_MAX)
		raw_height = INT32_MAX;
	*params->line_height = (int32_t)raw_height;
	half = *params->line_height / 2;
	*params->draw_start = app->window_height / 2 - half;
	if (*params->draw_start < 0)
		*params->draw_start = 0;
	*params->draw_end = app->window_height / 2 + half;
	if (*params->draw_end >= app->window_height)
		*params->draw_end = app->window_height - 1;
}

void	init_wall_params(t_wall_params *params, t_ray_data *ray_data)
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

void	draw_vertical_strip(t_app *app, t_vertical_strip_params *params)
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
			color = (color >> 1) & 0x7F7F7F;
		mlx_put_pixel(app->img->screen, params->x, y, color);
		y++;
	}
}

void	init_strip_params(t_vertical_strip_params *params, int32_t x,
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

void	calculate_and_draw_wall(t_app *app, int32_t x, t_ray_data *ray_data)
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
