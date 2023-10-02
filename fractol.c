/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 14:17:25 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/02 21:30:56 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft/libft.h"
#include "./minilibx-linux/mlx.h"
#include <complex.h>
#include <libft.h>
#include <math.h>
#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* TODO Add defaults to help menu */
#define DEF_IMG_WIDTH 1920
#define DEF_IMG_HEIGHT 1080
#define DEF_MIN_RE -3
#define DEF_MAX_RE 2
#define DEF_WIN_TITLE "fractol"
#define DEF_SPEED 0.1
#define DEF_ZOOM_FACTOR 1.1

#define DEF_MAX_ITERS 80
#define DEF_MODULUS 4

/* Same colorscheme that wikipedia uses */
#define COLOR_1 0x00000764
#define COLOR_2 0x00206BCB
#define COLOR_3 0x00EDFFFF
#define COLOR_4 0x00FFAA00
#define COLOR_5 0x00000200
#define GRAD_SIZE 2048
#define DEF_GRADIENT_PHASE 1100

#define MANDELBROT 0
#define JULIA 1
#define MULTIBROT 2
#define TRICORN 3

#define OPTIONS_WIDTH 27
#define DESC_WIDTH 40

#define KC_ESC   65307
#define KC_LEFT  65361
#define KC_UP    65362
#define KC_RIGHT 65363
#define KC_DOWN  65364

/* Not good, but bypasses -std=c89 flag */
#ifndef NAN
# define NAN 0.0
#endif

typedef struct s_img
{
	void		*img;
	char		*addr;
	int			bpp;
	int			line_length;
	int			endian;
}				t_img;

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
	double		multibrot_exponent;
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
	int		x;
	int		y;
	int		re;
	int		im;
	t_scale	scale_re;
	t_scale	scale_im;
}			t_dragging;
typedef struct s_vars
{
	void		*mlx;
	void		*win;
	t_img		img;
	t_img		img2;
	t_dragging	dragging;
	t_fractal	fractal;
}				t_vars;

