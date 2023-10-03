/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 14:17:25 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/03 08:41:59 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>
#include <mlx.h>

void	setup_hooks(t_vars *vars)
{
	mlx_hook(vars->win, KeyPress, KeyPressMask, keydown_hook, vars);
	mlx_hook(vars->win, ButtonPress, ButtonPressMask, mouse_down_hook, vars);
	mlx_hook(vars->win, ButtonRelease, ButtonReleaseMask, mouse_up_hook, vars);
	mlx_hook(vars->win, MotionNotify, PointerMotionMask, mouse_move_hook, vars);
	mlx_hook(vars->win, DestroyNotify, NoEventMask, close_mlx, vars);
	mlx_hook(vars->win, ConfigureNotify, StructureNotifyMask, resize_img, vars);
}

void	init(t_vars *vars)
{
	vars->mlx = mlx_init();
	if (!vars->mlx)
		(perror("mlx_init"), exit(EXIT_MLX_INIT));
	vars->win = mlx_new_resizable_window(vars->mlx, vars->fractal.img_width,
			vars->fractal.img_height, vars->fractal.title);
	init_images(vars);
	vars->dragging.x = -1;
	vars->dragging.y = -1;
	vars->dragging.scale_re = vars->fractal.scale_re;
	vars->dragging.scale_im = vars->fractal.scale_im;
	vars->render_mutex = False;
	vars->mouse_down = False;
}

int	main(int argc, char **argv)
{
	t_vars	vars;

	if (parse_help(argc, argv))
		return (EXIT_HELP);
	vars.fractal = parse_options(argc, argv);
	init(&vars);
	render(&vars, NO_CACHE);
	setup_hooks(&vars);
	mlx_loop(vars.mlx);
	return (EXIT_SUCCESS);
}
