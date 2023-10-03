/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractals.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 07:43:31 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/03 08:39:49 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>

int	render_mandelbrot(t_complex c, t_fractal *fractal)
{
	t_complex	z;
	int			iter;

	z.re = 0.0;
	z.im = 0.0;
	iter = -1;
	while (++iter < fractal->max_iters)
	{
		if (complex_modulus(z) > fractal->modulus)
			break ;
		z = complex_addition(complex_product(z, z), c);
	}
	return (color_from_iter(z, iter, fractal));
}

int	render_tricorn(t_complex c, t_fractal *fractal)
{
	t_complex	z;
	int			iter;

	z.re = 0.0;
	z.im = 0.0;
	iter = -1;
	while (++iter < fractal->max_iters)
	{
		if (complex_modulus(z) > fractal->modulus)
			break ;
		z = complex_addition(complex_product(conjugate(z), conjugate(z)), c);
	}
	return (color_from_iter(z, iter, fractal));
}

int	render_julia(t_complex z, t_fractal *fractal)
{
	int	iter;

	iter = -1;
	while (++iter < fractal->max_iters)
	{
		if (complex_modulus(z) > fractal->modulus)
			break ;
		z = complex_addition(complex_product(z, z), fractal->julia_c);
	}
	return (color_from_iter(z, iter, fractal));
}

int	compute_fractal_clr(t_complex c, t_fractal *fractal)
{
	if (MANDELBROT == fractal->fractal_type)
		return (render_mandelbrot(c, fractal));
	else if (JULIA == fractal->fractal_type)
		return (render_julia(c, fractal));
	else if (TRICORN == fractal->fractal_type)
		return (render_tricorn(c, fractal));
	return (0);
}

void	init_fractal(t_fractal *fractal)
{
	fractal->fractal_type = MANDELBROT;
	fractal->scale_re.old_min = 0;
	fractal->scale_re.old_max = DEF_IMG_WIDTH;
	fractal->scale_re.new_min = DEF_MIN_RE;
	fractal->scale_re.new_max = DEF_MAX_RE;
	fractal->scale_im.old_min = 0;
	fractal->scale_im.old_max = DEF_IMG_HEIGHT;
	fractal->scale_im.new_min = -((DEF_MAX_RE - DEF_MIN_RE) * DEF_IMG_HEIGHT)
		/ (2. * DEF_IMG_WIDTH);
	fractal->scale_im.new_max = ((DEF_MAX_RE - DEF_MIN_RE) * DEF_IMG_HEIGHT)
		/ (2. * DEF_IMG_WIDTH);
	fractal->min_re = DEF_MIN_RE;
	fractal->max_re = DEF_MAX_RE;
	fractal->max_iters = DEF_MAX_ITERS;
	fractal->img_width = DEF_IMG_WIDTH;
	fractal->img_height = DEF_IMG_HEIGHT;
	fractal->gradient_phase = DEF_GRADIENT_PHASE;
	fractal->title = DEF_WIN_TITLE;
	fractal->modulus = DEF_MODULUS;
	fractal->zoom_factor = DEF_ZOOM_FACTOR;
	fractal->speed = DEF_SPEED;
	fractal->julia_c.re = DEF_JULIA_RE;
	fractal->julia_c.im = DEF_JULIA_IM;
}