void	mlx_pixel_put_buf(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bpp / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

int	mlx_pixel_get_buf(t_img *data, int x, int y)
{
	return (*(int *)(data->addr + (y * data->line_length
			+ x * (data->bpp / 8))));
}

t_complex	complex_addition(t_complex z1, t_complex z2)
{
	t_complex	z;

	z.re = z1.re + z2.re;
	z.im = z1.im + z2.im;
	return (z);
}

t_complex	complex_product(t_complex z1, t_complex z2)
{
	t_complex	z;

	z.re = z1.re * z2.re - z1.im * z2.im;
	z.im = z1.re * z2.im + z2.re * z1.im;
	return (z);
}

double	complex_modulus(t_complex z)
{
	return (sqrt(z.re * z.re + z.im * z.im));
}

void	rgb_from_int(t_rgb *rgb, int color)
{
	rgb->b = color & 0xff;
	color >>= 8;
	rgb->g = color & 0xff;
	color >>= 8;
	rgb->r = color & 0xff;
}

int	int_from_rgb(t_rgb *rgb)
{
	int	color;

	color = 0;
	color <<= 8;
	color += rgb->r % 256;
	color <<= 8;
	color += rgb->g % 256;
	color <<= 8;
	color += rgb->b % 256;
	return (color);
}

int	interp_color(int i, double start, double end, int color_idx)
{
	t_rgb	color_rgb;
	t_rgb	rgb_start;
	t_rgb	rgb_end;
	double	unit;

	if (color_idx == 1)
		(rgb_from_int(&rgb_start, COLOR_1), rgb_from_int(&rgb_end, COLOR_2));
	else if (color_idx == 2)
		(rgb_from_int(&rgb_start, COLOR_2), rgb_from_int(&rgb_end, COLOR_3));
	else if (color_idx == 3)
		(rgb_from_int(&rgb_start, COLOR_3), rgb_from_int(&rgb_end, COLOR_4));
	else if (color_idx == 4)
		(rgb_from_int(&rgb_start, COLOR_4), rgb_from_int(&rgb_end, COLOR_5));
	else
		(rgb_from_int(&rgb_start, COLOR_5), rgb_from_int(&rgb_end, COLOR_1));
	unit = (i - start) / (end - start);
	color_rgb.r = (int)((rgb_end.r - rgb_start.r) * unit + rgb_start.r);
	color_rgb.g = (int)((rgb_end.g - rgb_start.g) * unit + rgb_start.g);
	color_rgb.b = (int)((rgb_end.b - rgb_start.b) * unit + rgb_start.b);
	return (int_from_rgb(&color_rgb));
}

double	log2(double x)
{
	return (log(x) / log(2));
}

void	print_color(int color)
{
	t_rgb	rgb;

	rgb_from_int(&rgb, color);
	printf("%d, %d, %d\n", rgb.r, rgb.g, rgb.b);
}

void	init_colors(int colors[GRAD_SIZE], t_fractal *fractal)
{
	int	off;
	int	i;

	if (fractal->gradient_phase % 10 == 1)
		fractal->gradient_phase -= 1;
	off = fractal->gradient_phase;
	i = -1;
	while (++i < GRAD_SIZE)
	{
		if (i < 0.16 * GRAD_SIZE)
			colors[(off + i) % GRAD_SIZE] = interp_color(i, 0, 0.16 * GRAD_SIZE,
					1);
		else if (i < 0.42 * GRAD_SIZE)
			colors[(off + i) % GRAD_SIZE] = interp_color(i, 0.16 * GRAD_SIZE,
					0.42 * GRAD_SIZE, 2);
		else if (i < 0.6425 * GRAD_SIZE)
			colors[(off + i) % GRAD_SIZE] = interp_color(i, 0.42 * GRAD_SIZE,
					0.6425 * GRAD_SIZE, 3);
		else if (i < 0.8575 * GRAD_SIZE)
			colors[(off + i) % GRAD_SIZE] = interp_color(i, 0.6425 * GRAD_SIZE,
					0.8575 * GRAD_SIZE, 4);
		else
			colors[(off + i) % GRAD_SIZE] = interp_color(i, 0.8575 * GRAD_SIZE,
					GRAD_SIZE, 5);
	}
}

int	color_from_iter(t_complex z, int iter, t_fractal *fractal)
{
	double		smoothed;
	static int	colors[GRAD_SIZE];
	int			color_idx;

	if (!colors[0] || fractal->gradient_phase % 10 == 1)
		init_colors(colors, fractal);
	if (iter == fractal->max_iters)
		return (0);
	smoothed = log2(log2(z.re * z.re + z.im * z.im) / 2.);
	color_idx = (int)(sqrt(iter + 10 - smoothed) * 256) % GRAD_SIZE;
	return (colors[color_idx]);
}

/* very slow */
t_complex	complex_power(t_complex z, double exp)
{
	double complex	z_;
	double complex	exp_;
	double complex	power_;
	t_complex		power;

	z_ = z.re + z.im * _Complex_I;
	exp_ = exp;
	power_ = cpow(z_, exp_);
	power.re = creal(power_);
	power.im = cimag(power_);
	return (power);
}

int	render_mandelbrot(t_complex c, t_fractal *fractal)
{
	t_complex	z;
	int			iter;

	z.re = 0.0;
	z.im = 0.0;
	iter = -1;
	while (++iter < fractal->max_iters)
	{
		if (complex_modulus(z) > fractal->modulus)
			break ;
		z = complex_addition(complex_product(z, z), c);
	}
	return (color_from_iter(z, iter, fractal));
}

t_complex	conjugate(t_complex z)
{
	z.im *= -1;
	return (z);
}

int	render_tricorn(t_complex c, t_fractal *fractal)
{
	t_complex	z;
	int			iter;

	z.re = 0.0;
	z.im = 0.0;
	iter = -1;
	while (++iter < fractal->max_iters)
	{
		if (complex_modulus(z) > fractal->modulus)
			break ;
		z = complex_addition(complex_product(conjugate(z), conjugate(z)), c);
	}
	return (color_from_iter(z, iter, fractal));
}

int	render_multibrot(t_complex c, t_fractal *fractal)
{
	t_complex	z;
	int			iter;

	z.re = 0.0;
	z.im = 0.0;
	iter = -1;
	while (++iter < fractal->max_iters)
	{
		if (complex_modulus(z) > fractal->modulus)
			break ;
		z = complex_addition(complex_power(z, fractal->multibrot_exponent), c);
	}
	return (color_from_iter(z, iter, fractal));
}

int	render_julia(t_complex z, t_fractal *fractal)
{
	int	iter;

	iter = -1;
	while (++iter < fractal->max_iters)
	{
		if (complex_modulus(z) > fractal->modulus)
			break ;
		z = complex_addition(complex_product(z, z), fractal->julia_c);
	}
	return (color_from_iter(z, iter, fractal));
}

double	rescale(double coord, t_scale scale)
{
	double	image;

	image = coord - scale.old_min;
	image *= scale.new_max - scale.new_min;
	image /= scale.old_max - scale.old_min;
	image += scale.new_min;
	return (image);
}

int	unrescale(double image, t_scale scale)
{
	double	coord;

	coord = image - scale.new_min;
	coord *= scale.old_max - scale.old_min;
	coord /= scale.new_max - scale.new_min;
	coord += scale.old_min;
	return ((int)coord);
}

int	compute_fractal_clr(t_complex c, t_fractal *fractal)
{
	if (fractal->fractal_type == MANDELBROT)
		return (render_mandelbrot(c, fractal));
	else if (fractal->fractal_type == JULIA)
		return (render_julia(c, fractal));
	else if (fractal->fractal_type == MULTIBROT)
		return (render_multibrot(c, fractal));
	else if (fractal->fractal_type == TRICORN)
		return (render_tricorn(c, fractal));
	return (0);
}

void	render(t_vars *vars, int use_cache)
{
	static int		parity = -1;
	static t_scale	old_scale_re;
	static t_scale	old_scale_im;
	t_complex		c;
	int				x;
	int				y;
	int				color;
	int				old_x;
	int				old_y;

	y = 0;
	if (!use_cache)
		parity = -1;
	while (y < vars->fractal.img_height)
	{
		x = 0;
		while (x < vars->fractal.img_width)
		{
			c.re = rescale((double)x, vars->fractal.scale_re);
			c.im = rescale((double)y, vars->fractal.scale_im);
			old_x = unrescale(c.re, old_scale_re);
			old_y = unrescale(c.im, old_scale_im);
			if (parity || !use_cache)
			{
				if (parity != -1 && old_x >= 0 && old_x < vars->fractal.img_width
					&& old_y >= 0 && old_y < vars->fractal.img_height && use_cache)
					color = mlx_pixel_get_buf(&vars->img2, old_x, old_y);
				else
					color = compute_fractal_clr(c, &vars->fractal);
				mlx_pixel_put_buf(&vars->img, x, y, color);
			}
			else
			{
				if (old_x >= 0 && old_x < vars->fractal.img_width
					&& old_y >= 0 && old_y < vars->fractal.img_height)
					color = mlx_pixel_get_buf(&vars->img, old_x, old_y);
				else
					color = compute_fractal_clr(c, &vars->fractal);
				mlx_pixel_put_buf(&vars->img2, x, y, color);
			}
			x += 1;
		}
		y += 1;
	}
	if (parity || !use_cache)
		mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	else
		mlx_put_image_to_window(vars->mlx, vars->win, vars->img2.img, 0, 0);
	parity = !parity;
	old_scale_re = vars->fractal.scale_re;
	old_scale_im = vars->fractal.scale_im;
}

size_t	ft_strwidth(const char *s, size_t tabsize)
{
	size_t	size;

	size = 0;
	while (s && *s++)
	{
		if (*(s - 1) == '\t')
			size += tabsize;
		else
			++size;
	}
	return (size);
}

void	ft_putstr_mult(char *s, int n)
{
	while (n--)
		ft_putstr_fd(s, 1);
}

void	print_option(const char *option, const char *args, const char *desc)
{
	int	w;

	w = OPTIONS_WIDTH - (int)(ft_strwidth(option, 4) + ft_strwidth(args, 4));
	if (option && printf("    \033[34m%s\033[m", option))
		w -= 4;
	if (args && printf(" \033[33m%s\033[m", args))
		w -= 1;
	while (w-- > 0)
		printf(" ");
	while (*desc)
	{
		w = -1;
		while (*desc && ++w < DESC_WIDTH)
			printf("%c", *desc++);
		while (*desc && !ft_isspace(*desc))
			printf("%c", *desc++);
		printf("\n");
		if (*desc && *desc++)
			ft_putstr_mult(" ", OPTIONS_WIDTH);
	}
}

void	print_runtime_bindings(void)
{
	printf("\033[97mRUNTIME KEYBINDINGS:\033[m\n");
	print_option("", "hjkl or arrows", "Navigate the mandelbrot set");
	print_option("", "mouse left hold", "Navigate the mandelbrot set");
	print_option("", "mouse right hold", "Navigate the mandelbrot set");
	print_option("", "z or spacebar", "Zoom in");
	print_option("", "x", "Zoom out");
	print_option("", "r", "Reset viewport");
	print_option("", ".", "Increase the gradient phase");
	print_option("", ",", "Decrease the gradient phase");
	print_option("", "+", "Increase the number of iterations");
	print_option("", "-", "Decrease the number of iterations");
	print_option("", "q or esc", "Quit");
}

void	print_more_help(void)
{
	print_option("--title", "TITLE", "The title of the window.");
	print_option("--winsize", "WIDTHxHEIGHT", "The window dimensions. Greatly"
		"influences performance.");
	print_option("--real-intv", "min,max", "Min/max initial values for "
		"real axis. Window ratio is kept. Defines what zoom==1 is.");
	print_option("--gradient-phase", "P", "Colors are indexed through an array "
		"(modulo arr-length). This parameter adds an offset.");
	print_option("--iterations", "I", "Number of iterations to perform until a "
		"point is deemed part of the set. More iterations -> deeper zoom.");
	print_option("--modulus", "R", "Minimum modulus (argument) of a complex "
		"number required to rule it out as not part of the set.");
	print_option("--help", NULL, "Show this help.\n");
}

void	print_help(char **argv)
{
	printf("\033[97mUSAGE:\033[m\n    \033[32m%s\033[m \033[34mOPTIONS\033[m\n\n"
		"\033[97mOPTIONS:\033[m\n", argv[0]);
	print_option("--mandelbrot", NULL,
		"Render the canonical mandelbrot set (z^2 + c).");
	print_option("--julia", "RE,IM",
		"Render the julia set for c = RE+IM*i. RE and IM are decimals"
		" with a dot for the thousands separator.");
	print_option("--multibrot", "D", "Render a multibrot (z^D + c). Beware that"
		" this is very slow due to using the cexp() function, which can"
		" also take decimal exponents.");
	print_option("--center", "RE,IM", "Make RE+IM*i the center of the window.");
	print_option("--zoom", "FACTOR", "Zoom the viewport by FACTOR.");
	print_option("--zoom-factor", "FACTOR", "The factor by which the factor "
			"will be changed.");
	print_option("--mvmt-speed", "SPEED", "Viewport translation speed/step.");
	print_more_help();
	print_runtime_bindings();
}


int	parse_help(int argc, char **argv)
{
	if (argc == 1)
		return (print_help(argv), 1);
	while (--argc)
		if (!ft_strncmp(argv[argc], "--help", 6))
			return (print_help(argv), 1);
	return (0);
}

double	ft_atof(const char *s)
{
	int		sign;
	double	number;
	double	power;

	number = 0.0;
	power = 1.0;
	while (ft_isspace(*s))
		++s;
	sign = ((*s != '-') - 1) * 2 + 1;
	if (*s == '-' || *s == '+')
		++s;
	while (ft_isdigit(*s))
	{
		number = 10.0 * number + (*s - '0');
		++s;
	}
	if (*s == '.')
		++s;
	while (ft_isdigit(*s))
	{
		number = 10.0 * number + (*s - '0');
		power *= 10.0;
		++s;
	}
	return (sign * number / power);
}

t_complex	parse_complex(const char *s)
{
	t_complex	z;
	int			i;

	z.re = ft_atof(s);
	i = -1;
	while (s[++i] && s[i] != ',')
		;
	if (!s[i])
		z.re = NAN;
	else
		z.im = ft_atof(s + i + 1);
	return (z);
}

void	init_fractal(t_fractal *fractal)
{
	fractal->fractal_type = MANDELBROT;
	fractal->scale_re.old_min = 0;
	fractal->scale_re.old_max = DEF_IMG_WIDTH;
	fractal->scale_re.new_min = DEF_MIN_RE;
	fractal->scale_re.new_max = DEF_MAX_RE;
	fractal->scale_im.old_min = 0;
	fractal->scale_im.old_max = DEF_IMG_HEIGHT;
	fractal->scale_im.new_min = -((DEF_MAX_RE - DEF_MIN_RE) * DEF_IMG_HEIGHT)
		/ (2. * DEF_IMG_WIDTH);
	fractal->scale_im.new_max = ((DEF_MAX_RE - DEF_MIN_RE) * DEF_IMG_HEIGHT)
		/ (2. * DEF_IMG_WIDTH);
	fractal->min_re = DEF_MIN_RE;
	fractal->max_re = DEF_MAX_RE;
	fractal->max_iters = DEF_MAX_ITERS;
	fractal->img_width = DEF_IMG_WIDTH;
	fractal->img_height = DEF_IMG_HEIGHT;
	fractal->gradient_phase = DEF_GRADIENT_PHASE;
	fractal->title = DEF_WIN_TITLE;
	fractal->modulus = DEF_MODULUS;
	fractal->zoom_factor = DEF_ZOOM_FACTOR;
	fractal->speed = DEF_SPEED;
}

void	parse_julia_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(printf("'%s' expects one parameter\n", **argv), exit(2));
	++(*argv);
	--(*argc);
	fractal->julia_c = parse_complex(**argv);
	if (fractal->julia_c.re == fractal->julia_c.re)
		fractal->fractal_type = JULIA;
	else
		(printf("Error parsing argument for '%s': NaN\n", **argv), exit(3));
}

