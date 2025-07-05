/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:17:10 by sruff             #+#    #+#             */
/*   Updated: 2025/07/05 17:46:21 by sruff            ###   ########.fr       */
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
	char	*path;
	char	**data;
	int32_t	width;
	int32_t	height;
} t_map;

typedef struct s_app
{
	mlx_t		*mlx;
	mlx_image_t	*image;
	t_map		*map;
} t_app;

//garbage collector
t_gc    *get_gc_addr(void);
t_gc_node    *gc_add_node(void *address);
void    gc_add_start(void *ptr);
void    *gc_malloc(size_t size);
t_gc    *gc_init(void);
void    gc_free_all(void);


//mlx
int32_t mlx_setup(t_app *app);

#endif