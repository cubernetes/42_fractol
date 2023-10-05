/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:01:23 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/06 00:53:01 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>
#include <X11/X.h>
#include <mlx.h>

int	close_mlx(t_vars *vars)
{
	mlx_destroy_image(vars->mlx, vars->img.img);
	mlx_destroy_image(vars->mlx, vars->img2.img);
	mlx_destroy_window(vars->mlx, vars->win);
	mlx_destroy_display(vars->mlx);
	free(vars->mlx);
	exit(EXIT_SUCCESS);
}

int	keydown_hook(int kc, t_vars *vars)
{
	int	ret;

	ret = 1;
	if ('z' == kc || ' ' == kc || 'x' == kc)
		ret = zoom_middle(kc, vars);
	else if ('0' == kc)
		ret = reset_viewport(kc, vars);
	else if (XK_Escape == kc || 'q' == kc)
		ret = close_mlx(vars);
	else if ('h' == kc || 'j' == kc || 'k' == kc
		|| 'l' == kc || (XK_Left == kc || XK_Up == kc
			|| XK_Right == kc || XK_Down == kc))
		ret = translate(kc, -1, -1, vars);
	else if ('r' == kc)
		return (render(vars, NO_CACHE), 0);
	else if ('f' == kc)
		return (vars->enable_caching = !vars->enable_caching, 0);
	else if ('1' <= kc && '9' >= kc)
		return (change_fractal(kc, vars), render(vars, NO_CACHE), 0);
	else if (',' == kc || '.' == kc)
		return (change_gradient_phase(kc, vars), render(vars, NO_CACHE), 0);
	else if ('-' == kc || '=' == kc)
		change_iterations(kc, vars);
	return (render(vars, USE_CACHE), ret);
}

int	mouse_down_hook(int button, int x, int y, t_vars *vars)
{
	int	ret;

	ret = 1;
	if (Button4 == button || Button5 == button)
	{
		ret = zoom_to_mouse(button, x, y, vars);
		if (Button4 == button)
			change_iterations(']', vars);
		if (Button5 == button)
			change_iterations('[', vars);
	}
	else if (Button1 == button)
	{
		vars->dragging.x = x;
		vars->dragging.y = y;
		vars->dragging.scale_re = vars->fractal.scale_re;
		vars->dragging.scale_im = vars->fractal.scale_im;
		vars->mouse_down = True;
		return (render(vars, NO_CACHE), 0);
	}
	return (render(vars, USE_CACHE), ret);
}

int	mouse_up_hook(int button, int x, int y, t_vars *vars)
{
	(void)x;
	(void)y;
	if (Button1 == button)
		return (vars->mouse_down = False, 0);
	return (1);
}

int	mouse_move_hook(int x, int y, t_vars *vars)
{
	if (vars->render_mutex || !vars->mouse_down)
		return (1);
	if (-1 == vars->dragging.x)
	{
		vars->dragging.x = x;
		vars->dragging.y = y;
	}
	translate(-1, x, y, vars);
	return (render(vars, USE_CACHE), 0);
}
