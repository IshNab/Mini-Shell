/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/09/23 11:51:44 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/lexer.h"
#include "../inc/minishell.h"

static void	skip_whitespace(const char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
}

static char	*tokenize_next(const char *input, int *i, int *in_squote, int *in_dquote);

char	**ms_tokenize(const char *input)
{
	char	**tokens;
	int		t;
	int		i;
	int		in_squote;
	int		in_dquote;
	char	*token;

	tokens = (char **)malloc(sizeof(char *) * 256);
	t = 0;
	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (input[i])
	{
		skip_whitespace(input, &i);
		token = tokenize_next(input, &i, &in_squote, &in_dquote);
		if (token)
			tokens[t++] = token;
	}
	tokens[t] = NULL;
	return (tokens);
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

#include "lexer_utils.c"
