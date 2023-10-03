/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 16:24:42 by tosuman           #+#    #+#             */
/*   Updated: 2023/06/02 20:00:15 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(char const *s, int c)
{
	char	*last;

	last = 0;
	while (*s)
		if (*s++ == (char) c)
			last = (char *)(s - 1);
	if (*s == (char) c)
		return ((char *) s);
	return (last);
}
