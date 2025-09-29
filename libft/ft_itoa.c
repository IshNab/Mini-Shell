/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:23:28 by maborges          #+#    #+#             */
/*   Updated: 2024/11/22 14:48:47 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nblen(long nb);

char	*ft_itoa(int n)
{
	char	*str;
	int		i;
	long	nb;

	nb = n;
	i = ft_nblen(nb);
	str = malloc((i + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[i] = '\0';
	if (nb < 0)
	{
		str[0] = '-';
		nb *= -1;
	}
	if (n == 0)
		str[0] = '0';
	while (nb > 0)
	{
		str[--i] = (nb % 10) + 48;
		nb /= 10;
	}
	return (str);
}

static int	ft_nblen(long nb)
{
	int	i;

	i = 0;
	if (nb <= 0)
		i = 1;
	while (nb != 0)
	{
		nb /= 10;
		i++;
	}
	return (i);
}
