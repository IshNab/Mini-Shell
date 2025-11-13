/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/11/10 15:22:05 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	check_special_chars(t_token *new, const char *input, int *i)
{
	if (input[*i] == '|')
	{
		new->type = TOKEN_PIPE;
		new->value = ft_strdup("|");
		(*i)++;
	}
/* 	else if (input[*i] == '=')
	{
		new->type = TOKEN_WORD;
		new->value = ft_strdup("=");
		(*i)++;
	} */
	else if (handle_redirections(new, input, i))
		return ;
	else if (input[*i] == '"')
		handle_double_quote(new, input, i);
	else if (input[*i] == '\'')
		handle_single_quote(new, input, i);
	else
		handle_regular_word(new, input, i);
}

static void	handle_double_quote(t_token *new, const char *input, int *i)
{
	int	start;

	start = *i + 1;
	(*i)++;
	while (input[*i] && input[*i] != '"')
		(*i)++;
	if (input[*i] == '"')
	{
		new->type = TOKEN_DQUOTE;
		new->value = ft_substr(input, start, *i - start);
		if (!new->value)
			new->value = ft_strdup("");
		(*i)++;
	}
	else
	{
		new->type = TOKEN_WORD;
		new->value = ft_substr(input, start, *i - start);
		if (!new->value)
			new->value = ft_strdup("");
	}
}

static void	handle_single_quote(t_token *new, const char *input, int *i)
{
	int	start;

	start = *i + 1;
	(*i)++;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (input[*i] == '\'')
	{
		new->type = TOKEN_SQUOTE;
		new->value = ft_substr(input, start, *i - start);
		if (!new->value)
			new->value = ft_strdup("");
		(*i)++;
	}
	else
	{
		new->type = TOKEN_WORD;
		new->value = ft_substr(input, start, *i - start);
		if (!new->value)
			new->value = ft_strdup("");
	}
}

static void	handle_regular_word(t_token *new, const char *input, int *i)
{
	int	start;

	start = *i;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
		&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>'
		&& input[*i] != '"' && input[*i] != '\'')
		(*i)++;
	new->type = TOKEN_WORD;
	new->value = ft_substr(input, start, *i - start);
}

t_token	*ms_tokenize(const char *input)
{
	t_token	*head;
	t_token	*tail;
	t_token	*new;
	int		i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && (input[i] == ' ' || input[i] == '\t'))
			i++;
		if (!input[i])
			break ;
		new = malloc(sizeof(t_token));
		if (!new)
			return (free_token_list(head), NULL);
		new->next = NULL;
		check_special_chars(new, input, &i);
		list_token_append(new, &head, &tail);
	}
	return (head);
}