/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 15:20:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/10/06 16:28:29 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	len;

	len = ft_strlen(s);
	if (n < len)
		len = n;
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, s, len);
	dup[len] = '\0';
	return (dup);
}

/* static t_token_type	get_token_type(const char *s)
{
	if (!strncmp(s, "|", 1))
		return (TOKEN_PIPE);
	else if (!strncmp(s, "<<", 2))
		return (TOKEN_REDIRECT_HEREDOC);
	else if (!strncmp(s, ">>", 2))
		return (TOKEN_REDIRECT_APPEND);
	else if (!strncmp(s, "<", 1))
		return (TOKEN_REDIRECT_IN);
	else if (!strncmp(s, ">", 1))
		return (TOKEN_REDIRECT_OUT);
	else if (!strncmp(s, "'", 1))
		return (TOKEN_SINGLE_QUOTE);
	else if (!strncmp(s, "\"", 1))
		return (TOKEN_DOUBLE_QUOTE);
	return (TOKEN_WORD);
} */

/* static void	add_token(t_token **head, t_token **tail, t_token *tok)
{
	if (!*head)
	{
		*head = tok;
		*tail = tok;
	}
	else
	{
		(*tail)->next = tok;
		*tail = tok;
	}
} */

/* void	handle_operator(const char *input, int *i,
	t_token **head, t_token **tail)
{
	int				op_len;
	t_token_type	type;
	t_token			*tok;

	op_len = is_operator(&input[*i]);
	type = get_token_type(&input[*i]);
	tok = new_token(type,
			ft_strndup(&input[*i], op_len));
	add_token(head, tail, tok);
	*i += op_len;
} */

void	handle_word(const char *input, int *i, t_token **head,
	t_token **tail)
{
	int		start;
	t_token	*tok;

	start = *i;
	while (input[*i] && !isspace(input[*i]) && !is_operator(&input[*i]))
		(*i)++;
	tok = new_token(TOKEN_WORD,
			ft_strndup(&input[start], *i - start));
	add_token(head, tail, tok);
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
