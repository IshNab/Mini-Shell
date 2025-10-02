/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/09/24 14:28:16 by inabakka         ###   ########.fr       */
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

char	*ms_tokenize_next(const char *input, int *i, int *in_squote,\ 
	 int *in_dquote)
{
	char	*token;
	int		start;
	int		len;

	if (!input[*i])
		return (NULL);
	start = *i;
	len = 0;
	// Handle quoted content
	if (input[*i] == '\'' && !(*in_dquote))
	{
		*in_squote = !(*in_squote);
		(*i)++;
		token = ft_strdup("'");
		return (token);
	}
	else if (input[*i] == '"' && !(*in_squote))
	{
		*in_dquote = !(*in_dquote);
		(*i)++;
		token = ft_strdup("\"");
		return (token);
	}
	// Handle quoted word content
	else if (*in_squote || *in_dquote)
	{
		while (input[*i] && ((*in_squote && input[*i] != '\'') || 
			   (*in_dquote && input[*i] != '"')))
		{
			(*i)++;
			len++;
		}
		token = ft_strndup(&input[start], len);
		return (token);
	}
	// Handle regular word
	else
	{
		while (input[*i] && input[*i] != ' ' && input[*i] != '\t' && 
			   input[*i] != '\'' && input[*i] != '"' && 
			   input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
		{
			(*i)++;
			len++;
		}
		if (len == 0)
			return (NULL);
		token = ft_strndup(&input[start], len);
		return (token);
	}
}

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
