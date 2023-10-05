/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putfmt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <timo42@proton.me>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 17:07:04 by tischmid          #+#    #+#             */
/*   Updated: 2023/10/05 17:07:11 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putfmt(const char **fmt, va_list ap)
{
	int	c;

	if (*(++*fmt) == 'c')
		c = ft_putchar(va_arg(ap, int));
	else if (**fmt == 'd' || **fmt == 'i')
		c = ft_putnbr((long long)va_arg(ap, int));
	else if (**fmt == 'u')
		c = ft_putnbr((long long)va_arg(ap, unsigned int));
	else if (**fmt == 'x')
		c = ft_puthex((long long)va_arg(ap, unsigned int), 0);
	else if (**fmt == 'X')
		c = ft_puthex((long long)va_arg(ap, unsigned int), 1);
	else if (**fmt == 's')
		c = ft_putstr(va_arg(ap, char *));
	else if (**fmt == 'p')
		c = ft_putptr(va_arg(ap, void *));
	else if (**fmt == '%')
		c = ft_putchar('%');
	else
		c = ft_printf("%%%c", **fmt);
	++*fmt;
	return (c);
}
