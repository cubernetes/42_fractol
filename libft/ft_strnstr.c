/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 17:05:27 by tosuman           #+#    #+#             */
/*   Updated: 2023/06/02 20:00:04 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(char const *big, char const *little, size_t len)
{
	size_t	l_len;
	size_t	b_len;

	if ((!big || !little) && len == 0)
		return (0);
	b_len = ft_strlen(big);
	l_len = ft_strlen(little);
	if (b_len < l_len || len < l_len)
		return (0);
	if (b_len - l_len + 1 < len)
		len = b_len - l_len + 1;
	else if (len >= l_len)
		len = len - l_len;
	len += !len;
	while (len--)
		if (!ft_memcmp(big++, little, l_len))
			return ((char *)--big);
	return (0);
}
