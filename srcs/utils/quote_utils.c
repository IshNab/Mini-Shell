/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/09/25 18:06:40 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

char	*ms_remove_quotes(const char *token)
{
	char	*res;
	int		in_squote;
	int		in_dquote;
	int		i;

	res = NULL;
	in_squote = 0;
	in_dquote = 0;
	i = 0;
	while (token[i])
	{
		if (token[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (token[i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		else
			res = str_append(res, token[i]);
		i++;
	}
	return (res);
}
