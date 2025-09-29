/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 12:25:13 by maborges          #+#    #+#             */
/*   Updated: 2024/11/28 18:00:35 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*tmpdest;
	const char	*tmpsrc;
	size_t		i;

	if (!dest && !src)
		return (dest);
	tmpdest = (char *)dest;
	tmpsrc = (const char *)src;
	i = 0;
	if (tmpdest > tmpsrc)
	{
		while (n-- > 0)
			tmpdest[n] = tmpsrc[n];
	}
	else
	{
		while (i < n)
		{
			tmpdest[i] = tmpsrc[i];
			i++;
		}
	}
	return (dest);
}
