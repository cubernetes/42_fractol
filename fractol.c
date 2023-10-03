/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 14:17:25 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/03 04:33:49 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "./libft/libft.h"
#include "./minilibx-linux/mlx.h"
#include "minilibx-linux/mlx_int.h"
/* #include <libft.h> */
/* #include <mlx.h> */
#include <X11/X.h>
#include <X11/Xlib.h>
#include <math.h>
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	setup_hooks(t_vars *vars);

int	ft_strcmp(char const *s1, char const *s2)
{
	unsigned char	*us1;
	unsigned char	*us2;

	us1 = (unsigned char *) s1;
	us2 = (unsigned char *) s2;
	while (*us1 && *us2 && *us1 == *us2)
	{
		++us1;
		++us2;
	}
	return (*us1 - *us2);
}

void	mlx_pixel_put_buf(t_fimg *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bpp / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

int	mlx_pixel_get_buf(t_fimg *data, int x, int y)
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

	if (1 == color_idx)
		(rgb_from_int(&rgb_start, COLOR_1), rgb_from_int(&rgb_end, COLOR_2));
	else if (2 == color_idx)
		(rgb_from_int(&rgb_start, COLOR_2), rgb_from_int(&rgb_end, COLOR_3));
	else if (3 == color_idx)
		(rgb_from_int(&rgb_start, COLOR_3), rgb_from_int(&rgb_end, COLOR_4));
	else if (4 == color_idx)
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

void	init_colors(int colors[GRAD_SIZE], t_fractal *fractal)
{
	int	i;
	int	off;

	if (1 == fractal->gradient_phase % 10)
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
	static int	colors[GRAD_SIZE];
	double		smoothed;
	int			color_idx;

	if (1 == fractal->gradient_phase % 10 || !colors[0])
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
	if (MANDELBROT == fractal->fractal_type)
		return (render_mandelbrot(c, fractal));
	else if (JULIA == fractal->fractal_type)
		return (render_julia(c, fractal));
	else if (MULTIBROT == fractal->fractal_type)
		return (render_multibrot(c, fractal));
	else if (TRICORN == fractal->fractal_type)
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

	vars->render_mutex = True;
	if (!use_cache)
		parity = -1;
	y = -1;
	while (++y < vars->fractal.img_height)
	{
		x = -1;
		while (++x < vars->fractal.img_width)
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
		}
	}
	if (parity || !use_cache)
		mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	else
		mlx_put_image_to_window(vars->mlx, vars->win, vars->img2.img, 0, 0);
	parity = !parity;
	old_scale_im = vars->fractal.scale_im;
	old_scale_re = vars->fractal.scale_re;
	vars->render_mutex = False;
}

size_t	ft_strwidth(const char *s, size_t tabsize)
{
	size_t	size;

	size = 0;
	while (s && *s++)
	{
		if ('\t' == *(s - 1))
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
	if (option && dprintf(1, "    \033[34m%s\033[m", option))
		w -= 4;
	if (args && dprintf(1, " \033[33m%s\033[m", args))
		w -= 1;
	while (w-- > 0)
		dprintf(1, " ");
	while (*desc)
	{
		w = -1;
		while (*desc && ++w < DESC_WIDTH)
			dprintf(1, "%c", *desc++);
		while (*desc && !ft_isspace(*desc))
			dprintf(1, "%c", *desc++);
		dprintf(1, "\n");
		if (*desc && *desc++)
			ft_putstr_mult(" ", OPTIONS_WIDTH);
	}
}

void	print_runtime_bindings(void)
{
	dprintf(1, "\033[97mRUNTIME KEYBINDINGS:\033[m\n");
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
	dprintf(1, "\033[97mUSAGE:\033[m\n    \033[32m%s\033[m \033[34mOPTIONS\033[m\n\n"
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
	if (1 == argc)
		return (print_help(argv), 1);
	while (--argc)
		if (!ft_strcmp(argv[argc], "--help"))
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
	sign = (('-' != *s) - 1) * 2 + 1;
	if ('-' == *s || '+' == *s)
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
	fractal->julia_c.re = -0.7;
	fractal->julia_c.im = 0.27015;
	fractal->multibrot_exponent = 3.1415926535897932384626;
}

void	parse_julia_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(dprintf(2, "'%s' expects one parameter\n", **argv), exit(EXIT_MISSING_PARAM));
	++(*argv);
	--(*argc);
	fractal->julia_c = parse_complex(**argv);
	if (fractal->julia_c.re == fractal->julia_c.re)
		fractal->fractal_type = JULIA;
	else
		(dprintf(2, "Error parsing argument for '%s': NaN\n", **argv), exit(EXIT_PARAM_NAN));
}

void	parse_multibrot_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(dprintf(2, "'%s' expects one parameter\n", **argv), exit(EXIT_MISSING_PARAM));
	++(*argv);
	--(*argc);
	fractal->multibrot_exponent = ft_atof(**argv);
	if (fractal->multibrot_exponent == fractal->multibrot_exponent)
		fractal->fractal_type = MULTIBROT;
	else
		(dprintf(2, "Error parsing argument for '%s': NaN\n", **argv), exit(EXIT_PARAM_NAN));
}

