/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 07:51:23 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/06 00:18:55 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>
#include <mlx.h>

void	update_img_buffer(t_vars *vars, int x, int y, t_render_state *rs)
{
	t_complex		c;
	int				color;
	int				old_x;
	int				old_y;

	c.re = rescale((double)x, vars->fractal.scale_re);
	c.im = rescale((double)y, vars->fractal.scale_im);
	old_x = unrescale(c.re, rs->old_scale_re);
	old_y = unrescale(c.im, rs->old_scale_im);
	if (rs->parity)
	{
		if (rs->parity != -1 && old_x >= 0 && old_x < vars->fractal.img_width
			&& old_y >= 0 && old_y < vars->fractal.img_height)
			color = mlx_pixel_get_buf(&vars->img2, old_x, old_y);
		else
			color = compute_fractal_clr(c, &vars->fractal);
		mlx_pixel_put_buf(&vars->img, x, y, color);
		return ;
	}
	if (old_x >= 0 && old_x < vars->fractal.img_width
		&& old_y >= 0 && old_y < vars->fractal.img_height)
		color = mlx_pixel_get_buf(&vars->img, old_x, old_y);
	else
		color = compute_fractal_clr(c, &vars->fractal);
	mlx_pixel_put_buf(&vars->img2, x, y, color);
}

void	render(t_vars *vars, int use_cache)
{
	static t_render_state	render_state = {-1, {0, 0, 0, 0,}, {0, 0, 0, 0}};
	int						x;
	int						y;

	vars->render_mutex = True;
	if (!use_cache)
		render_state.parity = -1;
	y = -1;
	while (++y < vars->fractal.img_height)
	{
		x = -1;
		while (++x < vars->fractal.img_width)
			update_img_buffer(vars, x, y, &render_state);
	}
	if (render_state.parity)
		mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	else
		mlx_put_image_to_window(vars->mlx, vars->win, vars->img2.img, 0, 0);
	render_state.parity = !render_state.parity;
	render_state.old_scale_im = vars->fractal.scale_im;
	render_state.old_scale_re = vars->fractal.scale_re;
	vars->render_mutex = False;
}
