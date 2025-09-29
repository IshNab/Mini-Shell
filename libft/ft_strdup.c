/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:44:02 by maborges          #+#    #+#             */
/*   Updated: 2024/11/28 18:28:49 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	i;
	char	*tmps1;

	i = 0;
	while (s[i] != '\0')
		i++;
	tmps1 = (char *) malloc(i * sizeof(char) + 1);
	if (!tmps1)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		tmps1[i] = s[i];
		i++;
	}
	tmps1[i] = '\0';
	return (tmps1);
}
