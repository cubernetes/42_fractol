/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 16:20:31 by tosuman           #+#    #+#             */
/*   Updated: 2023/06/02 19:58:22 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(char const *s, int c)
{
	while (*s)
		if (*s++ == (unsigned char) c)
			return ((char *)--s);
	if (*s == (unsigned char) c)
		return ((char *) s);
	else
		return (0);
}