t_complex	parse_center_param(int *argc, char ***argv)
{
	t_complex	center;

	if (*argc < 2)
		(dprintf(2, "'%s' expects one parameter\n", **argv), exit(EXIT_MISSING_PARAM));
	++(*argv);
	--(*argc);
	center = parse_complex(**argv);
	if (center.re == center.re)
		return (center);
	else
		(dprintf(2, "Error parsing argument for '%s': NaN\n", **argv), exit(EXIT_PARAM_NAN));
}

double	parse_zoom_param(int *argc, char ***argv)
{
	double	zoom;

	if (*argc < 2)
		(dprintf(2, "'%s' expects one parameter\n", **argv), exit(EXIT_MISSING_PARAM));
	++(*argv);
	--(*argc);
	zoom = ft_atof(**argv);
	if (zoom == zoom)
		return (zoom);
	else
		(dprintf(2, "Error parsing argument for '%s': NaN\n", **argv), exit(EXIT_PARAM_NAN));
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
		(dprintf(2, "'%s' expects one parameter\n", **argv), exit(EXIT_MISSING_PARAM));
	++(*argv);
	--(*argc);
	fractal->img_width = ft_atoi(**argv);
	fractal->scale_re.old_max = fractal->img_width;
	i = -1;
	while ((**argv)[++i] && ft_tolower((**argv)[i]) != 'x')
		;
	if (!(**argv)[i])
		(dprintf(2, "Error parsing argument for '%s': NaN\n", **argv), exit(EXIT_PARAM_NAN));
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
		(dprintf(2, "'%s' expects one parameter\n", **argv), exit(EXIT_MISSING_PARAM));
	++(*argv);
	--(*argc);
	fractal->gradient_phase = ft_atoi(**argv);
}

void	parse_iterations_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(dprintf(2, "'%s' expects one parameter\n", **argv), exit(EXIT_MISSING_PARAM));
	++(*argv);
	--(*argc);
	fractal->max_iters = ft_atoi(**argv);
}

void	parse_modulus_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(dprintf(2, "'%s' expects one parameter\n", **argv), exit(EXIT_MISSING_PARAM));
	++(*argv);
	--(*argc);
	fractal->modulus = ft_atoi(**argv);
}

void	parse_zoom_factor_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(dprintf(2, "'%s' expects one parameter\n", **argv), exit(EXIT_MISSING_PARAM));
	++(*argv);
	--(*argc);
	fractal->zoom_factor = ft_atof(**argv);
	if (fractal->zoom_factor != fractal->zoom_factor)
		(dprintf(2, "Error parsing argument for '%s': NaN\n", **argv), exit(EXIT_PARAM_NAN));
}

void	parse_mvmt_speed_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(dprintf(2, "'%s' expects one parameter\n", **argv), exit(EXIT_MISSING_PARAM));
	++(*argv);
	--(*argc);
	fractal->speed = ft_atof(**argv);
	if (fractal->speed != fractal->speed)
		(dprintf(2, "Error parsing argument for '%s': NaN\n", **argv), exit(EXIT_PARAM_NAN));
}

