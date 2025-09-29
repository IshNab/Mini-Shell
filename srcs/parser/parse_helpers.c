/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:40:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/09/29 13:53:41 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	syntax_error_pipe(int expect_word)
{
	if (expect_word)
	{
		printf("Syntax error: unexpected pipe\n");
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
		else if (curr->type == TOKEN_REDIRECT_IN
			|| curr->type == TOKEN_REDIRECT_OUT
			|| curr->type == TOKEN_REDIRECT_APPEND
			|| curr->type == TOKEN_REDIRECT_HEREDOC)
		{
			if (syntax_error_redir(curr))
				return (0);
			expect_word = 0;
		}
		else if (curr->type == TOKEN_WORD)
			expect_word = 0;
		curr = curr->next;
	}
	if (expect_word)
	{
		printf("Syntax error: incomplete command\n");
		return (0);
	}
	return (1);
}
