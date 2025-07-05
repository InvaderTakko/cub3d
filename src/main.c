/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:14:56 by sruff             #+#    #+#             */
/*   Updated: 2025/07/05 17:46:03 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

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

	// open and parse map
	mlx_setup(app);
	// img innit

	// key/ render loops
	gc_free_all();
	return (0);
}