void	parse_multibrot_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(printf("'%s' expects one parameter\n", **argv), exit(2));
	++(*argv);
	--(*argc);
	fractal->multibrot_exponent = ft_atof(**argv);
	if (fractal->multibrot_exponent == fractal->multibrot_exponent)
		fractal->fractal_type = MULTIBROT;
	else
		(printf("Error parsing argument for '%s': NaN\n", **argv), exit(3));
}

t_complex	parse_center_param(int *argc, char ***argv)
{
	t_complex	center;

	if (*argc < 2)
		(printf("'%s' expects one parameter\n", **argv), exit(2));
	++(*argv);
	--(*argc);
	center = parse_complex(**argv);
	if (center.re == center.re)
		return (center);
	else
		(printf("Error parsing argument for '%s': NaN\n", **argv), exit(3));
}

double	parse_zoom_param(int *argc, char ***argv)
{
	double	zoom;

	if (*argc < 2)
		(printf("'%s' expects one parameter\n", **argv), exit(2));
	++(*argv);
	--(*argc);
	zoom = ft_atof(**argv);
	if (zoom == zoom)
		return (zoom);
	else
		(printf("Error parsing argument for '%s': NaN\n", **argv), exit(3));
}

void	apply_viewport(t_complex center, double zoom, t_fractal *fractal)
{
	static double	diff_re;
	static double	diff_im;
	double			half;

	diff_re = fractal->max_re - fractal->min_re;
	diff_im = (fractal->max_re - fractal->min_re) * fractal->img_height
		/ fractal->img_width;
	half = diff_re / (2. * zoom);
	fractal->scale_re.new_min = center.re - half;
	fractal->scale_re.new_max = center.re + half;
	half = diff_im / (2. * zoom);
	fractal->scale_im.new_min = center.im - half;
	fractal->scale_im.new_max = center.im + half;
}

