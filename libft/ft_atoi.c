/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:52:27 by maborges          #+#    #+#             */
/*   Updated: 2024/11/21 19:45:31 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (nptr[i] == 32 || (nptr[i] > 8 && nptr[i] < 14))
		i++;
	if (nptr[i] == '-')
	{
		sign *= -1;
		i++;
	}
	else if (nptr[i] == '+')
		i++;
	while (nptr[i] != '\0')
	{
		if (ft_isdigit(nptr[i]))
			result = result * 10 + (nptr[i++] - 48);
		else
			break ;
	}
	return (result * sign);
}
		/* if (result * sign > INT_MAX)
            return (-1);
    	if (result * sign < INT_MIN)
            return (-1); */
