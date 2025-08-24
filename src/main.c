/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:14:56 by sruff             #+#    #+#             */
/*   Updated: 2025/08/24 18:55:03 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	main_loop_hook(void *param)
{
	t_app	*app;

	app = param;
	movehook(app);
	render(app);
}

int32_t	main(int32_t argc, char **argv)
{
	t_app	*app;

	gc_init();
	app = gc_malloc(sizeof(t_app));
	ft_memset(app, 0, sizeof(t_app));
	if (argc != 2)
	{
		ft_printf("Usage: %s <path to map_file>\n", argv[0]);
		return (1);
	}
	ft_printf("Welcome to Cub3D!\n");
	if (parse_map(app, argv[1]) != 0)
	{
		ft_printf("Error: Failed to parse map file\n");
		return (1);
	}
	app->window_width = 1024;
	app->window_height = 768;
	mlx_setup(app);
	mlx_loop_hook(app->mlx, main_loop_hook, app);
	mlx_loop(app->mlx);
	cleanup_mlx(app);
	gc_free_all();
	return (0);
}
// system("leaks cub3d");