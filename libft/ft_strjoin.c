/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 19:15:21 by maborges          #+#    #+#             */
/*   Updated: 2024/11/28 18:31:39 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*tmpstr;
	size_t	i;
	size_t	j;
	size_t	count;

	if (!s1 || !s2)
		return (NULL);
	i = ft_strlen(s1);
	j = ft_strlen(s2);
	tmpstr = malloc((i + j + 1) * sizeof(char));
	if (!tmpstr)
		return (NULL);
	count = 0;
	while (count < i)
	{
		tmpstr[count] = s1[count];
		count++;
	}
	while (count < j + i)
	{
		tmpstr[count] = s2[count - i];
		count++;
	}
	tmpstr[count] = '\0';
	return (tmpstr);
}
