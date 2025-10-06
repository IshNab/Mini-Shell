/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/10/05 14:36:54 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//char	*expand_variable(const char *token, int *i,
//			char **envp, int last_status);
//int		is_quote_to_skip(char c, int in_dquote, int in_squote);

static char	*expand_variable_env(const char *token, int *i, char **envp)
{
	int		j;
	int		len;
	char	var[256];
	char	*val;

	j = *i + 1;
	while (ft_isalnum(token[j]) || token[j] == '_')
		j++;
	len = j - *i - 1;
	ft_strlcpy(var, token + *i + 1, len + 1);
	val = get_env_value(var, envp);
	*i = j - 1;
	if (val)
		return (ft_strdup(val));
	return (NULL);
}

char	*expand_variable(const char *token, int *i,
			char **envp, int last_status)
{
	char	*res;

	res = NULL;
	if (token[*i + 1] == '?')
	{
		res = ft_itoa(last_status);
		(*i)++;
	}
	else if (ft_isalnum(token[*i + 1]) || token[*i + 1] == '_')
		res = expand_variable_env(token, i, envp);
	else
		res = str_append(NULL, '$');
	return (res);
}

void	expand_and_append(char **res, char *tmp)
{
	char	*old;

	old = *res;
	*res = ft_strjoin_free(*res, tmp);
	free(tmp);
	if (old)
		free(old);
}

static void	expand_token_loop_body(const char *token, int *i, int *in_squote,
			int *in_dquote, char **envp, int last_status, char **res)
{
	if (token[*i] == '$' && !(*in_squote))
	{
		char	*tmp;
		tmp = expand_variable(token, i, envp, last_status);
		if (tmp)
			expand_and_append(res, tmp);
	}
	else if (!is_quote_to_skip(token[*i], *in_dquote, *in_squote))
		*res = str_append(*res, token[*i]);
}

void	expand_token_loop(const char *token, char **res, char **envp,
			int last_status)
{
	int	in_squote;
	int	in_dquote;
	int	i;

	in_squote = 0;
	in_dquote = 0;
	i = 0;
	while (token[i])
	{
		// Handle quote state changes
		if (token[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (token[i] == '"' && !in_squote)
			in_dquote = !in_dquote;

		expand_token_loop_body(token, &i, &in_squote, &in_dquote, envp, last_status, res);
		i++;
	}
}

int	is_quote_to_skip(char c, int in_dquote, int in_squote)
{
	if ((c == '\'' && !in_dquote) || (c == '"' && !in_squote))
		return (1);
	return (0);
}
