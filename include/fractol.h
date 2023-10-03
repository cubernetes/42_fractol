/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 00:59:19 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/03 10:08:25 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include <X11/Xlib.h>
# include <mlx_int.h>
# include <stddef.h>

# define DEF_IMG_WIDTH 1920
# define DEF_IMG_HEIGHT 1080
# define DEF_MIN_RE -3
# define DEF_MAX_RE 2
# define DEF_WIN_TITLE "fractol"
# define DEF_SPEED 0.1
# define DEF_ZOOM_FACTOR 1.1

# define USE_CACHE 1
# define NO_CACHE 0

# define DEF_MAX_ITERS 80
# define MIN_ITERS 1
# define ITERS_INC 10
# define DEF_MODULUS 4

/* Same colorscheme that Wikipedia uses */
# define COLOR_1 0x00000764
# define COLOR_2 0x00206BCB
# define COLOR_3 0x00EDFFFF
# define COLOR_4 0x00FFAA00
# define COLOR_5 0x00000200
# define GRAD_SIZE 2048
# define DEF_GRADIENT_PHASE 1100
# define GRADIENT_PHASE_INC 100

# define MANDELBROT 0
# define JULIA 1
# define TRICORN 2

# define OPTIONS_WIDTH 27
# define DESC_WIDTH 40

/* Not good, but bypasses -std=c89 flag */
# ifndef NAN
#  define NAN 0.0
# endif

# define EXIT_HELP 1
# define EXIT_MISSING_PARAM 2
# define EXIT_PARAM_NAN 3
# define EXIT_MLX_INIT 4
# define EXIT_INV_PARAM 5

# define DEF_JULIA_RE -0.7
# define DEF_JULIA_IM 0.27015

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

typedef struct s_render_state
{
	int			parity;
	t_scale		old_scale_re;
	t_scale		old_scale_im;

}				t_render_state;

void			*mlx_new_resizable_window(t_xvar *xvar, int size_x, int size_y,
					char *title);
int				mlx_get_window_dim(void *mlx_ptr, void *win_ptr, int *width,
					int *height);
int				mlx_pixel_get_buf(t_fimg *data, int x, int y);
void			mlx_pixel_put_buf(t_fimg *data, int x, int y, int color);
double			log2(double x);
t_complex		conjugate(t_complex z);
t_complex		complex_addition(t_complex z1, t_complex z2);
t_complex		complex_product(t_complex z1, t_complex z2);
double			complex_modulus(t_complex z);
int				color_from_iter(t_complex z, int iter, t_fractal *fractal);
double			rescale(double coord, t_scale scale);
int				unrescale(double image, t_scale scale);
size_t			ft_strwidth(const char *s, size_t tabsize);
void			ft_putstr_mult(char *s, int n);
void			print_help(char **argv);
void			apply_viewport(t_complex center, double zoom,
					t_fractal *fractal);
int				zoom_middle(int kc, t_vars *vars);
int				zoom_to_mouse(int button, int x, int y, t_vars *vars);
int				translate(int kc, int x, int y, t_vars *vars);
int				reset_viewport(int kc, t_vars *vars);
void			render(t_vars *vars, int use_cache);
void			setup_hooks(t_vars *vars);
int				keydown_hook(int kc, t_vars *vars);
int				close_mlx(t_vars *vars);
int				mouse_down_hook(int button, int x, int y, t_vars *vars);
int				mouse_up_hook(int button, int x, int y, t_vars *vars);
int				mouse_move_hook(int x, int y, t_vars *vars);
void			init_fractal(t_fractal *fractal);
int				resize_img(t_vars *vars);
void			init_images(t_vars *vars);
t_fractal		parse_options(int argc, char **argv);
int				parse_help(int argc, char **argv);
int				compute_fractal_clr(t_complex c, t_fractal *fractal);
void			change_fractal(int kc, t_vars *vars);
void			change_gradient_phase(int kc, t_vars *vars);
void			change_iterations(int kc, t_vars *vars);
void			parse_modulus_param(int *argc, char ***argv,
					t_fractal *fractal);
void			parse_zoom_factor_param(int *argc, char ***argv,
					t_fractal *fractal);
void			parse_mvmt_speed_param(int *argc, char ***argv,
					t_fractal *fractal);
void			parse_title_param(int *argc, char ***argv, t_fractal *fractal);
void			parse_real_intv_param(int *argc, char ***argv,
					t_fractal *fractal);
t_complex		parse_center_param(int *argc, char ***argv);
double			parse_zoom_param(int *argc, char ***argv);
void			parse_winsize_param(int *argc, char ***argv,
					t_fractal *fractal);
void			parse_grad_phase_param(int *argc, char ***argv,
					t_fractal *fractal);
void			parse_iterations_param(int *argc, char ***argv,
					t_fractal *fractal);
void			check_param(int *argc, char ***argv);
t_complex		parse_complex(const char *s);
void			parse_julia_param(int *argc, char ***argv, t_fractal *fractal);

#endif