void	parse_winsize_param(int *argc, char ***argv, t_fractal *fractal)
{
	int	i;

	if (*argc < 2)
		(printf("'%s' expects one parameter\n", **argv), exit(2));
	++(*argv);
	--(*argc);
	fractal->img_width = ft_atoi(**argv);
	fractal->scale_re.old_max = fractal->img_width;
	i = -1;
	while ((**argv)[++i] && ft_tolower((**argv)[i]) != 'x')
		;
	if (!(**argv)[i])
		(printf("Error parsing argument for '%s': NaN\n", **argv), exit(3));
	else
	{
		fractal->img_height = ft_atoi(**argv + i + 1);
		fractal->scale_im.old_max = fractal->img_height;
	}
}

/* TODO
 * Only allow numeric arguments to atof/atoi
 */
void	parse_grad_phase_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(printf("'%s' expects one parameter\n", **argv), exit(2));
	++(*argv);
	--(*argc);
	fractal->gradient_phase = ft_atoi(**argv);
}

void	parse_iterations_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(printf("'%s' expects one parameter\n", **argv), exit(2));
	++(*argv);
	--(*argc);
	fractal->max_iters = ft_atoi(**argv);
}

void	parse_modulus_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(printf("'%s' expects one parameter\n", **argv), exit(2));
	++(*argv);
	--(*argc);
	fractal->modulus = ft_atoi(**argv);
}

