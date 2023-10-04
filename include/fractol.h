/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 00:59:19 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/04 19:30:43 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include <X11/Xlib.h>
# include <stddef.h>
# include <definitions.h>
# include <typedefs.h>
# include <math_utils.h>
# include <parsing.h>
# include <viewport.h>
# include <window.h>

int				color_from_iter(t_complex z, int iter, t_fractal *fractal);
size_t			ft_strwidth(const char *s, size_t tabsize);
void			ft_putstr_mult(char *s, int n);
void			print_help(char **argv);
void			setup_hooks(t_vars *vars);
int				keydown_hook(int kc, t_vars *vars);
int				mouse_down_hook(int button, int x, int y, t_vars *vars);
int				mouse_up_hook(int button, int x, int y, t_vars *vars);
int				mouse_move_hook(int x, int y, t_vars *vars);
int				resize_img(t_vars *vars);
void			init_images(t_vars *vars);
void			init_fractal(t_fractal *fractal);
int				compute_fractal_clr(t_complex c, t_fractal *fractal);
void			change_fractal(int kc, t_vars *vars);
void			change_gradient_phase(int kc, t_vars *vars);
void			change_iterations(int kc, t_vars *vars);
void			render(t_vars *vars, int use_cache);

#endif
