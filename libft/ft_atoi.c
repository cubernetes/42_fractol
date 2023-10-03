/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 17:28:18 by tosuman           #+#    #+#             */
/*   Updated: 2023/06/02 19:58:10 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isspace(int c);
int	ft_isdigit(int c);

int	ft_atoi(char const *nptr)
{
	int	res;
	int	sign;

	while (ft_isspace(*nptr++))
		;
	sign = 1;
	if (*--nptr == '+' || *nptr == '-')
		if (*nptr++ == '-')
			sign = -1;
	res = 0;
	while (ft_isdigit(*nptr))
		res = res * 10 + *nptr++ - '0';
	return (sign * res);
}
