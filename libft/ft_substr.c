/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:23:00 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/05 23:05:13 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	idx;
	size_t			real_len;
	char			*substr;
	char			*empty;

	idx = (unsigned int)-1;
	while (s[++idx] && idx < start)
		;
	if (!s[idx])
	{
		empty = ft_calloc(1, sizeof(*empty));
		if (!empty)
			return (0);
		return (empty);
	}
	real_len = 0;
	while (++real_len <= len && s[idx + real_len - 1])
		;
	substr = malloc(sizeof(*substr) * real_len);
	if (!substr)
		return (0);
	ft_strlcpy(substr, s + idx, real_len);
	return (substr);
}
