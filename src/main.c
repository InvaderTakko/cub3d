/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:14:56 by sruff             #+#    #+#             */
/*   Updated: 2025/08/07 15:33:41 by sruff            ###   ########.fr       */
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
	t_app *app;

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
		// gc_free_all();
		return (1);
	}
	// open and parse map
	mlx_setup(app);


	// key/render loops
	mlx_loop_hook(app->mlx, main_loop_hook, app);
	mlx_loop(app->mlx);
	gc_free_all();
	return (0);
}