void	parse_title_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(dprintf(2, "'%s' expects one parameter\n", **argv), exit(EXIT_MISSING_PARAM));
	++(*argv);
	--(*argc);
	fractal->title = **argv;
}

void	parse_real_intv_param(int *argc, char ***argv, t_fractal *fractal)
{
	t_complex	dummy;

	if (*argc < 2)
		(dprintf(2, "'%s' expects one parameter\n", **argv), exit(EXIT_MISSING_PARAM));
	++(*argv);
	--(*argc);
	dummy = parse_complex(**argv);
	fractal->min_re = dummy.re;
	fractal->max_re = dummy.im;
	if (fractal->min_re != fractal->min_re)
		(dprintf(2, "Error parsing argument for '%s': NaN\n", **argv), exit(EXIT_PARAM_NAN));
}

void	parse_more_options(int *argc, char ***argv, t_fractal *fractal)
{
	if (!ft_strcmp(**argv, "--winsize"))
		parse_winsize_param(argc, argv, fractal);
	else if (!ft_strcmp(**argv, "--gradient-phase"))
		parse_grad_phase_param(argc, argv, fractal);
	else if (!ft_strcmp(**argv, "--iterations"))
		parse_iterations_param(argc, argv, fractal);
	else if (!ft_strcmp(**argv, "--modulus"))
		parse_modulus_param(argc, argv, fractal);
	else if (!ft_strcmp(**argv, "--title"))
		parse_title_param(argc, argv, fractal);
	else if (!ft_strcmp(**argv, "--real-intv"))
		parse_real_intv_param(argc, argv, fractal);
	else if (!ft_strcmp(**argv, "--zoom-factor"))
		parse_zoom_factor_param(argc, argv, fractal);
	else if (!ft_strcmp(**argv, "--mvmt-speed"))
		parse_mvmt_speed_param(argc, argv, fractal);
}

t_fractal	parse_options(int argc, char **argv)
{
	t_fractal	fractal;
	t_complex	center;
	double		zoom;

	init_fractal(&fractal);
	zoom = 1.;
	center.re = (fractal.scale_re.new_max + fractal.scale_re.new_min) / 2.;
	center.im = (fractal.scale_im.new_max + fractal.scale_im.new_min) / 2.;
	while (--argc)
	{
		if (!ft_strcmp(*(++argv), "--mandelbrot"))
			fractal.fractal_type = MANDELBROT;
		else if (!ft_strcmp(*(argv), "--tricorn"))
			fractal.fractal_type = TRICORN;
		else if (!ft_strcmp(*(argv), "--julia"))
			parse_julia_param(&argc, &argv, &fractal);
		else if (!ft_strcmp(*(argv), "--multibrot"))
			parse_multibrot_param(&argc, &argv, &fractal);
		else if (!ft_strcmp(*(argv), "--center"))
			center = parse_center_param(&argc, &argv);
		else if (!ft_strcmp(*(argv), "--zoom"))
			zoom = parse_zoom_param(&argc, &argv);
		else
			parse_more_options(&argc, &argv, &fractal);
	}
	apply_viewport(center, zoom, &fractal);
	return (fractal);
}

int	close_mlx(t_vars *vars)
{
	mlx_destroy_image(vars->mlx, vars->img.img);
	mlx_destroy_image(vars->mlx, vars->img2.img);
	mlx_destroy_window(vars->mlx, vars->win);
	mlx_destroy_display(vars->mlx);
	free(vars->mlx);
	exit(EXIT_SUCCESS);
}

