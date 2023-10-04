/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 07:55:21 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/04 19:20:03 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>
#include <libft.h>
#include <stddef.h>

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
	print_option("", "left click + drag", "Navigate the mandelbrot set");
	print_option("", "r/left click", "Rerender");
	print_option("", "z/x/0", "Zoom in/Zoom out/Reset viewport");
	print_option("", "1/2/3", "Change fractal to mandelbrot/julia/tricorn");
	print_option("", "scroll up/down", "Zoom in/Zoom out");
	print_option("", "./,", "Increase/Decrease the gradient phase");
	print_option("", "+/-", "Increase/Decrease the number of iterations");
	print_option("", "q or esc", "Quit");
}

void	print_more_help(void)
{
	print_option("--title", "TITLE", "The title of the window"
		" (default: fractol).");
	print_option("--winsize", "WIDTHxHEIGHT", "The window dimensions. Greatly"
		"influences performance (default: 1920x1080).");
	print_option("--real-intv", "min,max", "Min/max initial values for "
		"real axis. Window ratio is kept. Defines what zoom==1 is"
		" (default: -3,2).");
	print_option("--gradient-phase", "P", "Colors are indexed through an array "
		"(modulo arr-length). This parameter adds an offset"
		" (default: 1100, phase-len: 2048).");
	print_option("--iterations", "I", "Number of iterations to perform until a "
		"point is deemed part of the set. More iterations -> deeper zoom"
		" (default: 80).");
	print_option("--modulus", "R", "Minimum modulus (argument) of a complex "
		"number required to rule it out as not part of the set (default: 2).");
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
	print_option("--center", "RE,IM", "Make RE+IM*i the center of the window.");
	print_option("--zoom", "FACTOR", "Zoom the viewport by FACTOR.");
	print_option("--zoom-factor", "FACTOR", "The factor by which the factor "
		"will be changed (default: 1.1).");
	print_option("--mvmt-speed", "SPEED", "Viewport translation speed/step"
		" (default: 0.1).");
	print_more_help();
	print_runtime_bindings();
}
