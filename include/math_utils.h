/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 19:24:47 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/04 19:25:06 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_UTILS_H
# define MATH_UTILS_H

# include <fractol.h>

double			log2(double x);
t_complex		conjugate(t_complex z);
t_complex		complex_addition(t_complex z1, t_complex z2);
t_complex		complex_product(t_complex z1, t_complex z2);
double			complex_modulus(t_complex z);
double			rescale(double coord, t_scale scale);
int				unrescale(double image, t_scale scale);

#endif
