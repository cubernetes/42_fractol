/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 14:17:25 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/02 09:39:07 by tosuman          ###   ########.fr       */
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

#define OPTIONS_WIDTH 27
#define DESC_WIDTH 40

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
	double		preimage_min;
	double		preimage_max;
	double		img_min;
	double		img_max;
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
	double		mvmt_speed;
}				t_fractal;

typedef struct s_rgb
{
	int			r;
	int			g;
	int			b;
}				t_rgb;

typedef struct s_vars
{
	void		*mlx;
	void		*win;
	t_img		img;
	t_img		img2;
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

	if (!colors[0])
	{
		init_colors(colors, fractal);
	}
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

	image = coord - scale.preimage_min;
	image *= scale.img_max - scale.img_min;
	image /= scale.preimage_max - scale.preimage_min;
	image += scale.img_min;
	return (image);
}

int	unrescale(double image, t_scale scale)
{
	double	coord;

	coord = image - scale.img_min;
	coord *= scale.preimage_max - scale.preimage_min;
	coord /= scale.img_max - scale.img_min;
	coord += scale.preimage_min;
	return ((int)coord);
}

int	simple(t_complex c)
{
	t_rgb	rgb;

	rgb.r = (int)(c.re + c.im);
	rgb.g = (int)(c.re + c.im * c.im);
	rgb.b = (int)(c.re + c.im * c.re);
	return (int_from_rgb(&rgb));
}

int	compute_fractal_clr(t_complex c, t_fractal *fractal)
{
	/* return (simple(c)); */
	if (fractal->fractal_type == MANDELBROT)
		return (render_mandelbrot(c, fractal));
	else if (fractal->fractal_type == JULIA)
		return (render_julia(c, fractal));
	else if (fractal->fractal_type == MULTIBROT)
		return (render_multibrot(c, fractal));
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

	printf("%f\n", vars->fractal.scale_re.img_min);
	printf("%f\n", vars->fractal.scale_re.img_max);
	printf("pre %f\n", vars->fractal.scale_re.preimage_min);
	printf("pre %f\n", vars->fractal.scale_re.preimage_max);
	printf("%f\n", vars->fractal.scale_im.img_min);
	printf("%f\n", vars->fractal.scale_im.img_max);
	printf("pre %f\n", vars->fractal.scale_im.preimage_min);
	printf("pre %f\n", vars->fractal.scale_im.preimage_max);
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
	print_option("", "r", "reset_viewport viewport");
	print_option("", ".", "Increase the gradient phase");
	print_option("", ",", "Decrease the gradient phase");
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
	fractal->scale_re.preimage_min = 0;
	fractal->scale_re.preimage_max = DEF_IMG_WIDTH;
	fractal->scale_re.img_min = DEF_MIN_RE;
	fractal->scale_re.img_max = DEF_MAX_RE;
	fractal->scale_im.preimage_min = 0;
	fractal->scale_im.preimage_max = DEF_IMG_HEIGHT;
	fractal->scale_im.img_min = -((DEF_MAX_RE - DEF_MIN_RE) * DEF_IMG_HEIGHT)
		/ (2. * DEF_IMG_WIDTH);
	fractal->scale_im.img_max = ((DEF_MAX_RE - DEF_MIN_RE) * DEF_IMG_HEIGHT)
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
	fractal->mvmt_speed = DEF_SPEED;
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
	fractal->scale_re.img_min = center.re - half;
	fractal->scale_re.img_max = center.re + half;
	half = diff_im / (2. * zoom);
	fractal->scale_im.img_min = center.im - half;
	fractal->scale_im.img_max = center.im + half;
}

