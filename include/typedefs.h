/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedefs.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 19:29:45 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/06 00:53:43 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEFS_H
# define TYPEDEFS_H

# include <fractol.h>

typedef struct s_fimg
{
	void		*img;
	char		*addr;
	int			bpp;
	int			line_length;
	int			endian;
}				t_fimg;

typedef struct s_complex
{
	double		re;
	double		im;
}				t_complex;

typedef struct s_scale
{
	double		old_min;
	double		old_max;
	double		new_min;
	double		new_max;
}				t_scale;

typedef struct s_fractal
{
	int			fractal_type;
	t_complex	julia_c;
	t_scale		scale_re;
	t_scale		scale_im;
	int			img_width;
	int			img_height;
	char		*title;
	int			max_iters;
	int			gradient_phase;
	int			modulus;
	double		min_re;
	double		max_re;
	double		zoom_factor;
	double		speed;
}				t_fractal;

typedef struct s_rgb
{
	int			r;
	int			g;
	int			b;
}				t_rgb;

typedef struct s_dragging
{
	int			x;
	int			y;
	t_scale		scale_re;
	t_scale		scale_im;
}				t_dragging;

typedef struct s_vars
{
	void		*mlx;
	void		*win;
	t_fimg		img;
	t_fimg		img2;
	t_dragging	dragging;
	t_fractal	fractal;
	Bool		render_mutex;
	Bool		mouse_down;
	Bool		enable_caching;
}				t_vars;

typedef struct s_render_state
{
	int			parity;
	t_scale		old_scale_re;
	t_scale		old_scale_im;

}				t_render_state;

#endif
