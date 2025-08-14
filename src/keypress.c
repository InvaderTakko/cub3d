/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keypress.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:16:04 by sruff             #+#    #+#             */
/*   Updated: 2025/08/14 16:34:19 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	movehook(void *param)
{
	t_app	*app;

	app = (t_app *)param;
	if (mlx_is_key_down(app->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(app->mlx);
	app->keys[0] = mlx_is_key_down(app->mlx, MLX_KEY_W);
	app->keys[1] = mlx_is_key_down(app->mlx, MLX_KEY_S);
	app->keys[2] = mlx_is_key_down(app->mlx, MLX_KEY_A);
	app->keys[3] = mlx_is_key_down(app->mlx, MLX_KEY_D);
	app->keys[4] = mlx_is_key_down(app->mlx, MLX_KEY_LEFT);
	app->keys[5] = mlx_is_key_down(app->mlx, MLX_KEY_RIGHT);
}
