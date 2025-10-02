/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 15:10:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/09/25 15:39:38 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// Helpers are now in lexer_helpers.c

t_token	*new_token(t_token_type type, char *value)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (tok == NULL)
		return (NULL);
	tok->type = type;
	if (value != NULL)
		tok->value = strdup(value);
	else
		tok->value = NULL;
	tok->next = NULL;
	return (tok);
}

static int	is_operator(const char *s)
{
	if (!strncmp(s, ">>", 2) || !strncmp(s, "<<", 2))
		return (2);
	if (*s == '|' || *s == '<' || *s == '>' || *s == '\'' || *s == '"')
		return (1);
	return (0);
}

// Prototypes for helpers
static t_token_type	get_token_type(const char *s, int op_len);
static void			add_token(t_token **head, t_token **tail, t_token *tok);
static void			handle_operator(const char *input, int *i, t_token **head,
								t_token **tail);
static void			handle_word(const char *input, int *i, t_token **head,
								t_token **tail);

t_token	*lexer(const char *input)
{
	t_token	*head;
	t_token	*tail;
	int		i;
	int		op_len;

	head = NULL;
	tail = NULL;
	i = 0;
	while (input[i])
	{
		while (isspace(input[i]))
			i++;
		if (!input[i])
			break ;
		op_len = is_operator(&input[i]);
		if (op_len)
		{
			handle_operator(input, &i, &head, &tail);
		}
		else
		{
			handle_word(input, &i, &head, &tail);
		}
	}
	return (head);
}

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
