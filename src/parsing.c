/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:15:48 by sruff             #+#    #+#             */
/*   Updated: 2025/08/14 16:40:22 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
#include <stdint.h>
#include <stdlib.h>

static bool	element_handler(char *line, char *value, t_app *app)
{
	t_map	*map;

	map = app->map;
	if (handle_texture_element(line, value, app, NORTH_TEXTURE,
			&map->north_texture_path))
		return (true);
	if (handle_texture_element(line, value, app, SOUTH_TEXTURE,
			&map->south_texture_path))
		return (true);
	if (handle_texture_element(line, value, app, WEST_TEXTURE,
			&map->west_texture_path))
		return (true);
	if (handle_texture_element(line, value, app, EAST_TEXTURE,
			&map->east_texture_path))
		return (true);
	if (handle_color_element(line, value, app, FLOOR_COLOR, map->floor_color))
		return (true);
	if (handle_color_element(line, value, app, CEILING_COLOR,
			map->ceiling_color))
		return (true);
	return (false);
}

static bool	parse_element(char *line, t_app *app)
{
	char	*value;
	char	*newline;

	while (*line && (*line == ' ' || *line == '\t'))
		line++;
	newline = ft_strchr(line, '\n');
	if (newline)
		*newline = '\0';
	if (*line == '\0')
		return (true);
	value = ft_strpbrk(line, " \t");
	if (!value)
		return (false);
	*value++ = '\0';
	while (*value && (*value == ' ' || *value == '\t'))
		value++;
	return (element_handler(line, value, app));
}

bool	process_element_line(t_parse_file_data *file_data, t_app *app)
{
	if (parse_element(file_data->trimmed_line, app))
	{
		file_data->all_found = true;
		file_data->i = 0;
		while (file_data->i < ELEMENT_COUNT)
		{
			if (!app->map->elements_found[file_data->i])
			{
				file_data->all_found = false;
				break ;
			}
			file_data->i++;
		}
		if (file_data->all_found)
			file_data->elements_fully_parsed = true;
		return (true);
	}
	else
	{
		free(file_data->line);
		close(file_data->fd);
		exit_with_error("Invalid line or missing elements before map grid.",
			app);
		return (false);
	}
}

static void	read_and_parse_file(const char *filename, t_app *app)
{
	t_parse_file_data	file_data;
	t_map_lines_data	map_data;
	char				**temp_map_lines;
	int32_t				temp_map_height;
	int32_t				temp_map_capacity;

	temp_map_lines = NULL;
	temp_map_height = 0;
	temp_map_capacity = 10;
	temp_map_lines = gc_malloc(sizeof(char *) * temp_map_capacity);
	if (!temp_map_lines)
		exit_with_error("Memory allocation failed for map lines.", app);
	map_data.temp_map_lines = &temp_map_lines;
	map_data.temp_map_height = &temp_map_height;
	map_data.temp_map_capacity = &temp_map_capacity;
	file_data.elements_fully_parsed = false;
	file_data.fd = open(filename, O_RDONLY);
	if (file_data.fd < 0)
		exit_with_error("Could not open map file.", NULL);
	process_file_lines(&file_data, app, &map_data);
	close(file_data.fd);
	if (!file_data.elements_fully_parsed)
		exit_with_error("Missing one or more map elements in file.", app);
	if (temp_map_height == 0)
		exit_with_error("Map grid not found in file.", app);
	app->map->grid = temp_map_lines;
	app->map->grid_height = temp_map_height;
}

int32_t	parse_map(t_app *app, const char *file)
{
	app->map = gc_malloc(sizeof(t_map));
	if (!app->map)
	{
		exit_with_error("Memory allocation for map failed", app);
	}
	ft_memset(app->map, 0, sizeof(t_map));
	if (!ft_strstr(file, ".cub"))
		exit_with_error("Map file must have a .cub extension.", NULL);
	read_and_parse_file(file, app);
	pad_map_grid(app);
	validate_map(app);
	init_player(app);
	return (0);
}
