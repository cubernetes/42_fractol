/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 07:54:26 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/03 08:34:26 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stddef.h>

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
