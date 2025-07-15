/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:15:48 by sruff             #+#    #+#             */
/*   Updated: 2025/07/15 16:03:23 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
#include <stdint.h>
#include <stdlib.h>

static int32_t	parse_element(const char *line, t_app *app)
{
	// parse the line for elements like NO, C, F, S, etc
	if (ft_strncmp(line, "NO ", 3) == 0)
	{
		app->map->path = ft_strdup(line + 3);
		if (!app->map->path)
			exit_with_error("Memory allocation failed for NO path.", app);
		app->map->elements_found[0] = true;
		return (1);
	}
	else if (ft_strncmp(line, "SO ", 3) == 0)
	{
		app->map->elements_found[1] = true;
		return (1);
	}
	else if (ft_strncmp(line, "EA ", 3) == 0)
	{
		app->map->elements_found[2] = true;
		return (1);
	}
	else if (ft_strncmp(line, "WE ", 3) == 0)
	{
		app->map->elements_found[3] = true;
		return (1);
	}
	else if (ft_strncmp(line, "F ", 2) == 0)
	{
		app->map->elements_found[4] = true;
		return (1);
	}
	else if (ft_strncmp(line, "C ", 2) == 0)
	{
		app->map->elements_found[5] = true;
		return (1);
	}
	return (0); // illegal element
}

static char	*trim_line(char *line)
{
	char	*trimmed;
	char	*newline;

	if (!line)
		return (NULL);
	trimmed = line;
	while (*trimmed && (*trimmed == ' ' || *trimmed == '\t'))
		trimmed++;
	newline = ft_strchr(trimmed, '\n');
	if (newline)
		*newline = '\0';
	if (*trimmed == '\0')
	{
		free(line);
		return (NULL);
	}
	return (line);
}

static bool	all_elements_found(t_app *app)
{
	int	i;

	i = 0;
	while (i < ELEMENT_COUNT)
	{
		if (!app->map->elements_found[i])
			return (false);
		i++;
	}
	return (true);
}

static void	validate_parsing_results(t_app *app, bool elements_parsed,
		int map_height)
{
	if (!elements_parsed)
		exit_with_error("Missing one or more map elements.", app);
	if (map_height == 0)
		exit_with_error("Map grid not found in file.", app);
}
static void	process_file_lines(t_parse_file_data *file_data, t_app *app,
		t_map_lines_data *map_data)
{
	while ((file_data->line = get_next_line(file_data->fd)))
	{
		file_data->trimmed_line = file_data->line;
		while (*file_data->trimmed_line && (*file_data->trimmed_line == ' '
				|| *file_data->trimmed_line == '\t'))
			file_data->trimmed_line++;
		file_data->newline = ft_strchr(file_data->trimmed_line, '\n');
		if (file_data->newline)
			*file_data->newline = '\0';
		if (*file_data->trimmed_line == '\0')
		{
			free(file_data->line);
			continue ;
		}
		if (!file_data->elements_fully_parsed)
		{
			// if (!process_element_line(file_data, app))
			return ;
		}
		// else
		// 	process_map_line(file_data, app, map_data);
		free(file_data->line);
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
	// pad the grid of the map
	// validate the map
	// init game map and player pos
	return (0);
}