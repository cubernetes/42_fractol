/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:09:48 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/03 09:17:35 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>
#include <libft.h>
#include <stdlib.h>

int	parse_more_options(int *argc, char ***argv, t_fractal *fractal)
{
	if (!ft_strcmp(**argv, "--winsize"))
		return (parse_winsize_param(argc, argv, fractal), 1);
	else if (!ft_strcmp(**argv, "--gradient-phase"))
		return (parse_grad_phase_param(argc, argv, fractal), 1);
	else if (!ft_strcmp(**argv, "--iterations"))
		return (parse_iterations_param(argc, argv, fractal), 1);
	else if (!ft_strcmp(**argv, "--modulus"))
		return (parse_modulus_param(argc, argv, fractal), 1);
	else if (!ft_strcmp(**argv, "--title"))
		return (parse_title_param(argc, argv, fractal), 1);
	else if (!ft_strcmp(**argv, "--real-intv"))
		return (parse_real_intv_param(argc, argv, fractal), 1);
	else if (!ft_strcmp(**argv, "--zoom-factor"))
		return (parse_zoom_factor_param(argc, argv, fractal), 1);
	else if (!ft_strcmp(**argv, "--mvmt-speed"))
		return (parse_mvmt_speed_param(argc, argv, fractal), 1);
	return (0);
}

int	parse_fractals(int *argc, char ***argv, t_fractal *fractal)
{
	if (!ft_strcmp(*(++*argv), "--mandelbrot"))
		return (fractal->fractal_type = MANDELBROT, 1);
	else if (!ft_strcmp(**argv, "--tricorn"))
		return (fractal->fractal_type = TRICORN, 1);
	else if (!ft_strcmp(**argv, "--julia"))
		return (parse_julia_param(argc, argv, fractal), 1);
	return (0);
}

t_fractal	parse_options(int argc, char **argv)
{
	t_fractal	fractal;
	t_complex	center;
	double		zoom;

	init_fractal(&fractal);
	zoom = 1.;
	center.re = (fractal.scale_re.new_max + fractal.scale_re.new_min) / 2.;
	center.im = (fractal.scale_im.new_max + fractal.scale_im.new_min) / 2.;
	while (--argc)
	{
		if (parse_fractals(&argc, &argv, &fractal))
			;
		else if (!ft_strcmp(*argv, "--center"))
			center = parse_center_param(&argc, &argv);
		else if (!ft_strcmp(*argv, "--zoom"))
			zoom = parse_zoom_param(&argc, &argv);
		else if (parse_more_options(&argc, &argv, &fractal))
			;
		else
			(dprintf(2, "Unknown parameter: '\033[31m%s\033[m'\n", *argv),
				exit(EXIT_INV_PARAM));
	}
	apply_viewport(center, zoom, &fractal);
	return (fractal);
}
