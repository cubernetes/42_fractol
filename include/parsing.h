/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 19:21:01 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/04 19:21:31 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <fractol.h>

t_fractal		parse_options(int argc, char **argv);
int				parse_help(int argc, char **argv);
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
