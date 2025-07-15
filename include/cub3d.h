/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:17:10 by sruff             #+#    #+#             */
/*   Updated: 2025/07/15 16:51:02 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <math.h>
# include <stdbool.h>
# include <stdint.h>
# include "../MLX42/include/MLX42/MLX42.h"
# include "../libft/libft.h"
# include "../libft/ft_printf_bonus.h"
# include "../libft/get_next_line.h"

// # define ELEMENT_COUNT 6 

typedef enum e_element
{
    NORTH_TEXTURE,
    SOUTH_TEXTURE,
    WEST_TEXTURE,
    EAST_TEXTURE,
    FLOOR_COLOR,
    CEILING_COLOR,
    ELEMENT_COUNT
}           t_element;

// Garbage Collector
typedef struct s_gc_node
{
    void    *ptr;
    struct s_gc_node    *next;
}    t_gc_node;

typedef struct s_gc
{
    t_gc_node    *first;
    t_gc_node    *last;
    size_t        len;
}    t_gc;

//cub3d
typedef struct s_map
{
	char	**grid;
	int32_t		grid_width;
	int32_t		grid_height;
	char		*north_texture_path;
	char		*south_texture_path;
	char		*west_texture_path;
	char		*east_texture_path;
	int32_t		floor_color[3];
	int32_t		ceiling_color[3];
	uint8_t		elements_found[ELEMENT_COUNT];
	int32_t		player_start_x;
	int32_t		player_start_y;
	int8_t		player_start_dir;
} t_map;

typedef struct s_app
{
	mlx_t		*mlx;
	mlx_image_t	*image;
	t_map		*map;
} t_app;

// parsing
typedef struct s_map_lines_data
{
	char	***temp_map_lines;
	int32_t	*temp_map_height;
	int32_t	*temp_map_capacity;
}	t_map_lines_data;

typedef struct s_parse_file_data
{
	int32_t		fd;
	char		*line;
	char		*trimmed_line;
	char		*newline;
	bool		elements_fully_parsed;
	int32_t		i;
	bool		all_found;
}	t_parse_file_data;

//garbage collector
t_gc    *get_gc_addr(void);
t_gc_node    *gc_add_node(void *address);
void    gc_add_start(void *ptr);
void    *gc_malloc(size_t size);

t_gc    *gc_init(void);
void    gc_free_all(void);

//utils
char	*ft_strpbrk(const char *s, const char *charset);
char    *ft_strstr(const char *haystack, const char *needle);
int32_t    exit_with_error(const char *message, t_app *app);
char    *gc_strdup(const char *s);
int32_t	ft_isspace(int32_t c);
int32_t	validate_texture_file(const char *path);

// mlx_setup
int32_t mlx_setup(t_app *app);

// map parsing
int32_t parse_map(t_app *app, const char *file);
#endif