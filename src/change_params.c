/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_params.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:05:50 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/05 21:39:24 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>
#include <libft.h>

void	change_fractal(int kc, t_vars *vars)
{
	if ('1' == kc)
		vars->fractal.fractal_type = MANDELBROT;
	else if ('2' == kc)
		vars->fractal.fractal_type = JULIA;
	else if ('3' == kc)
		vars->fractal.fractal_type = TRICORN;
}

void	change_gradient_phase(int kc, t_vars *vars)
{
	if (',' == kc)
		vars->fractal.gradient_phase -= GRADIENT_PHASE_INC;
	else if ('.' == kc)
		vars->fractal.gradient_phase += GRADIENT_PHASE_INC;
	if (0 == vars->fractal.gradient_phase % 10)
		++vars->fractal.gradient_phase;
}

void	change_iterations(int kc, t_vars *vars)
{
	if ('-' == kc)
		vars->fractal.max_iters -= ITERS_INC;
	else if ('=' == kc)
		vars->fractal.max_iters += ITERS_INC;
	if (vars->fractal.max_iters < 1)
		vars->fractal.max_iters = MIN_ITERS;
	ft_dprintf(1, "Max iterations: %d\n", vars->fractal.max_iters);
}
