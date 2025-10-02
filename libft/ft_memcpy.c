/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:15:32 by maborges          #+#    #+#             */
/*   Updated: 2024/11/28 17:58:16 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*strdest;
	const char	*strsrc;
	size_t		i;

	if (!dest && !src)
		return (dest);
	strdest = (char *)dest;
	strsrc = (const char *)src;
	i = 0;
	while (i < n)
	{
		strdest[i] = strsrc[i];
		i++;
	}
	return (strdest);
}
