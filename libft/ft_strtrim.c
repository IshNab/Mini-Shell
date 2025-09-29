/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 20:39:41 by maborges          #+#    #+#             */
/*   Updated: 2024/11/26 19:31:28 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_trimmer(char const *set, char c);

char	*ft_strtrim(char const *s1, char const *set)
{
	char		*tmpstr;
	size_t		i;
	size_t		j;
	size_t		newlen;

	if (!s1 || !set)
		return (NULL);
	if (*s1 == '\0')
		return (ft_strdup(""));
	j = ft_strlen(s1);
	i = 0;
	while (i < j && ft_trimmer(set, s1[i]))
		i++;
	while (j > i && ft_trimmer(set, s1[j - 1]))
		j--;
	newlen = j - i;
	tmpstr = malloc(newlen + 1);
	if (!tmpstr)
		return (NULL);
	ft_memcpy(tmpstr, s1 + i, newlen);
	tmpstr[newlen] = '\0';
	return (tmpstr);
}

static int	ft_trimmer(char const *set, char c)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}
