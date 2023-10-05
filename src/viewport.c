/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   viewport.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:00:00 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/06 00:56:54 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>
#include <libft.h>

void	apply_viewport(t_complex center, double zoom, t_fractal *fractal)
{
	static double	diff_re;
	static double	diff_im;
	double			half;

	diff_re = fractal->max_re - fractal->min_re;
	diff_im = (fractal->max_re - fractal->min_re) * fractal->img_height
		/ fractal->img_width;
	half = diff_re / (2. * zoom);
	fractal->scale_re.new_min = center.re - half;
	fractal->scale_re.new_max = center.re + half;
	half = diff_im / (2. * zoom);
	fractal->scale_im.new_min = center.im - half;
	fractal->scale_im.new_max = center.im + half;
}

int	reset_viewport(int kc, t_vars *vars)
{
	double	half_i;

	if (kc != '0')
		return (1);
	half_i = ((vars->fractal.max_re - vars->fractal.min_re)
			* vars->fractal.img_height) / (2. * vars->fractal.img_width);
	vars->fractal.scale_re.new_min = vars->fractal.min_re;
	vars->fractal.scale_re.new_max = vars->fractal.max_re;
	vars->fractal.scale_im.new_min = -half_i;
	vars->fractal.scale_im.new_max = half_i;
	vars->fractal.max_iters = DEF_MAX_ITERS;
	ft_dprintf(1, "Max Iterations: %d           \r", DEF_MAX_ITERS);
	return (0);
}
