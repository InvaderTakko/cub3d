/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:05:25 by sruff             #+#    #+#             */
/*   Updated: 2025/07/05 16:34:34 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int32_t mlx_setup(t_app *app)
{
    app->mlx = mlx_init(800, 600, "Cub3D", true);
    if (!app->mlx)
    {
        ft_printf("Error: MLX initialization failed\n");
        return (1);
    }
    app->image = mlx_new_image(app->mlx, 800, 600);
    if (!app->image)
    {
        ft_printf("Error: Image creation failed\n");
        mlx_terminate(app->mlx);
        return (1);
    }
    uint32_t *pixels = (uint32_t *)app->image->pixels;
    if (!pixels)
    {
        ft_printf("Error: Pixel allocation failed\n");
        mlx_delete_image(app->mlx, app->image);
        mlx_terminate(app->mlx);
        return (1);
    }
    // fill pixel buffer with red
    for (uint32_t i = 0; i < 800 * 600; i++)
        pixels[i] = 0xFF0000FF;

    mlx_image_to_window(app->mlx, app->image, 0, 0);

    // keep window open
    mlx_loop(app->mlx);

    return (0);
}