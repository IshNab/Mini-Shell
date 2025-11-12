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

void	print_tokens(t_token *tok)
{
	while (tok)
	{
		printf("Type: %d, Value: %s\n",
			tok->type, tok->value);
		tok = tok->next;
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