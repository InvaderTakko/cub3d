/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_walls.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:45:30 by sruff             #+#    #+#             */
/*   Updated: 2025/08/24 18:55:29 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

bool	flood_fill_recursive(char **grid, t_flood_fill_data *data)
{
	t_flood_fill_data	next_data;

	next_data = *data;
	next_data.x = data->x + 1;
	if (!flood_fill(grid, &next_data))
		return (false);
	next_data = *data;
	next_data.x = data->x - 1;
	if (!flood_fill(grid, &next_data))
		return (false);
	next_data = *data;
	next_data.y = data->y + 1;
	if (!flood_fill(grid, &next_data))
		return (false);
	next_data = *data;
	next_data.y = data->y - 1;
	if (!flood_fill(grid, &next_data))
		return (false);
	return (true);
}

bool	flood_fill(char **grid, t_flood_fill_data *data)
{
	if (data->x < 0 || data->x >= data->width || data->y < 0
		|| data->y >= data->height || grid[data->y][data->x] == ' ')
		return (false);
	if (grid[data->y][data->x] == '1' || grid[data->y][data->x] == 'F')
		return (true);
	grid[data->y][data->x] = 'F';
	return (flood_fill_recursive(grid, data));
}

void	check_walls_enclosed(t_app *app)
{
	char				**map_copy;
	t_map				*map;
	t_flood_fill_data	ff_data;

	map = app->map;
	map_copy = str_array_dup(map->grid, map->grid_height);
	if (!map_copy)
		exit_with_error("Failed to create map copy for validation.", app);
	ff_data.x = map->player_start_x;
	ff_data.y = map->player_start_y;
	ff_data.width = map->grid_width;
	ff_data.height = map->grid_height;
	if (!flood_fill(map_copy, &ff_data))
		exit_with_error("Map is not enclosed by walls.", app);
}
