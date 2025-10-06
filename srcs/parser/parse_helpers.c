/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:40:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/10/06 19:06:03 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	syntax_error_pipe(int expect_word)
{
	if (expect_word)
	{
		//find a way to handle error
		error_msg("Syntax error: unexpected pipe\n", 1, NULL);
		return (1);
	}
	return (0);
}

int	syntax_error_redir(t_token *curr)
{
	if (!curr->next || curr->next->type != TOKEN_WORD)
	{
		printf("Syntax error: redirection without target\n");
		return (1);
	}
	return (0);
}

int	validate_syntax(t_token *tokens)
{
	t_token	*curr;
	int		expect_word;

	curr = tokens;
	expect_word = 1;
	while (curr)
	{
		if (curr->type == TOKEN_PIPE)
		{
			if (syntax_error_pipe(expect_word))
				return (0);
			expect_word = 1;
		}
		else if (curr->type == TOKEN_REDIR_IN
			|| curr->type == TOKEN_REDIR_OUT
			|| curr->type == TOKEN_APPEND
			|| curr->type == TOKEN_HEREDOC)
		{
			if (syntax_error_redir(curr))
				return (0);
			expect_word = 0;
		}
		else if (curr->type == TOKEN_WORD || curr->type == TOKEN_SQUOTE)
			expect_word = 0;
		else if (curr->type == TOKEN_DQUOTE)
			;
		curr = curr->next;
	}
	if (expect_word)
	{
		printf("Syntax error: incomplete command\n");
		return (0);
	}
	return (1);
}
