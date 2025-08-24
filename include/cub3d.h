/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:17:10 by sruff             #+#    #+#             */
/*   Updated: 2025/08/24 16:40:19 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include "../MLX42/include/MLX42/MLX42.h"
# include "../libft/ft_printf_bonus.h"
# include "../libft/get_next_line.h"
# include "../libft/libft.h"
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

// # define ELEMENT_COUNT 6
# define MOVE_SPEED 0.1

typedef enum e_element
{
	NORTH_TEXTURE,
	SOUTH_TEXTURE,
	WEST_TEXTURE,
	EAST_TEXTURE,
	FLOOR_COLOR,
	CEILING_COLOR,
	ELEMENT_COUNT
}						t_element;

// Garbage Collector
typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}						t_gc_node;

typedef struct s_gc
{
	t_gc_node			*first;
	t_gc_node			*last;
	size_t				len;
}						t_gc;

// cub3d
typedef struct s_map
{
	char				**grid;
	int32_t				grid_width;
	int32_t				grid_height;
	char				*north_texture_path;
	char				*south_texture_path;
	char				*west_texture_path;
	char				*east_texture_path;
	int32_t				floor_color[3];
	int32_t				ceiling_color[3];
	uint8_t				elements_found[ELEMENT_COUNT];
	int32_t				player_start_x;
	int32_t				player_start_y;
	int8_t				player_start_dir;
}						t_map;

typedef struct s_player
{
	double				pos_x;
	double				pos_y;
	double				dir_x;
	double				dir_y;
	double				plane_x;
	double				plane_y;
}						t_player;

typedef struct s_images
{
	mlx_texture_t *txt_no; // Texture for North wall
	mlx_texture_t *txt_so; // Texture for South wall
	mlx_texture_t *txt_ea; // Texture for East wall
	mlx_texture_t *txt_we; // Texture for West wall
	mlx_image_t			*screen;
}						t_images;

typedef struct s_app
{
	mlx_t				*mlx;
	mlx_image_t			*image;
	t_map				*map;
	t_player			player;
	t_images			*img;
	bool keys[6]; // W, S, A, D, LEFT, RIGHT
	int32_t				window_width;
	int32_t				window_height;
}						t_app;

// parsing
typedef struct s_map_lines_data
{
	char				***temp_map_lines;
	int32_t				*temp_map_height;
	int32_t				*temp_map_capacity;
}						t_map_lines_data;

typedef struct s_parse_file_data
{
	int32_t				fd;
	char				*line;
	char				*trimmed_line;
	char				*newline;
	bool				elements_fully_parsed;
	int32_t				i;
	bool				all_found;
}						t_parse_file_data;

// flood fill
typedef struct s_flood_fill_data
{
	int32_t				x;
	int32_t				y;
	int32_t				width;
	int32_t				height;
}						t_flood_fill_data;

// garbage collector
t_gc					*get_gc_addr(void);
t_gc_node				*gc_add_node(void *address);
void					gc_add_start(void *ptr);
void					*gc_malloc(size_t size);
char					**gc_split(char const *s, char c);

t_gc					*gc_init(void);
void					gc_free_all(void);

// utils
char					*ft_strpbrk(const char *s, const char *charset);
char					*ft_strstr(const char *haystack, const char *needle);
int32_t					exit_with_error(const char *message, t_app *app);
char					*gc_strdup(const char *s);
int32_t					ft_isspace(int32_t c);
int32_t					validate_texture_file(const char *path);
char					**str_array_dup(char **src, int32_t height);

// mlx_setup

void					mlx_setup(t_app *app);
void					cleanup_mlx(t_app *app);
void					load_all_textures(t_app *app);
void					delete_textures(t_images *img);
// map parsing
int32_t					parse_map(t_app *app, const char *file);

// padding of grid
void					pad_map_grid(t_app *app);

// validate map
void					validate_map(t_app *app);

// check_walls
void					check_walls_enclosed(t_app *app);
bool					flood_fill(char **grid, t_flood_fill_data *data);
bool					flood_fill_recursive(char **grid,
							t_flood_fill_data *data);

// init_player
void					init_player(t_app *app);
void					movehook(void *param);

// move_player
void					handle_movement(t_app *app);

// render
void					render(t_app *app);
void					draw_frame(t_app *app);
int32_t					get_rgba(int32_t r, int32_t g, int32_t b, int32_t a);

typedef struct s_dda_params
{
	double				ray_dir_x;
	double				ray_dir_y;
	int32_t				*map_x;
	int32_t				*map_y;
	double				*side_dist_x;
	double				*side_dist_y;
	double				delta_dist_x;
	double				delta_dist_y;
	int32_t				*step_x;
	int32_t				*step_y;
	int32_t				*hit;
	int32_t				*side;
}						t_dda_params;

typedef struct s_wall_params
{
	double				ray_dir_x;
	double				ray_dir_y;
	int32_t				map_x;
	int32_t				map_y;
	int32_t				side;
	int32_t				step_x;
	int32_t				step_y;
	double				*perp_wall_dist;
	int32_t				*line_height;
	int32_t				*draw_start;
	int32_t				*draw_end;
}						t_wall_params;

typedef struct s_texture_params
{
	double				perp_wall_dist;
	double				ray_dir_x;
	double				ray_dir_y;
	int32_t				side;
	int32_t				*tex_num;
	double				*wall_x;
	int32_t				*tex_x;
	double				*tex_step;
	double				*tex_pos;
	int32_t				draw_start;
	int32_t				line_height;
}						t_texture_params;

typedef struct s_vertical_strip_params
{
	int32_t				x;
	int32_t				draw_start;
	int32_t				draw_end;
	int32_t				tex_num;
	int32_t				tex_x;
	double				tex_step;
	double				tex_pos;
	int32_t				side;
}						t_vertical_strip_params;

typedef struct s_ray_data
{
	double				camera_x;
	double				ray_dir_x;
	double				ray_dir_y;
	int32_t				map_x;
	int32_t				map_y;
	double				side_dist_x;
	double				side_dist_y;
	double				delta_dist_x;
	double				delta_dist_y;
	double				perp_wall_dist;
	int32_t				step_x;
	int32_t				step_y;
	int32_t				hit;
	int32_t				side;
	int32_t				line_height;
	int32_t				draw_start;
	int32_t				draw_end;
	int32_t				tex_num;
	double				wall_x;
	int32_t				tex_x;
	double				tex_step;
	double				tex_pos;
}						t_ray_data;

void					process_ray(t_app *app, int32_t x,
							t_ray_data *ray_data);

// render_dda.c
void					init_dda(t_app *app, t_dda_params *params);
void					execute_dda(t_app *app, t_dda_params *params);
void					init_dda_params(t_dda_params *params,
							t_ray_data *ray_data);

// render_texture.c
uint32_t				get_pixel(mlx_texture_t *texture, int32_t x, int32_t y);
uint32_t				get_texture_color(t_app *app, int32_t tex_num,
							int32_t tex_x, int32_t tex_y);
int32_t					get_texture_num(double ray_dir_x, double ray_dir_y,
							int32_t side);
void					calculate_texture_params(t_app *app,
							t_texture_params *params);
void					init_texture_params(t_texture_params *params,
							t_ray_data *ray_data);

// render_walls.c
void					calculate_wall_params(t_app *app,
							t_wall_params *params);
void					init_wall_params(t_wall_params *params,
							t_ray_data *ray_data);
void					draw_vertical_strip(t_app *app,
							t_vertical_strip_params *params);
void					init_strip_params(t_vertical_strip_params *params,
							int32_t x, t_ray_data *ray_data);
void					calculate_and_draw_wall(t_app *app, int32_t x,
							t_ray_data *ray_data);

// parse_colors.c
void					parse_color(char *line, int32_t color[3], t_app *app);

// parse_textures.c
bool					handle_texture_element(char *key, char *value,
							t_app *app, int32_t texture_type,
							char **texture_path_ptr);
bool					handle_color_element(char *key, char *value, t_app *app,
							int32_t color_type, int32_t *color_array);

// parse_file.c
void					process_file_lines(t_parse_file_data *file_data,
							t_app *app, t_map_lines_data *map_data);

// parsing.c
bool					process_element_line(t_parse_file_data *file_data,
							t_app *app);

#endif
