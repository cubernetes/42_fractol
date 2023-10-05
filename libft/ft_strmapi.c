/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 12:41:34 by tosuman           #+#    #+#             */
/*   Updated: 2023/10/05 23:07:00 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (f)(unsigned int, char))
{
	char				*str;
	unsigned int		idx;

	str = malloc(sizeof(*str) * (ft_strlen(s) + 1));
	if (!str)
		return (0);
	idx = (unsigned int)-1;
	while (s[++idx])
		str[idx] = (f)(idx, s[idx]);
	str[idx] = 0;
	return (str);
}
