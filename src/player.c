/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:17:10 by sruff             #+#    #+#             */
/*   Updated: 2025/08/07 14:29:39 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../include/cub3d.h"

static int32_t	is_wall(t_app *app, double x, double y)
{
	int32_t	map_x;
	int32_t	map_y;

	map_x = (int32_t)x;
	map_y = (int32_t)y;
	if (map_x < 0 || map_x >= app->map->grid_width || map_y < 0
		|| map_y >= app->map->grid_height)
		return (1);
	return (app->map->grid[map_y][map_x] == '1');
}

static void	rotate_player(t_player *player, double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = player->dir_x;
	player->dir_x = player->dir_x * cos(angle) - player->dir_y * sin(angle);
	player->dir_y = old_dir_x * sin(angle) + player->dir_y * cos(angle);
	old_plane_x = player->plane_x;
	player->plane_x = player->plane_x * cos(angle) - player->plane_y
		* sin(angle);
	player->plane_y = old_plane_x * sin(angle) + player->plane_y * cos(angle);
}

static void	handle_rotation(t_app *app)
{
	double	rot_speed;

	rot_speed = MOVE_SPEED * 0.7;
	if (app->keys[4]) // left
	{
		rotate_player(&app->player, rot_speed);
	}
	if (app->keys[5]) // right
	{
		rotate_player(&app->player, -rot_speed);
	}
}

static void	move_forward_backward(t_app *app, double move_dir)
{
	double	new_pos_x;
	double	new_pos_y;
	double	move_speed;

	move_speed = MOVE_SPEED;
	new_pos_x = app->player.pos_x + app->player.dir_x * move_speed * move_dir;
	new_pos_y = app->player.pos_y + app->player.dir_y * move_speed * move_dir;
	if (!is_wall(app, new_pos_x, app->player.pos_y))
		app->player.pos_x = new_pos_x;
	if (!is_wall(app, app->player.pos_x, new_pos_y))
		app->player.pos_y = new_pos_y;
}

static void	move_strafe(t_app *app, double strafe_dir)
{
	double	new_pos_x;
	double	new_pos_y;
	double	move_speed;

	move_speed = MOVE_SPEED;
	new_pos_x = app->player.pos_x + app->player.plane_x * move_speed
		* strafe_dir;
	new_pos_y = app->player.pos_y + app->player.plane_y * move_speed
		* strafe_dir;
	if (!is_wall(app, new_pos_x, app->player.pos_y))
		app->player.pos_x = new_pos_x;
	if (!is_wall(app, app->player.pos_x, new_pos_y))
		app->player.pos_y = new_pos_y;
}

void	handle_movement(t_app *app)
{
	handle_rotation(app);
	if (app->keys[0]) // W
		move_forward_backward(app, 1.0);
	if (app->keys[1]) // S
		move_forward_backward(app, -1.0);
	if (app->keys[2]) // A
		move_strafe(app, -1.0);
	if (app->keys[3]) // D
		move_strafe(app, 1.0);
}

void	movehook(void *param)
{
	t_app *app;

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