void	parse_zoom_factor_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(printf("'%s' expects one parameter\n", **argv), exit(2));
	++(*argv);
	--(*argc);
	fractal->zoom_factor = ft_atof(**argv);
	if (fractal->zoom_factor != fractal->zoom_factor)
		(printf("Error parsing argument for '%s': NaN\n", **argv), exit(3));
}

void	parse_mvmt_speed_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(printf("'%s' expects one parameter\n", **argv), exit(2));
	++(*argv);
	--(*argc);
	fractal->speed = ft_atof(**argv);
	if (fractal->speed != fractal->speed)
		(printf("Error parsing argument for '%s': NaN\n", **argv), exit(3));
}

void	parse_title_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(printf("'%s' expects one parameter\n", **argv), exit(2));
	++(*argv);
	--(*argc);
	fractal->title = **argv;
}

void	parse_real_intv_param(int *argc, char ***argv, t_fractal *fractal)
{
	t_complex	dummy;

	if (*argc < 2)
		(printf("'%s' expects one parameter\n", **argv), exit(2));
	++(*argv);
	--(*argc);
	dummy = parse_complex(**argv);
	fractal->min_re = dummy.re;
	fractal->max_re = dummy.im;
	if (fractal->min_re != fractal->min_re)
		(printf("Error parsing argument for '%s': NaN\n", **argv), exit(3));
}

