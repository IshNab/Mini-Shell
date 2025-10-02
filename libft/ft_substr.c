/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:09:09 by maborges          #+#    #+#             */
/*   Updated: 2024/11/28 18:30:55 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*tmps;
	size_t	slen;
	size_t	actual_len;
	size_t	j;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if ((start >= slen))
		return (ft_strdup(""));
	actual_len = slen - start;
	if (len > actual_len)
		len = actual_len;
	tmps = malloc((len + 1) * sizeof(char));
	if (!tmps)
		return (NULL);
	j = 0;
	while (j < len)
	{
		tmps[j] = s[start + j];
		j++;
	}
	tmps[j] = '\0';
	return (tmps);
}
