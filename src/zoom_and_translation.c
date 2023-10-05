/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zoom_and_translation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 09:45:27 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/06 00:09:42 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>
#include <libft.h>
#include <X11/X.h>
#include <X11/keysym.h>

int	zoom_middle(int kc, t_vars *vars)
{
	double	diff_re;
	double	diff_im;
	double	zoom_re;
	double	zoom_im;
	double	zoom;

	if ('z' == kc)
		zoom = vars->fractal.zoom_factor;
	else if ('x' == kc)
		zoom = 1 / vars->fractal.zoom_factor;
	else
		return (1);
	diff_re = vars->fractal.scale_re.new_max - vars->fractal.scale_re.new_min;
	diff_im = vars->fractal.scale_im.new_max - vars->fractal.scale_im.new_min;
	zoom_re = vars->fractal.scale_re.new_min + diff_re / 2.;
	zoom_im = vars->fractal.scale_im.new_min + diff_im / 2.;
	vars->fractal.scale_re.new_min = zoom_re - (vars->fractal.img_width
			/ 2. / (double)vars->fractal.img_width) * diff_re / zoom;
	vars->fractal.scale_re.new_max = zoom_re + (1. - vars->fractal.img_width
			/ 2. / (double)vars->fractal.img_width) * diff_re / zoom;
	vars->fractal.scale_im.new_min = zoom_im - (vars->fractal.img_height
			/ 2. / (double)vars->fractal.img_height) * diff_im / zoom;
	vars->fractal.scale_im.new_max = zoom_im + (1. - vars->fractal.img_height
			/ 2. / (double)vars->fractal.img_height) * diff_im / zoom;
	return (0);
}

int	zoom_to_mouse(int button, int x, int y, t_vars *vars)
{
	double	diff_re;
	double	diff_im;
	double	zoom_re;
	double	zoom_im;
	double	zoom;

	if (Button4 == button)
	{
		zoom = vars->fractal.zoom_factor;
		vars->fractal.max_iters += ITERS_INC / 5;
	}
	else if (Button5 == button)
	{
		zoom = 1 / vars->fractal.zoom_factor;
		vars->fractal.max_iters -= ITERS_INC / 5;
	}
	else
		return (1);
	ft_dprintf(1, "Iters: %d\n", vars->fractal.max_iters);
	diff_re = vars->fractal.scale_re.new_max - vars->fractal.scale_re.new_min;
	diff_im = vars->fractal.scale_im.new_max - vars->fractal.scale_im.new_min;
	zoom_re = rescale(x, vars->fractal.scale_re);
	zoom_im = rescale(y, vars->fractal.scale_im);
	vars->fractal.scale_re.new_min = zoom_re - (x
			/ (double)vars->fractal.img_width) * diff_re / zoom;
	vars->fractal.scale_re.new_max = zoom_re + (1. - x
			/ (double)vars->fractal.img_width) * diff_re / zoom;
	vars->fractal.scale_im.new_min = zoom_im - (y
			/ (double)vars->fractal.img_height) * diff_im / zoom;
	vars->fractal.scale_im.new_max = zoom_im + (1. - y
			/ (double)vars->fractal.img_height) * diff_im / zoom;
	return (0);
}

void	translate_to_xy(int x, int y, t_vars *vars)
{
	double	x_diff;
	double	y_diff;

	x_diff = (double)(vars->dragging.x - x) * (vars->fractal.scale_re.new_max
			- vars->fractal.scale_re.new_min)
		/ (double)vars->fractal.img_width;
	y_diff = (double)(vars->dragging.y - y) * (vars->fractal.scale_im.new_max
			- vars->fractal.scale_im.new_min)
		/ (double)vars->fractal.img_height;
	vars->fractal.scale_re.new_min = vars->dragging.scale_re.new_min + x_diff;
	vars->fractal.scale_re.new_max = vars->dragging.scale_re.new_max + x_diff;
	vars->fractal.scale_im.new_min = vars->dragging.scale_im.new_min + y_diff;
	vars->fractal.scale_im.new_max = vars->dragging.scale_im.new_max + y_diff;
}

int	translate(int kc, int x, int y, t_vars *vars)
{
	double	zoom;
	int		re;

	zoom = (vars->fractal.max_re - vars->fractal.min_re)
		/ (vars->fractal.scale_re.new_max - vars->fractal.scale_re.new_min);
	if (-1 == kc)
		return (translate_to_xy(x, y, vars), 0);
	if ('h' == kc || 'k' == kc || XK_Left == kc || XK_Up == kc)
		zoom *= -1;
	re = 1;
	if ('j' == kc || 'k' == kc || XK_Up == kc || XK_Down == kc)
		re = 0;
	vars->fractal.scale_im.new_min += vars->fractal.speed / zoom * !re;
	vars->fractal.scale_im.new_max += vars->fractal.speed / zoom * !re;
	vars->fractal.scale_re.new_min += vars->fractal.speed / zoom * re;
	vars->fractal.scale_re.new_max += vars->fractal.speed / zoom * re;
	return (0);
}
