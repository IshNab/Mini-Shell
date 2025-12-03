/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:40:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/11/14 16:06:26 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	syntax_error_pipe(int expect_word)
{
	if (expect_word)
	{
		error_msg("Syntax error: unexpected pipe\n", 1, NULL);
		return (1);
	}
	return (0);
}

static int	syntax_error_redir(t_token *curr)
{
	if (!curr->next || curr->next->type != TOKEN_WORD)
	{
		ft_putstr_fd("Syntax error: redirection without target\n", 2);
		return (1);
	}
	return (0);
}

static int	is_redir_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN
		|| type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND
		|| type == TOKEN_HEREDOC);
}

static int	validate_token_loop(t_token *tokens)
{
	t_token	*curr;
	int		expect_word;

	curr = tokens;
	expect_word = 1;
	while (curr)
	{
		if (curr->type == TOKEN_PIPE && syntax_error_pipe(expect_word))
			return (0);
		if (curr->type == TOKEN_PIPE)
			expect_word = 1;
		else if (is_redir_token(curr->type) && syntax_error_redir(curr))
			return (0);
		else if (is_redir_token(curr->type))
			expect_word = 0;
		else if (curr->type == TOKEN_WORD)
			expect_word = 0;
		curr = curr->next;
	}
	if (expect_word)
		return (ft_putstr_fd("Syntax error: incomplete command\n", 2), 0);
	return (1);
}

int	validate_syntax(t_token *tokens)
{
	if (!tokens || (tokens->type == TOKEN_PIPE && !tokens->next))
		return (0);
	return (validate_token_loop(tokens));
}
