/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_append.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/10/04 21:18:18 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*str_append(char *s, char c)
{
	char	*new_s;
	size_t	len;

	len = 0;
	if (s)
		len = ft_strlen(s);
	new_s = (char *)malloc(len + 2);
	if (!new_s)
		return (NULL);
	if (s)
		ft_strlcpy(new_s, s, len + 1);
	new_s[len] = c;
	new_s[len + 1] = '\0';
	free(s);
	return (new_s);
}
