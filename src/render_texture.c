/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:35:16 by stefan           #+#    #+#             */
/*   Updated: 2025/08/14 16:35:16 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

uint32_t	get_pixel(mlx_texture_t *texture, int32_t x, int32_t y)
{
	int32_t	pos;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;

	pos = (y * texture->width + x) * 4;
	r = texture->pixels[pos];
	g = texture->pixels[pos + 1];
	b = texture->pixels[pos + 2];
	a = texture->pixels[pos + 3];
	return (get_rgba(r, g, b, a));
}

uint32_t	get_texture_color(t_app *app, int32_t tex_num, int32_t tex_x,
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

int32_t	get_texture_num(double ray_dir_x, double ray_dir_y, int32_t side)
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

void	calculate_texture_params(t_app *app, t_texture_params *params)
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
	*(params->tex_pos) = (params->draw_start - app->window_height / 2
			+ params->line_height / 2) * (*(params->tex_step));
}

void	init_texture_params(t_texture_params *params, t_ray_data *ray_data)
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
