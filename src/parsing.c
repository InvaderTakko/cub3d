/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:15:48 by sruff             #+#    #+#             */
/*   Updated: 2025/07/05 20:05:00 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
#include <stdint.h>

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

static void	read_and_parse_file(const char *filename, t_app *app)
{
	int		fd;
	char	*line;
	char	**map_lines;
	int		map_height;
	bool	elements_parsed;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_with_error("Could not open map file.", app);
	map_lines = gc_malloc(sizeof(char *) * 100);
	if (!map_lines)
		exit_with_error("Memory allocation failed.", app);
	map_height = 0;
	elements_parsed = false;
	ft_printf("Starting to parse file: %s\n", filename); // Add debug
	while ((line = get_next_line(fd)))
	{
		line = trim_line(line);
		if (!line || *line == '\0')
			continue ;
		ft_printf("Processing line: '%s'\n", line); // Add debug
		if (!elements_parsed)
		{
			if (parse_element(line, app))
			{
				ft_printf("Element parsed successfully\n"); // Add debug
				if (all_elements_found(app))
				{
					elements_parsed = true;
					ft_printf("All elements found!\n"); // Add debug
				}
			}
			else
				exit_with_error("Invalid element or missing elements.", app);
		}
		else
		{
			map_lines[map_height++] = gc_strdup(line);
			ft_printf("Added map line: %s\n", line); // Add debug
		}
		free(line);
	}
	close(fd);
	ft_printf("Finished parsing. Elements parsed: %d, Map height: %d\n",
		elements_parsed, map_height); // Add debug
	validate_parsing_results(app, elements_parsed, map_height);
	app->map->grid = map_lines;
	app->map->grid_height = map_height;
}

int32_t	parse_map(t_app *app, const char *file)
{
	app->map = gc_malloc(sizeof(t_map));
	if (!app->map)
	{
		ft_printf("Error: Memory allocation for map failed\n");
		return (1);
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