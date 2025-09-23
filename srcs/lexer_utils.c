/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:10:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/09/23 11:55:00 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/lexer.h"
#include "../inc/minishell.h"

char	*str_append(char *s, char c)
{
	char	*new_s;
	int		len;

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

int	is_quote(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

char	*tokenize_next(const char *input, int *i, int *in_squote, int *in_dquote)
{
	char	*token;

	token = NULL;
	while (input[*i])
	{
		if (!*in_squote && !*in_dquote
			&& (input[*i] == ' ' || input[*i] == '\t'))
			break ;
		if (input[*i] == '\\' && input[*i + 1])
		{
			(*i)++;
			token = str_append(token, input[*i]);
		}
		else if (input[*i] == '\'' && !*in_dquote)
			*in_squote = !*in_squote;
		else if (input[*i] == '"' && !*in_squote)
			*in_dquote = !*in_dquote;
		else
			token = str_append(token, input[*i]);
		(*i)++;
	}
	return (token);
}

char	*get_env_value(const char *var, char **envp)
{
	int	k;
	int	len;

	k = 0;
	len = ft_strlen(var);
	while (envp && envp[k])
	{
		if (!ft_strncmp(envp[k], var, len))
		{
			if (envp[k][len] == '=')
				return (envp[k] + len + 1);
		}
		k++;
	}
	return (NULL);
}

char	*expand_status(int last_status)
{
	char	*buf;

	buf = (char *)malloc(16);
	if (!buf)
		return (NULL);
	ft_snprintf(buf, 16, "%d", last_status);
	return (buf);
}
