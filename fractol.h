/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 00:59:19 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/03 05:02:04 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "./minilibx-linux/mlx_int.h"

/* TODO Add defaults to help menu */
# define DEF_IMG_WIDTH   1920
# define DEF_IMG_HEIGHT  1080
# define DEF_MIN_RE        -3
# define DEF_MAX_RE         2
# define DEF_WIN_TITLE   "fractol"
# define DEF_SPEED          0.1
# define DEF_ZOOM_FACTOR    1.1

# define USE_CACHE          1
# define NO_CACHE           0

# define DEF_MAX_ITERS 80
# define MIN_ITERS      1
# define ITERS_INC     10
# define DEF_MODULUS    4

/* Same colorscheme that Wikipedia uses */
# define COLOR_1            0x00000764
# define COLOR_2            0x00206BCB
# define COLOR_3            0x00EDFFFF
# define COLOR_4            0x00FFAA00
# define COLOR_5            0x00000200
# define GRAD_SIZE          2048
# define DEF_GRADIENT_PHASE 1100
# define GRADIENT_PHASE_INC  100

# define MANDELBROT 0
# define JULIA      1
# define TRICORN    2

# define OPTIONS_WIDTH 27
# define DESC_WIDTH    40

/* Not good, but bypasses -std=c89 flag */
# ifndef NAN
#  define NAN 0.0
# endif

# define EXIT_HELP          1
# define EXIT_MISSING_PARAM 2
# define EXIT_PARAM_NAN     3
# define EXIT_MLX_INIT      4

# define DEF_JULIA_RE      -0.7
# define DEF_JULIA_IM       0.27015

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
}				t_vars;

void			*mlx_new_resizable_window(t_xvar *xvar, int size_x, int size_y,
					char *title);
int				mlx_get_window_dim(void *mlx_ptr, void *win_ptr, int *width,
					int *height);

#endif
