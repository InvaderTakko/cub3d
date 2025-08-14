/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:37:45 by stefan           #+#    #+#             */
/*   Updated: 2025/08/14 16:37:45 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

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

bool	handle_texture_element(char *key, char *value, t_app *app,
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

bool	handle_color_element(char *key, char *value, t_app *app,
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
