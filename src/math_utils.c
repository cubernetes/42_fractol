/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 07:50:17 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/03 08:40:16 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>
#include <math.h>

double	log2(double x)
{
	return (log(x) / log(2));
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