int	zoom_middle(int kc, t_vars *vars)
{
	double	diff_re;
	double	diff_im;
	double	zoom_re;
	double	zoom_im;
	double	zoom;

	if ('z' == kc || ' ' == kc)
		zoom = vars->fractal.zoom_factor;
	else if ('x' == kc)
		zoom = 1 / vars->fractal.zoom_factor;
	else
		return (1);
	diff_re = vars->fractal.scale_re.new_max - vars->fractal.scale_re.new_min;
	diff_im = vars->fractal.scale_im.new_max - vars->fractal.scale_im.new_min;
	zoom_re = vars->fractal.scale_re.new_min + diff_re / 2.;
	zoom_im = vars->fractal.scale_im.new_min + diff_im / 2.;
	vars->fractal.scale_re.new_min = zoom_re - (vars->fractal.img_width
			/ 2. / (double)vars->fractal.img_width) * diff_re / zoom;
	vars->fractal.scale_re.new_max = zoom_re + (1. - vars->fractal.img_width
			/ 2. / (double)vars->fractal.img_width) * diff_re / zoom;
	vars->fractal.scale_im.new_min = zoom_im - (vars->fractal.img_height
			/ 2. / (double)vars->fractal.img_height) * diff_im / zoom;
	vars->fractal.scale_im.new_max = zoom_im + (1. - vars->fractal.img_height
			/ 2. / (double)vars->fractal.img_height) * diff_im / zoom;
	return (0);
}

