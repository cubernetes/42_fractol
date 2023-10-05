/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <timo42@proton.me>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:50:49 by tischmid          #+#    #+#             */
/*   Updated: 2023/10/05 16:52:09 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_puthex(unsigned long long nbr, int uppercase)
{
	int	digit;
	int	count;

	count = 0;
	if (nbr < 16)
	{
		if (uppercase)
			count += ft_putchar(UHEX_DIGITS[nbr]);
		else
			count += ft_putchar(HEX_DIGITS[nbr]);
		return (count);
	}
	if (uppercase)
		digit = UHEX_DIGITS[nbr % 16];
	else
		digit = HEX_DIGITS[nbr % 16];
	nbr /= 16;
	count += ft_puthex(nbr, uppercase);
	count += ft_putchar(digit);
	return (count);
}
