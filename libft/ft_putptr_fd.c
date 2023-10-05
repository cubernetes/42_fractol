/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <timo42@proton.me>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:51:47 by tischmid          #+#    #+#             */
/*   Updated: 2023/10/05 21:44:25 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putptr_fd(void *ptr, int fd)
{
	if (!ptr)
		return (ft_putstr_fd(NIL_PTR_STR, fd));
	return (ft_putchar_fd('0', fd) + \
			ft_putchar_fd('x', fd) + \
			ft_puthex_fd((unsigned long long)ptr, 0, fd));
}