int	zoom_to_mouse(int button, int x, int y, t_vars *vars)
{
	double	diff_re;
	double	diff_im;
	double	zoom_re;
	double	zoom_im;
	double	zoom;

	if (4 == button)
		zoom = vars->fractal.zoom_factor;
	else if (5 == button)
		zoom = 1 / vars->fractal.zoom_factor;
	else
		return (1);
	diff_re = vars->fractal.scale_re.new_max - vars->fractal.scale_re.new_min;
	diff_im = vars->fractal.scale_im.new_max - vars->fractal.scale_im.new_min;
	zoom_re = rescale(x, vars->fractal.scale_re);
	zoom_im = rescale(y, vars->fractal.scale_im);
	vars->fractal.scale_re.new_min = zoom_re - (x
			/ (double)vars->fractal.img_width) * diff_re / zoom;
	vars->fractal.scale_re.new_max = zoom_re + (1. - x
			/ (double)vars->fractal.img_width) * diff_re / zoom;
	vars->fractal.scale_im.new_min = zoom_im - (y
			/ (double)vars->fractal.img_height) * diff_im / zoom;
	vars->fractal.scale_im.new_max = zoom_im + (1. - y
			/ (double)vars->fractal.img_height) * diff_im / zoom;
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
	if (-1 == kc)
	{
		if (-1 == vars->dragging.x)
		{
			vars->dragging.x = x;
			vars->dragging.y = y;
		}
		x_diff = (double)(vars->dragging.x - x)
			* (vars->fractal.scale_re.new_max - vars->fractal.scale_re.new_min)
			/ (double)vars->fractal.img_width;
		y_diff = (double)(vars->dragging.y - y)
			* (vars->fractal.scale_im.new_max - vars->fractal.scale_im.new_min)
			/ (double)vars->fractal.img_height;
		vars->fractal.scale_re.new_min = vars->dragging.scale_re.new_min
			+ x_diff;
		vars->fractal.scale_re.new_max = vars->dragging.scale_re.new_max
			+ x_diff;
		vars->fractal.scale_im.new_min = vars->dragging.scale_im.new_min
			+ y_diff;
		vars->fractal.scale_im.new_max = vars->dragging.scale_im.new_max
			+ y_diff;
		return (0);
	}
	if ('h' == kc || 'k' == kc || KC_LEFT == kc || KC_UP == kc)
		zoom *= -1;
	re = 1;
	if ('j' == kc || 'k' == kc || KC_UP == kc || KC_DOWN == kc)
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

void	change_fractal(int kc, t_vars *vars)
{
	if ('1' == kc)
		vars->fractal.fractal_type = MANDELBROT;
	else if ('2' == kc)
		vars->fractal.fractal_type = JULIA;
	else if ('3' == kc)
		vars->fractal.fractal_type = MULTIBROT;
	else if ('4' == kc)
		vars->fractal.fractal_type = TRICORN;
}

void	change_gradient_phase(int kc, t_vars *vars)
{
	if (',' == kc)
		vars->fractal.gradient_phase -= GRADIENT_PHASE_INC;
	else if ('.' == kc)
		vars->fractal.gradient_phase += GRADIENT_PHASE_INC;
	if (0 == vars->fractal.gradient_phase % 10)
		++vars->fractal.gradient_phase;
}

void	change_iterations(int kc, t_vars *vars)
{
	if ('-' == kc)
		vars->fractal.max_iters -= ITERS_INC;
	else if ('=' == kc)
		vars->fractal.max_iters += ITERS_INC;
	if (vars->fractal.max_iters < 1)
		vars->fractal.max_iters = MIN_ITERS;
	dprintf(1, "Max iterations: %d\n", vars->fractal.max_iters);
}

int	keydown_hook(int kc, t_vars *vars)
{
	int	ret;

	ret = 1;
	if ('z' == kc || ' ' == kc || 'x' == kc)
		ret = zoom_middle(kc, vars);
	else if ('0' == kc)
		ret = reset_viewport(kc, vars);
	else if (XK_Escape == kc || 'q' == kc)
		ret = close_mlx(vars);
	else if ('h' == kc || 'j' == kc || 'k' == kc
		|| 'l' == kc || (XK_Left == kc || XK_Up == kc
			|| XK_Right == kc || XK_Down == kc))
		ret = translate(kc, -1, -1, vars);
	else if ('r' == kc)
		return (render(vars, NO_CACHE), ret);
	else if ('1' <= kc && '9' >= kc)
		return (change_fractal(kc, vars), render(vars, NO_CACHE), 0);
	else if (',' == kc || '.' == kc)
		return (change_gradient_phase(kc, vars), render(vars, NO_CACHE), 0);
	else if ('-' == kc || '=' == kc)
		change_iterations(kc, vars);
	return (render(vars, USE_CACHE), ret);
}

int	mouse_down_hook(int button, int x, int y, t_vars *vars)
{
	int	ret;

	ret = 1;
	if (Button4 == button || Button5 == button)
		ret = zoom_to_mouse(button, x, y, vars);
	else if (Button1 == button)
	{
		vars->dragging.x = x;
		vars->dragging.y = y;
		vars->dragging.scale_re = vars->fractal.scale_re;
		vars->dragging.scale_im = vars->fractal.scale_im;
		vars->mouse_down = True;
		return (render(vars, NO_CACHE), 0);
	}
	return (render(vars, USE_CACHE), ret);
}

int	mouse_up_hook(int button, int x, int y, t_vars *vars)
{
	(void)x;
	(void)y;
	if (Button1 == button)
	{
		vars->mouse_down = False;
		return (0);
	}
	return (1);
}

int	mouse_move_hook(int x, int y, t_vars *vars)
{
	if (vars->render_mutex || !vars->mouse_down)
		return (1);
	translate(-1, x, y, vars);
	return (render(vars, USE_CACHE), 0);
}

void	init_images(t_vars *vars)
{
	vars->img.img = mlx_new_image(vars->mlx, vars->fractal.img_width,
			vars->fractal.img_height);
	vars->img.addr = mlx_get_data_addr(vars->img.img, &vars->img.bpp,
			&vars->img.line_length, &vars->img.endian);
	vars->img2.img = mlx_new_image(vars->mlx, vars->fractal.img_width,
			vars->fractal.img_height);
	vars->img2.addr = mlx_get_data_addr(vars->img2.img, &vars->img2.bpp,
			&vars->img2.line_length, &vars->img2.endian);
}

int	resize_img(t_vars *vars)
{
	int		width;
	int		height;
	Status	status;

	status = mlx_get_window_dim(vars->mlx, vars->win, &width, &height);
	if ((vars->fractal.img_width == width && vars->fractal.img_height == height)
		|| !status)
		return (1);
	vars->fractal.img_width = width;
	vars->fractal.img_height = height;
	vars->fractal.scale_re.old_max = width;
	vars->fractal.scale_im.old_max = height;
	mlx_destroy_image(vars->mlx, vars->img.img);
	mlx_destroy_image(vars->mlx, vars->img2.img);
	init_images(vars);
	setup_hooks(vars);
	return (render(vars, NO_CACHE), 0);
}

void	setup_hooks(t_vars *vars)
{
	mlx_hook(vars->win, KeyPress, KeyPressMask, keydown_hook, vars);
	mlx_hook(vars->win, KeyRelease, ButtonPressMask, mouse_down_hook, vars);
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
