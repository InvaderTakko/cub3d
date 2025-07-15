/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:05:25 by sruff             #+#    #+#             */
/*   Updated: 2025/07/15 19:58:45 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
#include <stdint.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

void	init_img(t_app *app)
{
	app->img->screen = mlx_new_image(app->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!app->img->screen || !app->img->screen->pixels)
		exit_with_error("Failed to alloc screen image", app);
	mlx_image_to_window(app->mlx, app->img->screen, 0, 0);
}

static void	delete_textures(t_images *img)
{
	if (img->txt_no)
		mlx_delete_texture(img->txt_no);
	if (img->txt_so)
		mlx_delete_texture(img->txt_so);
	if (img->txt_ea)
		mlx_delete_texture(img->txt_ea);
	if (img->txt_we)
		mlx_delete_texture(img->txt_we);
}

static void	load_north_texture(t_app *app)
{
	app->img->txt_no = mlx_load_png(app->map->north_texture_path);
	if (!app->img->txt_no)
	{
		delete_textures(app->img);
		exit_with_error("Failed to load north texture.", app);
	}
}

static void	load_south_texture(t_app *app)
{
	app->img->txt_so = mlx_load_png(app->map->south_texture_path);
	if (!app->img->txt_so)
	{
		delete_textures(app->img);
		exit_with_error("Failed to load south texture.", app);
	}
}

static void	load_east_texture(t_app *app)
{
	app->img->txt_ea = mlx_load_png(app->map->east_texture_path);
	if (!app->img->txt_ea)
	{
		delete_textures(app->img);
		exit_with_error("Failed to load east texture.", app);
	}
}

static void	load_west_texture(t_app *app)
{
	app->img->txt_we = mlx_load_png(app->map->west_texture_path);
	if (!app->img->txt_we)
	{
		delete_textures(app->img);
		exit_with_error("Failed to load west texture.", app);
	}
}

static void	load_all_textures(t_app *app)
{
	load_north_texture(app);
	load_south_texture(app);
	load_east_texture(app);
	load_west_texture(app);
}

void	mlx_setup(t_app *app)
{
	app->img = gc_malloc(sizeof(t_images));
	if (!app->img)
	{
		exit_with_error("Memory allocation failed for images.", app);
	}
	ft_bzero(app->img, sizeof(t_images));
	app->mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "Cub3D", true);
	if (!app->mlx)
	{
		exit_with_error("MLX initialization failed.", app);
	}
	load_all_textures(app);
	init_img(app);
}
