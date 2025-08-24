/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_dda.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:34:56 by stefan           #+#    #+#             */
/*   Updated: 2025/08/14 16:34:56 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	step_one_cell(t_dda_params *params)
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
}

static int32_t	is_oob(const t_app *app, const t_dda_params *params)
{
	return (*(params->map_x) < 0 || *(params->map_y) < 0
		|| *(params->map_x) >= app->map->grid_width
		|| *(params->map_y) >= app->map->grid_height);
}

void	execute_dda(t_app *app, t_dda_params *params)
{
	while (*(params->hit) == 0)
	{
		step_one_cell(params);
		if (is_oob(app, params))
			break ;
		if (app->map->grid[*(params->map_y)][*(params->map_x)] == '1')
			*(params->hit) = 1;
	}
}

void	init_dda_params(t_dda_params *params, t_ray_data *ray_data)
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
