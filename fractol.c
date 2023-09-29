/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 14:17:25 by tosuman           #+#    #+#             */
/*   Updated: 2023/09/29 06:37:44 by tosuman          ###   ########.fr       */
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

#define IMG_WIDTH 1920
#define IMG_HEIGHT 1080
#define MIN_RE -3
#define MAX_RE 2
#define WIN_TITLE "fractol"

#define MAX_ITER 300
#define MAX_MODULUS 4

/* Same colorscheme that wikipedia uses */
#define COLOR_1 0x00000764
#define COLOR_2 0x00206BCB
#define COLOR_3 0x00EDFFFF
#define COLOR_4 0x00FFAA00
#define COLOR_5 0x00000200
#define CRES 2048
/* -950 + 2048 */
#define CLRMAP_OFFSET 1098

#define MANDELBROT 0
#define JULIA 1
#define MULTIBROT 2

/* Not good, but bypasses -std=c89 flag */
#ifndef NAN
# define NAN 0.0
#endif

typedef struct s_data
{
	void		*img;
	char		*addr;
	int			bits_per_pixel;
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
	double		image_min;
	double		image_max;
}				t_scale;

typedef struct s_fractal
{
	int			fractal_type;
	double		multibrot_exponent;
	t_complex	julia_c;
	t_scale		scale_re;
	t_scale		scale_im;
}				t_fractal;

typedef struct s_rgb
{
	int			r;
	int			g;
	int			b;
}				t_rgb;

