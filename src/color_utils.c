/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 07:40:59 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/03 08:39:25 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>
#include <math.h>

void	rgb_from_int(t_rgb *rgb, int color)
{
	rgb->b = color & 0xff;
	color >>= 8;
	rgb->g = color & 0xff;
	color >>= 8;
	rgb->r = color & 0xff;
}

int	int_from_rgb(t_rgb *rgb)
{
	int	color;

	color = 0;
	color <<= 8;
	color += rgb->r % 256;
	color <<= 8;
	color += rgb->g % 256;
	color <<= 8;
	color += rgb->b % 256;
	return (color);
}

int	interp_color(int i, double start, double end, int color_idx)
{
	t_rgb	color_rgb;
	t_rgb	rgb_start;
	t_rgb	rgb_end;
	double	unit;

	if (1 == color_idx)
		(rgb_from_int(&rgb_start, COLOR_1), rgb_from_int(&rgb_end, COLOR_2));
	else if (2 == color_idx)
		(rgb_from_int(&rgb_start, COLOR_2), rgb_from_int(&rgb_end, COLOR_3));
	else if (3 == color_idx)
		(rgb_from_int(&rgb_start, COLOR_3), rgb_from_int(&rgb_end, COLOR_4));
	else if (4 == color_idx)
		(rgb_from_int(&rgb_start, COLOR_4), rgb_from_int(&rgb_end, COLOR_5));
	else
		(rgb_from_int(&rgb_start, COLOR_5), rgb_from_int(&rgb_end, COLOR_1));
	unit = (i - start) / (end - start);
	color_rgb.r = (int)((rgb_end.r - rgb_start.r) * unit + rgb_start.r);
	color_rgb.g = (int)((rgb_end.g - rgb_start.g) * unit + rgb_start.g);
	color_rgb.b = (int)((rgb_end.b - rgb_start.b) * unit + rgb_start.b);
	return (int_from_rgb(&color_rgb));
}

void	init_colors(int colors[GRAD_SIZE], t_fractal *fractal)
{
	int	i;
	int	off;

	if (1 == fractal->gradient_phase % 10)
		fractal->gradient_phase -= 1;
	off = fractal->gradient_phase;
	i = -1;
	while (++i < GRAD_SIZE)
	{
		if (i < 0.16 * GRAD_SIZE)
			colors[(off + i) % GRAD_SIZE] = interp_color(i, 0, 0.16 * GRAD_SIZE,
					1);
		else if (i < 0.42 * GRAD_SIZE)
			colors[(off + i) % GRAD_SIZE] = interp_color(i, 0.16 * GRAD_SIZE,
					0.42 * GRAD_SIZE, 2);
		else if (i < 0.6425 * GRAD_SIZE)
			colors[(off + i) % GRAD_SIZE] = interp_color(i, 0.42 * GRAD_SIZE,
					0.6425 * GRAD_SIZE, 3);
		else if (i < 0.8575 * GRAD_SIZE)
			colors[(off + i) % GRAD_SIZE] = interp_color(i, 0.6425 * GRAD_SIZE,
					0.8575 * GRAD_SIZE, 4);
		else
			colors[(off + i) % GRAD_SIZE] = interp_color(i, 0.8575 * GRAD_SIZE,
					GRAD_SIZE, 5);
	}
}

int	color_from_iter(t_complex z, int iter, t_fractal *fractal)
{
	static int	colors[GRAD_SIZE];
	double		smoothed;
	int			color_idx;

	if (1 == fractal->gradient_phase % 10 || !colors[0])
		init_colors(colors, fractal);
	if (iter == fractal->max_iters)
		return (0);
	smoothed = log2(log2(z.re * z.re + z.im * z.im) / 2.);
	color_idx = (int)(sqrt(iter + 10 - smoothed) * 256) % GRAD_SIZE;
	return (colors[color_idx]);
}
