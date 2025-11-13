/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 15:20:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/10/14 16:39:23 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_token_list(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->value);
		free(tmp);
	}
}

void	list_token_append(t_token *new, t_token **head, t_token **tail)
{
	if (!*head)
		*head = new;
	else
		(*tail)->next = new;
	*tail = new;
}

int	handle_redirections(t_token *new, const char *input, int *i)
{
	if (handle_input_redir(new, input, i))
		return (1);
	if (handle_output_redir(new, input, i))
		return (1);
	return (0);
}

static int	tok_handle_input_redir(t_token *new, const char *input, int *i)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		new->type = TOKEN_HEREDOC;
		new->value = ft_strdup("<<");
		(*i) += 2;
		return (1);
	}
	else if (input[*i] == '<')
	{
		new->type = TOKEN_REDIR_IN;
		new->value = ft_strdup("<");
		(*i)++;
		return (1);
	}
	return (0);
}

static int	tok_handle_output_redir(t_token *new, const char *input, int *i)
{
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		new->type = TOKEN_APPEND;
		new->value = ft_strdup(">>");
		(*i) += 2;
		return (1);
	}
	else if (input[*i] == '>')
	{
		new->type = TOKEN_REDIR_OUT;
		new->value = ft_strdup(">");
		(*i)++;
		return (1);
	}
	return (0);
}