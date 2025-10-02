/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:49:26 by maborges          #+#    #+#             */
/*   Updated: 2024/11/19 11:21:06 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t		i;
	const char	*tmps;

	i = 0;
	tmps = (const char *)s;
	while (i < n)
	{
		if (tmps[i] == (char)c)
			return ((void *)&tmps[i]);
		i++;
	}
	return (NULL);
}
