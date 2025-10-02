/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 15:20:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/09/25 16:11:03 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char	*ft_strndup(const char *s, size_t n)
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

static t_token_type	get_token_type(const char *s, int op_len)
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
	return (TOKEN_WORD);
}

static void	add_token(t_token **head, t_token **tail, t_token *tok)
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
}

static void	handle_operator(const char *input, int *i,
	t_token **head, t_token **tail)
{
	int				op_len;
	t_token_type	type;
	t_token			*tok;

	op_len = is_operator(&input[*i]);
	type = get_token_type(&input[*i], op_len);
	tok = new_token(type,
			ft_strndup(&input[*i], op_len));
	add_token(head, tail, tok);
	*i += op_len;
}

static void	handle_word(const char *input, int *i, t_token **head,
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
