/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:17:10 by sruff             #+#    #+#             */
/*   Updated: 2025/07/05 16:12:22 by sruff            ###   ########.fr       */
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


typedef struct s_app
{
	mlx_t		*mlx;
	mlx_image_t	*image;
	char		*map_path;
	int32_t			map_width;
	int32_t			map_height;
	char		**map_data;
} t_app;

int32_t mlx_setup(t_app *app);

#endif