void	parse_more_options(int *argc, char ***argv, t_fractal *fractal)
{
	if (!ft_strncmp(**argv, "--winsize", 10))
		parse_winsize_param(argc, argv, fractal);
	else if (!ft_strncmp(**argv, "--gradient-phase", 17))
		parse_grad_phase_param(argc, argv, fractal);
	else if (!ft_strncmp(**argv, "--iterations", 13))
		parse_iterations_param(argc, argv, fractal);
	else if (!ft_strncmp(**argv, "--modulus", 10))
		parse_modulus_param(argc, argv, fractal);
	else if (!ft_strncmp(**argv, "--title", 8))
		parse_title_param(argc, argv, fractal);
	else if (!ft_strncmp(**argv, "--real-intv", 12))
		parse_real_intv_param(argc, argv, fractal);
	else if (!ft_strncmp(**argv, "--zoom-factor", 14))
		parse_zoom_factor_param(argc, argv, fractal);
	else if (!ft_strncmp(**argv, "--mvmt-speed", 13))
		parse_mvmt_speed_param(argc, argv, fractal);
}

t_fractal	parse_options(int argc, char **argv)
{
	t_fractal	fractal;
	t_complex	center;
	double		zoom;

	/* fractal.fractal_type = MANDELBROT; */
	init_fractal(&fractal);
	zoom = 1;
	center.re = (fractal.scale_re.new_max + fractal.scale_re.new_min) / 2.;
	center.im = (fractal.scale_im.new_max + fractal.scale_im.new_min) / 2.;
	while (--argc)
	{
		if (!ft_strncmp(*(++argv), "--mandelbrot", 13))
			fractal.fractal_type = MANDELBROT;
		else if (!ft_strncmp(*(argv), "--tricorn", 10))
			fractal.fractal_type = TRICORN;
		else if (!ft_strncmp(*(argv), "--julia", 8))
			parse_julia_param(&argc, &argv, &fractal);
		else if (!ft_strncmp(*(argv), "--multibrot", 12))
			parse_multibrot_param(&argc, &argv, &fractal);
		else if (!ft_strncmp(*(argv), "--center", 9))
			center = parse_center_param(&argc, &argv);
		else if (!ft_strncmp(*(argv), "--zoom", 7))
			zoom = parse_zoom_param(&argc, &argv);
		else
			parse_more_options(&argc, &argv, &fractal);
	}
	apply_viewport(center, zoom, &fractal);
	return (fractal);
}

int	close_mlx(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	/* mlx_destroy_display(vars->mlx); */
	exit(0);
}

int	zoom_middle(int kc, t_vars *vars)
{
	double	diff_re;
	double	diff_im;
	double	zoom_re;
	double	zoom_im;
	double	zoom;

	if (kc == 'z' || kc == ' ')
		zoom = vars->fractal.zoom_factor;
	else if (kc == 'x')
		zoom = 1 / vars->fractal.zoom_factor;
	else
		return (1);
	diff_re = vars->fractal.scale_re.new_max - vars->fractal.scale_re.new_min;
	diff_im = vars->fractal.scale_im.new_max - vars->fractal.scale_im.new_min;
	zoom_re = vars->fractal.scale_re.new_min + diff_re / 2.;
	zoom_im = vars->fractal.scale_im.new_min + diff_im / 2.;
	vars->fractal.scale_re.new_min = zoom_re - (vars->fractal.img_width / 2. / (double)vars->fractal.img_width) * diff_re / zoom;
	vars->fractal.scale_re.new_max = zoom_re + (1. - vars->fractal.img_width / 2. / (double)vars->fractal.img_width) * diff_re / zoom;
	vars->fractal.scale_im.new_min = zoom_im - (vars->fractal.img_height / 2. / (double)vars->fractal.img_height) * diff_im / zoom;
	vars->fractal.scale_im.new_max = zoom_im + (1. - vars->fractal.img_height / 2. / (double)vars->fractal.img_height) * diff_im / zoom;
	return (0);
}

