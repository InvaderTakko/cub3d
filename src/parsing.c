/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:15:48 by sruff             #+#    #+#             */
/*   Updated: 2025/08/24 22:41:20 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
#include <stdint.h>
#include <stdlib.h>

static bool	element_handler(char *line, char *value, t_app *app)
{
	t_color_element_args	ca;

	if (handle_texture_elements(line, value, app))
		return (true);
	ca = (t_color_element_args){line, value, app, FLOOR_COLOR,
		app->map->floor_color};
	if (handle_color_element(&ca))
		return (true);
	ca.color_type = CEILING_COLOR;
	ca.color_array = app->map->ceiling_color;
	return (handle_color_element(&ca));
}

bool	parse_element(char *line, t_app *app)
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
