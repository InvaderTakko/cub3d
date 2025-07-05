/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:15:48 by sruff             #+#    #+#             */
/*   Updated: 2025/07/05 17:47:43 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int32_t parse_map(t_app *app, const char *path)
{
	app->map = gc_malloc(sizeof(t_map));
	if (!app->map)
	{
		ft_printf("Error: Memory allocation for map failed\n");
		return (1);
	}
	ft_memset(app->map, 0, sizeof(t_map));
	app->map->path = ft_strdup(path);
	if (!app->map->path)
	{
		ft_printf("Error: Memory allocation for map path failed\n");
		free(app->map);
		return (1);
	}
	return (0);
}