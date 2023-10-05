/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param_parsing1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 09:12:47 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/03 09:20:46 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>
#include <libft.h>
#include <stdlib.h>

void	parse_modulus_param(int *argc, char ***argv, t_fractal *fractal)
{
	check_param(argc, argv);
	fractal->modulus = ft_atoi(**argv);
}

void	parse_zoom_factor_param(int *argc, char ***argv, t_fractal *fractal)
{
	check_param(argc, argv);
	fractal->zoom_factor = ft_atof(**argv);
	if (fractal->zoom_factor != fractal->zoom_factor)
		(ft_dprintf(2, "Error parsing argument for '%s': NaN\n", **argv),
			exit(EXIT_PARAM_NAN));
}

void	parse_mvmt_speed_param(int *argc, char ***argv, t_fractal *fractal)
{
	check_param(argc, argv);
	fractal->speed = ft_atof(**argv);
	if (fractal->speed != fractal->speed)
		(ft_dprintf(2, "Error parsing argument for '%s': NaN\n", **argv),
			exit(EXIT_PARAM_NAN));
}

void	parse_title_param(int *argc, char ***argv, t_fractal *fractal)
{
	check_param(argc, argv);
	fractal->title = **argv;
}

void	parse_real_intv_param(int *argc, char ***argv, t_fractal *fractal)
{
	t_complex	dummy;

	check_param(argc, argv);
	dummy = parse_complex(**argv);
	fractal->min_re = dummy.re;
	fractal->max_re = dummy.im;
	if (fractal->min_re != fractal->min_re)
		(ft_dprintf(2, "Error parsing argument for '%s': NaN\n", **argv),
			exit(EXIT_PARAM_NAN));
}