void	mlx_pixel_put_buf(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

t_complex	complex_addition(t_complex z1, t_complex z2)
{
	t_complex	z;

	z.re = z1.re + z2.re;
	z.im = z1.im + z2.im;
	return (z);
}

/*
 * z1 = (a1 + b1 * i) // decomposed
 * z2 = (a2 + b2 * i) // decomposed
 * z1 * z2 = (a1 + b1*i) * (a2 + b2*i) // distribute
 *         = a1*a2 - b1*b2  +  (a1*b2 + a2*b1) * i // reordered, grouped
 */
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

/* alpha channel always 00 */
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

void	init_colors(int colors[CRES])
{
	int	i;

	i = -1;
	while (++i < CRES)
	{
		if (i < 0.16 * CRES)
			colors[(CLRMAP_OFFSET + i) % CRES] = interp_color(i, 0, 0.16 * CRES,
				1);
		else if (i < 0.42 * CRES)
			colors[(CLRMAP_OFFSET + i) % CRES] = interp_color(i, 0.16 * CRES,
				0.42 * CRES, 2);
		else if (i < 0.6425 * CRES)
			colors[(CLRMAP_OFFSET + i) % CRES] = interp_color(i, 0.42 * CRES,
				0.6425 * CRES, 3);
		else if (i < 0.8575 * CRES)
			colors[(CLRMAP_OFFSET + i) % CRES] = interp_color(i, 0.6425 * CRES,
				0.8575 * CRES, 4);
		else
			colors[(CLRMAP_OFFSET + i) % CRES] = interp_color(i, 0.8575 * CRES,
				CRES, 5);
	}
}

int	color_from_iter(t_complex z, int iter)
{
	double		smoothed;
	static int	colors[CRES];
	int			color_idx;

	if (!colors[0])
	{
		init_colors(colors);
	}
	if (iter == MAX_ITER)
		return (0);
	smoothed = log2(log2(z.re * z.re + z.im * z.im) / 2.0);
	color_idx = (int)(sqrt(iter + 10 - smoothed) * 256) % CRES;
	return (colors[color_idx]);
}

int	mandelbrot(t_complex c)
{
	t_complex	z;
	int			iter;

	z.re = 0.0;
	z.im = 0.0;
	iter = -1;
	while (++iter < MAX_ITER)
	{
		if (complex_modulus(z) > MAX_MODULUS)
			break ;
		z = complex_addition(complex_product(z, z), c);
	}
	return (color_from_iter(z, iter));
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

int	multibrot(t_complex c, double multibrot_exponent)
{
	t_complex	z;
	int			iter;

	z.re = 0.0;
	z.im = 0.0;
	iter = -1;
	while (++iter < MAX_ITER)
	{
		if (complex_modulus(z) > MAX_MODULUS)
			break ;
		z = complex_addition(complex_power(z, multibrot_exponent), c);
	}
	return (color_from_iter(z, iter));
}

int	julia(t_complex z, t_complex c)
{
	int	iter;

	iter = -1;
	while (++iter < MAX_ITER)
	{
		if (complex_modulus(z) > MAX_MODULUS)
			break ;
		z = complex_addition(complex_product(z, z), c);
	}
	return (color_from_iter(z, iter));
}

double	rescale(int coord, t_scale scale)
{
	double	image;

	image = coord - scale.preimage_min;
	image *= scale.image_max - scale.image_min;
	image /= scale.preimage_max - scale.preimage_min;
	image += scale.image_min;
	return (image);
}

int	compute_fractal_clr(t_complex c, t_fractal fractal)
{
	if (fractal.fractal_type == MANDELBROT)
		return (mandelbrot(c));
	else if (fractal.fractal_type == JULIA)
		return (julia(c, fractal.julia_c));
	else if (fractal.fractal_type == MULTIBROT)
		return (multibrot(c, fractal.multibrot_exponent));
	return (0);
}

void	render(void *mlx, void *mlx_win, t_img *img, t_fractal fractal)
{
	t_complex	c;
	int			x;
	int			y;

	y = -1;
	while (++y < IMG_HEIGHT)
	{
		x = -1;
		while (++x < IMG_WIDTH)
		{
			c.re = rescale(x, fractal.scale_re);
			c.im = rescale(y, fractal.scale_im);
			mlx_pixel_put_buf(img, x, y, compute_fractal_clr(c, fractal));
		}
	}
	mlx_put_image_to_window(mlx, mlx_win, img->img, 0, 0);
}

void	print_help(char **argv)
{
	printf("Usage:\t%s [options]\n\n"
			"options:\n\t"
			"--julia RE,IM\t"
			"Instead of rendering the\n\t\t\t"
			"mandelbrot set, render the\n\t\t\t"
			"julia set for c = RE + IM * i.\n\t\t\t"
			"RE and IM are decimals with a dot\n\t\t\t"
			"as the thousands separator.\n\t"
			"--center RE,IM\tMake RE + IM * i\n\t\t\t"
			"the center of the window\n\t"
			"--zoom FACTOR\tZoom the viewport by FACTOR\n\t"
			"--help\t\t"
			"Show this help\n",
			argv[0]);
}

int	parse_help(int argc, char **argv)
{
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
	fractal->scale_re.preimage_min = IMG_WIDTH;
	fractal->scale_re.image_min = MIN_RE;
	fractal->scale_re.image_max = MAX_RE;
	fractal->scale_im.preimage_min = 0;
	fractal->scale_im.preimage_min = IMG_HEIGHT;
	fractal->scale_im.image_min = -((MAX_RE - MIN_RE) * IMG_HEIGHT) / (2.0
			* IMG_WIDTH);
	fractal->scale_im.image_max = ((MAX_RE - MIN_RE) * IMG_HEIGHT) / (2.0
			* IMG_WIDTH);
}

void	parse_julia_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(printf("'%s' expects one parameter", **argv), exit(1));
	++(*argv);
	--(*argc);
	fractal->julia_c = parse_complex(**argv);
	if (fractal->julia_c.re == fractal->julia_c.re)
		fractal->fractal_type = JULIA;
	else
		(printf("Error parsing argument for '%s': NaN\n", **argv), exit(2));
}

void	parse_multibrot_param(int *argc, char ***argv, t_fractal *fractal)
{
	if (*argc < 2)
		(printf("'%s' expects one parameter", **argv), exit(1));
	++(*argv);
	--(*argc);
	fractal->multibrot_exponent = ft_atof(**argv);
	if (fractal->multibrot_exponent == fractal->multibrot_exponent)
		fractal->fractal_type = MULTIBROT;
	else
		(printf("Error parsing argument for '%s': NaN\n", **argv), exit(2));
}

t_fractal	parse_options(int argc, char **argv)
{
	t_fractal	fractal;

	init_fractal(&fractal);
	while (--argc)
	{
		if (!ft_strncmp(*(++argv), "--julia", 8))
			parse_julia_param(&argc, &argv, &fractal);
		else if (!ft_strncmp(*(argv), "--multibrot", 12))
			parse_multibrot_param(&argc, &argv, &fractal);
		else if (!ft_strncmp(*(argv), "--center", 9))
			parse_center_param(&argc, &argv, &fractal);
		else if (!ft_strncmp(*(argv), "--zoom", 7))
			parse_zoom_param(&argc, &argv, &fractal);
	}
	return (fractal);
}

void	init(void **mlx, void **mlx_win, t_img *img)
{
	*mlx = mlx_init();
	*mlx_win = mlx_new_window(*mlx, IMG_WIDTH, IMG_HEIGHT, WIN_TITLE);
	img->img = mlx_new_image(*mlx, IMG_WIDTH, IMG_HEIGHT);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel,
		&img->line_length, &img->endian);
}

int	main(int argc, char **argv)
{
	void		*mlx;
	void		*mlx_win;
	t_img		img;
	t_fractal	fractal;

	if (parse_help(argc, argv))
		return (1);
	fractal = parse_options(argc, argv);
	if (fractal.fractal_type == -1)
		return (print_help(argv), 2);
	init(&mlx, &mlx_win, &img);
	printf("fractal.julia_c.re: %f\n", fractal.julia_c.re);
	printf("fractal.julia_c.im: %f\n", fractal.julia_c.im);
	render(mlx, mlx_win, &img, fractal);
	mlx_loop(mlx);
	return (0);
}
