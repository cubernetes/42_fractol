/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 17:39:55 by tosuman           #+#    #+#             */
/*   Updated: 2023/05/22 17:39:56 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <limits.h>

void	ft_bzero(void *s, size_t n);

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ret;
	void	*empty;

	if (nmemb * size == 0)
	{
		empty = malloc(0);
		if (!empty)
			return (0);
		return (empty);
	}
	if (UINT_MAX / nmemb < size)
		return (0);
	ret = malloc(nmemb * size);
	if (!ret)
		return (0);
	ft_bzero(ret, nmemb * size);
	return (ret);
}
