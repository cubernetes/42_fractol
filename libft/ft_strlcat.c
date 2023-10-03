/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 16:55:00 by tosuman           #+#    #+#             */
/*   Updated: 2023/06/26 20:41:08 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

size_t	ft_strlcat(char *dst, char const *src, size_t size)
{
	size_t	idx;
	size_t	sidx;
	size_t	slen;
	size_t	dlen;

	slen = 0;
	while (src[slen])
		++slen;
	idx = 0;
	while (dst[idx] && idx < size)
		++idx;
	dlen = idx;
	if (dlen == size)
		return (slen + size);
	sidx = 0;
	size -= dlen;
	while (size-- > 1 && src[sidx])
		dst[idx++] = src[sidx++];
	dst[idx] = 0;
	return (dlen + slen);
}
