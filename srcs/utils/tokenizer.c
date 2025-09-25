/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/09/25 18:07:24 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <stdlib.h>
#include <string.h>

char	*ft_strtok(char *str, const char *delim)
{
	static char	*save_ptr = NULL;
	char		*token_start;
	int			i;

	if (str)
		save_ptr = str;
	if (!save_ptr || *save_ptr == '\0')
		return (NULL);
	while (*save_ptr && strchr(delim, *save_ptr))
		save_ptr++;
	if (*save_ptr == '\0')
		return (NULL);
	token_start = save_ptr;
	while (*save_ptr && !strchr(delim, *save_ptr))
		save_ptr++;
	if (*save_ptr)
	{
		*save_ptr = '\0';
		save_ptr++;
	}
	else
		save_ptr = NULL;
	return (token_start);
}

char	*ms_tokenize_next(const char *input, int *i, int *in_squote,
			int *in_dquote);

static void	token_append(char **tokens, int *t, char *token)
{
	if (token)
	{
		tokens[*t] = token;
		(*t)++;
	}
}

static void	init_tokenize_vars(int *t, int *i, int *in_squote, int *in_dquote)
{
	*t = 0;
	*i = 0;
	*in_squote = 0;
	*in_dquote = 0;
}

char	**ms_tokenize(const char *input)
{
	char	**tokens;
	int		t;
	int		i;
	int		in_squote;
	int		in_dquote;

	tokens = (char **)malloc(sizeof(char *) * 256);
	if (!tokens)
		return (NULL);
	init_tokenize_vars(&t, &i, &in_squote, &in_dquote);
	while (input[i])
	{
		char *token;
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		token = ms_tokenize_next(input, &i, &in_squote, &in_dquote);
		token_append(tokens, &t, token);
	}
	tokens[t] = NULL;
	return (tokens);
}
