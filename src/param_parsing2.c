/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param_parsing2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 09:14:22 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/03 09:21:00 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>
#include <libft.h>
#include <stdlib.h>

t_complex	parse_center_param(int *argc, char ***argv)
{
	t_complex	center;

	check_param(argc, argv);
	center = parse_complex(**argv);
	if (center.re == center.re)
		return (center);
	else
		(ft_dprintf(2, "Error parsing argument for '%s': NaN\n", **argv),
			exit(EXIT_PARAM_NAN));
}

double	parse_zoom_param(int *argc, char ***argv)
{
	double	zoom;

	check_param(argc, argv);
	zoom = ft_atof(**argv);
	if (zoom == zoom)
		return (zoom);
	else
		(ft_dprintf(2, "Error parsing argument for '%s': NaN\n", **argv),
			exit(EXIT_PARAM_NAN));
}

void	parse_winsize_param(int *argc, char ***argv, t_fractal *fractal)
{
	int	i;

	check_param(argc, argv);
	fractal->img_width = ft_atoi(**argv);
	fractal->scale_re.old_max = fractal->img_width;
	i = -1;
	while ((**argv)[++i] && ft_tolower((**argv)[i]) != 'x')
		;
	if (!(**argv)[i])
		(ft_dprintf(2, "Error parsing argument for '%s': NaN\n", **argv),
			exit(EXIT_PARAM_NAN));
	else
	{
		fractal->img_height = ft_atoi(**argv + i + 1);
		fractal->scale_im.old_max = fractal->img_height;
	}
}

/* TODO: Only allow numeric arguments to atof/atoi
*/
void	parse_grad_phase_param(int *argc, char ***argv, t_fractal *fractal)
{
	check_param(argc, argv);
	fractal->gradient_phase = ft_atoi(**argv);
}

void	parse_iterations_param(int *argc, char ***argv, t_fractal *fractal)
{
	check_param(argc, argv);
	fractal->max_iters = ft_atoi(**argv);
}