void	parse_winsize_param(int *argc, char ***argv, t_fractal *fractal)
{
	int	i;

	if (*argc < 2)
		(printf("'%s' expects one parameter\n", **argv), exit(2));
	++(*argv);
	--(*argc);
	fractal->img_width = ft_atoi(**argv);
	i = -1;
	while ((**argv)[++i] && ft_tolower((**argv)[i]) != 'x')
		;
	if (!(**argv)[i])
		(printf("Error parsing argument for '%s': NaN\n", **argv), exit(3));
	else
		fractal->img_height = ft_atoi(**argv + i + 1);
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
	fractal->mvmt_speed = ft_atof(**argv);
	if (fractal->mvmt_speed != fractal->mvmt_speed)
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

	init_fractal(&fractal);
	zoom = 1;
	center.re = (fractal.scale_re.img_max + fractal.scale_re.img_min) / 2.;
	center.im = (fractal.scale_im.img_max + fractal.scale_im.img_min) / 2.;
	while (--argc)
	{
		if (!ft_strncmp(*(++argv), "--mandelbrot", 13))
			fractal.fractal_type = MANDELBROT;
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

int	close_mlx(int keycode, t_vars *vars)
{
	if (keycode != 65307 && keycode != 'q')
		return (1);
	mlx_destroy_window(vars->mlx, vars->win);
	exit(0);
}

int	zoom_viewport(int keycode, t_vars *vars)
{
	double	zoom;
	double	diff_re;
	double	mid_re;
	double	diff_im;
	double	mid_im;

	printf("scale re img min: %f\n", vars->fractal.scale_re.img_min);
	printf("scale re img max: %f\n", vars->fractal.scale_re.img_max);
	printf("scale re preimg min: %f\n", vars->fractal.scale_re.preimage_min);
	printf("scale re preimg max: %f\n", vars->fractal.scale_re.preimage_max);
	if (keycode != 'z' && keycode != ' ' && keycode != 'x')
		return (1);
	diff_re = vars->fractal.scale_re.img_max - vars->fractal.scale_re.img_min;
	printf("diff: %f\n", diff_re);
	diff_im = vars->fractal.scale_im.img_max - vars->fractal.scale_im.img_min;
	zoom = (vars->fractal.max_re - vars->fractal.min_re) / diff_re;
	if (keycode == 'z' || keycode == ' ')
		zoom *= vars->fractal.zoom_factor;
	else if (keycode == 'x')
		zoom /= vars->fractal.zoom_factor;
	mid_re = vars->fractal.scale_re.img_min + diff_re / 2.;
	mid_im = vars->fractal.scale_im.img_min + diff_im / 2.;
	diff_re = vars->fractal.max_re - vars->fractal.min_re;
	diff_im = diff_re * vars->fractal.img_height / vars->fractal.img_width;
	vars->fractal.scale_re.img_min = mid_re - diff_re / (2. * zoom);
	vars->fractal.scale_re.img_max = mid_re + diff_re / (2. * zoom);
	vars->fractal.scale_im.img_min = mid_im - diff_im / (2. * zoom);
	vars->fractal.scale_im.img_max = mid_im + diff_im / (2. * zoom);
	return (0);
}

int	translate(int keycode, t_vars *vars)
{
	double	zoom;

	zoom = (vars->fractal.max_re - vars->fractal.min_re)
		/ (vars->fractal.scale_re.img_max - vars->fractal.scale_re.img_min);
	if (keycode == 'h')
	{
		vars->fractal.scale_re.img_min -= vars->fractal.mvmt_speed / zoom;
		vars->fractal.scale_re.img_max -= vars->fractal.mvmt_speed / zoom;
	}
	else if (keycode == 'j')
	{
		vars->fractal.scale_im.img_min += vars->fractal.mvmt_speed / zoom;
		vars->fractal.scale_im.img_max += vars->fractal.mvmt_speed / zoom;
	}
	else if (keycode == 'k')
	{
		vars->fractal.scale_im.img_min -= vars->fractal.mvmt_speed / zoom;
		vars->fractal.scale_im.img_max -= vars->fractal.mvmt_speed / zoom;
	}
	else if (keycode == 'l')
	{
		vars->fractal.scale_re.img_min += vars->fractal.mvmt_speed / zoom;
		vars->fractal.scale_re.img_max += vars->fractal.mvmt_speed / zoom;
	}
	return (0);
}

int	reset_viewport(int keycode, t_vars *vars)
{
	double	half_i;

	if (keycode != '0')
		return (1);
	half_i = ((vars->fractal.max_re - vars->fractal.min_re)
			* vars->fractal.img_height) / (2. * vars->fractal.img_width);
	vars->fractal.scale_re.img_min = vars->fractal.min_re;
	vars->fractal.scale_re.img_max = vars->fractal.max_re;
	vars->fractal.scale_im.img_min = -half_i;
	vars->fractal.scale_im.img_max = half_i;
	return (0);
}

int	handle_keypress(int keycode, t_vars *vars)
{
	int	ret;

	ret = 1;
	if (keycode == 'z' || keycode == ' ' || keycode == 'x')
		ret = zoom_viewport(keycode, vars);
	else if (keycode == '0')
		ret = reset_viewport(keycode, vars);
	else if (keycode == 65307 || keycode == 'q')
		ret = close_mlx(keycode, vars);
	else if (keycode == 'h' || keycode == 'j' || keycode == 'k' || keycode == 'l')
		ret = translate(keycode, vars);
	else if (keycode == 'r')
		return (render(vars, 0), ret);
	return (render(vars, 1), ret);
}

void	setup_hooks(t_vars *vars)
{
	mlx_hook(vars->win, 2, 1L << 0, handle_keypress, vars);
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
	init(&vars);
	render(&vars, 0);
	setup_hooks(&vars);
	mlx_loop(vars.mlx);
	return (0);
}
