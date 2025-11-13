/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 16:16:33 by maborges          #+#    #+#             */
/*   Updated: 2025/10/08 12:32:43 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_ast	*parser(char *input, t_mshell *shell)
{
	t_token	*tokens;
	t_ast	*ast;

	if (!input || !*input)
		return (NULL);
	tokens = ms_tokenize(input);
	if (!tokens)
		return (NULL);
	if (!validate_syntax(tokens))
	{
		free_token_list(tokens);
		shell->exit_status = 2;
		return (NULL);
	}
	expand_vars(tokens, shell);
	ast = build_ast(tokens);
	if (!ast)
		return (free_token_list(tokens), NULL);
	free_token_list(tokens);
	return (ast);
}