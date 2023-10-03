/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 12:41:34 by tosuman           #+#    #+#             */
/*   Updated: 2023/06/03 12:41:34 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (f)(unsigned int, char))
{
	char	*str;
	int		idx;

	str = malloc(sizeof(*str) * (ft_strlen(s) + 1));
	if (!str)
		return (0);
	idx = -1;
	while (s[++idx])
		str[idx] = (f)(idx, s[idx]);
	str[idx] = 0;
	return (str);
}
