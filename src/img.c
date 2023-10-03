/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:07:13 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/03 08:39:34 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>
#include <mlx.h>

void	init_images(t_vars *vars)
{
	vars->img.img = mlx_new_image(vars->mlx, vars->fractal.img_width,
			vars->fractal.img_height);
	vars->img.addr = mlx_get_data_addr(vars->img.img, &vars->img.bpp,
			&vars->img.line_length, &vars->img.endian);
	vars->img2.img = mlx_new_image(vars->mlx, vars->fractal.img_width,
			vars->fractal.img_height);
	vars->img2.addr = mlx_get_data_addr(vars->img2.img, &vars->img2.bpp,
			&vars->img2.line_length, &vars->img2.endian);
}

int	resize_img(t_vars *vars)
{
	int		width;
	int		height;
	Status	status;

	status = mlx_get_window_dim(vars->mlx, vars->win, &width, &height);
	if ((vars->fractal.img_width == width && vars->fractal.img_height == height)
		|| !status)
		return (1);
	vars->fractal.img_width = width;
	vars->fractal.img_height = height;
	vars->fractal.scale_re.old_max = width;
	vars->fractal.scale_im.old_max = height;
	mlx_destroy_image(vars->mlx, vars->img.img);
	mlx_destroy_image(vars->mlx, vars->img2.img);
	init_images(vars);
	setup_hooks(vars);
	return (render(vars, NO_CACHE), 0);
}
