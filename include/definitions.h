/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   definitions.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 19:27:49 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/04 19:37:21 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINITIONS_H
# define DEFINITIONS_H

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

#endif
