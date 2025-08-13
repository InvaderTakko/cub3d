/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:15:48 by sruff             #+#    #+#             */
/*   Updated: 2025/08/13 21:32:12 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
#include <stdint.h>
#include <stdlib.h>

// static int32_t	parse_element(const char *line, t_app *app)
// {
// 	// parse the line for elements like NO, C, F, S, etc
// 	if (ft_strncmp(line, "NO ", 3) == 0)
// 	{
// 		app->map->path = ft_strdup(line + 3);
// 		if (!app->map->path)
// 			exit_with_error("Memory allocation failed for NO path.", app);
// 		app->map->elements_found[0] = true;
// 		return (1);
// 	}
// 	else if (ft_strncmp(line, "SO ", 3) == 0)
// 	{
// 		app->map->elements_found[1] = true;
// 		return (1);
// 	}
// 	else if (ft_strncmp(line, "EA ", 3) == 0)
// 	{
// 		app->map->elements_found[2] = true;
// 		return (1);
// 	}
// 	else if (ft_strncmp(line, "WE ", 3) == 0)
// 	{
// 		app->map->elements_found[3] = true;
// 		return (1);
// 	}
// 	else if (ft_strncmp(line, "F ", 2) == 0)
// 	{
// 		app->map->elements_found[4] = true;
// 		return (1);
// 	}
// 	else if (ft_strncmp(line, "C ", 2) == 0)
// 	{
// 		app->map->elements_found[5] = true;
// 		return (1);
// 	}
// 	return (0); // illegal element
// }

static bool	check_valid_color_format(char *str, t_app *app)
{
	while (*str)
	{
		if (!ft_isdigit(*str) && *str != ',' && !ft_isspace(*str))
		{
			exit_with_error("Invalid RGB values: non-numeric characters found.",
				app);
			return (false);
		}
		str++;
	}
	return (true);
}

static bool	is_valid_color_component(char *str, int32_t *color_component,
		t_app *app)
{
	char	*end;

	*color_component = ft_atoi(str);
	end = str;
	while (*end && ft_isdigit(*end))
		end++;
	while (*end)
	{
		if (!ft_isspace(*end))
		{
			// free_str_array(rgb_values, count);
			exit_with_error("Invalid character after RGB value.", app);
			return (false);
		}
		end++;
	}
	if (*color_component < 0 || *color_component > 255)
	{
		// free_str_array(rgb_values, count);
		exit_with_error("RGB color values must be between 0 and 255.", app);
		return (false);
	}
	return (true);
}

// static char	*trim_line(char *line)
// {
// 	char	*trimmed;
// 	char	*newline;

// 	if (!line)
// 		return (NULL);
// 	trimmed = line;
// 	while (*trimmed && (*trimmed == ' ' || *trimmed == '\t'))
// 		trimmed++;
// 	newline = ft_strchr(trimmed, '\n');
// 	if (newline)
// 		*newline = '\0';
// 	if (*trimmed == '\0')
// 	{
// 		free(line);
// 		return (NULL);
// 	}
// 	return (line);
// }

// static bool	all_elements_found(t_app *app)
// {
// 	int	i;

// 	i = 0;
// 	while (i < ELEMENT_COUNT)
// 	{
// 		if (!app->map->elements_found[i])
// 			return (false);
// 		i++;
// 	}
// 	return (true);
// }
static bool	validate_and_assign_color_component(char *str_val,
		int32_t *color_component, t_app *app)
{
	char	*str;

	str = str_val;
	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	if (*str == '\0')
	{
		// free_str_array(rgb_values, count);
		exit_with_error("Empty value in RGB color specification.", app);
		return (false);
	}
	return (is_valid_color_component(str, color_component, app));
}

static void	assign_color_values(char **rgb_values, int32_t color[3], t_app *app)
{
	int32_t	i;

	i = 0;
	while (i < 3)
	{
		if (!validate_and_assign_color_component(rgb_values[i], &color[i], app))
			return ;
		i++;
	}
}

static void	parse_color(char *line, int32_t color[3], t_app *app)
{
	char	**rgb_values;
	int32_t	count;

	count = 0;
	if (!check_valid_color_format(line, app))
		return ;
	rgb_values = gc_split(line, ',');
	if (!rgb_values)
		exit_with_error("Memory allocation failed for color parsing.", app);
	while (rgb_values[count])
		count++;
	if (count != 3)
	{
		exit_with_error("Invalid RGB color format: must have exactly 3 values.",
			app);
		return ;
	}
	assign_color_values(rgb_values, color, app);
}

static bool	process_texture(char *value, t_app *app, int32_t texture_type,
		char **texture_path_ptr, const char *error_msg)
{
	t_map	*map;

	map = app->map;
	if (map->elements_found[texture_type])
		exit_with_error((char *)error_msg, app);
	if (!validate_texture_file(value))
		exit_with_error("Texture file not found or inaccessible.", app);
	*texture_path_ptr = gc_strdup(value);
	map->elements_found[texture_type] = true;
	return (true);
}

static bool	handle_texture_element(char *key, char *value, t_app *app,
		int32_t texture_type, char **texture_path_ptr)
{
	if (strcmp(key, "NO") == 0 && texture_type == NORTH_TEXTURE)
		return (process_texture(value, app, NORTH_TEXTURE, texture_path_ptr,
				"Duplicate NO texture."));
	else if (strcmp(key, "SO") == 0 && texture_type == SOUTH_TEXTURE)
		return (process_texture(value, app, SOUTH_TEXTURE, texture_path_ptr,
				"Duplicate SO texture."));
	else if (strcmp(key, "WE") == 0 && texture_type == WEST_TEXTURE)
		return (process_texture(value, app, WEST_TEXTURE, texture_path_ptr,
				"Duplicate WE texture."));
	else if (strcmp(key, "EA") == 0 && texture_type == EAST_TEXTURE)
		return (process_texture(value, app, EAST_TEXTURE, texture_path_ptr,
				"Duplicate EA texture."));
	return (false);
}

static bool	handle_color_element(char *key, char *value, t_app *app,
		int32_t color_type, int32_t *color_array)
{
	t_map	*map;

	map = app->map;
	if (strcmp(key, "F") == 0 && color_type == FLOOR_COLOR)
	{
		if (map->elements_found[FLOOR_COLOR])
			exit_with_error("Duplicate F color.", app);
		parse_color(value, color_array, app);
		map->elements_found[FLOOR_COLOR] = true;
		return (true);
	}
	else if (strcmp(key, "C") == 0 && color_type == CEILING_COLOR)
	{
		if (map->elements_found[CEILING_COLOR])
			exit_with_error("Duplicate C color.", app);
		parse_color(value, color_array, app);
		map->elements_found[CEILING_COLOR] = true;
		return (true);
	}
	return (false);
}

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

static bool	process_element_line(t_parse_file_data *file_data, t_app *app)
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

// static void	validate_parsing_results(t_app *app, bool elements_parsed,
// 		int map_height)
// {
// 	if (!elements_parsed)
// 		exit_with_error("Missing one or more map elements.", app);
// 	if (map_height == 0)
// 		exit_with_error("Map grid not found in file.", app);
// }

static void	handle_map_line(char *line, t_app *app, t_map_lines_data *data)
{
	char	**new_temp_map_lines;
	char	*duplicated_line;

	if (*(data->temp_map_height) >= *(data->temp_map_capacity))
	{
		*(data->temp_map_capacity) *= 2;
		new_temp_map_lines = gc_malloc(sizeof(char *)
				* (*(data->temp_map_capacity)));
		if (!new_temp_map_lines)
		{
			// free_str_array(*(data->temp_map_lines),
			// *(data->temp_map_height));
			exit_with_error("Memory allocation failed.", app);
		}
		ft_memcpy(new_temp_map_lines, *(data->temp_map_lines), sizeof(char *)
			* (*(data->temp_map_height)));
		*(data->temp_map_lines) = new_temp_map_lines;
	}
	duplicated_line = gc_strdup(line);
	if (!duplicated_line)
	{
		exit_with_error("Memory allocation failed for map line duplication.",
			app);
	}
	(*(data->temp_map_lines))[(*(data->temp_map_height))++] = duplicated_line;
}

static void	process_map_line(t_parse_file_data *file_data, t_app *app,
		t_map_lines_data *map_data)
{
	handle_map_line(file_data->line, app, map_data);
}

static void	process_file_lines(t_parse_file_data *file_data, t_app *app,
		t_map_lines_data *map_data)
{
	int32_t	consecutive_empty_lines;

	consecutive_empty_lines = 0;
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
			if (file_data->elements_fully_parsed)
			{
				consecutive_empty_lines++;
				if (consecutive_empty_lines > 1)
				{
					free(file_data->line);
					close(file_data->fd);
					exit_with_error("Multiple consecutive empty lines found in map section.", app);
				}
			}
			free(file_data->line);
			continue ;
		}
		consecutive_empty_lines = 0;
		if (!file_data->elements_fully_parsed)
		{
			if (!process_element_line(file_data, app))
				return ;
		}
		else
			process_map_line(file_data, app, map_data);
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
	pad_map_grid(app);
	// validate the map
	validate_map(app);
	// init game map and player pos
	init_player(app);
	return (0);
}
