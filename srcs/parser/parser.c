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

t_ast	*parser(char *input, char **envp, t_mshell *shell)
{
	t_token	*tokens;
	t_ast	*ast;

	(void)envp;
	if (!input || !*input)
		return (NULL);
	//error check? when to free the linked list?
	tokens = ms_tokenize(input);	//step 1. tokenize the input
	if (!tokens)
		return (NULL);
	if (!validate_syntax(tokens)) //step 2. validate the syntax
	{
		free_token_list(tokens);
		shell->exit_status = 2;
		return (NULL);
	}
	//TODO a way to check if we need to expand or not (flags?)
	expand_vars(tokens, shell); //step 3. expand the variables
	remove_quote_tokens(&tokens); //step 4. remove the quote tokens
	ast = build_ast(tokens); //step 5. build the AST
	if (!ast)
	{
		free_token_list(tokens);
		return (NULL);
	}
	free_token_list(tokens);
	return (ast);
}
