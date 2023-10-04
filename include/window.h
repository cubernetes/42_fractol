/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 19:23:31 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/04 19:24:03 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_H
# define WINDOW_H

# include <fractol.h>
# include <mlx_int.h>

void			*mlx_new_resizable_window(t_xvar *xvar, int size_x, int size_y,
					char *title);
int				mlx_get_window_dim(void *mlx_ptr, void *win_ptr, int *width,
					int *height);
int				mlx_pixel_get_buf(t_fimg *data, int x, int y);
void			mlx_pixel_put_buf(t_fimg *data, int x, int y, int color);
int				close_mlx(t_vars *vars);

#endif
