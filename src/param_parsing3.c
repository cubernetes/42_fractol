/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param_parsing3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 09:17:43 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/03 09:22:02 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>
#include <libft.h>
#include <stdlib.h>

int	parse_help(int argc, char **argv)
{
	if (1 == argc)
		return (print_help(argv), 1);
	while (--argc)
		if (!ft_strcmp(argv[argc], "--help") || !ft_strcmp(argv[argc], "-h"))
			return (print_help(argv), 1);
	return (0);
}

void	check_param(int *argc, char ***argv)
{
	if (*argc < 2)
		(dprintf(2, "'%s' expects one parameter\n", **argv),
			exit(EXIT_MISSING_PARAM));
	++(*argv);
	--(*argc);
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

void	parse_julia_param(int *argc, char ***argv, t_fractal *fractal)
{
	check_param(argc, argv);
	fractal->julia_c = parse_complex(**argv);
	if (fractal->julia_c.re == fractal->julia_c.re)
		fractal->fractal_type = JULIA;
	else
		(dprintf(2, "Error parsing argument for '%s': NaN\n", **argv),
			exit(EXIT_PARAM_NAN));
}
