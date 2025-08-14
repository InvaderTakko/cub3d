/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:15:15 by sruff             #+#    #+#             */
/*   Updated: 2025/08/14 16:37:05 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"


void	process_ray(t_app *app, int32_t x, t_ray_data *ray_data)
{
	t_dda_params	dda_params;

	ray_data->camera_x = 2 * x / (double)app->window_width - 1;
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