int	zoom_to_mouse(int button, int x, int y, t_vars *vars)
{
	double	diff_re;
	double	diff_im;
	double	zoom_re;
	double	zoom_im;
	double	zoom;

	if (button == 4)
		zoom = vars->fractal.zoom_factor;
	else if (button == 5)
		zoom = 1 / vars->fractal.zoom_factor;
	else
		return (1);
	diff_re = vars->fractal.scale_re.new_max - vars->fractal.scale_re.new_min;
	diff_im = vars->fractal.scale_im.new_max - vars->fractal.scale_im.new_min;
	zoom_re = rescale(x, vars->fractal.scale_re);
	zoom_im = rescale(y, vars->fractal.scale_im);
	vars->fractal.scale_re.new_min = zoom_re - (x / (double)vars->fractal.img_width) * diff_re / zoom;
	vars->fractal.scale_re.new_max = zoom_re + (1. - x / (double)vars->fractal.img_width) * diff_re / zoom;
	vars->fractal.scale_im.new_min = zoom_im - (y / (double)vars->fractal.img_height) * diff_im / zoom;
	vars->fractal.scale_im.new_max = zoom_im + (1. - y / (double)vars->fractal.img_height) * diff_im / zoom;
	return (0);
}

int	translate(int kc, int x, int y, t_vars *vars)
{
	double	zoom;
	int		re;
	double	x_diff;
	double	y_diff;

	zoom = (vars->fractal.max_re - vars->fractal.min_re)
		/ (vars->fractal.scale_re.new_max - vars->fractal.scale_re.new_min);
	if (kc == -1)
	{
		x_diff = (double)(vars->dragging.x - x) * (vars->fractal.scale_re.new_max - vars->fractal.scale_re.new_min) / (double)vars->fractal.img_width;
		y_diff = (double)(vars->dragging.y - y) * (vars->fractal.scale_im.new_max - vars->fractal.scale_im.new_min) / (double)vars->fractal.img_height;
		vars->fractal.scale_re.new_min = vars->dragging.scale_re.new_min + x_diff;
		vars->fractal.scale_re.new_max = vars->dragging.scale_re.new_max + x_diff;
		vars->fractal.scale_im.new_min = vars->dragging.scale_im.new_min + y_diff;
		vars->fractal.scale_im.new_max = vars->dragging.scale_im.new_max + y_diff;
		return (0);
	}
	if (kc == 'h' || kc == 'k' || kc == KC_LEFT || kc == KC_UP)
		zoom *= -1;
	re = 1;
	if (kc == 'j' || kc == 'k' || kc == KC_UP || kc == KC_DOWN)
		re = 0;
	vars->fractal.scale_re.new_min += vars->fractal.speed / zoom * re;
	vars->fractal.scale_re.new_max += vars->fractal.speed / zoom * re;
	vars->fractal.scale_im.new_min += vars->fractal.speed / zoom * !re;
	vars->fractal.scale_im.new_max += vars->fractal.speed / zoom * !re;
	return (0);
}

int	reset_viewport(int kc, t_vars *vars)
{
	double	half_i;

	if (kc != '0')
		return (1);
	half_i = ((vars->fractal.max_re - vars->fractal.min_re)
			* vars->fractal.img_height) / (2. * vars->fractal.img_width);
	vars->fractal.scale_re.new_min = vars->fractal.min_re;
	vars->fractal.scale_re.new_max = vars->fractal.max_re;
	vars->fractal.scale_im.new_min = -half_i;
	vars->fractal.scale_im.new_max = half_i;
	return (0);
}

