/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/10/07 15:07:24 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	list_token_append(t_token *new, t_token **head, t_token **tail)
{
	if (!*head)
		*head = new;
	else
		(*tail)->next = new;
	*tail = new;
}

// Check for special characters
static void	check_special_chars(t_token *new, const char *input, int *i)
{
	int	start;

	if (input[*i] == '|')
	{
		new->type = TOKEN_PIPE;
		new->value = ft_strdup("|");
		(*i)++;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		new->type = TOKEN_HEREDOC;
		new->value = ft_strdup("<<");
		(*i) += 2;
	}
	else if (input[*i] == '<')
	{
		new->type = TOKEN_REDIR_IN;
		new->value = ft_strdup("<");
		(*i)++;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		new->type = TOKEN_APPEND;
		new->value = ft_strdup(">>");
		(*i) += 2;
	}
	else if (input[*i] == '>')
	{
		new->type = TOKEN_REDIR_OUT;
		new->value = ft_strdup(">");
		(*i)++;
	}
	else if (input[*i] == '"')
	{
		(*i)++;
		new->type = TOKEN_DQUOTE;
		new->value = ft_strdup("\"");
	}
	else if (input[*i] == '\'')
	{
		(*i)++;
		new->type = TOKEN_SQUOTE;
		new->value = ft_strdup("'");
	}
	else
	{
		// Regular word
		start = *i;
		while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
			&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>'
			&& input[*i] != '"' && input[*i] != '\'')
			(*i)++;
		new->type = TOKEN_WORD;
		new->value = ft_substr(input, start, *i - start);
	}
}

t_token	*ms_tokenize(const char *input)	//convert input string into linked list of tokens
{
	t_token	*head;
	t_token	*tail;
	t_token	*new;
	int		i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (input[i])	//initialization
	{
		while (input[i] && (input[i] == ' ' || input[i] == '\t'))
			i++;
		if (!input[i])
			break ;
		new = malloc(sizeof(t_token));
		if (!new)
		{
			free_token_list(head);
			return (NULL);
		}
		new->next = NULL;
		check_special_chars(new, input, &i);	//check for special characters; identify tokens
		list_token_append(new, &head, &tail);
	}
	return (head);
}
