/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   viewport.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 19:25:49 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/04 19:26:02 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIEWPORT_H
# define VIEWPORT_H

# include <fractol.h>

void			apply_viewport(t_complex center, double zoom,
					t_fractal *fractal);
int				zoom_middle(int kc, t_vars *vars);
int				zoom_to_mouse(int button, int x, int y, t_vars *vars);
int				translate(int kc, int x, int y, t_vars *vars);
int				reset_viewport(int kc, t_vars *vars);

#endif