int	change_fractal(int kc, t_vars *vars)
{
	if (kc == '1')
		return (vars->fractal.fractal_type = MANDELBROT, 0);
	else if (kc == '2')
	{
		vars->fractal.julia_c.re = -0.7;
		vars->fractal.julia_c.im = 0.27015;
		return (vars->fractal.fractal_type = JULIA, 0);
	}
	else if (kc == '3')
	{
		vars->fractal.multibrot_exponent = 3.1415926535897932384626;
		return (vars->fractal.fractal_type = MULTIBROT, 0);
	}
	else if (kc == '4')
		return (vars->fractal.fractal_type = TRICORN, 0);
	return (1);
}

void	change_gradient_phase(int kc, t_vars *vars)
{
	if (kc == ',')
		vars->fractal.gradient_phase -= 100;
	else if (kc == '.')
		vars->fractal.gradient_phase += 100;
	if (vars->fractal.gradient_phase % 10 == 0)
		++vars->fractal.gradient_phase;
}

void	change_iterations(int kc, t_vars *vars)
{
	if (kc == '-')
		vars->fractal.max_iters -= 1;
	else if (kc == '=')
		vars->fractal.max_iters += 1;
	if (vars->fractal.max_iters < 1)
		vars->fractal.max_iters = 1;
	printf("Max iterations: %d\n", vars->fractal.max_iters);
}

int	keydown_hook(int kc, t_vars *vars)
{
	int	ret;

	ret = 1;
	if (kc == 'z' || kc == ' ' || kc == 'x')
		ret = zoom_middle(kc, vars);
	else if (kc == '0')
		ret = reset_viewport(kc, vars);
	else if (kc == KC_ESC || kc == 'q')
		ret = close_mlx(vars);
	else if (kc == 'h' || kc == 'j' || kc == 'k'
		|| kc == 'l' || (kc >= KC_LEFT && kc <= KC_DOWN))
		ret = translate(kc, -1, -1, vars);
	else if (kc == 'r')
		return (render(vars, 0), ret);
	else if (kc >= '1' && kc <= '9')
		ret = change_fractal(kc, vars);
	else if (kc == ',' || kc == '.')
		return (change_gradient_phase(kc, vars), render(vars, 0), 0);
	else if (kc == '-' || kc == '=')
		change_iterations(kc, vars);
	return (render(vars, 1), ret);
}

int	mouse_down_hook(int button, int x, int y, t_vars *vars)
{
	int	ret;

	ret = 1;
	if (button == 4 || button == 5)
		ret = zoom_to_mouse(button, x, y, vars);
	else if (button == 1)
	{
		vars->dragging.x = x;
		vars->dragging.y = y;
		vars->dragging.scale_re = vars->fractal.scale_re;
		vars->dragging.scale_im = vars->fractal.scale_im;
		return (render(vars, 0), 0);
	}
	return (render(vars, 1), ret);
}

int	mouse_move_hook(int x, int y, t_vars *vars)
{
	translate(-1, x, y, vars);
	return (render(vars, 1), 0);
}

void	setup_hooks(t_vars *vars)
{
	mlx_hook(vars->win, 2, 1L << 0, keydown_hook, vars);
	mlx_hook(vars->win, 4, 1L << 2, mouse_down_hook, vars);
	mlx_hook(vars->win, 6, 1L << 8, mouse_move_hook, vars);
	mlx_hook(vars->win, 17, 0L, close_mlx, vars);
}

void	init(t_vars *vars)
{
	vars->mlx = mlx_init();
	vars->win = mlx_new_window(vars->mlx, vars->fractal.img_width,
			vars->fractal.img_height, vars->fractal.title);
	vars->img.img = mlx_new_image(vars->mlx, vars->fractal.img_width,
			vars->fractal.img_height);
	vars->img.addr = mlx_get_data_addr(vars->img.img, &vars->img.bpp,
			&vars->img.line_length, &vars->img.endian);
	vars->img2.img = mlx_new_image(vars->mlx, vars->fractal.img_width,
			vars->fractal.img_height);
	vars->img2.addr = mlx_get_data_addr(vars->img2.img, &vars->img2.bpp,
			&vars->img2.line_length, &vars->img2.endian);
}

int	main(int argc, char **argv)
{
	t_vars	vars;

	if (parse_help(argc, argv))
		return (1);
	vars.fractal = parse_options(argc, argv);
	vars.fractal.gradient_phase = 100;
	vars.fractal.max_iters = 100;
	init(&vars);
	render(&vars, 0);
	setup_hooks(&vars);
	mlx_loop(vars.mlx);
	return (0);
}
