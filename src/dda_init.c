/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 22:36:53 by stefan           #+#    #+#             */
/*   Updated: 2025/08/24 22:36:53 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	init_dda_x_axis(t_app *app, t_dda_params *params)
{
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
}

static void	init_dda_y_axis(t_app *app, t_dda_params *params)
{
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

void	init_dda(t_app *app, t_dda_params *params)
{
	*(params->map_x) = (int32_t)app->player.pos_x;
	*(params->map_y) = (int32_t)app->player.pos_y;
	init_dda_x_axis(app, params);
	init_dda_y_axis(app, params);
}
